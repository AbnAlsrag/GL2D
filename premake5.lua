workspace "GL2D"
    configurations { "Debug", "Dev", "Release" }
    platforms { "Static32", "Shared32", "Static64", "Shared64" }

    startproject "Sandbox"
    compileas "C"

    output_dir = "%{cfg.buildcfg}-%{cfg.platform}-%{cfg.system}-%{cfg.architecture}"

    -- //GL2D// --
    project "GL2D"
        location "GL2D"
        language "C"
        floatingpoint "Fast"
        cdialect "C99"
        warnings "Everything"
        floatingpoint "Fast"

        targetdir ("bin/" .. output_dir .. "/%{prj.name}")
        objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.c",
        }

        includedirs {
            "%{prj.name}/src",
            "%{prj.name}/external/%{cfg.system}-%{cfg.architecture}"
        }

        links {
            "GL2D/external/%{cfg.system}-%{cfg.architecture}/glad/glad",
            "GL2D/external/%{cfg.system}-%{cfg.architecture}/stb/stb_image",
        }

        filter "system:windows"
            links {
                "opengl32",
                "gdi32",
                "user32",
            }   

        filter "configurations:Debug"
            defines { "GL2D_DEBUG" }
            symbols "On"
            optimize "Off"
            
        filter "configurations:Dev"
            defines { "GL2D_DEV" }
            symbols "On"
            optimize "Debug"

        filter "configurations:Release"
            defines { "GL2D_RELEASE" }
            symbols "Off"
            optimize "Full"

        filter "platforms:Static*"
            kind "StaticLib"

        filter "platforms:Shared*"
            defines { "GL2D_DLL_BUILD" }
            kind "SharedLib"

        filter "platforms:*32"
            architecture "x32"

        filter "platforms:*64"
            architecture "x64"

    -- //Sandbox// --
    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C"
        cdialect "C99"

        targetdir ("bin/" .. output_dir .. "/%{prj.name}")
        objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.c",
        }

        includedirs {
            "GL2D/src",
            "%{prj.name}/src",
            "%{prj.name}/external/%{cfg.system}"
        }

        links {
            "GL2D",
            "Sandbox/external/%{cfg.system}/glfw/glfw3",
        }

        filter "system:windows"
            systemversion "latest"

        filter "platforms:*32"
            architecture "x32"

        filter "platforms:*64"
            architecture "x64"

        filter { "platforms:Shared*", "system:windows" }
            postbuildcommands {
                "{COPYFILE} ../bin/" .. output_dir .. "/GL2D/GL2D.dll ../bin/" .. output_dir .. "/%{prj.name}"
            }

        filter "configurations:Debug"
            defines { "GL2D_DEBUG" }
            symbols "On"
            optimize "Off"
            
        filter "configurations:Dev"
            defines { "GL2D_DEV" }
            symbols "On"
            optimize "Debug"

        filter "configurations:Release"
            defines { "GL2D_RELEASE" }
            symbols "Off"
            optimize "Full"
