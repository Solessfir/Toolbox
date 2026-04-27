// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Engine/EngineBaseTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolboxMiscFunctionLibrary.generated.h"

UCLASS()
class TOOLBOX_API UToolboxMiscFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Retrieves the Aim Offsets Pitch and Yaw based on the Rotation of the Controller
	* Pitch and Yaw are meant to be used with a Blend Space going from -90, -90 to 90, 90
	* @param Pawn Pawn to get Control Rotation from
	* @return X = Pitch, Y = Yaw
	*/
	UFUNCTION(BlueprintPure, Category = "Toolbox|Misc Library")
	static FVector2D GetAimOffset(const APawn* Pawn);

	UFUNCTION(BlueprintPure, Meta = (Keywords = "Screen"), Category = "Toolbox|Misc Library")
	static FVector2D GetViewportCenter();

	UFUNCTION(BlueprintCallable, Category = "Toolbox|Misc Library")
	static void SetViewportViewMode(const EViewModeIndex ViewMode);

	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject"), Category = "Toolbox|Misc Library")
	static void GetTraceVectorsFromCameraViewPoint(const UObject* WorldContextObject, FVector& TraceStart, FVector& TraceEnd, const double StartOffset, const double TraceDistance = 100.0);

	/**
	* Projects an Actor's world bounding box to screen space
	* @param Actor Actor to project
	* @param ScreenMin Top-left screen coordinate of the bounding box
	* @param ScreenMax Bottom-right screen coordinate of the bounding box
	* @return True if at least one corner of the bounding box is visible on screen
	*/
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", Keywords = "Screen Bounds UI HUD"), Category = "Toolbox|Misc Library")
	static bool GetActorScreenBounds(const UObject* WorldContextObject, const AActor* Actor, FVector2D& ScreenMin, FVector2D& ScreenMax);

	/**
	* Calculates World Location and Rotation for an object orbiting a central point
	* @param OrbitalCenter World location to orbit around
	* @param OrbitRadius Distance from center point (minimum 1 unit)
	* @param ElevationAngle Vertical angle from horizon in degrees (+90 = above, -90 = below)
	* @param AzimuthAngle Horizontal angle in degrees (0 = forward, 90 = right)
	* @param Location Calculated World Location of orbiting object
	* @param Rotation Calculated World Rotation facing orbital center
	*/
	UFUNCTION(BlueprintPure, Meta = (AutoCreateRefTerm = "OrbitalCenter", Keywords = "Rotate Around"), Category = "Toolbox|Misc Library")
	static void CalculateOrbitalTransform(const FVector& OrbitalCenter, const float OrbitRadius, const float ElevationAngle, const float AzimuthAngle, FVector& Location, FRotator& Rotation);

	/**
	* Shows or hides the mouse cursor and adjusts input mode accordingly
	*
	* Common usage patterns:
	* - FPS / pure game input:          bShowMouseCursor = false, bGameCapturesMouse = true  - no cursor, engine captures all clicks including first click after alt-tab
	* - Diablo-style cursor + UI input: bShowMouseCursor = true,  bGameCapturesMouse = false - cursor visible, OS handles clicks normally, UI widgets react
	* - Cursor visible, no UI input:    bShowMouseCursor = true,  bGameCapturesMouse = true  - cursor visible but engine steals clicks before UI gets them
	*
	* @param bShowMouseCursor Whether to show or hide the mouse cursor
	* @param bGameCapturesMouse Whether window grab OS mouse on click
	*/
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject"), Category = "Toolbox|Misc Library")
	static void ShowMouseCursor(const UObject* WorldContextObject, const bool bShowMouseCursor, const bool bGameCapturesMouse = true);
};
