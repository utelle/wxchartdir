# Process this file with automake to create Makefile.in for wxchartdir library
#
# Copyright (C) 2018-2021 Ulrich Telle <ulrich@telle-online.de>
#
# This file is covered by the same licence as the entire wxChartDir package.

ACLOCAL_AMFLAGS = -I admin/m4

# Flags used for compiling all the targets and linking all the executables
# (libraries use LIBADD which is set for lib@WXCHARTDIR_LIBNAME@.la only).
AM_CXXFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/wxdemo/common $(WX_CXXFLAGS)
AM_CXXFLAGS += -DCHARTDIR_HIDE_OBSOLETE

LDADD = lib@WXCHARTDIR_LIBNAME@.la -lchartdir $(WX_LIBS)

# Target-specific assignments are GNU Make-specific, but there doesn't seem to
# be any other way to define different compilation optiosn when building
# shared and static libraries with libtool.
if USE_SHARED
%.lo: AM_CXXFLAGS += -DWXMAKINGDLL_WXCHARTVIEWER
%.o: AM_CXXFLAGS += -DWXUSINGDLL_WXCHARTVIEWER
else
%.lo: AM_CXXFLAGS += -DWXMAKINGLIB_WXCHARTVIEWER
%.o: AM_CXXFLAGS += -DWXUSINGLIB_WXCHARTVIEWER
endif

if HAS_CHARTDIR_PATH
LD_CHARTDIR = -L@CHARTDIR_PATH@
RPATH_CHARTDIR = -rpath @CHARTDIR_PATH@
else
if USE_GTK
if OS_IS64BIT
LD_CHARTDIR = -L$(realpath $(top_srcdir))/lib/linux64
RPATH_CHARTDIR = -rpath $(realpath $(top_srcdir))/lib/linux64
else
LD_CHARTDIR = -L$(realpath $(top_srcdir))/lib/linux32
RPATH_CHARTDIR = -rpath $(realpath $(top_srcdir))/lib/linux32
endif
endif
if USE_MAC
LD_CHARTDIR = -L$(realpath $(top_srcdir))/lib/mac
RPATH_CHARTDIR = -rpath $(realpath $(top_srcdir))/lib/mac
endif
endif
LDFLAGS_SAMPLES = -no-install $(LD_CHARTDIR) $(RPATH_CHARTDIR)

# Define the directory where the library headers are installed.
includewxdir = $(includedir)/wx

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = wxchartdir.pc

# The main library. Notice that it uses dynamic, i.e. determined when running
# configure, name because it depends on the version of wxWidgets used. This is
# rather unusual, but Automake doesn't seem to mind.
lib_LTLIBRARIES = lib@WXCHARTDIR_LIBNAME@.la

lib@WXCHARTDIR_LIBNAME@_la_SOURCES = \
    wxdemo/common/wxchartviewer.cpp

includewx_HEADERS = \
    wxdemo/common/wxchartviewer.h \
    wxdemo/common/wxchartviewer_defs.h \
    wxdemo/common/wxchartviewer_version.h

# TODO: Do we need to list all headers from samples?
# noinst_HEADERS = \
#    wxdemo/*.h

lib@WXCHARTDIR_LIBNAME@_la_LDFLAGS = -no-undefined $(LD_CHARTDIR) $(AM_LDFLAGS)
lib@WXCHARTDIR_LIBNAME@_la_LIBADD = -lchartdir $(WX_LIBS)

# Samples (don't need to be installed).
noinst_PROGRAMS = \
    cppdemo/cppdemo \
    wxdemo/helloworld/helloworld \
    wxdemo/wxdemo/wxdemo \
    wxdemo/realtimemeasure/realtimemeasure \
    wxdemo/realtimemultithread/realtimemultithread \
    wxdemo/realtimesweep/realtimesweep \
    wxdemo/realtimeviewport/realtimeviewport


cppdemo_cppdemo_SOURCES = \
    cppdemo/cppdemo.cpp \
    cppdemo/cppdemo_gen.h \
    cppdemo/cppdemo_inc.h

cppdemo_cppdemo_LDADD = $(LDADD) 
cppdemo_cppdemo_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_helloworld_helloworld_SOURCES = \
    wxdemo/helloworld/helloworld.cpp \
    wxdemo/helloworld/helloworldapp.cpp \
    wxdemo/helloworld/helloworld.h \
    wxdemo/helloworld/helloworldapp.h

