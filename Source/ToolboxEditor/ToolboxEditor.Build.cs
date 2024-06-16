// Copyright Solessfir. All Rights Reserved.

using UnrealBuildTool;

public class ToolboxEditor : ModuleRules
{
	public ToolboxEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject"
		});
	}
}
