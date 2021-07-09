/*
** Name:        tracklabel.h
** Purpose:     Track Label sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKLABEL_H_
#define TRACKLABEL_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_TRACKLABEL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACKLABEL_TITLE _("Track Line with Data Labels")
#define SYMBOL_TRACKLABEL_IDNAME ID_TRACKLABEL
#define SYMBOL_TRACKLABEL_SIZE wxSize(400, 300)
#define SYMBOL_TRACKLABEL_POSITION wxDefaultPosition

/*!
 * TrackLabel class declaration
 */

class TrackLabel : public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( TrackLabel )
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_TRACKLABEL = 10000,
    ID_CHARTVIEWER
  };

  /// Constructors
  TrackLabel();
  TrackLabel( wxWindow* parent, wxWindowID id = SYMBOL_TRACKLABEL_IDNAME, const wxString& caption = SYMBOL_TRACKLABEL_TITLE, const wxPoint& pos = SYMBOL_TRACKLABEL_POSITION, const wxSize& size = SYMBOL_TRACKLABEL_SIZE, long style = SYMBOL_TRACKLABEL_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACKLABEL_IDNAME, const wxString& caption = SYMBOL_TRACKLABEL_TITLE, const wxPoint& pos = SYMBOL_TRACKLABEL_POSITION, const wxSize& size = SYMBOL_TRACKLABEL_SIZE, long style = SYMBOL_TRACKLABEL_STYLE );

  /// Destructor
  ~TrackLabel();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);          // Draw chart
  void TrackLineLabel(XYChart* c, int mouseX);    // Draw Track Cursor
  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_TRACKLABEL
  void OnCloseWindow(wxCloseEvent& event);

  wxChartViewer* m_chartViewer;
};

#endif
    // TRACKLABEL_H_
