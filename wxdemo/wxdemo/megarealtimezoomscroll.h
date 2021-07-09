/*
** Name:        megarealtimezoomscroll.h
** Purpose:     Mega Real-Time Chart with Zoom/Scroll with Track Line
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef MEGAREALTIMEZOOMSCROLL_H_
#define MEGAREALTIMEZOOMSCROLL_H_

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

#define SYMBOL_MEGAREALTIMEZOOMSCROLL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_MEGAREALTIMEZOOMSCROLL_TITLE _("Mega Chart Zoom/Scroll with Track Line")
#define SYMBOL_MEGAREALTIMEZOOMSCROLL_IDNAME ID_ZOOMSCROLLTRACK
#define SYMBOL_MEGAREALTIMEZOOMSCROLL_SIZE wxSize(800, 420)
#define SYMBOL_MEGAREALTIMEZOOMSCROLL_POSITION wxDefaultPosition

/*!
 * MegaRealTimeZoomScroll class declaration
 */

class MegaRealTimeZoomScroll : public wxDialog
{    
  DECLARE_CLASS(MegaRealTimeZoomScroll)
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_ZOOMSCROLLTRACK = 10000,
    ID_CHARTVIEWER,
    ID_POINTER,
    ID_START,
    ID_DATA_TIMER,
    ID_UPDATE_TIMER,
    ID_SCROLLBAR
  };

  /// Constructors
  MegaRealTimeZoomScroll();
  MegaRealTimeZoomScroll( wxWindow* parent, wxWindowID id = SYMBOL_MEGAREALTIMEZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_MEGAREALTIMEZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_MEGAREALTIMEZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_MEGAREALTIMEZOOMSCROLL_SIZE, long style = SYMBOL_MEGAREALTIMEZOOMSCROLL_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MEGAREALTIMEZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_MEGAREALTIMEZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_MEGAREALTIMEZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_MEGAREALTIMEZOOMSCROLL_SIZE, long style = SYMBOL_MEGAREALTIMEZOOMSCROLL_STYLE );

  /// Destructor
  ~MegaRealTimeZoomScroll();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays

  bool getRealTimeData();
  // Update the axis scale to accomodate new data
  void updateAxisScale(wxChartViewer* viewer);



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

  void OnChartUpdateTimer(wxTimerEvent& event);

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

  // One real-time random number generator
  RanSeries m_realTimeData;
  wxDateTime m_lastUpdateTime;

  wxTimer* m_chartUpdateTimer;

  // The index position that new data will be added.
  int m_currentIndex;

  // Flag to indicate the chart has been drawn so the zoom/scroll and track cursor
  // event handlers can run.
  bool m_hasFinishedInitialization;

  // Data Accelerator for handle large data sets
  DataAccelerator* m_fastData;

  wxChartViewer* m_chartViewer;
};

#endif
    // MEGAREALTIMEZOOMSCROLL_H_
