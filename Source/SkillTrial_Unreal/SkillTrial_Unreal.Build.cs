// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkillTrial_Unreal : ModuleRules
{
	public SkillTrial_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		
		PublicDependencyModuleNames.AddRange(Modules());

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
	private string[] Modules()
	{
		return new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"HairStrandsCore",
			"Niagara",
			"GeometryCollectionEngine",
			"UMG",
			"AIModule"
		};
    }
}
