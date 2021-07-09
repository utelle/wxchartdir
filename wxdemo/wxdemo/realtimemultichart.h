/*
** Name:        realtimemultichart.h
** Purpose:     Realtime Chart with Zoom/Scroll and Track Line
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEMULTICHART_H_
#define REALTIMEMULTICHART_H_

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

#define SYMBOL_REALTIMEMULTICHART_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMEMULTICHART_TITLE _("Real-Time Multi Chart with Zoom/Scroll and Track Line")
#define SYMBOL_REALTIMEMULTICHART_IDNAME ID_REALTIMEZOOMSCROLL
#define SYMBOL_REALTIMEMULTICHART_SIZE wxSize(800, 500)
#define SYMBOL_REALTIMEMULTICHART_POSITION wxDefaultPosition

/*!
 * RealTimeMultiChart class declaration
 */

class RealTimeMultiChart : public wxDialog
{    
  DECLARE_CLASS( RealTimeZoomScroll )
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_REALTIMEZOOMSCROLL = 10000,
    ID_CHARTVIEWER,
    ID_POINTER,
    ID_SCROLLBAR,
    ID_UPDATE_PERIOD,
    ID_DATA_TIMER,
    ID_UPDATE_TIMER
  };

  /// Constructors
  RealTimeMultiChart();
  RealTimeMultiChart( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEMULTICHART_IDNAME, const wxString& caption = SYMBOL_REALTIMEMULTICHART_TITLE, const wxPoint& pos = SYMBOL_REALTIMEMULTICHART_POSITION, const wxSize& size = SYMBOL_REALTIMEMULTICHART_SIZE, long style = SYMBOL_REALTIMEMULTICHART_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEMULTICHART_IDNAME, const wxString& caption = SYMBOL_REALTIMEMULTICHART_TITLE, const wxPoint& pos = SYMBOL_REALTIMEMULTICHART_POSITION, const wxSize& size = SYMBOL_REALTIMEMULTICHART_SIZE, long style = SYMBOL_REALTIMEMULTICHART_STYLE );

  /// Destructor
  ~RealTimeMultiChart();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Draw chart
  XYChart* DrawXYChart(wxChartViewer* viewer, const double* dataSeries, const char* name, int color, Axis* xAxisScale, bool hasXAxis);
  void DrawMultiChart(wxChartViewer* viewer);
  void DeleteMultiChart(MultiChart* m);

  // Draw track line
  void DrawMultiTrackLine(MultiChart* m, int mouseX);
  void DrawXYTrackLine(DrawArea* d, XYChart* c, int mouseX, bool hasXAxis);

  void UpdateControls(wxChartViewer* viewer);      // Update other controls as viewport changes

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick(wxCommandEvent& event);

  void OnSave(wxCommandEvent& event);

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UPDATE_PERIOD
  void OnUpdatePeriodSelected( wxCommandEvent& event );

  void OnMouseUsageChanged(int mouseUsage);

  void OnViewPortChanged(wxCommandEvent& event);

  void OnScrollBarChanged(int value);

  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnUpdatePeriodChanged(const wxString& value);// The chart update timer interval has changed.

  void OnDataTimer(wxTimerEvent& event);
  void OnChartUpdateTimer(wxTimerEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxColour        m_bgColour;
  wxScrollBar*    m_scrollBar;

  wxChoice* m_updatePeriod;
  wxTextCtrl* m_alphaValue;                   // Label to display the realtime value A
  wxTextCtrl* m_betaValue;                   // Label to display the realtime value B
  wxTextCtrl* m_gammaValue;                   // Label to display the realtime value C

  wxTimer* m_dataRateTimer;
  wxTimer* m_chartUpdateTimer;
    
  // The number of samples per data series used in this demo
  static const int sampleSize = 10000;

  // The initial full range is set to 60 seconds of data.
  static const int initialFullRange = 60;

  // The maximum zoom in is 10 seconds.
  static const int zoomInLimit = 10;

  double m_timeStamps[sampleSize];	// The timestamps for the data series
  double m_dataSeriesA[sampleSize];	// The values for the data series A
  double m_dataSeriesB[sampleSize];	// The values for the data series B
  double m_dataSeriesC[sampleSize];	// The values for the data series C

  int m_currentIndex;                 // Index of the array position to which new values are added.

  wxDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

  wxChartViewer* m_chartViewer;
};

#endif
    // REALTIMEMULTICHART_H_
