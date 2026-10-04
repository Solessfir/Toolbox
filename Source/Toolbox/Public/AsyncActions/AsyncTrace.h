// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/CancellableAsyncAction.h"
#include "WorldCollision.h"
#include "AsyncTrace.generated.h"

UENUM(BlueprintType)
enum class EToolboxAsyncTraceType : uint8
{
    Test,
    Single,
    Multi
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FToolboxAsyncTraceResult, bool, bHit, const FHitResult&, OutHit, const TArray<FHitResult>&, OutHits);

/**
 * Runs a collision query and returns its result on the next world frame that ticks actors.
 * World pause can defer completion.
 * Test returns empty hit outputs, Single returns one hit, and Multi returns the hit array.
 * bHit reports blocking hits for channel/profile queries and any hit for object queries.
 * OnFailed reports invalid requests. Cancel prevents result delegates from firing.
 */
UCLASS(Meta = (ExposedAsyncProxy = "AsyncAction"))
class TOOLBOX_API UAsyncTrace_AsyncAction : public UCancellableAsyncAction
{
    GENERATED_BODY()

public:
    virtual void Activate() override;
    virtual void Cancel() override;

    UPROPERTY(BlueprintAssignable, Category = "Collision|Async")
    FToolboxAsyncTraceResult OnCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Collision|Async")
    FToolboxAsyncTraceResult OnFailed;

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Line Trace By Channel", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncLineTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Line Trace By Profile", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncLineTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Line Trace For Objects", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncLineTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Sphere Trace By Channel", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncSphereTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Sphere Trace By Profile", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncSphereTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Sphere Trace For Objects", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncSphereTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Capsule Trace By Channel", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncCapsuleTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Capsule Trace By Profile", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncCapsuleTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Capsule Trace For Objects", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncCapsuleTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float Radius, UPARAM(meta = (ClampMin = "0.0", UIMin = "0.0")) const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Box Trace By Channel", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncBoxTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Box Trace By Profile", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncBoxTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

    UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = true, WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore, ObjectTypes", AdvancedDisplay = "TraceMode, bIgnoreSelf"), DisplayName = "Async Box Trace For Objects", Category = "Collision|Async")
    static UAsyncTrace_AsyncAction* AsyncBoxTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode = EToolboxAsyncTraceType::Single, const bool bIgnoreSelf = true);

private:
    static UAsyncTrace_AsyncAction* Create(const UObject* WorldContextObject, const FVector& Start, const FVector& End, const FCollisionShape& Shape, const FQuat& Rotation, EToolboxAsyncTraceType TraceMode, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf);
    void SetTraceChannel(ETraceTypeQuery TraceChannel);
    void SetObjectTypes(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes);
    void SetProfileName(FName ProfileName);
    void HandleTraceCompleted(const FTraceHandle& Handle, FTraceDatum& TraceData);
    void HandleWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);
    void Finish(bool bFailed, const TArray<FHitResult>& Hits);

    enum class EFilter : uint8
    {
        Channel,
        Profile,
        Object
    };

    TWeakObjectPtr<UWorld> TraceWorld;
    FVector TraceStart = FVector::ZeroVector;
    FVector TraceEnd = FVector::ZeroVector;
    FQuat TraceRotation = FQuat::Identity;
    FCollisionShape TraceShape = FCollisionShape();
    FCollisionQueryParams QueryParams;
    FCollisionObjectQueryParams ObjectParams;
    FName ProfileName;
    ECollisionChannel TraceChannel = ECC_MAX;
    EAsyncTraceType NativeTraceType = EAsyncTraceType::Single;
    EFilter Filter = EFilter::Channel;
    FDelegateHandle WorldCleanupHandle;
    bool bValidRequest = true;
    bool bActivated = false;
    bool bFinished = false;
};
