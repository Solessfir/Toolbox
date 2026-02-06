// Copyright Solessfir. All Rights Reserved.

#include "ToolboxSoftwareFunctionLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ToolboxHelpers.h"

#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Blueprint/BlueprintExceptionInfo.h"
#include "Kismet2/KismetDebugUtilities.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/UObjectToken.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ToolboxSoftwareFunctionLibrary)

DEFINE_LOG_CATEGORY_STATIC(LogBlueprintException, Log, All);

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

void UToolboxSoftwareFunctionLibrary::GetThreadsTime(float& FrameTime, float& GameThreadTime, float& RenderThreadTime, float& RHIThreadTime, float& GPUFrameTime)
{
	extern uint32 GGameThreadTime;
	extern uint32 GRenderThreadTime;
	extern uint32 GRHIThreadTime;

	FrameTime = static_cast<float>((FApp::GetCurrentTime() - FApp::GetLastTime()) * 1000.0);
	GameThreadTime = FPlatformTime::ToMilliseconds(GGameThreadTime);
	RenderThreadTime = FPlatformTime::ToMilliseconds(GRenderThreadTime);
	RHIThreadTime = FPlatformTime::ToMilliseconds(GRHIThreadTime);
	GPUFrameTime = FPlatformTime::ToMilliseconds(RHIGetGPUFrameCycles());
}

bool UToolboxSoftwareFunctionLibrary::IsGameInForeground()
{
	return FPlatformApplicationMisc::IsThisApplicationForeground();
}

void UToolboxSoftwareFunctionLibrary::FlashGameOnTaskBar(const bool bBackgroundOnly)
{
	if (!GEngine)
	{
		return;
	}

	if (!bBackgroundOnly || !IsGameInForeground())
	{
		GEngine->GameViewport->GetWindow()->DrawAttention(FWindowDrawAttentionParameters(EWindowDrawAttentionRequestType::UntilActivated));
	}
}

void UToolboxSoftwareFunctionLibrary::SetWindowMode(const EWindowModeType Mode)
{
	if (!GEngine || Mode == EWindowModeType::Invalid)
	{
		return;
	}

	UGameViewportClient* ViewportClient = GEngine->GameViewport;
	if (!ViewportClient)
	{
		return;
	}

	if (SWindow* Window = ViewportClient->GetWindow().Get())
	{
		Window->SetWindowMode(static_cast<EWindowMode::Type>(Mode));
	}
}

EWindowModeType UToolboxSoftwareFunctionLibrary::GetWindowMode()
{
	if (!GEngine)
	{
		return EWindowModeType::Invalid;
	}

	UGameViewportClient* ViewportClient = GEngine->GameViewport;
	if (!ViewportClient)
	{
		return EWindowModeType::Invalid;
	}

	if (const SWindow* Window = ViewportClient->GetWindow().Get())
	{
		return static_cast<EWindowModeType>(Window->GetWindowMode());
	}

	return EWindowModeType::Invalid;
}

#if WITH_EDITOR
static const FBlueprintDebugData* GetDebugDataFor(const UBlueprintGeneratedClass* BlueprintClass)
{
	if (BlueprintClass->DebugData.IsValid())
	{
		return &BlueprintClass->DebugData;
	}

	const UClass* SuperClass = BlueprintClass->GetSuperClass();
	const UBlueprint* SuperBlueprint = SuperClass ? Cast<UBlueprint>(SuperClass->ClassGeneratedBy) : nullptr;
	const UBlueprintGeneratedClass* SuperGeneratedClass = SuperBlueprint ? Cast<UBlueprintGeneratedClass>(SuperBlueprint->GeneratedClass.Get()) : nullptr;
	return SuperGeneratedClass ? GetDebugDataFor(SuperGeneratedClass) : nullptr;
}
#endif

