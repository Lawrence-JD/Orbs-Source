// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GuidingLight : ModuleRules
{
    public GuidingLight(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTags" });
		
		MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
	}
}