wxdemo_helloworld_helloworld_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_helloworld_helloworld_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_wxdemo_wxdemo_SOURCES = \
    wxdemo/wxdemo/wxdemo.cpp \
    wxdemo/wxdemo/wxdemo.h \
    wxdemo/wxdemo/wxdemoapp.cpp \
    wxdemo/wxdemo/wxdemoapp.h \
    wxdemo/wxdemo/democharts.cpp \
    wxdemo/wxdemo/democharts.h \
    wxdemo/wxdemo/hotspotdialog.cpp \
    wxdemo/wxdemo/hotspotdialog.h \
    wxdemo/wxdemo/contourcrosssection.cpp \
    wxdemo/wxdemo/contourcrosssection.h \
    wxdemo/wxdemo/contourzoomscroll.cpp \
    wxdemo/wxdemo/contourzoomscroll.h \
    wxdemo/wxdemo/crosshair.cpp \
    wxdemo/wxdemo/crosshair.h \
    wxdemo/wxdemo/financedemo.cpp \
    wxdemo/wxdemo/financedemo.h \
    wxdemo/wxdemo/megarealtimezoomscroll.cpp \
    wxdemo/wxdemo/megarealtimezoomscroll.h \
    wxdemo/wxdemo/megazoomscroll.cpp \
    wxdemo/wxdemo/megazoomscroll.h \
    wxdemo/wxdemo/realtimedemo.cpp \
    wxdemo/wxdemo/realtimedemo.h \
    wxdemo/wxdemo/realtimemultichart.cpp \
    wxdemo/wxdemo/realtimemultichart.h \
    wxdemo/wxdemo/realtimetrack.cpp \
    wxdemo/wxdemo/realtimetrack.h \
    wxdemo/wxdemo/realtimezoomscroll.cpp \
    wxdemo/wxdemo/realtimezoomscroll.h \
    wxdemo/wxdemo/simplezoomscroll.cpp \
    wxdemo/wxdemo/simplezoomscroll.h \
    wxdemo/wxdemo/threedchartrotation.cpp \
    wxdemo/wxdemo/threedchartrotation.h \
    wxdemo/wxdemo/trackaxis.cpp \
    wxdemo/wxdemo/trackaxis.h \
    wxdemo/wxdemo/trackbox.cpp \
    wxdemo/wxdemo/trackbox.h \
    wxdemo/wxdemo/trackfinance.cpp \
    wxdemo/wxdemo/trackfinance.h \
    wxdemo/wxdemo/tracklabel.cpp \
    wxdemo/wxdemo/tracklabel.h \
    wxdemo/wxdemo/tracklegend.cpp \
    wxdemo/wxdemo/tracklegend.h \
    wxdemo/wxdemo/viewportcontroldemo.cpp \
    wxdemo/wxdemo/viewportcontroldemo.h \
    wxdemo/wxdemo/xyzoomscroll.cpp \
    wxdemo/wxdemo/xyzoomscroll.h \
    wxdemo/wxdemo/zoomscrollpdf.cpp \
    wxdemo/wxdemo/zoomscrollpdf.h \
    wxdemo/wxdemo/zoomscrolltrack.cpp \
    wxdemo/wxdemo/zoomscrolltrack.h \
    wxdemo/wxdemo/zoomscrolltrack2.cpp \
    wxdemo/wxdemo/zoomscrolltrack2.h

wxdemo_wxdemo_wxdemo_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_wxdemo_wxdemo_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimemeasure_realtimemeasure_SOURCES = \
    wxdemo/realtimemeasure/realtimemeasure.cpp \
    wxdemo/realtimemeasure/realtimemeasureapp.cpp \
    wxdemo/realtimemeasure/realtimemeasure.h \
    wxdemo/realtimemeasure/realtimemeasureapp.h \
    wxdemo/realtimemeasure/wxdoublebufferedqueue.h

wxdemo_realtimemeasure_realtimemeasure_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimemeasure_realtimemeasure_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimemultithread_realtimemultithread_SOURCES = \
    wxdemo/realtimemultithread/realtimemultithread.cpp \
    wxdemo/realtimemultithread/realtimemultithreadapp.cpp \
    wxdemo/realtimemultithread/randomwalk.cpp \
    wxdemo/realtimemultithread/realtimemultithread.h \
    wxdemo/realtimemultithread/realtimemultithreadapp.h \
    wxdemo/realtimemultithread/randomwalk.h \
    wxdemo/realtimemultithread/wxdoublebufferedqueue.h

wxdemo_realtimemultithread_realtimemultithread_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimemultithread_realtimemultithread_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimesweep_realtimesweep_SOURCES = \
    wxdemo/realtimesweep/realtimesweep.cpp \
    wxdemo/realtimesweep/realtimesweepapp.cpp \
    wxdemo/realtimesweep/randomwave.cpp \
    wxdemo/realtimesweep/realtimesweep.h \
    wxdemo/realtimesweep/realtimesweepapp.h \
    wxdemo/realtimesweep/randomwave.h \
    wxdemo/realtimesweep/wxdoublebufferedqueue.h

wxdemo_realtimesweep_realtimesweep_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimesweep_realtimesweep_LDFLAGS = $(LDFLAGS_SAMPLES)


wxdemo_realtimeviewport_realtimeviewport_SOURCES = \
    wxdemo/realtimeviewport/realtimeviewport.cpp \
    wxdemo/realtimeviewport/realtimeviewportapp.cpp \
    wxdemo/realtimeviewport/randomwalk.cpp \
    wxdemo/realtimeviewport/realtimeviewport.h \
    wxdemo/realtimeviewport/realtimeviewportapp.h \
    wxdemo/realtimeviewport/randomwalk.h \
    wxdemo/realtimeviewport/wxdoublebufferedqueue.h

wxdemo_realtimeviewport_realtimeviewport_LDADD = $(WX_LIBS_CHARTDIR_SAMPLE) $(LDADD)
wxdemo_realtimeviewport_realtimeviewport_LDFLAGS = $(LDFLAGS_SAMPLES)
