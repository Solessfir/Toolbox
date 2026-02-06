// Copyright Solessfir. All Rights Reserved.

#include "AsyncActions/LerpToAttachmentSocket.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ULerpToAttachmentSocket_AsyncAction* ULerpToAttachmentSocket_AsyncAction::LerpToAttachmentSocket(USceneComponent* InComponent, const FName InTargetSocket, const FTransform InRelativeOffset, const float Duration)
{
    if (!InComponent)
    {
        return nullptr;
    }

    ULerpToAttachmentSocket_AsyncAction* AsyncAction = NewObject<ULerpToAttachmentSocket_AsyncAction>();
    AsyncAction->SceneComponent = InComponent;
    AsyncAction->TargetSocket = InTargetSocket;
    AsyncAction->TargetTransform = InRelativeOffset;
    AsyncAction->Duration = FMath::Max(0.001f, Duration);
    return AsyncAction;
}

void ULerpToAttachmentSocket_AsyncAction::Activate()
{
    if (!SceneComponent || !SceneComponent->GetAttachParent())
    {
        OnFinished.Broadcast();
        SetReadyToDestroy();
        return;
    }

    // Register with Game Instance to prevent Garbage Collection while Ticking
    RegisterWithGameInstance(SceneComponent->GetWorld());

    // Attach immediately but keep World Position. This calculates the necessary Relative offset to the new Socket automatically
    SceneComponent->AttachToComponent(SceneComponent->GetAttachParent(), FAttachmentTransformRules::KeepWorldTransform, TargetSocket);
    StartTransformRelative = SceneComponent->GetRelativeTransform();
    bActive = true;
}

void ULerpToAttachmentSocket_AsyncAction::Tick(float DeltaTime)
{
    if (!bActive || !SceneComponent)
    {
        return;
    }

    CurrentDuration += DeltaTime;
    const float Alpha = FMath::Clamp(CurrentDuration / Duration, 0.f, 1.f);

    // DualQuatInterp is used for smoother Rotation handling
    const FTransform CurrentRelTransform = UKismetMathLibrary::TLerp(StartTransformRelative, TargetTransform, Alpha, ELerpInterpolationMode::DualQuatInterp);
    SceneComponent->SetRelativeTransform(CurrentRelTransform);
    OnUpdated.Broadcast();

    if (Alpha >= 1.f)
    {
        bActive = false;
        OnFinished.Broadcast();
        SetReadyToDestroy();
    }
}

bool ULerpToAttachmentSocket_AsyncAction::IsTickable() const
{
    return bActive && !HasAnyFlags(RF_BeginDestroyed);
}

TStatId ULerpToAttachmentSocket_AsyncAction::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(ULerpToAttachmentSocket_AsyncAction, STATGROUP_Tickables);
}

void ULerpToAttachmentSocket_AsyncAction::DrawDebugLocation(const float Radius, const FLinearColor Color, const float DrawDuration) const
{
    if (SceneComponent)
    {
        UKismetSystemLibrary::DrawDebugSphere(SceneComponent->GetWorld(), SceneComponent->GetComponentLocation(), Radius, 12, Color, DrawDuration);
    }
}
