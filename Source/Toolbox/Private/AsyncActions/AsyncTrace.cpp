// Copyright Solessfir. All Rights Reserved.

#include "AsyncActions/AsyncTrace.h"

#include "Engine/CollisionProfile.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "UObject/StrongObjectPtr.h"

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::Create(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FQuat& Rotation, EToolboxAsyncTraceType TraceMode, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = NewObject<UAsyncTrace_AsyncAction>();
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull) : nullptr;
    Action->TraceWorld = World;
    Action->TraceStart = Start;
    Action->TraceEnd = End;
    Action->TraceShape = Shape;
    Action->TraceRotation = Rotation;
    Action->bValidRequest = IsValid(World) && World->GetGameInstance() && !Start.ContainsNaN() && !End.ContainsNaN() && !Rotation.ContainsNaN();

    switch (TraceMode)
    {
        case EToolboxAsyncTraceType::Test: Action->NativeTraceType = EAsyncTraceType::Test; break;
        case EToolboxAsyncTraceType::Single: Action->NativeTraceType = EAsyncTraceType::Single; break;
        case EToolboxAsyncTraceType::Multi: Action->NativeTraceType = EAsyncTraceType::Multi; break;
        default: Action->bValidRequest = false; break;
    }

    Action->QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(ToolboxAsyncTrace), bTraceComplex);
    Action->QueryParams.bReturnPhysicalMaterial = true;
    Action->QueryParams.bReturnFaceIndex = !UPhysicsSettings::Get()->bSuppressFaceRemapTable;
    Action->QueryParams.AddIgnoredActors(ActorsToIgnore);
    if (bIgnoreSelf)
    {
        for (const UObject* Object = WorldContextObject; Object; Object = Object->GetOuter())
        {
            if (const AActor* Actor = Cast<AActor>(Object))
            {
                Action->QueryParams.AddIgnoredActor(Actor);
                break;
            }
        }
    }

    if (IsValid(World))
    {
        Action->RegisterWithGameInstance(World->GetGameInstance());
        Action->WorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddUObject(Action, &UAsyncTrace_AsyncAction::HandleWorldCleanup);
    }
    return Action;
}

void UAsyncTrace_AsyncAction::SetTraceChannel(ETraceTypeQuery Channel)
{
    Filter = EFilter::Channel;
    TraceChannel = UEngineTypes::ConvertToCollisionChannel(Channel);
    bValidRequest &= TraceChannel < ECC_MAX;
}

void UAsyncTrace_AsyncAction::SetObjectTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
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

void UAsyncTrace_AsyncAction::SetProfileName(FName Name)
{
    Filter = EFilter::Profile;
    ProfileName = Name;
    ECollisionChannel Channel;
    FCollisionResponseParams ResponseParams;
    bValidRequest &= UCollisionProfile::Get()->GetChannelAndResponseParams(Name, Channel, ResponseParams);
}

void UAsyncTrace_AsyncAction::Activate()
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

    const FTraceDelegate Delegate = FTraceDelegate::CreateUObject(this, &UAsyncTrace_AsyncAction::HandleTraceCompleted);
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

void UAsyncTrace_AsyncAction::HandleTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
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

void UAsyncTrace_AsyncAction::Finish(bool bFailed, const TArray<FHitResult>& Hits)
{
    if (bFinished)
    {
        return;
    }
    const TStrongObjectPtr<UAsyncTrace_AsyncAction> KeepAlive(this);
    bFinished = true;
    FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
    WorldCleanupHandle.Reset();

    if (bFailed)
    {
        OnFailed.Broadcast(false, FHitResult(), {});
    }
    else
    {
        const FHitResult* BlockingHit = Hits.FindByPredicate([](const FHitResult& Hit) { return Hit.bBlockingHit; });
        const bool bHit = Filter == EFilter::Object ? !Hits.IsEmpty() : BlockingHit != nullptr;
        if (NativeTraceType == EAsyncTraceType::Test)
        {
            // Test results contain only a synthetic blocking flag, not usable hit geometry.
            OnCompleted.Broadcast(bHit, FHitResult(), {});
        }
        else
        {
            const FHitResult OutHit = BlockingHit ? *BlockingHit : (Hits.IsEmpty() ? FHitResult(TraceStart, TraceEnd) : Hits[0]);
            OnCompleted.Broadcast(bHit, OutHit, Hits);
        }
    }
    SetReadyToDestroy();
}

void UAsyncTrace_AsyncAction::Cancel()
{
    bFinished = true;
    FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
    WorldCleanupHandle.Reset();
    Super::Cancel();
}

void UAsyncTrace_AsyncAction::HandleWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
    if (World == TraceWorld.Get())
    {
        Cancel();
    }
}
