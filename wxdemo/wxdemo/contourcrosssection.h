/*
** Name:        contourcrosssection.h
** Purpose:     Contour Plot with Cross Section
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef CONTOURCROSSSECTION_H_
#define CONTOURCROSSSECTION_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_CONTOURCROSSSECTION_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CONTOURCROSSSECTION_TITLE _("Contour Plot with Cross Section")
#define SYMBOL_CONTOURCROSSSECTION_IDNAME ID_CROSSHAIR
#define SYMBOL_CONTOURCROSSSECTION_SIZE wxSize(400, 300)
#define SYMBOL_CONTOURCROSSSECTION_POSITION wxDefaultPosition

/*!
 * ContourCrossSection class declaration
 */

class ContourCrossSection : public wxDialog
{    
    DECLARE_DYNAMIC_CLASS(ContourCrossSection)
    DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_CROSSHAIR = 10000,
    ID_CHARTVIEWER,
    ID_CROSSSECTION_CHARTVIEWER_X,
    ID_CROSSSECTION_CHARTVIEWER_Y
  };

  /// Constructors
  ContourCrossSection();
  ContourCrossSection( wxWindow* parent, wxWindowID id = SYMBOL_CONTOURCROSSSECTION_IDNAME, const wxString& caption = SYMBOL_CONTOURCROSSSECTION_TITLE, const wxPoint& pos = SYMBOL_CONTOURCROSSSECTION_POSITION, const wxSize& size = SYMBOL_CONTOURCROSSSECTION_SIZE, long style = SYMBOL_CONTOURCROSSSECTION_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONTOURCROSSSECTION_IDNAME, const wxString& caption = SYMBOL_CONTOURCROSSSECTION_TITLE, const wxPoint& pos = SYMBOL_CONTOURCROSSSECTION_POSITION, const wxSize& size = SYMBOL_CONTOURCROSSSECTION_SIZE, long style = SYMBOL_CONTOURCROSSSECTION_STYLE );

  /// Destructor
  ~ContourCrossSection();

  /// Initializes member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Keep track of mouse dragging state
  enum DragState { Drag_None, Drag_Vertical, Drag_Horizontal };
  DragState m_isDragging;
  int m_dragOffset;
  int m_xTrackPosition;
  int m_yTrackPosition;

  // Draw chart, cross section  and crosshair
  void DrawChart(wxChartViewer* viewer);
  void DrawCrossSectionX(int x);
  void DrawCrossSectionX(wxChartViewer* viewer, XYChart* mainChart, ContourLayer* layer, int x);
  void DrawCrossSectionY(int y);
  void DrawCrossSectionY(wxChartViewer* viewer, XYChart* mainChart, ContourLayer* layer, int y);
  void DrawCrossHair(wxChartViewer* viewer);

  /// Draw Track Cursor
  void Crosshair(XYChart* c, int mouseX, int mouseY);
  
  // Event handler declarations

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_CROSSHAIR
  void OnCloseWindow(wxCloseEvent& event);

  void OnMouseMovePlotArea(wxCommandEvent& event);

  wxChartViewer* m_chartViewer;
  wxChartViewer* m_crossSectionViewerX;
  wxChartViewer* m_crossSectionViewerY;
  ContourLayer* m_contourLayer;

};

#endif
    // CONTOURCROSSSECTION_H_
