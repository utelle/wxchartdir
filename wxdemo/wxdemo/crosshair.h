/*
** Name:        crosshair.h
** Purpose:     Crosshair with Axis Labels sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef CROSSHAIR_H_
#define CROSSHAIR_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_CROSSHAIR_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CROSSHAIR_TITLE _("Crosshair with Axis Labels")
#define SYMBOL_CROSSHAIR_IDNAME ID_CROSSHAIR
#define SYMBOL_CROSSHAIR_SIZE wxSize(400, 300)
#define SYMBOL_CROSSHAIR_POSITION wxDefaultPosition

/*!
 * CrossHair class declaration
 */

class CrossHair: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( CrossHair )
    DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_CROSSHAIR = 10000,
    ID_CHARTVIEWER
  };

  /// Constructors
  CrossHair();
  CrossHair( wxWindow* parent, wxWindowID id = SYMBOL_CROSSHAIR_IDNAME, const wxString& caption = SYMBOL_CROSSHAIR_TITLE, const wxPoint& pos = SYMBOL_CROSSHAIR_POSITION, const wxSize& size = SYMBOL_CROSSHAIR_SIZE, long style = SYMBOL_CROSSHAIR_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CROSSHAIR_IDNAME, const wxString& caption = SYMBOL_CROSSHAIR_TITLE, const wxPoint& pos = SYMBOL_CROSSHAIR_POSITION, const wxSize& size = SYMBOL_CROSSHAIR_SIZE, long style = SYMBOL_CROSSHAIR_STYLE );

  /// Destructor
  ~CrossHair();

  /// Initializes member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  /// Draw chart
  void DrawChart(wxChartViewer* viewer);
  
  /// Draw Track Cursor
  void Crosshair(XYChart* c, int mouseX, int mouseY);
  
  // Event handler declarations

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_CROSSHAIR
  void OnCloseWindow(wxCloseEvent& event);

  void OnMouseMovePlotArea(wxCommandEvent& event);

  wxChartViewer* m_chartViewer;
};

#endif
    // CROSSHAIR_H_
