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
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "websocketpp", "include"));
        PublicIncludePaths.Add(Path.Combine(LibrariesPath, "boost_1_68_0"));

        //BOOST  
        //New toolset version
        PublicDefinitions.Add("BOOST_DISABLE_ABI_HEADERS = 1");
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
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", 
            "RHI", "RenderCore", "XmlParser", "Json", "JsonUtilities", "ProceduralMeshComponent", "HTTP", 
            "UMG","MediaAssets","Sockets","Networking"}); //, "ShaderCore"
        PrivateDependencyModuleNames.Add("OpenSSL");
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Messaging"});
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        

        LoadLibWebP(Target);   
        LoadLibvpx(Target);
        LoadLibYUV(Target);
        //LoadWAMP(Target);

        PublicDefinitions.Add("MALLOC_LEAKDETECTION = 1");
    }
}
