// Fill out your copyright notice in the Description page of Project Settings.
using System.IO;
using UnrealBuildTool;

public class Synopticon : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../SDKs")); }
    }

    //=============================WAMP=============================================
    public void LoadWAMP(ReadOnlyTargetRules Targett)
    {
        //Definitions.Add("WIN32_LEAN_AND_MEAN");
        PublicDefinitions.Add("MSGPACK_DEFAULT_API_VERSION = 1");
        string LibrariesPath = Path.Combine(ThirdPartyPath, "WAMP");

        //Include paths
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "autobahn"));
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "msgpack-c", "include"));
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "openssl", "include64"));
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "websocketpp", "include"));
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "boost_1_68_0"));

        //Old version
        //PublicIncludePaths.Add(Path.Combine(LibrariesPath, "boost_1_63_0"));

        //New toolset version
        //PublicIncludePaths.Add(Path.Combine(LibrariesPath, "boost_1_68_0"));

        //OPENSSL
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "WAMP", "openssl", "lib64", "libeay32MT.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "WAMP", "openssl", "lib64", "ssleay32MT.lib"));

        //BOOST  

        //Old version     
        //PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS = 1");
        //PublicDefinitions.Add("BOOST_USE_WINDOWS_H");
        //string boostLibraryPath = Path.Combine(LibrariesPath, "boost_1_63_0", "stage", "Release", "lib");
        ////string boostLibraryPath = Path.Combine(LibrariesPath, "boost_1_63_0", "stage", "Release");
        //PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_system-vc140-mt-1_63.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_thread-vc140-mt-1_63.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_date_time-vc140-mt-1_63.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_chrono-vc140-mt-1_63.lib"));
        //PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_regex-vc140-mt-1_63.lib"));

        //New toolset version
        PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS = 1");
        //PublicDefinitions.Add("BOOST_USE_WINDOWS_H");
        string boostLibraryPath = Path.Combine(LibrariesPath, "boost_1_68_0", "libs");
        PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_system-vc141-mt-x64-1_68.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_thread-vc141-mt-x64-1_68.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_date_time-vc141-mt-x64-1_68.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_chrono-vc141-mt-x64-1_68.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(boostLibraryPath, "libboost_regex-vc141-mt-x64-1_68.lib"));

        bUseRTTI = true; // turn on RTTI  
        PublicDefinitions.Add(string.Format("WITH_WAMP_BINDING={0}", 1));
    }

    //===============================LIBVPX=====================================
    public void LoadLibvpx(ReadOnlyTargetRules Target)
    {
        string LibrariesPath = Path.Combine(ThirdPartyPath, "libvpx", "lib");

        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "vpxmt.lib"));

        // Include path
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "libvpx", "include"));

        PublicDefinitions.Add(string.Format("WITH_LIBVPX_BINDING={0}", 1));
    }

    public void LoadLibYUV(ReadOnlyTargetRules Target)
    {
        string LibrariesPath = Path.Combine(ThirdPartyPath, "libyuv", "lib");

        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libyuv.lib"));

        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "libyuv", "include"));

        PublicDefinitions.Add(string.Format("WITH_LIBYUV_BINDING={0}", 1));
    }

    //===========================LibWebP========================================
    public void LoadLibWebP(ReadOnlyTargetRules Target)
    {
        string LibrariesPath = Path.Combine(ThirdPartyPath, "LibWebP");

        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "lib", "libwebp.lib"));
        // Include path
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "include"));

        PublicDefinitions.Add(string.Format("WITH_LibWebP_BINDING={0}", 1));
    }

    public Synopticon(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore", "XmlParser", "Json", "JsonUtilities", "ProceduralMeshComponent", "Http", "UMG"}); //, "ShaderCore"
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        
        //bUseRTTI = true;

        LoadLibWebP(Target);   
        LoadLibvpx(Target);
        LoadLibYUV(Target);

        LoadWAMP(Target);

        PublicDefinitions.Add("MALLOC_LEAKDETECTION = 1");
    }
}
