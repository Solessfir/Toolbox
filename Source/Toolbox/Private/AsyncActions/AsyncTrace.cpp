// Copyright Solessfir. All Rights Reserved.

#include "AsyncActions/AsyncTrace.h"

#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "KismetTraceUtils.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "UObject/StrongObjectPtr.h"

void UAsyncTraceBase_AsyncAction::Initialize(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FQuat& Rotation, EAsyncTraceType TraceMode, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, EDrawDebugTrace::Type InDrawDebugType, FLinearColor InTraceColor, FLinearColor InTraceHitColor, float InDrawTime)
{
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull) : nullptr;
    TraceWorld = World;
    TraceStart = Start;
    TraceEnd = End;
    TraceShape = Shape;
    TraceRotation = Rotation;
    NativeTraceType = TraceMode;
    DrawDebugType = InDrawDebugType;
    TraceColor = InTraceColor;
    TraceHitColor = InTraceHitColor;
    DrawTime = FMath::IsFinite(InDrawTime) ? FMath::Max(0.f, InDrawTime) : 0.f;
    bValidRequest = IsValid(World) && World->GetGameInstance() && !Start.ContainsNaN() && !End.ContainsNaN() && !Rotation.ContainsNaN();

    QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(ToolboxAsyncTrace), bTraceComplex);
    QueryParams.bReturnPhysicalMaterial = true;
    QueryParams.bReturnFaceIndex = !UPhysicsSettings::Get()->bSuppressFaceRemapTable;
    QueryParams.AddIgnoredActors(ActorsToIgnore);
    if (bIgnoreSelf)
    {
        for (const UObject* Object = WorldContextObject; Object; Object = Object->GetOuter())
        {
            if (const AActor* Actor = Cast<AActor>(Object))
            {
                QueryParams.AddIgnoredActor(Actor);
                break;
            }
        }
    }

    if (IsValid(World))
    {
        RegisterWithGameInstance(World->GetGameInstance());
        WorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddUObject(this, &UAsyncTraceBase_AsyncAction::HandleWorldCleanup);
    }
}

void UAsyncTraceBase_AsyncAction::SetTraceChannel(ETraceTypeQuery Channel)
{
    Filter = EFilter::Channel;
    TraceChannel = UEngineTypes::ConvertToCollisionChannel(Channel);
    bValidRequest &= TraceChannel < ECC_MAX;
}

void UAsyncTraceBase_AsyncAction::SetObjectTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
{
    Filter = EFilter::Object;
    for (const EObjectTypeQuery ObjectType : ObjectTypes)
    {
        const ECollisionChannel Channel = UEngineTypes::ConvertToCollisionChannel(ObjectType);
        if (Channel < ECC_MAX && FCollisionObjectQueryParams::IsValidObjectQuery(Channel))
        {
            ObjectParams.AddObjectTypesToQuery(Channel);
        }
        else
        {
            bValidRequest = false;
        }
    }
    bValidRequest &= ObjectParams.IsValid();
}

void UAsyncTraceBase_AsyncAction::SetProfileName(FName Name)
{
    Filter = EFilter::Profile;
    ProfileName = Name;
    ECollisionChannel Channel;
    FCollisionResponseParams ResponseParams;
    bValidRequest &= UCollisionProfile::Get()->GetChannelAndResponseParams(Name, Channel, ResponseParams);
}

void UAsyncTraceBase_AsyncAction::Activate()
{
    if (bActivated || bFinished)
    {
        return;
    }
    bActivated = true;

    UWorld* World = TraceWorld.Get();
    if (!bValidRequest || !IsValid(World) || !World->bIsWorldInitialized || World->bIsTearingDown || !World->GetPhysicsScene() || !ShouldBroadcastDelegates())
    {
        Finish(true, {});
        return;
    }

    const FTraceDelegate Delegate = FTraceDelegate::CreateUObject(this, &UAsyncTraceBase_AsyncAction::HandleTraceCompleted);
    // Native sweeps dispatch line shapes as raycasts, so all shapes share the same submission path.
    switch (Filter)
    {
        case EFilter::Channel:
            World->AsyncSweepByChannel(NativeTraceType, TraceStart, TraceEnd, TraceRotation, TraceChannel, TraceShape, QueryParams, FCollisionResponseParams::DefaultResponseParam, &Delegate);
            break;
        case EFilter::Profile:
            World->AsyncSweepByProfile(NativeTraceType, TraceStart, TraceEnd, TraceRotation, ProfileName, TraceShape, QueryParams, &Delegate);
            break;
        case EFilter::Object:
            World->AsyncSweepByObjectType(NativeTraceType, TraceStart, TraceEnd, TraceRotation, ObjectParams, TraceShape, QueryParams, &Delegate);
            break;
    }
}

void UAsyncTraceBase_AsyncAction::HandleTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
    if (!bFinished && ShouldBroadcastDelegates())
    {
        Finish(false, Data.OutHits);
    }
    else
    {
        Cancel();
    }
}

