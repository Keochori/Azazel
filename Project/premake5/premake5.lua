workspace "Azazel"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
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

    AssimpDLL = "Azazel/vendor/assimp/assimp-vc143-mt.dll"
    postbuildcommands { 
        "{MKDIR} %{cfg.targetdir}/ProjectSettings",
        "{MKDIR} %{cfg.targetdir}/resources/models",
        "{COPYDIR} ../Assets/Models %{cfg.targetdir}/resources/models",
        "{MKDIR} %{cfg.targetdir}/resources/textures",
        "{COPYDIR} ../Assets/Textures %{cfg.targetdir}/resources/textures",
        "{MKDIR} %{cfg.targetdir}/resources/animations",
        "{COPYDIR} ../Assets/Animations %{cfg.targetdir}/resources/animations",
        "{COPYFILE} %[" .. AssimpDLL .. "] %{cfg.targetdir}",
    }

    libdirs {
        "lib"
    }

    links {
        "d3d11.lib",
        "D3DCompiler.lib",
        "dxguid.lib",
        "assimp-vc143-mt.lib"
        }   

    files {
        "Azazel/resources/**",
        "Azazel/src/**.cpp",
        "Azazel/src/**.hpp",
        "Azazel/src/**.h",
        "Azazel/src/**.hlsl",
        "Azazel/vendor/**"
        }

    includedirs {
        "Azazel/resources",
        "Azazel/src",
        "Azazel/vendor"
    }
    
    defines {
        "IMGUI_DEFINE_MATH_OPERATORS"
    }

-- Configurations
    filter("configurations:Debug")
        runtime "Debug"
        symbols "on"
        libdirs {"lib/Debug"}
        links {"DirectXTex.lib"}
    filter {}
    
    filter("configurations:Release")
        runtime "Release"
        optimize "on"
        libdirs {"lib/Release"}
        links {"DirectXTex.lib"}
    filter {}
-----------------
    

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
----------