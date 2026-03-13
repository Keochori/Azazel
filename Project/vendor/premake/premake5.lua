workspace "Azazel"
    architecture "x64"

    configurations {
        "Debug"
    }

    TargetDir = "bin/%{cfg.buildcfg}"
    ObjectDir = "bin-int/%{cfg.buildcfg}"

project "Azazel"
    location "Azazel"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    pchheader "pch.h"
    pchsource "Azazel/src/pch.cpp"

    targetdir (TargetDir)
    objdir(ObjectDir)
    debugdir(TargetDir)

    links {
        "d3d11.lib",
        "D3DCompiler.lib",
        "dxguid.lib"
    }

files {
        "Azazel/resources/**",
        "Azazel/src/**.cpp",
        "Azazel/src/**.h",
        "Azazel/src/**.hlsl"
    }

includedirs {
        "Azazel/resources",
        "Azazel/src"
    }

    filter "configurations:Debug"
    defines "DEBUG"
    runtime "Debug"
    symbols "on"

-- Shaders
    shaderobjectfileoutput("%{cfg.targetdir}/resources/shaders/%%(Filename).cso")

    filter("files:Azazel/src/**_vs.hlsl")
    shadertype("Vertex")
    shadermodel "5.0"
    filter {}
    
    filter("files:Azazel/src/**_ps.hlsl")
    shadertype("Pixel")
    shadermodel "5.0"
    filter {}