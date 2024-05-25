﻿// Copyright Solessfir. All Rights Reserved.

using UnrealBuildTool;

public class Toolbox : ModuleRules
{
	public Toolbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.Add("Core");
		
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"ApplicationCore",
			"Engine",
			"RHI",
			"SlateCore"
		});
	}
}
