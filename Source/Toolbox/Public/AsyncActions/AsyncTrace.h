// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/CancellableAsyncAction.h"
#include "WorldCollision.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsyncTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FToolboxAsyncTraceResult, bool, bHit, const FHitResult&, OutHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FToolboxAsyncMultiTraceResult, bool, bHit, const TArray<FHitResult>&, OutHits);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FToolboxAsyncTestTraceResult, bool, bHit);

/**
 * Runs a collision query and returns its result on the next world frame that ticks actors.
 * World pause can defer completion.
 * bHit reports blocking hits for channel/profile queries and any hit for object queries.
 * Invalid requests complete with bHit false and empty results. Cancel prevents completion.
 */
UCLASS(Abstract, Meta = (ExposedAsyncProxy = "AsyncAction"))
class TOOLBOX_API UAsyncTraceBase_AsyncAction : public UCancellableAsyncAction
{
    GENERATED_BODY()

public:
    virtual void Activate() override;
    virtual void Cancel() override;

protected:
    template <typename TAction>
    static TAction* Create(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FQuat& Rotation, EAsyncTraceType TraceMode, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
    {
        TAction* Action = NewObject<TAction>();
        Action->Initialize(WorldContextObject, Start, End, Shape, Rotation, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
        return Action;
    }

    void Initialize(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FQuat& Rotation, EAsyncTraceType TraceMode, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
    void SetTraceChannel(ETraceTypeQuery TraceChannel);
    void SetObjectTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes);
    void SetProfileName(FName ProfileName);
    virtual void BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits) PURE_VIRTUAL(UAsyncTraceBase_AsyncAction::BroadcastResult, );

    FVector TraceStart = FVector::ZeroVector;
    FVector TraceEnd = FVector::ZeroVector;
    bool bValidRequest = true;

private:
    void HandleTraceCompleted(const FTraceHandle& Handle, FTraceDatum& TraceData);
    void HandleWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);
    void Finish(bool bFailed, const TArray<FHitResult>& Hits);
    void DrawDebug(const TArray<FHitResult>& Hits, bool bHit) const;

    enum class EFilter : uint8
    {
        Channel,
        Profile,
        Object
    };

    TWeakObjectPtr<UWorld> TraceWorld;
    FQuat TraceRotation = FQuat::Identity;
    FCollisionShape TraceShape = FCollisionShape();
    FCollisionQueryParams QueryParams;
    FCollisionObjectQueryParams ObjectParams;
    FName ProfileName;
    ECollisionChannel TraceChannel = ECC_MAX;
    EAsyncTraceType NativeTraceType = EAsyncTraceType::Single;
    EFilter Filter = EFilter::Channel;
    FDelegateHandle WorldCleanupHandle;
    EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
    FLinearColor TraceColor = FLinearColor::Red;
    FLinearColor TraceHitColor = FLinearColor::Green;
    float DrawTime = 5.f;
    bool bActivated = false;
    bool bFinished = false;
};

/** Returns a single hit result. */
UCLASS(Meta = (ExposedAsyncProxy = "AsyncAction"))
class TOOLBOX_API UAsyncTrace_AsyncAction : public UAsyncTraceBase_AsyncAction
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Toolbox|Collision|Async")
    FToolboxAsyncTraceResult OnCompleted;

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncLineTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncLineTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncLineTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncSphereTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncSphereTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncSphereTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncCapsuleTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncCapsuleTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncCapsuleTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncBoxTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncBoxTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncBoxTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

private:
    virtual void BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits) override;
};

/** Returns all hit results. */
UCLASS(Meta = (ExposedAsyncProxy = "AsyncAction"))
class TOOLBOX_API UAsyncMultiTrace_AsyncAction : public UAsyncTraceBase_AsyncAction
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Toolbox|Collision|Async")
    FToolboxAsyncMultiTraceResult OnCompleted;

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace Multi By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncLineTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace Multi By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncLineTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace Multi For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncLineTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace Multi By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncSphereTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace Multi By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncSphereTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace Multi For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncSphereTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace Multi By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncCapsuleTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace Multi By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncCapsuleTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace Multi For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncCapsuleTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace Multi By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncBoxTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace Multi By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncBoxTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace Multi For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncMultiTrace_AsyncAction* AsyncBoxTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

private:
    virtual void BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits) override;
};

/** Tests for a hit without returning hit results. */
UCLASS(Meta = (ExposedAsyncProxy = "AsyncAction"))
class TOOLBOX_API UAsyncTestTrace_AsyncAction : public UAsyncTraceBase_AsyncAction
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Toolbox|Collision|Async")
    FToolboxAsyncTestTraceResult OnCompleted;

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace Test By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncLineTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace Test By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncLineTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Line Trace Test For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncLineTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace Test By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncSphereTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace Test By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncSphereTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Sphere Trace Test For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncSphereTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace Test By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncCapsuleTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace Test By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncCapsuleTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Capsule Trace Test For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncCapsuleTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace Test By Channel", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncBoxTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace Test By Profile", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncBoxTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "bIgnoreSelf, TraceColor, TraceHitColor, DrawTime"), DisplayName = "Async Box Trace Test For Objects", Category = "Toolbox|Collision|Async")
    static UAsyncTestTrace_AsyncAction* AsyncBoxTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None, const bool bIgnoreSelf = true, const FLinearColor TraceColor = FLinearColor::Red, const FLinearColor TraceHitColor = FLinearColor::Green, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float DrawTime = 5.0f);

private:
    virtual void BroadcastResult(bool bFailed, bool bHit, const TArray<FHitResult>& Hits) override;
};
