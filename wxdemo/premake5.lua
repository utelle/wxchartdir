-- wxChartDirector configuration file for premake5
--
-- Copyright (C) 2018-2021 Ulrich Telle <ulrich@telle-online.de>
--
-- This file is covered by the same licence as the entire wxchartdir package. 

dofile "premake/wxwidgets.lua"

BUILDDIR = _OPTIONS["builddir"] or "build"

function links_cdlib()
  links { "chartdir70" }
  filter { "platforms:Win32" }
    libdirs { "../lib/win32" }
  filter { "platforms:Win64" }
    libdirs { "../lib/win64" }
  filter {}
end

workspace "wxchartdir"
  configurations { "Debug", "Release", "Debug wxDLL", "Release wxDLL", "Debug DLL", "Release DLL" }
  platforms { "Win32", "Win64" }
  location(BUILDDIR)

  if (is_msvc) then
    local wks = workspace()
    wks.filename = "wxchartdir_" .. vc_with_ver
  end

  defines {
    "wxUSE_DPI_AWARE_MANIFEST=2",
    "_WINDOWS",
    "_CRT_SECURE_NO_WARNINGS",
    "_CRT_SECURE_NO_DEPRECATE",
    "_CRT_NONSTDC_NO_DEPRECATE"
  }

  init_filters()

-- wxChartViewer library
project "wxchartviewer"
  location(BUILDDIR)
  language "C++"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_chartviewer"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "wxchartdir_mono"
  end

  links { "chartdir70" }
  filter { "platforms:Win32" }
    libdirs { "../lib/win32" }
  filter { "platforms:Win64" }
    libdirs { "../lib/win64" }
  filter {}

  make_filters( "WXCHARTVIEWER", "wxchartdir", "core,xml" )

  files { "common/*.cpp", "common/*.h", "common/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "common", "../include" }
  characterset "Unicode"

  defines {
    "CHARTDIR_HIDE_OBSOLETE"
  }

-- wxChartDir helloworld
project "helloworld"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_helloworld"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "helloworld_mono"
  end

  use_filters( "WXCHARTVIEWER", "helloworld", "adv,html,richtext,core,xml" )

  files { "helloworld/*.cpp", "helloworld/*.h", "helloworld/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "helloworld", "common", "../include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxchartviewer" }
  links_cdlib()
  dpiawareness "HighPerMonitor"

-- wxChartDir crosshair
-- wxChartDir trackaxis
-- wxChartDir trackbox
-- wxChartDir trackfinance
-- wxChartDir tracklabel
-- wxChartDir tracklegend

-- wxChartDir wxdemo
project "wxdemo"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_wxdemo"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "wxdemo_mono"
  end

  use_filters( "WXCHARTVIEWER", "wxdemo", "adv,html,richtext,core,xml" )

  files { "wxdemo/*.cpp", "wxdemo/*.h", "wxdemo/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "wxdemo", "common", "../include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxchartviewer" }
  links_cdlib()
  dpiawareness "HighPerMonitor"

-- simplezoomscroll
-- viewportcontroldemo
-- xyzoomscroll
-- zoomscrolltrack
-- zoomscrolltrack2
-- realtimedemo
-- realtimetrack
-- realtimezoomscroll

-- realtimesweep
project "realtimesweep"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimesweep"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimesweep_mono"
  end

  use_filters( "WXCHARTVIEWER", "realtimesweep", "adv,html,richtext,core,xml" )

  files { "realtimesweep/*.cpp", "realtimesweep/*.h", "realtimesweep/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimesweep", "common", "../include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxchartviewer" }
  links_cdlib()
  dpiawareness "HighPerMonitor"

-- realtimemultithread
project "realtimemultithread"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimemultithread"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimemultithread_mono"
  end

  use_filters( "WXCHARTVIEWER", "realtimemultithread", "adv,html,richtext,core,xml" )

  files { "realtimemultithread/*.cpp", "realtimemultithread/*.h", "realtimemultithread/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimemultithread", "common", "../include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxchartviewer" }
  links_cdlib()
  dpiawareness "HighPerMonitor"

-- realtimemeasure
project "realtimemeasure"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimemeasure"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimemeasure_mono"
  end

  use_filters( "WXCHARTVIEWER", "realtimemeasure", "adv,html,richtext,core,xml" )

  files { "realtimemeasure/*.cpp", "realtimemeasure/*.h", "realtimemeasure/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimemeasure", "common", "../include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxchartviewer" }
  links_cdlib()
  dpiawareness "HighPerMonitor"

-- realtimeviewport
project "realtimeviewport"
  location(BUILDDIR)
  language "C++"
  kind "WindowedApp"

  if (is_msvc) then
    local prj = project()
    prj.filename = "wxchartdir_" .. vc_with_ver .. "_realtimeviewport"
  end
  if wxMonolithic then
    local prj = project()
    prj.filename = "realtimeviewport_mono"
  end

  use_filters( "WXCHARTVIEWER", "realtimeviewport", "adv,html,richtext,core,xml" )

  files { "realtimeviewport/*.cpp", "realtimeviewport/*.h", "realtimeviewport/*.rc" }
  vpaths {
    ["Header Files"] = { "**.h" },
    ["Source Files"] = { "**.cpp", "**.rc" }
  }
  includedirs { "realtimeviewport", "common", "../include" }
  characterset "Unicode"
  entrypoint "WinMainCRTStartup"
  links { "wxchartviewer" }
  links_cdlib()
  dpiawareness "HighPerMonitor"

-- financedemo
