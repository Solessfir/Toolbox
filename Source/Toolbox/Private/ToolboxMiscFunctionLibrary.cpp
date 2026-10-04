// Copyright Solessfir. All Rights Reserved.

#include "ToolboxMiscFunctionLibrary.h"
#include "ToolboxHelpers.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "SceneView.h"
#include "Slate/SceneViewport.h"
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

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	FSceneViewProjectionData ProjectionData;
	if (!LocalPlayer || !LocalPlayer->ViewportClient
		|| !LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, ProjectionData)
		|| !ProjectionData.IsValidViewRectangle())
	{
		return false;
	}

	const FIntRect& ViewRect = ProjectionData.GetConstrainedViewRect();
	const FMatrix ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();

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

	bool bAnyPointProjected = false;
	const auto AddProjectedPoint = [&](const FVector& WorldPoint)
	{
		FVector2D ScreenPos;
		if (FSceneView::ProjectWorldToScreen(WorldPoint, ViewRect, ViewProjectionMatrix, ScreenPos)
			&& PlayerController->PostProcessWorldToScreen(WorldPoint, ScreenPos, false))
		{
			ScreenMin.X = FMath::Min(ScreenMin.X, ScreenPos.X);
			ScreenMin.Y = FMath::Min(ScreenMin.Y, ScreenPos.Y);
			ScreenMax.X = FMath::Max(ScreenMax.X, ScreenPos.X);
			ScreenMax.Y = FMath::Max(ScreenMax.Y, ScreenPos.Y);
			bAnyPointProjected = true;
		}
	};

	double NearPlaneDistances[8];
	for (int32 CornerIndex = 0; CornerIndex < 8; ++CornerIndex)
	{
		const FVector4 ClipPosition = ViewProjectionMatrix.TransformFVector4(FVector4(Corners[CornerIndex], 1.0));
		// Reversed Z places the near plane at Z = W for perspective and orthographic projections.
		NearPlaneDistances[CornerIndex] = ClipPosition.W - ClipPosition.Z;
		if (NearPlaneDistances[CornerIndex] >= 0.0)
		{
			AddProjectedPoint(Corners[CornerIndex]);
		}
	}

	constexpr int32 Edges[12][2] =
	{
		{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
		{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}
	};
	for (const auto& Edge : Edges)
	{
		const double StartDistance = NearPlaneDistances[Edge[0]];
		const double EndDistance = NearPlaneDistances[Edge[1]];
		if ((StartDistance >= 0.0) != (EndDistance >= 0.0))
		{
			const double Alpha = StartDistance / (StartDistance - EndDistance);
			AddProjectedPoint(FMath::Lerp(Corners[Edge[0]], Corners[Edge[1]], Alpha));
		}
	}

	const bool bBoundsIntersectViewport = bAnyPointProjected
		&& ScreenMax.X >= static_cast<double>(ViewRect.Min.X)
		&& ScreenMax.Y >= static_cast<double>(ViewRect.Min.Y)
		&& ScreenMin.X <= static_cast<double>(ViewRect.Max.X)
		&& ScreenMin.Y <= static_cast<double>(ViewRect.Max.Y);

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
