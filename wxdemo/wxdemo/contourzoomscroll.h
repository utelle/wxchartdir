/*
** Name:        contourzoomscrollapp.cpp
** Purpose:     Contour Plot Zooming and Scrolling
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef CONTOURZOOMSCROLL_H_
#define CONTOURZOOMSCROLL_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"

#include "wxchartviewer.h"

#include <vector>

/*!
 * Control identifiers
 */

#define SYMBOL_CONTOURZOOMSCROLL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CONTOURZOOMSCROLL_TITLE _("Contour Plot Zooming and Scrolling")
#define SYMBOL_CONTOURZOOMSCROLL_IDNAME ID_CONTOURZOOMSCROLL
#define SYMBOL_CONTOURZOOMSCROLL_SIZE wxSize(650, 520)
#define SYMBOL_CONTOURZOOMSCROLL_POSITION wxDefaultPosition

/*!
 * ContourZoomScroll class declaration
 */

class ContourZoomScroll : public wxDialog
{    
  DECLARE_CLASS( ContourZoomScroll )
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_CONTOURZOOMSCROLL = 10000,
    ID_CHARTVIEWER,
    ID_POINTER,
    ID_SLIDER,
    ID_VIEWPORT
  };

  /// Constructors
  ContourZoomScroll();
  ContourZoomScroll( wxWindow* parent, wxWindowID id = SYMBOL_CONTOURZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_CONTOURZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_CONTOURZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_CONTOURZOOMSCROLL_SIZE, long style = SYMBOL_CONTOURZOOMSCROLL_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONTOURZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_CONTOURZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_CONTOURZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_CONTOURZOOMSCROLL_SIZE, long style = SYMBOL_CONTOURZOOMSCROLL_STYLE );

  /// Destructor
  ~ContourZoomScroll();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Load data into data arrays
  void LoadData();

  // Draw chart
  void DrawChart(wxChartViewer* viewer);
  void DrawFullChart(wxViewPortControl* vpc, wxChartViewer* viewer);

  // Update image map
  void UpdateImageMap(wxChartViewer* viewer);

  // Update other controls when viewport changed
  void UpdateControls(wxChartViewer* viewer);

  // utility to convert std::vector to a DoubleArray
  DoubleArray vectorToArray(std::vector<double> &v);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnSave(wxCommandEvent& event);

  /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
  void OnSliderUpdated( wxCommandEvent& event );

  void OnZoomBarChanged(int value);

  void OnMouseUsageChanged(int mouseUsage);
  void OnViewPortChanged(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxColour        m_bgColour;
  wxSlider*       m_zoomBar;

  // Data arrays for the scrollable / zoomable chart.
  std::vector<double> dataX;
  std::vector<double> dataY;
  std::vector<double> dataZ;

  // To ensure color consistency, we need to save the first color axis and
  // synchronize the color axis of the zoom-in charts with the this axis.
  XYChart* firstChart;
  ColorAxis* firstColorAxis;

  wxChartViewer* m_chartViewer;
  wxViewPortControl* m_viewPortControl;
};

#endif
    // CONTOURZOOMSCROLL_H_
