/*
** Name:        zoomscrollpdf.h
** Purpose:     PDF Report Demonstration
** Author:      Ulrich Telle
** Created:     2021-06-25
** Copyright:   (C) 2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef ZOOMSCROLLPDF_H_
#define ZOOMSCROLLPDF_H_

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

#define SYMBOL_ZOOMSCROLLPDF_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ZOOMSCROLLPDF_TITLE _("PDF Report Demonstration")
#define SYMBOL_ZOOMSCROLLPDF_IDNAME ID_ZOOMSCROLLPDF
#define SYMBOL_ZOOMSCROLLPDF_SIZE wxSize(800, 420)
#define SYMBOL_ZOOMSCROLLPDF_POSITION wxDefaultPosition

/*!
 * ZoomScrollTrack class declaration
 */

class ZoomScrollPdf : public wxDialog
{    
  DECLARE_CLASS( ZoomScrollPdf )
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_ZOOMSCROLLPDF = 10000,
    ID_CHARTVIEWER,
    ID_POINTER,
    ID_PDF,
    ID_SCROLLBAR
  };

  /// Constructors
  ZoomScrollPdf();
  ZoomScrollPdf( wxWindow* parent, wxWindowID id = SYMBOL_ZOOMSCROLLPDF_IDNAME, const wxString& caption = SYMBOL_ZOOMSCROLLPDF_TITLE, const wxPoint& pos = SYMBOL_ZOOMSCROLLPDF_POSITION, const wxSize& size = SYMBOL_ZOOMSCROLLPDF_SIZE, long style = SYMBOL_ZOOMSCROLLPDF_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ZOOMSCROLLPDF_IDNAME, const wxString& caption = SYMBOL_ZOOMSCROLLPDF_TITLE, const wxPoint& pos = SYMBOL_ZOOMSCROLLPDF_POSITION, const wxSize& size = SYMBOL_ZOOMSCROLLPDF_SIZE, long style = SYMBOL_ZOOMSCROLLPDF_STYLE );

  /// Destructor
  ~ZoomScrollPdf();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays
  void InitChartViewer(wxChartViewer* viewer);    // Initialize the wxChartViewer
  void UpdateControls(wxChartViewer* viewer);
  void DrawChart(wxChartViewer* viewer);          // Draw chart
  XYChart* DrawXYChart(double startX, double endX);   // Draw chart in given x range
  void TrackLineLabel(XYChart* c, int mouseX);       // Draw Track Cursor
  void CreatePdfReport(const char* filename);         // Generate PDF report

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnSave(wxCommandEvent& event);
  void OnPdf(wxCommandEvent& event);

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
  wxButton*       m_saveButton;
  wxButton*       m_pdfButton;
  wxColour        m_bgColour;
  wxScrollBar*    m_scrollBar;

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
    // ZOOMSCROLLPDF_H_
