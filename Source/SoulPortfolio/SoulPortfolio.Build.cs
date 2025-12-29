// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SoulPortfolio : ModuleRules
{
	public SoulPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"SoulPortfolio",
			"SoulPortfolio/Variant_Platforming",
			"SoulPortfolio/Variant_Platforming/Animation",
			"SoulPortfolio/Variant_Combat",
			"SoulPortfolio/Variant_Combat/AI",
			"SoulPortfolio/Variant_Combat/Animation",
			"SoulPortfolio/Variant_Combat/Gameplay",
			"SoulPortfolio/Variant_Combat/Interfaces",
			"SoulPortfolio/Variant_Combat/UI",
			"SoulPortfolio/Variant_SideScrolling",
			"SoulPortfolio/Variant_SideScrolling/AI",
			"SoulPortfolio/Variant_SideScrolling/Gameplay",
			"SoulPortfolio/Variant_SideScrolling/Interfaces",
			"SoulPortfolio/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
