// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGProject : ModuleRules
{
	public RPGProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem",
	        "AIModule", "Niagara", "EnhancedInput", "GameplayTags", "GameplayTasks", "GameplayAbilities",
	        "GameplayMessageRuntime",  "Slate", 
	        "SlateCore", "MotionWarping", "LevelSequence",
	        "MovieScene",
	        "MovieSceneTracks"
        });
        
        PrivateDependencyModuleNames.AddRange(new string[] {  });

        PrivateIncludePaths.AddRange(new string[]
        {
	        "RPGProject/",
        });
    }
}
