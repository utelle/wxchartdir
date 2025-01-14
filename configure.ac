dnl Process this script with autoconf to create configure for wxchartdir library
dnl
dnl Copyright (C) 2018-2025 Ulrich Telle <ulrich@telle-online.de>
dnl
dnl This file is covered by the same licence as the entire wxChartDir package.

AC_INIT([wxchartdir], [2.0.2], [ulrich@telle-online.de])

dnl This is the version tested with, might work with earlier ones.
AC_PREREQ([2.69])

AC_CONFIG_SRCDIR([wxdemo/common/wxchartviewer.h])
AC_CONFIG_AUX_DIR([admin/build-aux])
AC_CONFIG_MACRO_DIR([admin/m4])

AM_INIT_AUTOMAKE([1.11 foreign subdir-objects])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

LT_INIT()
AM_CONDITIONAL([USE_SHARED], [test "x$enable_shared" = xyes])

AC_PROG_CXX
AC_LANG(C++)

AC_ARG_VAR(WX_CONFIG, [Path to wx-config script (default: search in PATH).])
WX_CONFIG_OPTIONS
WX_CONFIG_CHECK([3.0.0],
    [],
    [AC_MSG_FAILURE([wxWidgets required but not detected.])],
    [base,core]
)

AC_ARG_WITH([cddir],
    AS_HELP_STRING([--with-cddir=DIR],
                   [Directory where the ChartDirector binaries are installed (optional; default is /usr/local)]),
    [ if test "$withval" = "yes"; then
        chartdirpath=""
      elif test "$withval" = "no"; then
        chartdirpath=""
      else
        chartdirpath="$withval"
      fi
    ],
    [ chartdirpath="" ]
)
CHARTDIR_PATH=$chartdirpath
AC_SUBST(CHARTDIR_PATH)
AM_CONDITIONAL([HAS_CHARTDIR_PATH], [test "x$chartdirpath" != "x"])

dnl We only need the libraries above for the main library itself, but the
dnl samples have additional requirements, check for them too
core_WX_LIBS=$WX_LIBS
WX_CONFIG_CHECK([3.0.0],
    [WX_LIBS_CHARTDIR_SAMPLE=$WX_LIBS],
    [AC_MSG_WARN([Some wxWidgets libraries not available, samples won't be built])],
    [adv,html,richtext,base,core,xml]
)
WX_LIBS=$core_WX_LIBS
AM_CONDITIONAL([BUILD_CHARTDIR_SAMPLE], [test -n "$WX_LIBS_CHARTDIR_SAMPLE"])

dnl Different ChartDirector libraries are required with wxGTK or wxMAC.
case "$WX_CXXFLAGS" in
    *-D__WXGTK__* )
        USE_GTK=1
        ;;
    *-D__WXMAC__* )
        USE_MAC=1
        ;;
esac
AM_CONDITIONAL(USE_GTK, [test -n "$USE_GTK"])
AM_CONDITIONAL(USE_MAC, [test -n "$USE_MAC"])

dnl Check 
AC_CHECK_SIZEOF([size_t])
AM_CONDITIONAL(OS_IS64BIT, [test "$ac_cv_sizeof_size_t" -eq 8])

dnl This is needed by WX_LIKE_LIBNAME
WX_DETECT_STANDARD_OPTION_VALUES
dnl This macro is used to preserve the same name as was used with the previous
dnl build systems only.
WX_LIKE_LIBNAME([WXCHARTDIR_LIBNAME], [wxcode], [wxchartdir])

AC_SUBST(WXCHARTDIR_LIBNAME)
AC_SUBST(WX_LIBS_CHARTDIR_SAMPLE)
AC_SUBST( LIBDIR, "lib" )

dnl Ensure that the samples can find the files they need when running from the
dnl build directory.
AC_CONFIG_LINKS([cppdemo/bg.png:cppdemo/background/bg.png
     cppdemo/tile.png:cppdemo/background/tile.png
     cppdemo/small_computer.png:cppdemo/customsymbolline/small_computer.png
     cppdemo/small_user.png:cppdemo/customsymbolline/small_user.png
     cppdemo/small_molecule.png:cppdemo/enhancedarea/small_molecule.png
     cppdemo/star.png:cppdemo/enhancedarea/star.png
     cppdemo/clock.png:cppdemo/errline/clock.png
     cppdemo/marble.png:cppdemo/fontpie/marble.png
     cppdemo/marble2.png:cppdemo/fontpie/marble2.png
     cppdemo/wood.png:cppdemo/fontpie/wood.png
     cppdemo/robot1.png:cppdemo/hboxwhisker/robot1.png
     cppdemo/robot2.png:cppdemo/hboxwhisker/robot2.png
     cppdemo/robot5.png:cppdemo/hboxwhisker/robot5.png
     cppdemo/gas.png:cppdemo/iconameter/gas.png
     cppdemo/cloud.png:cppdemo/icondonut/cloud.png
     cppdemo/rain.png:cppdemo/icondonut/rain.png
     cppdemo/snowy.png:cppdemo/icondonut/snowy.png
     cppdemo/sun.png:cppdemo/icondonut/sun.png
     cppdemo/angry.png:cppdemo/iconpie2/angry.png
     cppdemo/doc.png:cppdemo/iconpie2/doc.png
     cppdemo/laugh.png:cppdemo/iconpie2/laugh.png
     cppdemo/molecule.png:cppdemo/iconpie2/molecule.png
     cppdemo/nocomment.png:cppdemo/iconpie2/nocomment.png
     cppdemo/sad.png:cppdemo/iconpie2/sad.png
     cppdemo/smile.png:cppdemo/iconpie2/smile.png
     cppdemo/computer.png:cppdemo/multistackbar/computer.png
     cppdemo/service.png:cppdemo/multistackbar/service.png
     cppdemo/software.png:cppdemo/multistackbar/software.png
     cppdemo/snow.png:cppdemo/patternarea/snow.png
     cppdemo/robot3.png:cppdemo/scattersymbols/robot3.png
     cppdemo/robot4.png:cppdemo/scattersymbols/robot4.png
     cppdemo/maptexture.png:cppdemo/surfacetexture/maptexture.png
     cppdemo/marble3.png:cppdemo/texturedonut/marble3.png
     cppdemo/rock.png:cppdemo/texturedonut/rock.png
])

AC_CONFIG_FILES([Makefile wxchartdir.pc])
AC_OUTPUT
