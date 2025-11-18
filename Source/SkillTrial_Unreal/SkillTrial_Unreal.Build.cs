// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkillTrial_Unreal : ModuleRules
{
	public SkillTrial_Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		AddPublicModuleDependencies();
		AddEditorOnlyDependencies();
		AddPrivateDependencies();

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
	private void AddPublicModuleDependencies()
	{
        PublicDependencyModuleNames.AddRange(GetStandardPublicModules());
    }
	private string[] GetStandardPublicModules()
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
	private void AddEditorOnlyDependencies()
	{
		if (!Target.bBuildEditor)
			return;

        PrivateDependencyModuleNames.AddRange(GetEditorOnlyModules());
    }
   
    private void AddPrivateDependencies()
    {
        PublicDependencyModuleNames.AddRange(GetStandardPrivateModules());
		if (!Target.bBuildEditor)
			return;
		PublicDependencyModuleNames.AddRange(GetEditorOnlyModules());
    }
    private string[] GetStandardPrivateModules()
    {
		return new string[]
		{
            "Slate",
			"SlateCore",
		}; 
    }
    private string[] GetEditorOnlyModules()
    {
        return new string[]
        {
            "UnrealEd",      // 에디터 핵심 기능
			"AssetTools",    // 에셋 도구
			"Blutility",     // UEditorUtilityWidget 사용
			"EditorSubsystem",// 에디터 서브시스템
            "DesktopPlatform",
            "EditorStyle",
            "AssetRegistry",
        };
    }
}