void UAsyncTraceBase_AsyncAction::Finish(bool bFailed, const TArray<FHitResult>& Hits)
{
    if (bFinished)
    {
        return;
    }
    const TStrongObjectPtr<UAsyncTraceBase_AsyncAction> KeepAlive(this);
    bFinished = true;
    FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
    WorldCleanupHandle.Reset();

    const bool bHit = !bFailed && (Filter == EFilter::Object ? !Hits.IsEmpty() : Hits.ContainsByPredicate([](const FHitResult& Hit) { return Hit.bBlockingHit; }));
    if (!bFailed)
    {
        DrawDebug(Hits, bHit);
    }
    BroadcastResult(bFailed, bHit, Hits);
    SetReadyToDestroy();
}

void UAsyncTraceBase_AsyncAction::Cancel()
{
    bFinished = true;
    FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
    WorldCleanupHandle.Reset();
    Super::Cancel();
}

void UAsyncTraceBase_AsyncAction::HandleWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
    if (World == TraceWorld.Get())
    {
        Cancel();
    }
}

void UAsyncTraceBase_AsyncAction::DrawDebug(const TArray<FHitResult>& Hits, bool bHit) const
{
#if ENABLE_DRAW_DEBUG
    const UWorld* World = TraceWorld.Get();
    if (!IsValid(World) || World->bIsTearingDown || DrawDebugType == EDrawDebugTrace::None)
    {
        return;
    }

    // Test queries have no impact geometry, so color the whole path instead of drawing synthetic hits.
    const bool bTest = NativeTraceType == EAsyncTraceType::Test;
    const FLinearColor DebugTraceColor = bTest && bHit ? TraceHitColor : TraceColor;
    const bool bDrawHit = !bTest && bHit;
    const FHitResult OutHit = bTest || Hits.IsEmpty() ? FHitResult() : Hits[0];
    const bool bMulti = NativeTraceType == EAsyncTraceType::Multi;

    if (TraceShape.IsNearlyZero())
    {
        if (bMulti)
        {
            DrawDebugLineTraceMulti(World, TraceStart, TraceEnd, DrawDebugType, bDrawHit, Hits, DebugTraceColor, TraceHitColor, DrawTime);
        }
        else
        {
            DrawDebugLineTraceSingle(World, TraceStart, TraceEnd, DrawDebugType, bDrawHit, OutHit, DebugTraceColor, TraceHitColor, DrawTime);
        }
    }
    else if (TraceShape.IsSphere())
    {
        if (bMulti)
        {
            DrawDebugSphereTraceMulti(World, TraceStart, TraceEnd, TraceShape.GetSphereRadius(), DrawDebugType, bDrawHit, Hits, DebugTraceColor, TraceHitColor, DrawTime);
        }
        else
        {
            DrawDebugSphereTraceSingle(World, TraceStart, TraceEnd, TraceShape.GetSphereRadius(), DrawDebugType, bDrawHit, OutHit, DebugTraceColor, TraceHitColor, DrawTime);
        }
    }
    else if (TraceShape.IsCapsule())
    {
        if (bMulti)
        {
            DrawDebugCapsuleTraceMulti(World, TraceStart, TraceEnd, TraceShape.GetCapsuleRadius(), TraceShape.GetCapsuleHalfHeight(), DrawDebugType, bDrawHit, Hits, DebugTraceColor, TraceHitColor, DrawTime);
        }
        else
        {
            DrawDebugCapsuleTraceSingle(World, TraceStart, TraceEnd, TraceShape.GetCapsuleRadius(), TraceShape.GetCapsuleHalfHeight(), DrawDebugType, bDrawHit, OutHit, DebugTraceColor, TraceHitColor, DrawTime);
        }
    }
    else if (TraceShape.IsBox())
    {
        if (bMulti)
        {
            DrawDebugBoxTraceMulti(World, TraceStart, TraceEnd, TraceShape.GetBox(), TraceRotation.Rotator(), DrawDebugType, bDrawHit, Hits, DebugTraceColor, TraceHitColor, DrawTime);
        }
        else
        {
            DrawDebugBoxTraceSingle(World, TraceStart, TraceEnd, TraceShape.GetBox(), TraceRotation.Rotator(), DrawDebugType, bDrawHit, OutHit, DebugTraceColor, TraceHitColor, DrawTime);
        }
    }
#endif
}

void UAsyncTrace_AsyncAction::BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits)
{
    const FHitResult* BlockingHit = Hits.FindByPredicate([](const FHitResult& Hit) { return Hit.bBlockingHit; });
    const FHitResult OutHit = bFailed ? FHitResult() : (BlockingHit ? *BlockingHit : (Hits.IsEmpty() ? FHitResult(TraceStart, TraceEnd) : Hits[0]));
    OnCompleted.Broadcast(bHit, OutHit);
}

void UAsyncMultiTrace_AsyncAction::BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits)
{
    OnCompleted.Broadcast(bHit, Hits);
}

void UAsyncTestTrace_AsyncAction::BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits)
{
    OnCompleted.Broadcast(bHit);
}
