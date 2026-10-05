// Copyright Solessfir. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "AsyncActions/LerpToAttachmentSocket.h"
#include "Components/LineBatchComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "UObject/UnrealType.h"

#if ENABLE_DRAW_DEBUG

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLerpToAttachmentSocketDebugTest, "Toolbox.LerpToAttachmentSocket.DebugPreview", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FLerpToAttachmentSocketDebugTest::RunTest(const FString& Parameters)
{
    UFunction* Factory = ULerpToAttachmentSocket_AsyncAction::StaticClass()->FindFunctionByName(TEXT("LerpToAttachmentSocket"));
    if (TestNotNull(TEXT("Lerp factory is reflected"), Factory))
    {
        TestNotNull(TEXT("Draw Debug is a bool input"), FindFProperty<FBoolProperty>(Factory, TEXT("bDrawDebug")));
    }

    UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
    World->SetGameInstance(NewObject<UGameInstance>(World));
    FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
    WorldContext.SetCurrentWorld(World);
    WorldContext.OwningGameInstance = World->GetGameInstance();
    AActor* Actor = World->SpawnActor<AActor>();
    USceneComponent* Parent = NewObject<USceneComponent>(Actor);
    Actor->SetRootComponent(Parent);
    Parent->RegisterComponent();
    Parent->SetWorldLocation(FVector(100.0, 200.0, 300.0));
    USceneComponent* Child = NewObject<USceneComponent>(Actor);
    Child->SetupAttachment(Parent);
    Child->RegisterComponent();
    World->UpdateWorldComponents(false, false);
    ULineBatchComponent* FrameBatcher = World->GetLineBatcher(UWorld::ELineBatcherType::World);
    ULineBatchComponent* DurationBatcher = World->GetLineBatcher(UWorld::ELineBatcherType::WorldPersistent);
    const FTransform Target(FVector(80.0, 0.0, 0.0));

    ULerpToAttachmentSocket_AsyncAction* Disabled = ULerpToAttachmentSocket_AsyncAction::LerpToAttachmentSocket(Child, NAME_None, Target, 1.f);
    Disabled->Activate();
    Disabled->Tick(0.5f);
    TestTrue(TEXT("Debug is disabled by default"), FrameBatcher->BatchedLines.IsEmpty() && DurationBatcher->BatchedLines.IsEmpty());
    Disabled->Tick(0.5f);

    Child->SetRelativeTransform(FTransform::Identity);
    const float Radius = 17.f;
    const float DrawDuration = 7.f;
    const FLinearColor Color = FLinearColor::Green;
    ULerpToAttachmentSocket_AsyncAction* Enabled = ULerpToAttachmentSocket_AsyncAction::LerpToAttachmentSocket(Child, NAME_None, Target, 1.f, true, Radius, Color, DrawDuration);
    Enabled->Activate();
    Enabled->Tick(0.5f);
    const FVector Center = Child->GetComponentLocation();
    TestTrue(TEXT("Tick updates the component before drawing"), Center.Equals(FVector(140.0, 200.0, 300.0)));
    TestFalse(TEXT("Enabled debug draws a sphere"), DurationBatcher->BatchedLines.IsEmpty());
    for (const FBatchedLine& Line : DurationBatcher->BatchedLines)
    {
        TestTrue(TEXT("Sphere uses the updated location and radius"), FMath::IsNearlyEqual(FVector::Distance(Line.Start, Center), static_cast<double>(Radius), 0.001) && FMath::IsNearlyEqual(FVector::Distance(Line.End, Center), static_cast<double>(Radius), 0.001));
        TestEqual(TEXT("Sphere uses the requested color"), Line.Color, FLinearColor(Color.ToFColor(true)));
        TestEqual(TEXT("Sphere uses the requested duration"), Line.RemainingLifeTime, DrawDuration);
    }

    Enabled->Tick(0.5f);
    World->DestroyWorld(false);
    GEngine->DestroyWorldContext(World);
    return !HasAnyErrors();
}

#endif

#endif
