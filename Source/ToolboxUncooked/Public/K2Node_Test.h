// Copyright Solessfir. All Rights Reserved.

#pragma once

#include "K2Node.h"
#include "K2Node_Test.generated.h"

/**
* Enum of Global Ini Files
* We use this along with our lookup map to determine which ini is being used for the config action
*/
UENUM(BlueprintType)
enum class EConfigIniFileNames : uint8
{
    /** Engine */
    GEngineIni				UMETA(DisplayName = "Engine"),

    /** Game */
    GGameIni				UMETA(DisplayName = "Game"),
    GGameUserSettingsIni	UMETA(DisplayName = "Game User Settings"),

    /** Editor */
    GEditorIni				UMETA(DisplayName = "Editor"),
    GEditorKeyBindingsIni	UMETA(DisplayName = "Editor Key Bindings"),
    GEditorLayoutIni		UMETA(DisplayName = "Editor Layout"),
    GEditorSettingsIni		UMETA(DisplayName = "Editor Settings"),
    GEditorPerProjectIni	UMETA(DisplayName = "Editor Per Project"),

    /** Misc */
    GCompatIni				UMETA(DisplayName = "Compat"),
    GLightmassIni			UMETA(DisplayName = "Lightmass"),
    GScalabilityIni			UMETA(DisplayName = "Scalability"),
    GHardwareIni			UMETA(DisplayName = "Hardware"),
    GInputIni				UMETA(DisplayName = "Input"),
    GRuntimeOptionsIni		UMETA(DisplayName = "Runtime Options"),
    GInstallBundleIni		UMETA(DisplayName = "Install Bundle"),
    GDeviceProfilesIni		UMETA(DisplayName = "Device Profiles")
};

UCLASS()
class UK2NodeTest : public UK2Node
{
    GENERATED_BODY()
public:
    // Begin UK2Node Interface
    
    // will be called during compilation. Allows to perform validation of internal values
    virtual void ValidateNodeDuringCompilation(FCompilerResultsLog& MessageLog) const override;
    
    // core method, during compilation will generate resulting data
    virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
    
    // used to place this node into add blueprint node popup
    virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
    
    // will set node category in add blueprint node popup
    virtual FText GetMenuCategory() const override;
    
    // node corner icon
    virtual FName GetCornerIcon() const override;
    
    // return true if we want do show node properties in Details panel
    virtual bool ShouldShowNodeProperties() const override { return true; }
    
    // should return unique node signature
    // if we generate multiple nodes depending on different data, use this to differentiate them by signature
    virtual FBlueprintNodeSignature GetSignature() const override;

    // used to show and process context menu on node or node pins
    virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

    
    // End UK2Node Interface


    
    
    // Begin UEdGraphNode Interface
    
    virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;

    // used to add pins for node
    virtual void AllocateDefaultPins() override;
    
    // used to add node title color
    virtual FLinearColor GetNodeTitleColor() const override;
    
    // used to generate node title
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    
    // for tooltip
    virtual FText GetTooltipText() const override;
    
    // for icon and tint
    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
    
    // true if compatible with selected blueprint editor graph
    virtual bool IsCompatibleWithGraph(UEdGraph const* Graph) const override;

    
    //End UEdGraphNode Interface.
    
private:
    UPROPERTY(EditAnywhere, Category = "Data")
    int32 NodeNumber = 0;

    // pin names
    inline static const FName IntPinName = FName("IntInputParam");

    inline static const FName ConfigFilePinName = FName("ConfigFile");
    
    inline static const TMap<EConfigIniFileNames, FString> IniNameLookupMap =
    {
        {EConfigIniFileNames::GEngineIni, GEngineIni},
        {EConfigIniFileNames::GGameIni, GGameIni},
        {EConfigIniFileNames::GGameUserSettingsIni, GGameUserSettingsIni},
        {EConfigIniFileNames::GEditorIni, GEditorIni},
        {EConfigIniFileNames::GEditorKeyBindingsIni, GEditorKeyBindingsIni},
        {EConfigIniFileNames::GEditorLayoutIni, GEditorLayoutIni},
        {EConfigIniFileNames::GEditorSettingsIni, GEditorSettingsIni},
        {EConfigIniFileNames::GEditorPerProjectIni, GEditorPerProjectIni},
        {EConfigIniFileNames::GCompatIni, GCompatIni},
        {EConfigIniFileNames::GLightmassIni, GLightmassIni},
        {EConfigIniFileNames::GScalabilityIni, GScalabilityIni},
        {EConfigIniFileNames::GHardwareIni, GHardwareIni},
        {EConfigIniFileNames::GInputIni, GInputIni},
        {EConfigIniFileNames::GRuntimeOptionsIni, GRuntimeOptionsIni},
        {EConfigIniFileNames::GInstallBundleIni, GInstallBundleIni},
        {EConfigIniFileNames::GDeviceProfilesIni, GDeviceProfilesIni}
    };
};
