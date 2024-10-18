// Copyright Solessfir. All Rights Reserved.

#include "ToolboxSoftwareFunctionLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ToolboxHelpers.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxSoftwareFunctionLibrary)

bool UToolboxSoftwareFunctionLibrary::IsInEditor()
{
	return GIsEditor;
}

bool UToolboxSoftwareFunctionLibrary::IsShippingBuild()
{
	#if UE_BUILD_SHIPPING || UE_BUILD_TEST
	return true;
	#else
	return false;
	#endif
}

bool UToolboxSoftwareFunctionLibrary::IsPlayInEditor(const UObject* WorldContextObject)
{
	if (const UWorld* World = ToolboxHelpers::GetWorld(WorldContextObject))
	{
		return World->IsPlayInEditor();
	}

	return false;
}

bool UToolboxSoftwareFunctionLibrary::IsInEditorPreviewWindow(const UObject* WorldContextObject)
{
	#if WITH_EDITOR
	if (const UWorld* World = ToolboxHelpers::GetWorld(WorldContextObject))
	{
		return World->WorldType == EWorldType::EditorPreview;
	}
	return false;
	#else
	return false;
	#endif
}

int32 UToolboxSoftwareFunctionLibrary::GetFPS(const UObject* WorldContextObject)
{
	if (const UWorld* World = ToolboxHelpers::GetWorld(WorldContextObject))
	{
		return FMath::FloorToInt(1.f / World->GetDeltaSeconds());
	}
	
	return 0;
}

void UToolboxSoftwareFunctionLibrary::SetFrameRateCap(const int32 FrameRateCap)
{
	if (!GEngine)
	{
		return;
	}
	
	if (FrameRateCap > 0)
	{
		GEngine->bUseFixedFrameRate = true;
		GEngine->FixedFrameRate = FMath::Max<float>(static_cast<float>(FrameRateCap), 15.f);
		return;
	}
	
	GEngine->bUseFixedFrameRate = false;
}

bool UToolboxSoftwareFunctionLibrary::IsApplicationInForeground()
{
	return FPlatformApplicationMisc::IsThisApplicationForeground();
}

void UToolboxSoftwareFunctionLibrary::FlashGameOnTaskBar(const bool bBackgroundOnly)
{
	if (!GEngine)
	{
		return;
	}

	if (!bBackgroundOnly || !IsApplicationInForeground())
	{
		GEngine->GameViewport->GetWindow()->DrawAttention(FWindowDrawAttentionParameters(EWindowDrawAttentionRequestType::UntilActivated));
	}
}
