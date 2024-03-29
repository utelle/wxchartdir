/*
** Name:        realtimemultithreadapp.cpp
** Purpose:     Simple Zoom & Scroll sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "realtimemultithreadapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeMultiThreadApp )

/*
 * RealTimeMultiThreadApp type definition
 */

IMPLEMENT_CLASS( RealTimeMultiThreadApp, wxApp )

/*
 * RealTimeMultiThreadApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeMultiThreadApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeMultiThreadApp
 */

RealTimeMultiThreadApp::RealTimeMultiThreadApp()
{
  Init();
}


/*
 * Member initialisation
 */

void
RealTimeMultiThreadApp::Init()
{
}

/*
 * Initialisation for RealTimeMultiThreadApp
 */

bool
RealTimeMultiThreadApp::OnInit()
{    
#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	RealTimeMultiThread* mainWindow = new RealTimeMultiThread(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for RealTimeMultiThreadApp
 */

int
RealTimeMultiThreadApp::OnExit()
{    
	return wxApp::OnExit();
}