void UToolboxSoftwareFunctionLibrary::PrintMessageLog(const UObject* WorldContextObject, const ENotificationSeverityType Severity, const FName LogCategory, const FString Message)
{
	#if WITH_EDITOR
	if (Message.IsEmpty())
	{
		return;
	}

	const UClass* ObjectClass = WorldContextObject->GetClass();
	const UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(ObjectClass);
	const FBlueprintDebugData* DebugData = GeneratedClass ? GetDebugDataFor(GeneratedClass) : nullptr;
	const TArrayView<FFrame* const>& ScriptStack = FBlueprintContextTracker::Get().GetCurrentScriptStackWritable();
	if (!GeneratedClass || !DebugData || ScriptStack.Num() == 0 || !ScriptStack.Last())
	{
		return;
	}

	const auto Highlighter = FOnMessageTokenActivated::CreateLambda([](const TSharedRef<IMessageToken>& Token)
	{
		if (Token->GetType() == EMessageToken::Object)
		{
			const TSharedRef<FUObjectToken> ObjectToken = StaticCastSharedRef<FUObjectToken>(Token);
			if (UObject* ObjectPtr = ObjectToken->GetObject().Get())
			{
				if (const UBlueprint* Blueprint = Cast<UBlueprint>(ObjectPtr))
				{
					GEditor->SyncBrowserToObject(Blueprint);
					return;
				}

				FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(ObjectPtr);
			}
		}
	});

	const FFrame* StackFrame = ScriptStack.Last();
	const int32 BreakpointOffset = StackFrame->Code - StackFrame->Node->Script.GetData() - 1;
	const UEdGraphNode* BlueprintNode = DebugData->FindSourceNodeFromCodeLocation(StackFrame->Node, BreakpointOffset, true);
	const UEdGraph* Graph = BlueprintNode ? BlueprintNode->GetGraph() : nullptr;
	const UBlueprint* BlueprintAsset = UBlueprint::GetBlueprintFromClass(GeneratedClass);

	FMessageLog Log(LogCategory);
	const TSharedRef<FTokenizedMessage> LogMessage = Log.Message(Severity == ENotificationSeverityType::Error ? EMessageSeverity::Error : Severity == ENotificationSeverityType::Warning ? EMessageSeverity::Warning : EMessageSeverity::Info);

	const auto NodeToken = FUObjectToken::Create(BlueprintNode, BlueprintNode ? BlueprintNode->GetNodeTitle(ENodeTitleType::ListView) : INVTEXT("Unknown"));
	const auto GraphToken = FUObjectToken::Create(Graph, FText::FromString(GetNameSafe(Graph)));
	const auto BlueprintToken = FUObjectToken::Create(BlueprintAsset, FText::FromString(BlueprintAsset->GetName()));

	NodeToken->OnMessageTokenActivated(Highlighter);
	GraphToken->OnMessageTokenActivated(Highlighter);
	BlueprintToken->OnMessageTokenActivated(Highlighter);

	LogMessage->AddToken(FTextToken::Create(FText::FromString(Message)))
		->AddToken(FTextToken::Create(FText::GetEmpty()))->AddToken(NodeToken)
		->AddToken(FTextToken::Create(INVTEXT("in ")))->AddToken(BlueprintToken);
	#endif
}

