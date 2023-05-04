// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AZ_MHW : ModuleRules
{
	public AZ_MHW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
                "HeadMountedDisplay",
                "HTTP",
                "UMG", 
				"Sockets",
                "MoviePlayer",
                "Networking", 
				"AIModule", 
				"NavigationSystem", 
				"SlateCore", 
				"Slate" ,
                "RenderCore",
                "Projects",
                "GameplayTasks",

                "MovieScene",
                "LevelSequence",
                "ActorSequence",
                "Paper2D",
                "CinematicCamera",
                "MovieSceneTracks",
                "MediaAssets",

                "BuildPatchServices",
                "OnlineSubsystem",
                "Json",
                "JsonUtilities",
                "PakFile",
                "Landscape",

                "WebBrowserWidget",
            });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
