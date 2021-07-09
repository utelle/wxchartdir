-- wxChartDir cppdemo configuration file for premake5
--
-- Copyright (C) 2018-2021 Ulrich Telle <ulrich@telle-online.de>
--
-- This file is covered by the same licence as the entire wxChartDir package. 

dofile "wx_config.lua"

BUILDDIR = _OPTIONS["builddir"] or "build"

workspace "cppdemo"
  configurations { "Debug", "Release" }
  platforms { "Win32", "Win64" }
  location(BUILDDIR)

  if (is_msvc) then
    local wks = workspace()
    wks.filename = "cppdemo_" .. vc_with_ver
  end

  wxWorkspaceCommon()

-- Minimal sample
project "cppdemoall"
  location(BUILDDIR)
  language "C++"
  kind "ConsoleApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "cppdemo_" .. vc_with_ver .. "_all"
    if (msvc_useProps) then
      wxUseProps(true)
    end
  else
    toolset("gcc")
  end
  makesettings { "include config.gcc" }

  files { "cppdemo.cpp", "cppdemo_*.h" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "../include" }
  characterset "Unicode"

  postbuildcommands {
    "{COPY} %{wks.location}/../background/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../customsymbolline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../enhancedarea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../errline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../fontpie/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../hboxwhisker/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../iconameter/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../icondonut/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../iconpie/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../iconpie2/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../linecompare/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../multistackbar/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../patternarea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../percentarea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../percentbar/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../polararea/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../polarline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../scattersymbols/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../splineline/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../surfacetexture/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../texturedonut/*.png %{wks.location}/../bin",
    "{COPY} %{wks.location}/../yzonecolor/*.png %{wks.location}/../bin"
  }

  links { "chartdir70" }
  filter { "platforms:Win32" }
    libdirs { "../lib/win32" }
    debugenvs { "PATH=../../lib/win32;%PATH%" }
  filter { "platforms:Win64" }
    libdirs { "../lib/win64" }
    debugenvs { "PATH=../../lib/win64;%PATH%" }
  filter {}

  -- Intermediate directory
  if (is_msvc) then
    objdir (BUILDDIR .. "/obj/" .. vc_with_ver)
  else
    objdir (BUILDDIR .. "/obj/gcc")
  end

  -- Target directory
  filter { "configurations:Debug*", "platforms:Win32" }
    debugdir "bin"
    if (is_msvc) then
      targetdir ("bin/" .. vc_with_ver .. "/Win32/Debug")
    else
      targetdir "bin/gcc/Win32/Debug"
    end
  filter { "configurations:Debug*", "platforms:Win64" }
    debugdir "bin"
    if (is_msvc) then
      targetdir ("bin/" .. vc_with_ver .. "/Win64/Debug")
    else
      targetdir "bin/gcc/Win64/Debug"
    end
  filter { "configurations:Release*", "platforms:Win32" }
    if (is_msvc) then
      targetdir ("bin/" .. vc_with_ver .. "/Win32/Release")
    else
      targetdir "bin/gcc/Win32/Release"
    end
