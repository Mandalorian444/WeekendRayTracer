workspace "WeekendRayTracer"
    architecture "x86_64"
    configurations { "Debug", "Release" }

project "WeekendRayTracer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    files { "src/**.h", "src/**.cpp" }

    includedirs { "src/" }

    filter {"system:windows"}
        flags { "MultiProcessorCompile" }

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