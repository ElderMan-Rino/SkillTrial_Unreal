// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkillTrial01_Unreal : ModuleRules
{
	public SkillTrial01_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Niagara",
			"AIModule",
			"NavigationSystem",
			"GameplayTasks",
			"UMG"
		});
	}
}
