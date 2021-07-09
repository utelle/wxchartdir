/*
** Name:        financedemo.h
** Purpose:     Financial Chart Demonstration
** Author:      Ulrich Telle
** Created:     2018-07-01
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef FINANCEDEMO_H_
#define FINANCEDEMO_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"

#include "wxchartviewer.h"

/*!
 * Control identifiers
 */

#define SYMBOL_FINANCEDEMO_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FINANCEDEMO_TITLE _("ChartDirector Interactive Financial Chart Demonstration")
#define SYMBOL_FINANCEDEMO_IDNAME ID_FINANCEDEMO
#define SYMBOL_FINANCEDEMO_SIZE wxSize(1000, 575)
#define SYMBOL_FINANCEDEMO_POSITION wxDefaultPosition

/*!
 * FinanceDemo class declaration
 */

class FinanceDemo : public wxDialog
{    
  DECLARE_CLASS( FinanceDemo )
  DECLARE_EVENT_TABLE()

public:
  /// Control identifiers
  enum {
    ID_FINANCEDEMO = 10000,
    ID_POINTER,
    ID_TICKER_SYMBOL,
    ID_COMPARE_WITH,
    ID_TIME_PERIOD,
    ID_CHART_SIZE,
    ID_SHOW_VOLUME_BAR,
    ID_PARABOLIC_SAR,
    ID_LOG_SCALE,
    ID_PERCENTAGE_SCALE,
    ID_CHART_TYPE,
    ID_PRICE_BAND,
    ID_MOVING_AVG1,
    ID_VALUE_AVG1,
    ID_MOVING_AVG2,
    ID_VALUE_AVG2,
    ID_TECH_INDICATOR1,
    ID_TECH_INDICATOR2,
    ID_CHARTVIEWER
  };

  /// Constructors
  FinanceDemo();
  FinanceDemo( wxWindow* parent, wxWindowID id = SYMBOL_FINANCEDEMO_IDNAME, const wxString& caption = SYMBOL_FINANCEDEMO_TITLE, const wxPoint& pos = SYMBOL_FINANCEDEMO_POSITION, const wxSize& size = SYMBOL_FINANCEDEMO_SIZE, long style = SYMBOL_FINANCEDEMO_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FINANCEDEMO_IDNAME, const wxString& caption = SYMBOL_FINANCEDEMO_TITLE, const wxPoint& pos = SYMBOL_FINANCEDEMO_POSITION, const wxSize& size = SYMBOL_FINANCEDEMO_SIZE, long style = SYMBOL_FINANCEDEMO_STYLE );

  /// Destructor
  ~FinanceDemo();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Event handler declarations

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TICKER_SYMBOL
  void OnTickerSymbolEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_COMPARE_WITH
  void OnCompareWithEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIME_PERIOD
  void OnTimePeriodSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_SIZE
  void OnChartSizeSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SHOW_VOLUME_BAR
  void OnShowVolumeBarClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PARABOLIC_SAR
  void OnParabolicSarClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LOG_SCALE
  void OnLogScaleClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PERCENTAGE_SCALE
  void OnPercentageScaleClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_TYPE
  void OnChartTypeSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PRICE_BAND
  void OnPriceBandSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOVING_AVG1/2
  void OnMovingAvgSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_VALUE_AVG1/2
  void OnMovingAvgUpdated( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TECH_INDICATOR 1/2
  void OnTechIndicatorSelected( wxCommandEvent& event );

  // Member function declarations

  void OnMouseUsageChanged(int mouseUsage);

  void OnMouseMovePlotArea(wxCommandEvent& event);
  void OnViewPortChanged(wxCommandEvent& event);

  void OnLineEditChanged();

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  virtual void DrawChart(wxChartViewer* viewer);

  void TrackFinance(MultiChart* m, int mouseX);

private:
  struct PriceData
  {
    std::vector<double> timeStamps;
    std::vector<double> highData;
    std::vector<double> lowData;
    std::vector<double> openData;
    std::vector<double> closeData;
    std::vector<double> volData;

    // An extra field for price comparison
    std::vector<double> compareData;
  };

  // Utilities to aggregate data if the data source do not already have aggregated data
  virtual void AggregateData(const ArrayMath& aggregator, PriceData& p);

  // Routines to get data into the data arrays
  void loadData(const wxString& ticker, const wxString& compare);

  // Utility to convert a std::vector to a DoubleArray
  DoubleArray vectorToArray(const std::vector<double>& v, int startIndex = 0, int length = -1);
  // utility to convert std::vector to a DoubleArray
  std::vector<double> arrayToVector(DoubleArray a);

  // The main ticker key and the compare to ticker key
  wxString m_tickerKey;
  wxString m_compareKey;

  // In this example, we can use daily, weekly or monthly data depending on the time range
  PriceData m_dailyPrice;
  PriceData m_weeklyPrice;
  PriceData m_monthlyPrice;

  // The resolution of the data in seconds. 1 day = 86400 seconds.
  int m_resolution;

  // The moving average periods
  int m_avgPeriod1;
  int m_avgPeriod2;

  wxArrayString m_timePeriodStrings;
  wxArrayInt    m_timePeriods;
  wxArrayString m_chartTypeStrings;
  wxArrayString m_chartTypes;
  wxArrayString m_priceBandStrings;
  wxArrayString m_priceBands;
  wxArrayString m_movingAvgStrings;
  wxArrayString m_movingAvgs;
  wxArrayString m_techIndicatorStrings;
  wxArrayString m_techIndicators;

  // The user interface

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;

  wxTextCtrl* m_tickerSymbol;
  wxTextCtrl* m_compareWith;
  wxChoice* m_timeRange;
  wxCheckBox* m_volumeBars;
  wxCheckBox* m_parabolicSar;
  wxCheckBox* m_logScale;
  wxCheckBox* m_percentageScale;
  wxChoice* m_chartType;
  wxChoice* m_priceBand;
  wxChoice* m_avgType1;
  wxTextCtrl* m_movAvg1;
  wxChoice* m_avgType2;
  wxTextCtrl* m_movAvg2;
  wxChoice* m_techIndicator1;
  wxChoice* m_techIndicator2;

  int m_indicator1;
  int m_indicator2;

  wxChartViewer* m_chartViewer;
};

#endif
    // FINANCEDEMO_H_
