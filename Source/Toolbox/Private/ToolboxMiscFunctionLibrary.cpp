// Copyright Solessfir. All Rights Reserved.

#include "ToolboxMiscFunctionLibrary.h"
#include "ToolboxHelpers.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxMiscFunctionLibrary)

FVector2D UToolboxMiscFunctionLibrary::GetAimOffset(const APawn* Pawn)
{
	if (!IsValid(Pawn))
	{
		return FVector2D::ZeroVector;
	}
	
	// Inverse of Actor Transform matrix is an Actor's Direction in Local Space. This is what we want for the Aim offset
	const FVector& ControlRotationDirection = Pawn->GetControlRotation().Vector();
	const FVector& LocalDirection = Pawn->GetTransform().InverseTransformVectorNoScale(ControlRotationDirection);
	const FRotator& LocalRotation = LocalDirection.Rotation();
	return FVector2D(LocalRotation.Pitch, LocalRotation.Yaw);
}

FVector2D UToolboxMiscFunctionLibrary::GetViewportCenter()
{
	if (!GEngine)
	{
		return FVector2D::ZeroVector;
	}
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	return ViewportSize * 0.5;
}

void UToolboxMiscFunctionLibrary::SetViewportViewMode(const EViewModeIndex ViewMode)
{
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewModeIndex != static_cast<int32>(ViewMode))
	{
		ApplyViewMode(ViewMode, false, GEngine->GameViewport->EngineShowFlags);
		GEngine->GameViewport->ViewModeIndex = static_cast<int32>(ViewMode);
	}
}

void UToolboxMiscFunctionLibrary::GetTraceVectorsFromCameraViewPoint(const UObject* WorldContextObject, FVector& TraceStart, FVector& TraceEnd, const double StartOffset, const double TraceDistance)
{
	const APlayerController* PlayerController = ToolboxHelpers::GetLocalPlayerController(WorldContextObject);
	if (!IsValid(PlayerController) || !PlayerController->PlayerCameraManager)
	{
		return;
	}
	
	const FVector& CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector& CameraForwardVector = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();

	TraceStart = CameraLocation + CameraForwardVector * FMath::Max<double>(StartOffset, 1.0);
	TraceEnd = TraceStart + CameraForwardVector * FMath::Max<double>(TraceDistance, 10.0);
}
