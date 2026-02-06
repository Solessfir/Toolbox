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

    virtual TStatId GetStatId() const override;

    /**
    * Smoothly lerps a Scene Component to a new socket on its current parent
    * @param InComponent Component to move
    * @param InTargetSocket Socket name on the Parent to Attach to
    * @param InRelativeOffset Additional Relative Transform Attach offset
    * @param Duration How long the transition should take
    */
    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, AdvancedDisplay = "InRelativeOffset"), Category = "Transformation")
    static ULerpToAttachmentSocket_AsyncAction* LerpToAttachmentSocket(USceneComponent* InComponent, const FName InTargetSocket, const FTransform InRelativeOffset, const float Duration = 0.25f);

    UFUNCTION(BlueprintCallable, Category = "Transformation|Debug")
    void DrawDebugLocation(float Radius = 10.f, FLinearColor Color = FLinearColor::Red, float DrawDuration = 0.1f) const;

    UPROPERTY(BlueprintAssignable)
    FOnLerpStep OnUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnLerpStep OnFinished;

private:
    UPROPERTY()
    TObjectPtr<USceneComponent> SceneComponent;

    FName TargetSocket;

    float Duration = 0.f;

    FTransform TargetTransform;

    float CurrentDuration = 0.f;

    bool bActive = false;

    FTransform StartTransformRelative;
};
