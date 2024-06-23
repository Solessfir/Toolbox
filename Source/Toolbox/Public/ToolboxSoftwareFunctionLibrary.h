// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolboxSoftwareFunctionLibrary.generated.h"

UCLASS()
class TOOLBOX_API UToolboxSoftwareFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Returns true if this is the Editor
	* Note that this is still true when using Play In Editor. You may want to use IsPlayInEditor in that case
	*/
	UFUNCTION(BlueprintPure, Meta = (CompactNodeTitle = "Is Editor"), Category = "Toolbox|Software Library")
	static bool IsInEditor();

	/**
	* Returns true if this is a Shipping build if the game
	*/
	UFUNCTION(BlueprintPure, Meta = (CompactNodeTitle = "Is Shipping"), Category = "Toolbox|Software Library")
	static bool IsShippingBuild();

	/**
	* Returns true if a Game is being played in Editor
	*/
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", CompactNodeTitle = "Is PIE"), Category = "Toolbox|Software Library")
	static bool IsPlayInEditor(const UObject* WorldContextObject);
	
	/**
	* Returns true if this is a Preview World for an Editor tool. Ex. when previewing Mesh or Animation
	*/
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", CompactNodeTitle = "Is Editor Preview"), Category = "Toolbox|Software Library")
	static bool IsInEditorPreviewWindow(const UObject* WorldContextObject);

	/**
	* Returns current Game FPS
	*/
	UFUNCTION(BlueprintPure, Meta = (WorldContext = "WorldContextObject", CompactNodeTitle = "FPS"), Category = "Toolbox|Software Library")
	static int32 GetFPS(const UObject* WorldContextObject);

	/**
	* Set desired FPS cap. Min 30 FPS
	* @param FrameRateCap Set to 0 for uncapped FPS
	*/
	UFUNCTION(BlueprintCallable, Category = "Toolbox|Software Library")
	static void SetFrameRateCap(const int32 FrameRateCap);
	
	UFUNCTION(BlueprintPure, Meta = (Keywords = "GPU Info Video"), Category = "Toolbox|Software Library")
	static bool IsApplicationInForeground();
	
	/**
	* Flashes the game on the Task Bar
	* @param bBackgroundOnly Flash the Game only if it is not focused/minimized
	*/
	UFUNCTION(BlueprintCallable, Meta = (Keywords = "Flash Attention Task Bar"), Category = "Toolbox|Software Library")
	static void FlashGameOnTaskBar(const bool bBackgroundOnly);
};
