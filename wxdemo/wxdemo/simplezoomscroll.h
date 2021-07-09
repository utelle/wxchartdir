/*
** Name:        simplezoomscroll.h
** Purpose:     Simple Zooming and Scrolling
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef SIMPLEZOOMSCROLL_H_
#define SIMPLEZOOMSCROLL_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_SIMPLEZOOMSCROLL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SIMPLEZOOMSCROLL_TITLE _("Simple Zooming and Scrolling")
#define SYMBOL_SIMPLEZOOMSCROLL_IDNAME ID_SIMPLEZOOMSCROLL
#define SYMBOL_SIMPLEZOOMSCROLL_SIZE wxSize(775, 350)
#define SYMBOL_SIMPLEZOOMSCROLL_POSITION wxDefaultPosition

/*!
 * SimpleZoomScroll class declaration
 */

class SimpleZoomScroll : public wxDialog
{    
  DECLARE_CLASS( SimpleZoomScroll )
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_SIMPLEZOOMSCROLL = 10000,
    ID_CHARTVIEWER,
    ID_POINTER
  };

  /// Constructors
  SimpleZoomScroll();
  SimpleZoomScroll( wxWindow* parent, wxWindowID id = SYMBOL_SIMPLEZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_SIMPLEZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_SIMPLEZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_SIMPLEZOOMSCROLL_SIZE, long style = SYMBOL_SIMPLEZOOMSCROLL_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SIMPLEZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_SIMPLEZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_SIMPLEZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_SIMPLEZOOMSCROLL_SIZE, long style = SYMBOL_SIMPLEZOOMSCROLL_STYLE );

  /// Destructor
  ~SimpleZoomScroll();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays
  void InitChartViewer(wxChartViewer* viewer);    // Initialize the wxChartViewer
  void DrawChart(wxChartViewer* viewer);          // Draw chart
  void UpdateImageMap(wxChartViewer* viewer);     // Update the image map

  void OnMouseUsageChanged(int mouseUsage);

  void OnViewPortChanged(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnMouseMovePlotArea(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxColour        m_bgColour;

  // Data arrays for the scrollable / zoomable chart.
  // - In this demo, we just use a RanTable object to generate random data for the chart.
  RanSeries*  m_ranSeries;
  DoubleArray m_timeStamps;
  DoubleArray m_dataSeriesA;
  DoubleArray m_dataSeriesB;
  DoubleArray m_dataSeriesC;

  wxChartViewer* m_chartViewer;
};

#endif
    // SIMPLEZOOMSCROLL_H_
