/*
** Name:        threedchartrotation.h
** Purpose:     3D Chart Interactive Rotation
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef THREEDCHARTROTATION_H_
#define THREEDCHARTROTATION_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_THREEDCHARTROTATION_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_THREEDCHARTROTATION_TITLE _("3D Chart Interactive Rotation")
#define SYMBOL_THREEDCHARTROTATION_IDNAME ID_THREEDCHARTROTATION
#define SYMBOL_THREEDCHARTROTATION_SIZE wxSize(400, 300)
#define SYMBOL_THREEDCHARTROTATION_POSITION wxDefaultPosition

/*!
 * ThreeDChartRotation class declaration
 */

class ThreeDChartRotation : public wxDialog
{    
    DECLARE_DYNAMIC_CLASS(ThreeDChartRotation)
    DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_THREEDCHARTROTATION = 10000,
    ID_CHARTVIEWER,
    ID_DRAWFRAME
  };

  /// Constructors
  ThreeDChartRotation();
  ThreeDChartRotation( wxWindow* parent, wxWindowID id = SYMBOL_THREEDCHARTROTATION_IDNAME, const wxString& caption = SYMBOL_THREEDCHARTROTATION_TITLE, const wxPoint& pos = SYMBOL_THREEDCHARTROTATION_POSITION, const wxSize& size = SYMBOL_THREEDCHARTROTATION_SIZE, long style = SYMBOL_THREEDCHARTROTATION_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_THREEDCHARTROTATION_IDNAME, const wxString& caption = SYMBOL_THREEDCHARTROTATION_TITLE, const wxPoint& pos = SYMBOL_THREEDCHARTROTATION_POSITION, const wxSize& size = SYMBOL_THREEDCHARTROTATION_SIZE, long style = SYMBOL_THREEDCHARTROTATION_STYLE );

  /// Destructor
  ~ThreeDChartRotation();

  /// Initializes member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // 3D view angles
  double m_elevationAngle;
  double m_rotationAngle;

  // Keep track of mouse drag
  int m_lastMouseX;
  int m_lastMouseY;
  bool m_isDragging;

  // Draw chart, cross section  and crosshair
  void DrawChart(wxChartViewer* viewer);

  // Event handler declarations

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_CROSSHAIR
  void OnCloseWindow(wxCloseEvent& event);

  void OnClickChart(wxCommandEvent& event);
  void OnViewPortChanged(wxCommandEvent& event);
  void OnMouseMoveChart(wxCommandEvent& event);

  wxChartViewer* m_chartViewer;
  wxCheckBox* m_drawFrameOnRotate;
};

#endif
    // THREEDCHARTROTATION_H_
