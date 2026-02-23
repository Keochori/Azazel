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
    objdir (ObjectDir)

    files {
        "Azazel/src/**.cpp",
        "Azazel/src/**.h"
    }

    includedirs {
        "Azazel/src"
    }

    filter "configurations:Debug"
    defines "DEBUG"
    runtime "Debug"
    symbols "on"