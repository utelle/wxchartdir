# ![wxChartDir Logo](docs/wxchartdir.png) wxChartDir - ChartDirector based charting components for wxWidgets applications

**wxChartDir** provides components for the integration
and use of the _commercial_ C++ library [ChartDirector](https://www.advsofteng.com/index.html),
a universal chart component developed by **Advanced Software Engineering Ltd**,
in [wxWidgets](https://www.wxwidgets.org) based applications.

**ChartDirector** is an extremely versatile, stable and affordable charting library, that
supports many different chart types like pie, donut, bar, line, spline, step line,
trend line, curve-fitting, inter-line filling, area, band, scatter, bubble, floating box,
box-whisker, waterfall, contour, heat map, surface, vector, finance, gantt, radar, polar,
rose, pyramid, cone, funnel and more. Charts can be created as vector graphics in PDF and
SVG and as raster graphics in PNG, JPG, GIF and BMP. Please visit the
[ChartDirector gallery](https://www.advsofteng.com/gallery.html) to get an impression of
the capabilities of this library.

**wxChartDir** includes the following charting controls:

1. **wxChartViewer** - a wxWidgets control that inherits from **wxPanel** and **ViewPortManager**.
   A viewport can be imagined as a rectangular window of an underlying rectangular surface.
   
2. **wxViewPortControl** - a wxWidgets control that inherits from **wxPanel** and **ViewPortControlBase**.
   A viewport control is a user interface element to let the user visualize and manipulate the viewport managed by the ViewPortManager.

Additionally, **wxChartDir** contains wxWidgets based versions of all sample applications coming with
**ChartDirector** and of the [extra samples](https://www.advsofteng.com/tutorials/extra.html)
available from the **ChartDirector** website. **ChartDirector** itself contains versions for QT and MFC.

Finally, a C++ command-line application is included that allows to generate many sample charts
in a single run (see folder `cppdemo`).

All relevant parts of the **ChartDirector** trial version for C++ (header files, link libraries,
and DLLs) are included in the **wxChartDir** distribution. This allows to compile and test the
sample applications without having to download and install **ChartDirector** separately.

The **ChartDirector** trial version will produce yellow banner messages at the bottom
of the chart images generated by it. To eliminate these banners you need to
[purchase](https://www.advsofteng.com/purchase.html) a commercial **ChartDirector** license.

## Table of Contents

- [Version history](#history)
- [Installation](#install)
- [Documentation](#documentation)
- [License](#license)

## <a name="history"></a>Version history

* 1.0.0 - *July 2018*
  - First public release

## <a name="install" />Installation

The build files for Windows platforms are generated by a (slightly modified)
version of [Premake 5](https://premake.github.io/) (based on Premake 5.0 alpha 11).

Ready to use project files are provided for Visual C++ 2010, 2012, 2013,
2015, and 2017. Additionally, GNU Makefiles are provided supporting for
example TDM-GCC MinGW.

For Visual Studio 2010+ solutions it is possible to customize the build
by creating a `wx_local.props` file in the build directory which is used,
if it exists, by the projects. The settings in that file override the
default values for the properties. The typical way to make the file is
to copy `wx_setup.props` to `wx_local.props` and then edit locally.

For GNU Makefiles the file `config.gcc` serves the same purpose as the
file wx_setup.props for Visual C++ projects.

The customization files `wx_setup.props` resp. `config.gcc` allow to
customize certain settings like for example the version number and the
root directory of the wxWidgets library.

### wxMSW

When building on Win32 or Win64, you can use the makefiles or one of the
Microsoft Visual Studio solution files in the `build` folder.

For Visual C++ the debugging properties are set up in such a way that
debugging the sample applications should work right out of the box. For
release builds you may need to copy the ChartDirector DLL and -
depending on the build - the wxChartDir DLL or add the appropriate
`lib` folder paths to the Windows search path (PATH environment variable).

### wxGTK / wxMac

Cureently **wxGTK** and **wxMac** are not yet supported. It is intended to
add support for these wxWidgets builds with one of next versions of **wxChartDir**.

## <a name="documentation" />Documentation

An overview about **wxChartDir** can be found [here](https://utelle.github.io/wxchartdir/).

The doxygen-generated documentation for this component is available online [here](http://utelle.github.io/wxchartdir/docs/html/index.html).

Extensive documentation for **ChartDirector** can be found on the
[Advanced Software Engineering Ltd website](https://www.advsofteng.com/doc/cdcpp.htm).

## <a name="license" />License

**wxChartDir** is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License version 3
or later as published by the Free Software Foundation,
with the wxWindows 3.1 exception.

**wxChartDir** depends on the _commercial_ C++ library
[ChartDirector](https://www.advsofteng.com/index.html). The license terms for
**ChartDirector** can be found in the file
[`CHARTDIRECTOR-LICENSE.TXT`](CHARTDIRECTOR-LICENSE.TXT).
