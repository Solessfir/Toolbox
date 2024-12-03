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
	UFUNCTION(BlueprintPure, Meta = (Keywords = "Aim Offset"), Category = "Toolbox|Misc Library")
	static FVector2D GetAimOffset(const APawn* Pawn);
	
	UFUNCTION(BlueprintPure, Meta = (Keywords = "Screen Viewport Center"), Category = "Toolbox|Misc Library")
	static FVector2D GetViewportCenter();

	UFUNCTION(BlueprintCallable, Meta = (Keywords = "View Mode"), Category = "Toolbox|Misc Library")
	static void SetViewportViewMode(const EViewModeIndex ViewMode);

	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", Keywords = "Trace Camera View"), Category = "Toolbox|Misc Library")
	static void GetTraceVectorsFromCameraViewPoint(const UObject* WorldContextObject, FVector& TraceStart, FVector& TraceEnd, const double StartOffset, const double TraceDistance = 100.0);
};
