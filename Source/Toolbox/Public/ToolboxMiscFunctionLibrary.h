// Copyright Solessfir. All Rights Reserved.

#pragma once

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
};
