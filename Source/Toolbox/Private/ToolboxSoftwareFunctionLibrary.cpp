// Copyright Solessfir. All Rights Reserved.

#include "ToolboxSoftwareFunctionLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
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
	if (!WorldContextObject)
	{
		return false;
	}
	
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return false;
	}

	return World->IsPlayInEditor();
}

bool UToolboxSoftwareFunctionLibrary::IsInEditorPreviewWindow(const UObject* WorldContextObject)
{
	#if WITH_EDITOR
	if (!WorldContextObject)
	{
		return false;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return false;
	}
	
	return World->WorldType == EWorldType::EditorPreview;
	#else
	return false;
	#endif
}

int32 UToolboxSoftwareFunctionLibrary::GetFPS(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return 0.f;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return 0.f;
	}
	
	return static_cast<int32>(FMath::RoundHalfToEven(1.f / World->GetDeltaSeconds()));
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
