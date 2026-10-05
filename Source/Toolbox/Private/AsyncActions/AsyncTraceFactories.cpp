// Copyright Solessfir. All Rights Reserved.

#include "AsyncActions/AsyncTrace.h"

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncLineTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncLineTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncSphereTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncSphereTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncSphereTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncCapsuleTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncCapsuleTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncCapsuleTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncBoxTraceByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncBoxTraceByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncTrace_AsyncAction* UAsyncTrace_AsyncAction::AsyncBoxTraceForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTrace_AsyncAction* Action = Create<UAsyncTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Single, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}
UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncLineTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncLineTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncLineTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncSphereTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncSphereTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncSphereTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncCapsuleTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncCapsuleTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncCapsuleTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncBoxTraceMultiByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncBoxTraceMultiByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncMultiTrace_AsyncAction* UAsyncMultiTrace_AsyncAction::AsyncBoxTraceMultiForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncMultiTrace_AsyncAction* Action = Create<UAsyncMultiTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Multi, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}
UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncLineTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncLineTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncLineTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape(), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncSphereTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncSphereTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncSphereTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeSphere(Radius), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f;
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncCapsuleTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncCapsuleTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncCapsuleTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const float Radius, const float HalfHeight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, FCollisionShape::MakeCapsule(Radius, HalfHeight), FQuat::Identity, EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && FMath::IsFinite(Radius) && Radius >= 0.f && FMath::IsFinite(HalfHeight) && HalfHeight >= 0.f;
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncBoxTraceTestByChannel(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const ETraceTypeQuery TraceChannel, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetTraceChannel(TraceChannel);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncBoxTraceTestByProfile(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const FName ProfileName, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetProfileName(ProfileName);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}

UAsyncTestTrace_AsyncAction* UAsyncTestTrace_AsyncAction::AsyncBoxTraceTestForObjects(const UObject* WorldContextObject, const FVector Start, const FVector End, const FVector HalfSize, const FRotator Orientation, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, const EDrawDebugTrace::Type DrawDebugType, const bool bIgnoreSelf, const FLinearColor TraceColor, const FLinearColor TraceHitColor, const float DrawTime)
{
    const bool bValidSize = !HalfSize.ContainsNaN()
        && HalfSize.X >= 0.0 && HalfSize.Y >= 0.0 && HalfSize.Z >= 0.0
        && HalfSize.X <= MAX_flt && HalfSize.Y <= MAX_flt && HalfSize.Z <= MAX_flt;
    UAsyncTestTrace_AsyncAction* Action = Create<UAsyncTestTrace_AsyncAction>(WorldContextObject, Start, End, bValidSize ? FCollisionShape::MakeBox(HalfSize) : FCollisionShape(), Orientation.ContainsNaN() ? FQuat::Identity : Orientation.Quaternion(), EAsyncTraceType::Test, bTraceComplex, ActorsToIgnore, bIgnoreSelf, DrawDebugType, TraceColor, TraceHitColor, DrawTime);
    Action->SetObjectTypes(ObjectTypes);
    Action->bValidRequest = Action->bValidRequest && bValidSize && !Orientation.ContainsNaN();
    return Action;
}
