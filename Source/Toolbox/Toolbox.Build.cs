// Copyright Solessfir. All Rights Reserved.

using UnrealBuildTool;

public class Toolbox : ModuleRules
{
	public Toolbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);

		PrivateDependencyModuleNames.AddRange([
			"ApplicationCore",
			"RHI",
			"RenderCore",
			"CommonInput",
			"Slate",
			"SlateCore"
		]);

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			AddEngineThirdPartyPrivateStaticDependencies(Target, "SDL3");
		}
	}
}
