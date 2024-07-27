// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TDS : ModuleRules
{
	public TDS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PublicIncludePaths.AddRange(new string[] {"TDS"});
        PrivateDependencyModuleNames.AddRange(new string[] 
		{ 

			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"HeadMountedDisplay",
			"Niagara",
			"PhysicsCore",
			"AIModule",
			"GameplayTasks",
			"GameplayTags",
			"NavigationSystem",
			"OnlineSubsystem",
			"OnlineSubsystemNull" ,
			"NetCore",
			"FieldSystemEngine",
			"GeometryCollectionEngine",
			"AnimGraphRuntime",
			"EnhancedInput"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}

}
