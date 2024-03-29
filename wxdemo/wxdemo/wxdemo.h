/*
** Name:        wxdemo.h
** Purpose:     wxWidgets Demo sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef WXDEMO_H_
#define WXDEMO_H_

/*!
 * Includes
 */

#include <wx/listctrl.h>
#include <wx/treectrl.h>

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_WXDEMO         10000
#define ID_TREECTRL       10001
#define ID_SCROLLEDWINDOW 10002
#define ID_CHARTVIEWER    10003
#define ID_SHOWMODAL      10004
#define SYMBOL_WXDEMO_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_WXDEMO_TITLE _("ChartDirector wxWidgets Demo")
#define SYMBOL_WXDEMO_IDNAME ID_WXDEMO
#define SYMBOL_WXDEMO_SIZE wxSize(800, 500)
#define SYMBOL_WXDEMO_POSITION wxDefaultPosition

const int noOfChartViewers = 8;

/*!
 * WxDemo class declaration
 */

class WxDemo: public wxFrame
{    
  DECLARE_DYNAMIC_CLASS( WxDemo )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  WxDemo();
  WxDemo( wxWindow* parent, wxWindowID id = SYMBOL_WXDEMO_IDNAME, const wxString& caption = SYMBOL_WXDEMO_TITLE, const wxPoint& pos = SYMBOL_WXDEMO_POSITION, const wxSize& size = SYMBOL_WXDEMO_SIZE, long style = SYMBOL_WXDEMO_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WXDEMO_IDNAME, const wxString& caption = SYMBOL_WXDEMO_TITLE, const wxPoint& pos = SYMBOL_WXDEMO_POSITION, const wxSize& size = SYMBOL_WXDEMO_SIZE, long style = SYMBOL_WXDEMO_STYLE );

  /// Destructor
  ~WxDemo();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void OnChartClicked(wxCommandEvent& event);

  /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_LISTBOX
  void OnTreeCtrlSelected(wxTreeEvent& event);

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_WXDEMO
  void OnCloseWindow(wxCloseEvent& event);

  void OnShowModalDemo(wxCommandEvent& event);

  /// Should we show tooltips?
  static bool ShowToolTips();

  wxTreeCtrl* m_demoTree;
  wxScrolledWindow* m_scrolledWindow;
  wxGridSizer* m_gridSizer;
  wxChartViewer* m_chartViewer[noOfChartViewers];
};

#endif
    // WXDEMO_H_
