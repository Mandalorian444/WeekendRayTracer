outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "WeekendRayTracer"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    location "."
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    files { "src/**.h", "src/**.cpp" }

    defines { "GLEW_NO_GLU" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    filter {"system:windows"}
        defines { "WRT_WINDOWS" }
        flags { "MultiProcessorCompile" }

        prebuildcommands { "vcpkg install" }

    filter {"system:linux"}
        defines { "WRT_LINUX" }
        toolset "gcc"

        prebuildcommands { "vcpkg install --triplet=x64-linux-dynamic" }

        linkoptions { "-Wl,-rpath,'$$ORIGIN/lib'" }

        includedirs {
            "vcpkg_installed/x64-linux-dynamic/include"
        }

        libdirs {
            "vcpkg_installed/x64-linux-dynamic/lib"
        }

        links {
            "dl",
            "glfw",
            "GLEW",
            "OpenGL",
            "imgui"
        }

        postbuildcommands {
            "{COPYDIR} vcpkg_installed/x64-linux-dynamic/lib %{cfg.targetdir}"
        }

project "WeekendRayTracer"

    includedirs { "src/" }

    filter { "configurations:Debug" }
      defines { "DEBUG" }
      symbols "On"

    filter { "configurations:Release" }
      defines { "NDEBUG" }
      optimize "On"

require('vstudio')

local function vcpkgEnable(prj)
    premake.w('<PropertyGroup Label="Vcpkg">')
    premake.w('<VcpkgEnableManifest>true</VcpkgEnableManifest>')
    premake.w('</PropertyGroup>')
end

premake.override(premake.vstudio.vc2010.elements, "project", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, premake.vstudio.vc2010.outputPropertiesGroup, vcpkgEnable)
    return calls
end)