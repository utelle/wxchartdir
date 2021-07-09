/*
** Name:        megazoomscroll.h
** Purpose:     Mega Chart Zoom/Scroll with Track Line
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef MEGAZOOMSCROLL_H_
#define MEGAZOOMSCROLL_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/scrolbar.h"

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_MEGAZOOMSCROLL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MEGAZOOMSCROLL_TITLE _("Mega Chart Zoom/Scroll with Track Line")
#define SYMBOL_MEGAZOOMSCROLL_IDNAME ID_ZOOMSCROLLTRACK
#define SYMBOL_MEGAZOOMSCROLL_SIZE wxSize(800, 420)
#define SYMBOL_MEGAZOOMSCROLL_POSITION wxDefaultPosition

/*!
 * MegaZoomScroll class declaration
 */

class MegaZoomScroll : public wxDialog
{    
  DECLARE_CLASS(MegaZoomScroll)
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_ZOOMSCROLLTRACK = 10000,
    ID_CHARTVIEWER,
    ID_POINTER,
    ID_START,
    ID_SCROLLBAR
  };

  /// Constructors
  MegaZoomScroll();
  MegaZoomScroll( wxWindow* parent, wxWindowID id = SYMBOL_MEGAZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_MEGAZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_MEGAZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_MEGAZOOMSCROLL_SIZE, long style = SYMBOL_MEGAZOOMSCROLL_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MEGAZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_MEGAZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_MEGAZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_MEGAZOOMSCROLL_SIZE, long style = SYMBOL_MEGAZOOMSCROLL_STYLE );

  /// Destructor
  ~MegaZoomScroll();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays
  void InitChartViewer(wxChartViewer* viewer);    // Initialize the wxChartViewer
  void UpdateControls(wxChartViewer* viewer);
  void DrawChart(wxChartViewer* viewer);          // Draw chart
  void TrackLineLabel(XYChart *c, int mouseX);   // Draw Track Cursor

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_START
  void OnPlotChart(wxCommandEvent& event);

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  void OnMouseUsageChanged(int mouseUsage);
  void OnViewPortChanged(wxCommandEvent& event);
  void OnScrollBarChanged(int value);
  void OnMouseWheelChart(wxCommandEvent& event);
  void OnMouseMovePlotArea(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_startButton;
  wxColour        m_bgColour;
  wxScrollBar*    m_scrollBar;

  // Data arrays
  std::vector<double> m_dataSeriesA;
  std::vector<double> m_dataSeriesB;
  std::vector<double> m_dataSeriesC;
  std::vector<double> m_timeStamps;

  // Flag to indicate the chart has been drawn so the zoom/scroll and track cursor
  // event handlers can run.
  bool m_hasFinishedInitialization;

  // Data Accelerator for handle large data sets
  DataAccelerator* m_fastData;

  wxChartViewer* m_chartViewer;
};

#endif
    // MEGAZOOMSCROLL_H_
