// Copyright Solessfir. All Rights Reserved.

#include "ToolboxMiscFunctionLibrary.h"
#include "ToolboxHelpers.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
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
	if (!GEngine || !GEngine->GameViewport)
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
	TraceStart = FVector::ZeroVector;
	TraceEnd = FVector::ZeroVector;

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

bool UToolboxMiscFunctionLibrary::GetActorScreenBounds(const UObject* WorldContextObject, const AActor* Actor, FVector2D& ScreenMin, FVector2D& ScreenMax)
{
	ScreenMin = FVector2D::ZeroVector;
	ScreenMax = FVector2D::ZeroVector;

	if (!IsValid(Actor))
	{
		return false;
	}

	const APlayerController* PlayerController = ToolboxHelpers::GetLocalPlayerController(WorldContextObject);
	if (!IsValid(PlayerController))
	{
		return false;
	}

	FVector Origin;
	FVector Extent;
	Actor->GetActorBounds(false, Origin, Extent);

	const FVector Corners[8] =
	{
		Origin + FVector( Extent.X,  Extent.Y,  Extent.Z),
		Origin + FVector( Extent.X,  Extent.Y, -Extent.Z),
		Origin + FVector( Extent.X, -Extent.Y,  Extent.Z),
		Origin + FVector( Extent.X, -Extent.Y, -Extent.Z),
		Origin + FVector(-Extent.X,  Extent.Y,  Extent.Z),
		Origin + FVector(-Extent.X,  Extent.Y, -Extent.Z),
		Origin + FVector(-Extent.X, -Extent.Y,  Extent.Z),
		Origin + FVector(-Extent.X, -Extent.Y, -Extent.Z),
	};

	ScreenMin = FVector2D(MAX_FLT);
	ScreenMax = FVector2D(-MAX_FLT);

	bool bAnyCornerProjected = false;
	for (const FVector& Corner : Corners)
	{
		FVector2D ScreenPos;
		if (PlayerController->ProjectWorldLocationToScreen(Corner, ScreenPos))
		{
			ScreenMin.X = FMath::Min(ScreenMin.X, ScreenPos.X);
			ScreenMin.Y = FMath::Min(ScreenMin.Y, ScreenPos.Y);
			ScreenMax.X = FMath::Max(ScreenMax.X, ScreenPos.X);
			ScreenMax.Y = FMath::Max(ScreenMax.Y, ScreenPos.Y);
			bAnyCornerProjected = true;
		}
	}

	int32 ViewportWidth = 0;
	int32 ViewportHeight = 0;
	PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

	const bool bBoundsIntersectViewport = bAnyCornerProjected
		&& ViewportWidth > 0
		&& ViewportHeight > 0
		&& ScreenMax.X >= 0.f
		&& ScreenMax.Y >= 0.f
		&& ScreenMin.X <= static_cast<double>(ViewportWidth)
		&& ScreenMin.Y <= static_cast<double>(ViewportHeight);

	if (!bBoundsIntersectViewport)
	{
		ScreenMin = FVector2D::ZeroVector;
		ScreenMax = FVector2D::ZeroVector;
	}

	return bBoundsIntersectViewport;
}

void UToolboxMiscFunctionLibrary::ShowMouseCursor(const UObject* WorldContextObject, const bool bShowMouseCursor, const bool bGameCapturesMouse)
{
	APlayerController* PlayerController = ToolboxHelpers::GetLocalPlayerController(WorldContextObject);
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->bShowMouseCursor = bShowMouseCursor;
	UGameViewportClient* GameViewport = PlayerController->GetWorld() ? PlayerController->GetWorld()->GetGameViewport() : nullptr;

	if (bShowMouseCursor)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);

		if (GameViewport)
		{
			GameViewport->SetMouseCaptureMode(bGameCapturesMouse ? EMouseCaptureMode::CaptureDuringMouseDown : EMouseCaptureMode::NoCapture);
		}

		return;
	}

	const FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);

	if (GameViewport)
	{
		GameViewport->SetMouseCaptureMode(bGameCapturesMouse ? EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown : EMouseCaptureMode::NoCapture);
	}
}

void UToolboxMiscFunctionLibrary::CalculateOrbitalTransform(const FVector& OrbitalCenter, const float OrbitRadius, const float ElevationAngle, const float AzimuthAngle, FVector& Location, FRotator& Rotation)
{
	const float ElevationRad = FMath::DegreesToRadians(ElevationAngle);
	const float AzimuthRad = FMath::DegreesToRadians(AzimuthAngle);

	const float ClampedRadius = FMath::Max(OrbitRadius, 1.f);
	const FVector RelativeLocation =
	{
		// Convert spherical coordinates (distance + angles) to Cartesian coordinates
		ClampedRadius * FMath::Cos(ElevationRad) * FMath::Cos(AzimuthRad),
		ClampedRadius * FMath::Cos(ElevationRad) * FMath::Sin(AzimuthRad),
		ClampedRadius * FMath::Sin(ElevationRad)
	};

	Location = OrbitalCenter + RelativeLocation;
	Rotation = (-RelativeLocation.GetSafeNormal()).Rotation();
}
