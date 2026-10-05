// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Tickable.h"
#include "LerpToAttachmentSocket.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLerpStep);

UCLASS(Meta = (ExposedAsyncProxy = "AsyncAction"))
class TOOLBOX_API ULerpToAttachmentSocket_AsyncAction : public UBlueprintAsyncActionBase, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void Activate() override;

    virtual void Tick(float DeltaTime) override;

    virtual bool IsTickable() const override;

    virtual UWorld* GetTickableGameObjectWorld() const override;

    virtual TStatId GetStatId() const override;

    /**
    * Smoothly lerps a Scene Component to a new socket on its current parent
    * Completes immediately if the attachment is rejected
    * @param InComponent Component to move
    * @param InTargetSocket Socket name on the Parent to Attach to
    * @param InTargetTransform Desired final Relative Transform once attached to the target socket. Use zero for no offset
    * @param Duration Transition length in game time, respecting world pause and time dilation
    * @param bDrawDebug Draw a sphere at the component location on each interpolation update
    */
    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, AdvancedDisplay = "InTargetTransform, DebugRadius, DebugColor, DebugDrawDuration", AutoCreateRefTerm = "InTargetTransform"), DisplayName = "Lerp To Attachment Socket", Category = "Toolbox|Transformation")
    static ULerpToAttachmentSocket_AsyncAction* LerpToAttachmentSocket(USceneComponent* InComponent, const FName InTargetSocket, const FTransform& InTargetTransform, const float Duration = 0.25f, const bool bDrawDebug = false, const float DebugRadius = 10.f, const FLinearColor DebugColor = FLinearColor::Red, const float DebugDrawDuration = 0.1f);

    UPROPERTY(BlueprintAssignable, Category = "Toolbox|Transformation")
    FOnLerpStep OnUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Toolbox|Transformation")
    FOnLerpStep OnFinished;

private:
    UPROPERTY()
    TObjectPtr<USceneComponent> SceneComponent;

    FName TargetSocket;

    float Duration = 0.f;

    FTransform TargetTransform;

    float CurrentDuration = 0.f;

    bool bActive = false;

    bool bDrawDebug = false;

    float DebugRadius = 10.f;

    FLinearColor DebugColor = FLinearColor::Red;

    float DebugDrawDuration = 0.1f;

    FTransform StartTransformRelative;
};