void UToolboxSoftwareFunctionLibrary::ShowNotification(const UObject* WorldContextObject, const ENotificationSeverityType Severity, const float Duration, const float FadeInDuration, const float FadeOutDuration, const float Width, const FString Title, const FString Message)
{
	#if WITH_EDITOR
    if (!WorldContextObject || Duration <= 0.f)
    {
        return;
    }

    const UClass* ObjectClass = WorldContextObject->GetClass();
    const UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(ObjectClass);
    const FBlueprintDebugData* DebugData = GeneratedClass ? GetDebugDataFor(GeneratedClass) : nullptr;
    const TArrayView<FFrame* const>& ScriptStack = FBlueprintContextTracker::Get().GetCurrentScriptStackWritable();
    if (!GeneratedClass || !DebugData || ScriptStack.Num() == 0 || !ScriptStack.Last())
    {
        return;
    }

    const FFrame* StackFrame = ScriptStack.Last();
    const int32 BreakpointOffset = StackFrame->Code - StackFrame->Node->Script.GetData() - 1;
    const UEdGraphNode* BlueprintNode = DebugData->FindSourceNodeFromCodeLocation(StackFrame->Node, BreakpointOffset, true);
    UEdGraph* Graph = BlueprintNode ? BlueprintNode->GetGraph() : nullptr;

	PrintMessageLog(WorldContextObject, Severity, FName("PIE"), Message.IsEmpty() ? Title : Message);

    // Notification popup
    FNotificationInfo Info(FText::FromString(Title));
    Info.SubText = FText::FromString(Message);
    Info.ExpireDuration = Duration;
    Info.FadeInDuration = FMath::Max(0.f, FadeInDuration);
    Info.FadeOutDuration = FMath::Max(0.f, FadeOutDuration);
    Info.WidthOverride = FMath::Max(50.f, Width);
    Info.bFireAndForget = true;

    switch (Severity)
    {
        case ENotificationSeverityType::Warning:
            Info.Image = FAppStyle::Get().GetBrush(TEXT("Icons.WarningWithColor.Large"));
            break;
        case ENotificationSeverityType::Success:
            Info.Image = FAppStyle::Get().GetBrush(TEXT("Icons.SuccessWithColor.Large"));
            break;
        case ENotificationSeverityType::Error:
            Info.Image = FAppStyle::Get().GetBrush(TEXT("Icons.ErrorWithColor.Large"));
            break;
        default:
            Info.Image = FAppStyle::Get().GetBrush(TEXT("Icons.InfoWithColor.Large"));
            break;
    }

    // Hyperlink to focus Node, Graph, or fallback to Blueprint asset
    if (BlueprintNode)
    {
        Info.Hyperlink = FSimpleDelegate::CreateLambda([WeakNode = TWeakObjectPtr<const UObject>(BlueprintNode)]()
        {
            if (WeakNode.IsValid())
            {
                FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(WeakNode.Get());
            }
        });
        Info.HyperlinkText = FText::FromString(TEXT("Focus Node"));
    }
    else if (Graph)
    {
        Info.Hyperlink = FSimpleDelegate::CreateLambda([WeakGraph = TWeakObjectPtr<const UObject>(Graph)]()
        {
            if (WeakGraph.IsValid())
            {
                FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(WeakGraph.Get());
            }
        });
        Info.HyperlinkText = FText::FromString(TEXT("Focus Graph"));
    }
    else if (GeneratedClass)
    {
	    if (const UObject* BlueprintAsset = GeneratedClass->ClassGeneratedBy)
        {
            Info.Hyperlink = FSimpleDelegate::CreateLambda([WeakBlueprint = TWeakObjectPtr<const UObject>(BlueprintAsset)]()
            {
                if (WeakBlueprint.IsValid())
                {
                    FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(WeakBlueprint.Get());
                }
            });
            Info.HyperlinkText = FText::FromString(TEXT("Focus Blueprint"));
        }
    }

    FSlateNotificationManager::Get().AddNotification(Info);
	#endif
}

void UToolboxSoftwareFunctionLibrary::BlueprintException(const UObject* WorldContextObject, const FString& Message)
{
	#if WITH_EDITOR
	if (!WorldContextObject)
	{
		return;
	}

	ShowNotification(WorldContextObject, ENotificationSeverityType::Error, 2.f, 0.5f, 2.f, 320.f, TEXT("Blueprint Exception"), Message);

	// Throw in Editor BP exception if possible
	const TArrayView<FFrame* const>& ScriptStack = FBlueprintContextTracker::Get().GetCurrentScriptStackWritable();
	if (ScriptStack.Num() > 0 && ScriptStack.Last())
	{
		FFrame* StackFrame = ScriptStack.Last();
		const UClass* ClassContainingCode = FKismetDebugUtilities::FindClassForNode(WorldContextObject, StackFrame->Node);
		if (UBlueprint* Blueprint = ClassContainingCode ? Cast<UBlueprint>(ClassContainingCode->ClassGeneratedBy) : nullptr)
		{
			Blueprint->SetObjectBeingDebugged(const_cast<UObject*>(WorldContextObject));
			if (!GIsAutomationTesting)
			{
				// FText::GetEmpty because we handle Notification ourselves in ShowNotification. Otherwise, we'll get two notifications
				const FBlueprintExceptionInfo ExceptionInfo(EBlueprintExceptionType::Breakpoint, FText::GetEmpty());
				FBlueprintCoreDelegates::ThrowScriptException(WorldContextObject, *StackFrame, ExceptionInfo);
			}
		}

		UE_LOG(LogBlueprintException, Error, TEXT("%s"), *StackFrame->GetStackTrace());
	}
	#else
	UE_LOG(LogBlueprintException, Error, TEXT("%s"), *Message);
	ensureAlwaysMsgf(false, TEXT("%s"), *Message);
	#endif
}
