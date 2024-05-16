// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SynopticonTarget : TargetRules
{
	public SynopticonTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Game;
        ExtraModuleNames.Add("Synopticon");

        //bForceEnableRTTI = true;
        //bForceEnableExceptions = true;
        bAllowLTCG = true;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        BuildEnvironment = TargetBuildEnvironment.Shared;
    }
}
