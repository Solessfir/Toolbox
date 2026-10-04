// Copyright Solessfir. All Rights Reserved.

#include "AsyncActions/AsyncTrace.h"

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetTraceChannel(TraceChannel);
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncLineTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetProfileName(ProfileName);
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncLineTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetObjectTypes(ObjectTypes);
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncSphereTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncSphereTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncSphereTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncCapsuleTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncCapsuleTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncCapsuleTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncBoxTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncBoxTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncBoxTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EToolboxAsyncTraceType TraceMode, const bool bIgnoreSelf)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTrace_AsyncAction* Action = Create(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), TraceMode, bTraceComplex, ActorsToIgnore, bIgnoreSelf);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}
