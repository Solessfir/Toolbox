// Copyright Solessfir. All Rights Reserved.

using UnrealBuildTool;

public class ToolboxUncooked : ModuleRules
{
	public ToolboxUncooked(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
			"SlateCore",
			"KismetCompiler",
			"BlueprintGraph",
			"Toolbox"
		]);
	}
}
