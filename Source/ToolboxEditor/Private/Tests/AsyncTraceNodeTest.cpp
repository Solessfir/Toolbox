// Copyright Solessfir. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "AsyncActions/AsyncTrace.h"
#include "AsyncTraceCompletionTestReceiver.h"
#include "Components/BoxComponent.h"
#include "Components/LineBatchComponent.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "K2Node_AsyncAction.h"
#include "K2Node_CustomEvent.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/AutomationTest.h"
#include "UObject/UnrealType.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAsyncTraceNodeTest, "Toolbox.AsyncTrace.NodeOutputs", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAsyncTraceNodeTest::RunTest(const FString& Parameters)
{
    UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(AActor::StaticClass(), GetTransientPackage(), MakeUniqueObjectName(GetTransientPackage(), UBlueprint::StaticClass(), TEXT("AsyncTraceNodeTest")), BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass());
    UEdGraph* Graph = FBlueprintEditorUtils::CreateNewGraph(Blueprint, TEXT("EventGraph"), UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
    FBlueprintEditorUtils::AddUbergraphPage(Blueprint, Graph);
    const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

    for (UClass* ProxyClass : { UAsyncTrace_AsyncAction::StaticClass(), UAsyncMultiTrace_AsyncAction::StaticClass(), UAsyncTestTrace_AsyncAction::StaticClass() })
    {
        const bool bSingle = ProxyClass == UAsyncTrace_AsyncAction::StaticClass();
        const bool bMulti = ProxyClass == UAsyncMultiTrace_AsyncAction::StaticClass();
        int32 FactoryCount = 0;
        for (TFieldIterator<UFunction> It(ProxyClass, EFieldIteratorFlags::ExcludeSuper); It; ++It)
        {
            UFunction* Factory = *It;
            if (!Factory->HasMetaData(TEXT("BlueprintInternalUseOnly")))
            {
                continue;
            }
            ++FactoryCount;

            UK2Node_AsyncAction* Node = NewObject<UK2Node_AsyncAction>(Graph);
            CastFieldChecked<FNameProperty>(Node->GetClass()->FindPropertyByName(TEXT("ProxyFactoryFunctionName")))->SetPropertyValue_InContainer(Node, Factory->GetFName());
            CastFieldChecked<FObjectPropertyBase>(Node->GetClass()->FindPropertyByName(TEXT("ProxyFactoryClass")))->SetObjectPropertyValue_InContainer(Node, ProxyClass);
            CastFieldChecked<FObjectPropertyBase>(Node->GetClass()->FindPropertyByName(TEXT("ProxyClass")))->SetObjectPropertyValue_InContainer(Node, ProxyClass);
            Graph->AddNode(Node, false, false);
            Node->CreateNewGuid();
            Node->AllocateDefaultPins();

            const FString Name = Factory->GetName();
            TestNull(Name + TEXT(" has no Trace Mode"), Node->FindPin(TEXT("TraceMode")));
            UEdGraphPin* DebugPin = Node->FindPin(TEXT("DrawDebugType"), EGPD_Input);
            if (TestNotNull(Name + TEXT(" has Draw Debug Type"), DebugPin))
            {
                TestEqual(Name + TEXT(" disables debug by default"), DebugPin->DefaultValue, FString(TEXT("None")));
                TestFalse(Name + TEXT(" exposes debug selector"), DebugPin->bAdvancedView);
                DebugPin->DefaultValue = TEXT("ForDuration");
            }
            for (const TCHAR* PinName : { TEXT("TraceColor"), TEXT("TraceHitColor"), TEXT("DrawTime") })
            {
                UEdGraphPin* DebugOption = Node->FindPin(PinName, EGPD_Input);
                if (TestNotNull(Name + TEXT(" has ") + PinName, DebugOption))
                {
                    TestTrue(Name + TEXT(" keeps debug option advanced ") + PinName, DebugOption->bAdvancedView);
                }
            }
            TestEqual(Name + TEXT(" has only Single hit output"), Node->FindPin(TEXT("OutHit")) != nullptr, bSingle);
            TestEqual(Name + TEXT(" has only Multi hit array"), Node->FindPin(TEXT("OutHits")) != nullptr, bMulti);
            UEdGraphPin* HitPin = Node->FindPin(TEXT("bHit"), EGPD_Output);
            if (TestNotNull(Name + TEXT(" has Hit output"), HitPin))
            {
                TestEqual(Name + TEXT(" Hit is bool"), HitPin->PinType.PinCategory, UEdGraphSchema_K2::PC_Boolean);
            }
            if (bSingle || bMulti)
            {
                UEdGraphPin* ResultPin = Node->FindPin(bSingle ? TEXT("OutHit") : TEXT("OutHits"), EGPD_Output);
                if (TestNotNull(Name + TEXT(" has result output"), ResultPin))
                {
                    TestEqual(Name + TEXT(" result is FHitResult"), ResultPin->PinType.PinSubCategoryObject.Get(), static_cast<UObject*>(FHitResult::StaticStruct()));
                    TestEqual(Name + TEXT(" result has correct container"), ResultPin->PinType.ContainerType, bSingle ? EPinContainerType::None : EPinContainerType::Array);
                }
            }
            TestNotNull(Name + TEXT(" has On Completed"), Node->FindPin(TEXT("OnCompleted"), EGPD_Output));
            TestNull(Name + TEXT(" has no separate failure output"), Node->FindPin(TEXT("OnFailed")));
            UEdGraphPin* ActionPin = Node->FindPin(TEXT("AsyncTaskProxy"), EGPD_Output);
            if (TestNotNull(Name + TEXT(" exposes cancellable action"), ActionPin))
            {
                TestEqual(Name + TEXT(" action uses correct proxy"), ActionPin->PinType.PinSubCategoryObject.Get(), static_cast<UObject*>(ProxyClass));
            }

            UK2Node_CustomEvent* Event = NewObject<UK2Node_CustomEvent>(Graph);
            Event->CustomFunctionName = FName(*FString::Printf(TEXT("Run_%s"), *Name));
            Graph->AddNode(Event, false, false);
            Event->CreateNewGuid();
            Event->AllocateDefaultPins();
            TestTrue(Name + TEXT(" connects to execution"), Schema->TryCreateConnection(Event->FindPinChecked(UEdGraphSchema_K2::PN_Then), Node->FindPinChecked(UEdGraphSchema_K2::PN_Execute)));
        }
        TestEqual(ProxyClass->GetName() + TEXT(" exposes all shape/filter combinations"), FactoryCount, 12);
    }

    FKismetEditorUtilities::CompileBlueprint(Blueprint);
    TestTrue(TEXT("All 36 async trace nodes compile together"), Blueprint->Status != BS_Error);
    return !HasAnyErrors();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAsyncTraceInvalidRequestTest, "Toolbox.AsyncTrace.InvalidRequestCompletes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAsyncTraceInvalidRequestTest::RunTest(const FString& Parameters)
{
    UAsyncTraceCompletionTestReceiver* Receiver = NewObject<UAsyncTraceCompletionTestReceiver>();
    const FVector Start(1.0, 2.0, 3.0);
    const FVector End(4.0, 5.0, 6.0);
    const ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

    UAsyncTrace_AsyncAction* Single = UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(nullptr, Start, End, Channel, false, {});
    Single->OnCompleted.AddDynamic(Receiver, &UAsyncTraceCompletionTestReceiver::ReceiveSingle);
    Single->Activate();
    Single->Activate();
    TestEqual(TEXT("Invalid Single completes exactly once"), Receiver->CompletionCount, 1);
    TestFalse(TEXT("Invalid Single reports no hit"), Receiver->bHit);
    TestFalse(TEXT("Invalid Single has no blocking hit"), Receiver->OutHit.bBlockingHit);
    TestTrue(TEXT("Invalid Single returns an empty result"), Receiver->OutHit.TraceStart.IsZero() && Receiver->OutHit.TraceEnd.IsZero());

    UAsyncMultiTrace_AsyncAction* Multi = UAsyncMultiTrace_AsyncAction::AsyncLineTraceMultiByChannel(nullptr, Start, End, Channel, false, {});
    Multi->OnCompleted.AddDynamic(Receiver, &UAsyncTraceCompletionTestReceiver::ReceiveMulti);
    Multi->Activate();
    TestEqual(TEXT("Invalid Multi completes"), Receiver->CompletionCount, 2);
    TestFalse(TEXT("Invalid Multi reports no hit"), Receiver->bHit);
    TestTrue(TEXT("Invalid Multi returns an empty array"), Receiver->OutHits.IsEmpty());

    UAsyncTestTrace_AsyncAction* Test = UAsyncTestTrace_AsyncAction::AsyncLineTraceTestByChannel(nullptr, Start, End, Channel, false, {});
    Test->OnCompleted.AddDynamic(Receiver, &UAsyncTraceCompletionTestReceiver::ReceiveTest);
    Test->Activate();
    TestEqual(TEXT("Invalid Test completes"), Receiver->CompletionCount, 3);
    TestFalse(TEXT("Invalid Test reports no hit"), Receiver->bHit);

    UAsyncTrace_AsyncAction* Canceled = UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(nullptr, Start, End, Channel, false, {});
    Canceled->OnCompleted.AddDynamic(Receiver, &UAsyncTraceCompletionTestReceiver::ReceiveSingle);
    Canceled->Cancel();
    Canceled->Activate();
    TestEqual(TEXT("Canceled requests do not complete"), Receiver->CompletionCount, 3);
    return !HasAnyErrors();
}

#if ENABLE_DRAW_DEBUG

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAsyncTraceDebugTest, "Toolbox.AsyncTrace.DebugPreview", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAsyncTraceDebugTest::RunTest(const FString& Parameters)
{
    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    World->SetGameInstance(NewObject<UGameInstance>(World));
    FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
    WorldContext.SetCurrentWorld(World);
    WorldContext.OwningGameInstance = World->GetGameInstance();
    AActor* Blocker = World->SpawnActor<AActor>();
    UBoxComponent* Box = NewObject<UBoxComponent>(Blocker);
    Blocker->SetRootComponent(Box);
    Box->SetBoxExtent(FVector(50.0));
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionResponseToAllChannels(ECR_Block);
    Box->RegisterComponent();
    Blocker->SetActorLocation(FVector(500.0, 0.0, 0.0));
    World->UpdateWorldComponents(false, false);
    ULineBatchComponent* FrameBatcher = World->GetLineBatcher(UWorld::ELineBatcherType::World);
    ULineBatchComponent* DurationBatcher = World->GetLineBatcher(UWorld::ELineBatcherType::WorldPersistent);

    const FVector Start = FVector::ZeroVector;
    const FVector End(1000.0, 0.0, 0.0);
    const ETraceTypeQuery Channel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
    const FLinearColor TraceColor = FLinearColor::Blue;
    const FLinearColor HitColor = FLinearColor::Yellow;
    const auto Complete = [World](UAsyncTraceBase_AsyncAction* Action)
    {
        Action->Activate();
        for (int32 Frame = 0; Frame < 4 && Action->IsActive(); ++Frame)
        {
            World->Tick(LEVELTICK_All, 1.f / 60.f);
        }
    };
    const auto ClearPreview = [FrameBatcher, DurationBatcher]()
    {
        FrameBatcher->Flush();
        DurationBatcher->Flush();
    };

    TArray<UAsyncTraceBase_AsyncAction*> Actions = {
        UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(World, Start, End, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncTrace_AsyncAction::AsyncSphereTraceByChannel(World, Start, End, 10.f, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncTrace_AsyncAction::AsyncCapsuleTraceByChannel(World, Start, End, 10.f, 20.f, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncTrace_AsyncAction::AsyncBoxTraceByChannel(World, Start, End, FVector(10.0), FRotator(0.0, 45.0, 0.0), Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncMultiTrace_AsyncAction::AsyncLineTraceMultiByChannel(World, Start, End, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncMultiTrace_AsyncAction::AsyncSphereTraceMultiByChannel(World, Start, End, 10.f, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncMultiTrace_AsyncAction::AsyncCapsuleTraceMultiByChannel(World, Start, End, 10.f, 20.f, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f),
        UAsyncMultiTrace_AsyncAction::AsyncBoxTraceMultiByChannel(World, Start, End, FVector(10.0), FRotator(0.0, 45.0, 0.0), Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f)
    };
    for (int32 Index = 0; Index < Actions.Num(); ++Index)
    {
        ClearPreview();
        Complete(Actions[Index]);
        const FString Name = FString::Printf(TEXT("Preview %d"), Index);
        TestFalse(Name + TEXT(" completes asynchronously"), Actions[Index]->IsActive());
        const TArray<FBatchedLine>& Lines = DurationBatcher->BatchedLines;
        TestTrue(Name + TEXT(" draws trace color"), Lines.ContainsByPredicate([TraceColor](const FBatchedLine& Line) { return Line.Color == FLinearColor(TraceColor.ToFColor(true)); }));
        TestTrue(Name + TEXT(" draws hit color"), Lines.ContainsByPredicate([HitColor](const FBatchedLine& Line) { return Line.Color == FLinearColor(HitColor.ToFColor(true)); }));
        TestTrue(Name + TEXT(" honors duration"), Lines.ContainsByPredicate([](const FBatchedLine& Line) { return Line.RemainingLifeTime > 6.f && Line.RemainingLifeTime <= 7.f; }));
        TestTrue(Name + TEXT(" marks actual impact"), DurationBatcher->BatchedPoints.ContainsByPredicate([](const FBatchedPoint& Point) { return Point.Position.X > 400.0 && Point.Position.X < 600.0; }));
    }

    ClearPreview();
    Complete(UAsyncTestTrace_AsyncAction::AsyncLineTraceTestByChannel(World, Start, End, Channel, false, {}, EDrawDebugTrace::ForDuration, true, TraceColor, HitColor, 7.f));
    TestEqual(TEXT("Test preview draws a whole hit-colored line"), DurationBatcher->BatchedLines.Num(), 1);
    TestTrue(TEXT("Test preview uses hit color"), DurationBatcher->BatchedLines.ContainsByPredicate([HitColor](const FBatchedLine& Line) { return Line.Color == FLinearColor(HitColor.ToFColor(true)); }));
    TestTrue(TEXT("Test preview has no synthetic impact point"), DurationBatcher->BatchedPoints.IsEmpty());

    ClearPreview();
    Complete(UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(World, Start, End, Channel, false, {}, EDrawDebugTrace::None));
    TestTrue(TEXT("None draws no preview"), DurationBatcher->BatchedLines.IsEmpty() && FrameBatcher->BatchedLines.IsEmpty());

    Complete(UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(World, Start, End, Channel, false, {}, EDrawDebugTrace::ForOneFrame));
    TestFalse(TEXT("For One Frame uses the frame batch"), FrameBatcher->BatchedLines.IsEmpty());
    TestTrue(TEXT("For One Frame has no duration batch"), DurationBatcher->BatchedLines.IsEmpty());

    ClearPreview();
    Complete(UAsyncTrace_AsyncAction::AsyncLineTraceByChannel(World, Start, End, Channel, false, {}, EDrawDebugTrace::Persistent));
    TestTrue(TEXT("Persistent keeps preview indefinitely"), DurationBatcher->BatchedLines.ContainsByPredicate([](const FBatchedLine& Line) { return Line.RemainingLifeTime < 0.f; }));
    World->DestroyWorld(false);
    GEngine->DestroyWorldContext(World);
    return !HasAnyErrors();
}

#endif

#endif
