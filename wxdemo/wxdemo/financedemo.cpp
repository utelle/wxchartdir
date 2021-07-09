/*
** Name:        financedemo.cpp
** Purpose:     Financial Chart Demonstration
** Author:      Ulrich Telle
** Created:     2018-07-01
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <sstream> 

#include "financedemo.h"
#include "FinanceChart.h"

#include "mondrian.xpm"

/*
 * FinanceDemo type definition
 */

IMPLEMENT_CLASS( FinanceDemo, wxDialog )

/*
 * FinanceDemo event table definition
 */

BEGIN_EVENT_TABLE( FinanceDemo, wxDialog )

  EVT_TOGGLEBUTTON(ID_POINTER, FinanceDemo::OnPointerClick)
  EVT_TOGGLEBUTTON(wxID_ZOOM_IN, FinanceDemo::OnZoomInClick)
  EVT_TOGGLEBUTTON(wxID_ZOOM_OUT, FinanceDemo::OnZoomOutClick)

  EVT_TEXT_ENTER( ID_TICKER_SYMBOL, FinanceDemo::OnTickerSymbolEnter )
  EVT_TEXT_ENTER( ID_COMPARE_WITH, FinanceDemo::OnCompareWithEnter )
  EVT_CHOICE( ID_TIME_PERIOD, FinanceDemo::OnTimePeriodSelected )
  EVT_CHECKBOX( ID_SHOW_VOLUME_BAR, FinanceDemo::OnShowVolumeBarClick )
  EVT_CHECKBOX( ID_PARABOLIC_SAR, FinanceDemo::OnParabolicSarClick )
  EVT_CHECKBOX( ID_LOG_SCALE, FinanceDemo::OnLogScaleClick )
  EVT_CHECKBOX( ID_PERCENTAGE_SCALE, FinanceDemo::OnPercentageScaleClick )
  EVT_CHOICE( ID_CHART_TYPE, FinanceDemo::OnChartTypeSelected )
  EVT_CHOICE( ID_PRICE_BAND, FinanceDemo::OnPriceBandSelected )
  EVT_CHOICE( ID_MOVING_AVG1, FinanceDemo::OnMovingAvgSelected )
  EVT_CHOICE( ID_MOVING_AVG2, FinanceDemo::OnMovingAvgSelected)
  EVT_TEXT_ENTER( ID_VALUE_AVG1, FinanceDemo::OnMovingAvgUpdated)
  EVT_TEXT_ENTER( ID_VALUE_AVG2, FinanceDemo::OnMovingAvgUpdated )
  EVT_CHOICE( ID_TECH_INDICATOR1, FinanceDemo::OnTechIndicatorSelected )
  EVT_CHOICE( ID_TECH_INDICATOR2, FinanceDemo::OnTechIndicatorSelected )

  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, FinanceDemo::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, FinanceDemo::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * FinanceDemo constructors
 */

FinanceDemo::FinanceDemo()
{
  Init();
}

FinanceDemo::FinanceDemo( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * FinanceDemo creator
 */

bool
FinanceDemo::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  SetInitialSize();
  Centre();

  return true;
}

/*
 * FinanceDemo destructor
 */

FinanceDemo::~FinanceDemo()
{
  delete m_chartViewer->getChart();
}

/*
 * Member initialisation
 */

void
FinanceDemo::Init()
{
  m_tickerSymbol = NULL;
  m_compareWith = NULL;
  m_timeRange = NULL;
  m_volumeBars = NULL;
  m_parabolicSar = NULL;
  m_logScale = NULL;
  m_percentageScale = NULL;
  m_chartType = NULL;
  m_priceBand = NULL;
  m_avgType1 = NULL;
  m_movAvg1 = NULL;
  m_avgType2 = NULL;
  m_movAvg2 = NULL;
  m_techIndicator1 = NULL;
  m_techIndicator2 = NULL;

  // Initialize member variables

  m_resolution = 86400;

  m_timePeriodStrings.Add(_("1 month"));
  m_timePeriodStrings.Add(_("2 months"));
  m_timePeriodStrings.Add(_("3 months"));
  m_timePeriodStrings.Add(_("6 months"));
  m_timePeriodStrings.Add(_("1 year"));
  m_timePeriodStrings.Add(_("2 years"));
  m_timePeriodStrings.Add(_("3 years"));
  m_timePeriodStrings.Add(_("4 years"));
  m_timePeriodStrings.Add(_("5 years"));
  m_timePeriodStrings.Add(_("10 years"));

  m_timePeriods.Add(30);
  m_timePeriods.Add(60);
  m_timePeriods.Add(90);
  m_timePeriods.Add(180);
  m_timePeriods.Add(360);
  m_timePeriods.Add(720);
  m_timePeriods.Add(1080);
  m_timePeriods.Add(1440);
  m_timePeriods.Add(1800);
  m_timePeriods.Add(3600);

  m_chartTypeStrings.Add(_("None"));
  m_chartTypeStrings.Add(_("CandleStick"));
  m_chartTypeStrings.Add(_("Closing Price"));
  m_chartTypeStrings.Add(_("Median Price"));
  m_chartTypeStrings.Add(_("OHLC"));
  m_chartTypeStrings.Add(_("Typical Price"));
  m_chartTypeStrings.Add(_("Weighted Close"));

  m_chartTypes.Add(wxS("None"));
  m_chartTypes.Add(wxS("CandleStick"));
  m_chartTypes.Add(wxS("Close"));
  m_chartTypes.Add(wxS("Median"));
  m_chartTypes.Add(wxS("OHLC"));
  m_chartTypes.Add(wxS("TP"));
  m_chartTypes.Add(wxS("WC"));

  m_priceBandStrings.Add(_("None"));
  m_priceBandStrings.Add(_("Bollinger Band"));
  m_priceBandStrings.Add(_("Donchain Channel"));
  m_priceBandStrings.Add(_("Envelop (SMA 20 +/- 10%)"));

  m_priceBands.Add(wxS("None"));
  m_priceBands.Add(wxS("BB"));
  m_priceBands.Add(wxS("DC"));
  m_priceBands.Add(wxS("Envelop"));

  m_movingAvgStrings.Add(_("None"));
  m_movingAvgStrings.Add(_("Simple"));
  m_movingAvgStrings.Add(_("Exponential"));
  m_movingAvgStrings.Add(_("Triangular"));
  m_movingAvgStrings.Add(_("Weighted"));

  m_movingAvgs.Add(wxS("None"));
  m_movingAvgs.Add(wxS("SMA"));
  m_movingAvgs.Add(wxS("EMA"));
  m_movingAvgs.Add(wxS("TMA"));
  m_movingAvgs.Add(wxS("WMA"));

  m_techIndicatorStrings.Add(_("None"));
  m_techIndicatorStrings.Add(_("Accumulation/Distribution"));
  m_techIndicatorStrings.Add(_("Aroon Oscillator"));
  m_techIndicatorStrings.Add(_("Aroon Up/Down"));
  m_techIndicatorStrings.Add(_("Avg Directional Index"));
  m_techIndicatorStrings.Add(_("Avg True Range"));
  m_techIndicatorStrings.Add(_("Bollinger Band Width"));
  m_techIndicatorStrings.Add(_("Chaikin Money Flow"));
  m_techIndicatorStrings.Add(_("Chaikin Oscillator"));
  m_techIndicatorStrings.Add(_("Chaikin Volatility"));
  m_techIndicatorStrings.Add(_("Close Location Value"));
  m_techIndicatorStrings.Add(_("Commodity Channel Index"));
  m_techIndicatorStrings.Add(_("Detrended Price Osc"));
  m_techIndicatorStrings.Add(_("Donchian Channel Width"));
  m_techIndicatorStrings.Add(_("Ease of Movement"));
  m_techIndicatorStrings.Add(_("Fast Stochastic"));
  m_techIndicatorStrings.Add(_("MACD"));
  m_techIndicatorStrings.Add(_("Mass Index"));
  m_techIndicatorStrings.Add(_("Momentum"));
  m_techIndicatorStrings.Add(_("Money Flow Index"));
  m_techIndicatorStrings.Add(_("Neg Volume Index"));
  m_techIndicatorStrings.Add(_("On Balance Volume"));
  m_techIndicatorStrings.Add(_("Performance"));
  m_techIndicatorStrings.Add(_("% Price Oscillator"));
  m_techIndicatorStrings.Add(_("% Volume Oscillator"));
  m_techIndicatorStrings.Add(_("Pos Volume Index"));
  m_techIndicatorStrings.Add(_("Price Volume Trend"));
  m_techIndicatorStrings.Add(_("Rate of Change"));
  m_techIndicatorStrings.Add(_("RSI"));
  m_techIndicatorStrings.Add(_("Slow Stochastic"));
  m_techIndicatorStrings.Add(_("StochRSI"));
  m_techIndicatorStrings.Add(_("TRIX"));
  m_techIndicatorStrings.Add(_("Ultimate Oscillator"));
  m_techIndicatorStrings.Add(_("Volume"));
  m_techIndicatorStrings.Add(_("William's %R"));

  m_techIndicators.Add(wxS("None"));
  m_techIndicators.Add(wxS("AccDist"));
  m_techIndicators.Add(wxS("AroonOsc"));
  m_techIndicators.Add(wxS("Aroon"));
  m_techIndicators.Add(wxS("ADX"));
  m_techIndicators.Add(wxS("ATR"));
  m_techIndicators.Add(wxS("BBW"));
  m_techIndicators.Add(wxS("CMF"));
  m_techIndicators.Add(wxS("COscillator"));
  m_techIndicators.Add(wxS("CVolatility"));
  m_techIndicators.Add(wxS("CLV"));
  m_techIndicators.Add(wxS("CCI"));
  m_techIndicators.Add(wxS("DPO"));
  m_techIndicators.Add(wxS("DCW"));
  m_techIndicators.Add(wxS("EMV"));
  m_techIndicators.Add(wxS("FStoch"));
  m_techIndicators.Add(wxS("MACD"));
  m_techIndicators.Add(wxS("MDX"));
  m_techIndicators.Add(wxS("Momentum"));
  m_techIndicators.Add(wxS("MFI"));
  m_techIndicators.Add(wxS("NVI"));
  m_techIndicators.Add(wxS("OBV"));
  m_techIndicators.Add(wxS("Performance"));
  m_techIndicators.Add(wxS("PPO"));
  m_techIndicators.Add(wxS("PVO"));
  m_techIndicators.Add(wxS("PVI"));
  m_techIndicators.Add(wxS("PVT"));
  m_techIndicators.Add(wxS("ROC"));
  m_techIndicators.Add(wxS("RSI"));
  m_techIndicators.Add(wxS("SStoch"));
  m_techIndicators.Add(wxS("StochRSI"));
  m_techIndicators.Add(wxS("TRIX"));
  m_techIndicators.Add(wxS("UO"));
  m_techIndicators.Add(wxS("Vol"));
  m_techIndicators.Add(wxS("WilliamR"));

  m_indicator1 = 28;
  m_indicator2 = 16;
}


/*
 * Control creation for FinanceDemo
 */

void
FinanceDemo::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  FinanceDemo* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer1);

  wxPanel* itemPanel2 = new wxPanel( itemFrame1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer1->Add(itemPanel2, 1, wxGROW|wxALL, FromDIP(0));

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
//  itemPanel2->SetSizer(itemBoxSizer3);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemPanel2, wxID_ANY, wxEmptyString);
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, FromDIP(3));

  wxBoxSizer* itemBoxSizer3a = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer3a, 0, wxGROW | wxALL, FromDIP(0));

  m_pointerButton = new wxToggleButton(itemPanel2, ID_POINTER, wxT(""), wxDefaultPosition, FromDIP(wxSize(32, 32)), wxBU_EXACTFIT);
  m_pointerButton->SetBitmap(GetBitmapResource("pointer.png"));
  m_pointerButton->SetBitmapMargins(0, 0);
  // Initially set the mouse to drag to scroll mode
  m_pointerButton->SetValue(true);
  itemBoxSizer3a->Add(m_pointerButton, 0, wxLEFT | wxALL, FromDIP(3));

  m_zoominButton = new wxToggleButton(itemPanel2, wxID_ZOOM_IN, wxT(""), wxDefaultPosition, FromDIP(wxSize(32, 32)), wxBU_EXACTFIT);
  m_zoominButton->SetBitmap(GetBitmapResource("zoomin.png"));
  m_zoominButton->SetBitmapMargins(0, 0);
  m_zoominButton->SetValue(false);
  itemBoxSizer3a->Add(m_zoominButton, 0, wxLEFT | wxALL, FromDIP(3));

  m_zoomoutButton = new wxToggleButton(itemPanel2, wxID_ZOOM_OUT, wxT(""), wxDefaultPosition, FromDIP(wxSize(32, 32)), wxBU_EXACTFIT);
  m_zoomoutButton->SetBitmap(GetBitmapResource("zoomout.png"));
  m_zoomoutButton->SetBitmapMargins(0, 0);
  m_zoomoutButton->SetValue(false);
  itemBoxSizer3a->Add(m_zoomoutButton, 0, wxGROW | wxALL, FromDIP(3));

  wxStaticText* itemStaticText5 = new wxStaticText(itemPanel2, wxID_STATIC, _("Ticker Symbol"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, FromDIP(3));

  m_tickerSymbol = new wxTextCtrl(itemPanel2, ID_TICKER_SYMBOL, wxS("ASE"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  itemStaticBoxSizer4->Add(m_tickerSymbol, 0, wxGROW|wxALL, 1);

  wxStaticText* itemStaticText7 = new wxStaticText(itemPanel2, wxID_STATIC, _("Compare With"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText7, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, FromDIP(3));

  m_compareWith = new wxTextCtrl(itemPanel2, ID_COMPARE_WITH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  itemStaticBoxSizer4->Add(m_compareWith, 0, wxGROW|wxALL, 1);

  wxStaticText* itemStaticText9 = new wxStaticText(itemPanel2, wxID_STATIC, _("Time Period"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText9, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, FromDIP(3));

  m_timeRange = new wxChoice(itemPanel2, ID_TIME_PERIOD, wxDefaultPosition, wxDefaultSize, m_timePeriodStrings, 0 );
  m_timeRange->SetSelection(2);
  itemStaticBoxSizer4->Add(m_timeRange, 0, wxGROW|wxALL, FromDIP(1));

  itemStaticBoxSizer4->Add(3, 3, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(3));

  m_volumeBars = new wxCheckBox(itemPanel2, ID_SHOW_VOLUME_BAR, _("Show Volume Bars"), wxDefaultPosition, wxDefaultSize, 0 );
  m_volumeBars->SetValue(true);
  itemStaticBoxSizer4->Add(m_volumeBars, 0, wxALIGN_LEFT|wxALL, FromDIP(3));

  m_parabolicSar = new wxCheckBox(itemPanel2, ID_PARABOLIC_SAR, _("Parabolic SAR"), wxDefaultPosition, wxDefaultSize, 0 );
  m_parabolicSar->SetValue(false);
  itemStaticBoxSizer4->Add(m_parabolicSar, 0, wxALIGN_LEFT|wxALL, FromDIP(3));

  m_logScale = new wxCheckBox(itemPanel2, ID_LOG_SCALE, _("Log Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  m_logScale->SetValue(false);
  itemStaticBoxSizer4->Add(m_logScale, 0, wxALIGN_LEFT|wxALL, FromDIP(3));

  m_percentageScale = new wxCheckBox(itemPanel2, ID_PERCENTAGE_SCALE, _("Percentage Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  m_percentageScale->SetValue(false);
  itemStaticBoxSizer4->Add(m_percentageScale, 0, wxALIGN_LEFT|wxALL, FromDIP(3));

  itemStaticBoxSizer4->Add(3, 3, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(3));

  wxStaticText* itemStaticText17 = new wxStaticText(itemPanel2, wxID_STATIC, _("Chart Type"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText17, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, FromDIP(3));

  m_chartType = new wxChoice(itemPanel2, ID_CHART_TYPE, wxDefaultPosition, wxDefaultSize, m_chartTypeStrings, 0 );
  m_chartType->SetSelection(1);
  itemStaticBoxSizer4->Add(m_chartType, 0, wxGROW|wxALL, FromDIP(1));

  wxStaticText* itemStaticText19 = new wxStaticText(itemPanel2, wxID_STATIC, _("Price Band"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText19, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_priceBand = new wxChoice(itemPanel2, ID_PRICE_BAND, wxDefaultPosition, wxDefaultSize, m_priceBandStrings, 0 );
  m_priceBand->SetSelection(1);
  itemStaticBoxSizer4->Add(m_priceBand, 0, wxGROW|wxALL, FromDIP(1));

  wxStaticText* itemStaticText21 = new wxStaticText(itemPanel2, wxID_STATIC, _("Moving Averages"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText21, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, FromDIP(3));

  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer22, 0, wxGROW|wxALL, 0);

  m_avgType1 = new wxChoice(itemPanel2, ID_MOVING_AVG1, wxDefaultPosition, wxDefaultSize, m_movingAvgStrings, 0 );
  m_avgType1->SetSelection(1);
  itemBoxSizer22->Add(m_avgType1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, FromDIP(1));

  m_movAvg1 = new wxTextCtrl(itemPanel2, ID_VALUE_AVG1, wxS("5"), wxDefaultPosition, FromDIP(wxSize(30, -1)), wxTE_PROCESS_ENTER );
  itemBoxSizer22->Add(m_movAvg1, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, FromDIP(1));

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer25, 0, wxGROW|wxALL, 0);

  m_avgType2 = new wxChoice(itemPanel2, ID_MOVING_AVG2, wxDefaultPosition, wxDefaultSize, m_movingAvgStrings, 0 );
  m_avgType2->SetSelection(1);
  itemBoxSizer25->Add(m_avgType2, 1, wxALIGN_CENTER_VERTICAL|wxALL, FromDIP(1));

  m_movAvg2 = new wxTextCtrl(itemPanel2, ID_VALUE_AVG2, wxS("20"), wxDefaultPosition, FromDIP(wxSize(30, -1)), wxTE_PROCESS_ENTER );
  itemBoxSizer25->Add(m_movAvg2, 0, wxALIGN_CENTER_VERTICAL|wxALL, FromDIP(1));

  wxStaticText* itemStaticText28 = new wxStaticText(itemPanel2, wxID_STATIC, _("Technical Indicators"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText28, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_techIndicator1 = new wxChoice(itemPanel2, ID_TECH_INDICATOR1, wxDefaultPosition, wxDefaultSize, m_techIndicatorStrings, 0 );
  m_techIndicator1->SetSelection(m_indicator1);
  itemStaticBoxSizer4->Add(m_techIndicator1, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, FromDIP(1));

  m_techIndicator2 = new wxChoice(itemPanel2, ID_TECH_INDICATOR2, wxDefaultPosition, wxDefaultSize, m_techIndicatorStrings, 0 );
  m_techIndicator2->SetSelection(m_indicator2);
  itemStaticBoxSizer4->Add(m_techIndicator2, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, FromDIP(1));

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW | wxALL, 0);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, FromDIP(wxSize(780, 425)), wxTAB_TRAVERSAL | wxNO_BORDER);
  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, FromDIP(3));
  
  itemPanel2->SetSizerAndFit(itemBoxSizer3);

  OnMouseUsageChanged(Chart::MouseUsageScroll);
  OnLineEditChanged();

  // Update the chart
  DrawChart(m_chartViewer);
}


/*
 * Should we show tooltips?
 */

bool
FinanceDemo::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
FinanceDemo::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
  wxImage image;
  if (name == wxT("pointer.png"))
  {
    image.LoadFile(wxT("./icons/scroll_icon.png"), wxBITMAP_TYPE_PNG);
    image.Rescale(FromDIP(16), FromDIP(16));
    return wxBitmap(image);
  }
  else if (name == wxT("zoomin.png"))
  {
    image.LoadFile(wxT("./icons/zoomin_icon.png"), wxBITMAP_TYPE_PNG);
    image.Rescale(FromDIP(16), FromDIP(16));
    return wxBitmap(image);
  }
  else if (name == wxT("zoomout.png"))
  {
    image.LoadFile(wxT("./icons/zoomout_icon.png"), wxBITMAP_TYPE_PNG);
    image.Rescale(FromDIP(16), FromDIP(16));
    return wxBitmap(image);
  }
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
FinanceDemo::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TECH_INDICATOR 1/2
 */

void
FinanceDemo::OnTechIndicatorSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOVING_AVG2
 */

void
FinanceDemo::OnMovingAvgSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PRICE_BAND
 */

void
FinanceDemo::OnPriceBandSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_TYPE
 */

void
FinanceDemo::OnChartTypeSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SHOW_VOLUME_BAR
 */

void
FinanceDemo::OnShowVolumeBarClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PARABOLIC_SAR
 */

void
FinanceDemo::OnParabolicSarClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LOG_SCALE
 */

void
FinanceDemo::OnLogScaleClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PERCENTAGE_SCALE
 */

void
FinanceDemo::OnPercentageScaleClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIME_PERIOD
 */

void
FinanceDemo::OnTimePeriodSelected( wxCommandEvent& event )
{
  int durationInDays = m_timePeriods[m_timeRange->GetSelection()];
  m_chartViewer->setViewPortWidth(durationInDays / (double) m_dailyPrice.timeStamps.size());
  m_chartViewer->setViewPortLeft(1 - m_chartViewer->getViewPortWidth());
  m_chartViewer->updateViewPort(true, true);
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_COMPARE_WITH
 */

void
FinanceDemo::OnCompareWithEnter( wxCommandEvent& event )
{
  OnLineEditChanged();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TICKER_SYMBOL
 */

void
FinanceDemo::OnTickerSymbolEnter( wxCommandEvent& event )
{
  OnLineEditChanged();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_VALUE_AVG1
 */

void
FinanceDemo::OnMovingAvgUpdated( wxCommandEvent& event )
{
  long avgValue = 0;
  if (!m_movAvg1->GetValue().ToLong(&avgValue))
  {
    m_movAvg1->SetValue(wxS("0"));
  }
  int new_avgPeriod1 = avgValue;

  if (!m_movAvg2->GetValue().ToLong(&avgValue))
  {
    m_movAvg2->SetValue(wxS("0"));
  }
  int new_avgPeriod2 = avgValue;

  if (new_avgPeriod1 != m_avgPeriod1 || new_avgPeriod2 != m_avgPeriod2)
  {
    DrawChart(m_chartViewer);
  }
}

void FinanceDemo::OnLineEditChanged()
{
  long avg1Value = 0;
  m_movAvg1->GetValue().ToLong(&avg1Value);
  long avg2Value = 0;
  m_movAvg2->GetValue().ToLong(&avg2Value);

  wxString tickerKey = m_tickerSymbol->GetValue();
  wxString compareKey = m_compareWith->GetValue();

  bool needReloadData = (m_tickerKey != tickerKey) || (m_compareKey != compareKey);
  if (needReloadData)
    loadData(tickerKey, compareKey);

  if (needReloadData || (avg1Value != m_avgPeriod1) || (avg2Value != m_avgPeriod2))
    DrawChart(m_chartViewer);
}

/////////////////////////////////////////////////////////////////////////////
// Data source

void
FinanceDemo::loadData(const wxString& ticker, const wxString& compare)
{
  // The financial simulator
  if (m_tickerKey != ticker)
  {
    m_tickerKey = ticker;

    // Simulator to generate realistic random OHLC values
    FinanceSimulator db(ticker.ToUTF8(), Chart::chartTime(2010, 1, 1), Chart::chartTime(2020, 12, 31), 86400);

    // Copy daily data to the data arrays
    m_dailyPrice.timeStamps = arrayToVector(db.getTimeStamps());
    m_dailyPrice.highData = arrayToVector(db.getHighData());
    m_dailyPrice.lowData = arrayToVector(db.getLowData());
    m_dailyPrice.openData = arrayToVector(db.getOpenData());
    m_dailyPrice.closeData = arrayToVector(db.getCloseData());
    m_dailyPrice.volData = arrayToVector(db.getVolData());

    // Set up the viewport to display the duration selected by the user

    m_chartViewer->setFullRange("x", 0, (int)m_dailyPrice.timeStamps.size() - 1);
    int durationInDays = m_timePeriods[m_timeRange->GetSelection()];
    m_chartViewer->setViewPortWidth(durationInDays / (double)m_dailyPrice.timeStamps.size());
    m_chartViewer->setViewPortLeft(1 - m_chartViewer->getViewPortWidth());
  }

  if (m_compareKey != compare)
  {
    m_compareKey = compare;

    // Simulator to generate realistic random OHLC values
    FinanceSimulator db2(compare.ToUTF8(), Chart::chartTime(2010, 1, 1), Chart::chartTime(2020, 12, 31), 86400);
    m_dailyPrice.compareData = arrayToVector(db2.getCloseData());
  }

  // In this example, we will compute the weekly and monthly prices on demand. We just
  // need to clear the old data here.
  m_weeklyPrice.timeStamps.clear();
  m_monthlyPrice.timeStamps.clear();
} 

/// <summary>
/// An internal method used to aggregate daily data.
/// </summary>
void
FinanceDemo::AggregateData(const ArrayMath& aggregator, PriceData& p)
{
  int size = aggregator.aggregate(vectorToArray(p.timeStamps), Chart::AggregateFirst).len;
  p.timeStamps.resize(size);
  size = aggregator.aggregate(vectorToArray(p.highData), Chart::AggregateMax).len;
  p.highData.resize(size);
  size = aggregator.aggregate(vectorToArray(p.lowData), Chart::AggregateMin).len;
  p.lowData.resize(size);
  size = aggregator.aggregate(vectorToArray(p.openData), Chart::AggregateFirst).len;
  p.openData.resize(size);
  size = aggregator.aggregate(vectorToArray(p.closeData), Chart::AggregateLast).len;
  p.closeData.resize(size);
  size = aggregator.aggregate(vectorToArray(p.volData), Chart::AggregateSum).len;
  p.volData.resize(size);
  size = aggregator.aggregate(vectorToArray(p.compareData), Chart::AggregateLast).len;
  p.compareData.resize(size);
}

/////////////////////////////////////////////////////////////////////////////
// Chart Creation

/// <summary>
/// Add a moving average line to the FinanceChart object.
/// </summary>
/// <param name="m">The FinanceChart object to add the line to.</param>
/// <param name="avgType">The moving average type (SMA/EMA/TMA/WMA).</param>
/// <param name="avgPeriod">The moving average period.</param>
/// <param name="color">The color of the line.</param>
static LineLayer*
AddMovingAvg(FinanceChart* m, const wxString& avgType, int avgPeriod, int color)
{
  if (avgPeriod > 1)
  {
    if (avgType.IsSameAs(wxS("SMA")))
      return m->addSimpleMovingAvg(avgPeriod, color);
    else if (avgType.IsSameAs(wxS("EMA")))
      return m->addExpMovingAvg(avgPeriod, color);
    else if (avgType.IsSameAs(wxS("TMA")))
      return m->addTriMovingAvg(avgPeriod, color);
    else if (avgType.IsSameAs(wxS("WMA")))
      return m->addWeightedMovingAvg(avgPeriod, color);
  }

  return 0;
}

/// <summary>
/// Add an indicator chart to the FinanceChart object. In this demo example, the indicator
/// parameters (such as the period used to compute RSI, colors of the lines, etc.) are hard
/// coded to commonly used values. You are welcome to design a more complex user interface
/// to allow users to set the parameters.
/// </summary>
/// <param name="m">The FinanceChart object to add the line to.</param>
/// <param name="indicator">The selected indicator.</param>
/// <param name="height">Height of the chart in pixels</param>
static XYChart*
AddIndicator(FinanceChart* m, const wxString& indicator, int height)
{
  if (indicator.IsSameAs(wxS("RSI")))
    return m->addRSI(height, 14, 0x800080, 20, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("StochRSI")))
    return m->addStochRSI(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("MACD")))
    return m->addMACD(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
  else if (indicator.IsSameAs(wxS("FStoch")))
    return m->addFastStochastic(height, 14, 3, 0x6060, 0x606000);
  else if (indicator.IsSameAs(wxS("SStoch")))
    return m->addSlowStochastic(height, 14, 3, 0x6060, 0x606000);
  else if (indicator.IsSameAs(wxS("ATR")))
    return m->addATR(height, 14, 0x808080, 0xff);
  else if (indicator.IsSameAs(wxS("ADX")))
    return m->addADX(height, 14, 0x8000, 0x800000, 0x80);
  else if (indicator.IsSameAs(wxS("DCW")))
    return m->addDonchianWidth(height, 20, 0xff);
  else if (indicator.IsSameAs(wxS("BBW")))
    return m->addBollingerWidth(height, 20, 2, 0xff);
  else if (indicator.IsSameAs(wxS("DPO")))
    return m->addDPO(height, 20, 0xff);
  else if (indicator.IsSameAs(wxS("PVT")))
    return m->addPVT(height, 0xff);
  else if (indicator.IsSameAs(wxS("Momentum")))
    return m->addMomentum(height, 12, 0xff);
  else if (indicator.IsSameAs(wxS("Performance")))
    return m->addPerformance(height, 0xff);
  else if (indicator.IsSameAs(wxS("ROC")))
    return m->addROC(height, 12, 0xff);
  else if (indicator.IsSameAs(wxS("OBV")))
    return m->addOBV(height, 0xff);
  else if (indicator.IsSameAs(wxS("AccDist")))
    return m->addAccDist(height, 0xff);
  else if (indicator.IsSameAs(wxS("CLV")))
    return m->addCLV(height, 0xff);
  else if (indicator.IsSameAs(wxS("WilliamR")))
    return m->addWilliamR(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("Aroon")))
    return m->addAroon(height, 14, 0x339933, 0x333399);
  else if (indicator.IsSameAs(wxS("AroonOsc")))
    return m->addAroonOsc(height, 14, 0xff);
  else if (indicator.IsSameAs(wxS("CCI")))
    return m->addCCI(height, 20, 0x800080, 100, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("EMV")))
    return m->addEaseOfMovement(height, 9, 0x6060, 0x606000);
  else if (indicator.IsSameAs(wxS("MDX")))
    return m->addMassIndex(height, 0x800080, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("CVolatility")))
    return m->addChaikinVolatility(height, 10, 10, 0xff);
  else if (indicator.IsSameAs(wxS("COscillator")))
    return m->addChaikinOscillator(height, 0xff);
  else if (indicator.IsSameAs(wxS("CMF")))
    return m->addChaikinMoneyFlow(height, 21, 0x8000);
  else if (indicator.IsSameAs(wxS("NVI")))
    return m->addNVI(height, 255, 0xff, 0x883333);
  else if (indicator.IsSameAs(wxS("PVI")))
    return m->addPVI(height, 255, 0xff, 0x883333);
  else if (indicator.IsSameAs(wxS("MFI")))
    return m->addMFI(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("PVO")))
    return m->addPVO(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
  else if (indicator.IsSameAs(wxS("PPO")))
    return m->addPPO(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
  else if (indicator.IsSameAs(wxS("UO")))
    return m->addUltimateOscillator(height, 7, 14, 28, 0x800080, 20, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("Vol")))
    return m->addVolIndicator(height, 0x99ff99, 0xff9999, 0xc0c0c0);
  else if (indicator.IsSameAs(wxS("TRIX")))
    return m->addTRIX(height, 12, 0xff);
  else
    return 0;
}

/// <summary>
/// Creates a dummy chart to show an error message.
/// </summary>
/// <param name="viewer">The WinChartViewer to display the error message.</param>
/// <param name="msg">The error message</param>
static void
ErrorMessage(wxChartViewer* viewer, const wxString& msg)
{
  MultiChart m(400, 200);
  m.addTitle(Chart::Center, (const char*) msg.ToUTF8(), "Arial", 10)->setMaxWidth(m.getWidth());
  viewer->setChart(&m);
}

/// <summary>
/// Draw the chart according to user selection and display it in the ChartViewer.
/// </summary>
/// <param name="viewer">The ChartViewer object to display the chart.</param>
void
FinanceDemo::DrawChart(wxChartViewer* viewer)
{
  // The first moving average period selected by the user.
  long avg1Value = 0;
  m_movAvg1->GetValue().ToLong(&avg1Value);
  m_avgPeriod1 = (avg1Value < 0) ? 0 : (avg1Value > 300) ? 300 : avg1Value;

  // The second moving average period selected by the user.
  long avg2Value = 0;
  m_movAvg2->GetValue().ToLong(&avg2Value);
  m_avgPeriod2 = (avg2Value < 0) ? 0 : (avg2Value > 300) ? 300 : avg2Value;

  // We need extra leading data points in order to compute moving averages.
  int extraPoints = (m_avgPeriod1 > m_avgPeriod2) ? m_avgPeriod1 : m_avgPeriod2;
  if (extraPoints < 25)
  {
    extraPoints = 25;
  }

  // Get the start and end index visible on the viewport
  int startIndex = (int)floor(viewer->getValueAtViewPort("x", viewer->getViewPortLeft()));
  int endIndex = (int)ceil(viewer->getValueAtViewPort("x", viewer->getViewPortRight()));

  PriceData* p = &m_dailyPrice;
  if (endIndex - startIndex >= 400)
  {
    if (endIndex - startIndex < 2000)
    {
      p = &m_weeklyPrice;
      if (p->timeStamps.empty())
      {
        ArrayMath a(vectorToArray(m_dailyPrice.timeStamps));
        AggregateData(a.selectStartOfWeek(), *p = m_dailyPrice);
      }
    }
    else
    {
      p = &m_monthlyPrice;
      if (p->timeStamps.empty())
      {
        ArrayMath a(vectorToArray(m_dailyPrice.timeStamps));
        AggregateData(a.selectStartOfMonth(), *p = m_dailyPrice);
      }
    }

    // Re-compute the index based on weekly data
    DoubleArray ts = vectorToArray(p->timeStamps);
    startIndex = (int)floor(Chart::bSearch(ts, m_dailyPrice.timeStamps[startIndex]));
    endIndex = (int)ceil(Chart::bSearch(ts, m_dailyPrice.timeStamps[endIndex]));
  }

  // At this stage, we have determined the price data to use. We need to obtain the part
  // of the arrays that are within the visible x-axis range.

  // Add extra points before the startIndex, as moving averages and may technical indicators
  // requires data before the first visible point.
  extraPoints = (std::min)(extraPoints, startIndex);
  startIndex -= extraPoints;
  int duration = endIndex - startIndex + 1;

  // Obtain the visible part of the data plus the extra points
  DoubleArray timeStamps = vectorToArray(p->timeStamps, startIndex, duration);
  DoubleArray highData = vectorToArray(p->highData, startIndex, duration);
  DoubleArray lowData = vectorToArray(p->lowData, startIndex, duration);
  DoubleArray openData = vectorToArray(p->openData, startIndex, duration);
  DoubleArray closeData = vectorToArray(p->closeData, startIndex, duration);
  DoubleArray volData = vectorToArray(p->volData, startIndex, duration);
  DoubleArray compareData = vectorToArray(p->compareData, startIndex, duration);

  // Now, we have obtain the data. We can plot the chart.
  int width = 780;
  int mainHeight = 260;
  int indicatorHeight = 80;

  // Create the chart object using the selected size
  FinanceChart* m = new FinanceChart(width);

  // Disable default legend box, as we are using dynamic legend
  m->setLegendStyle("normal", 8, Chart::Transparent, Chart::Transparent);

  // Set the data into the chart object
  m->setData(timeStamps, highData, lowData, openData, closeData, volData, extraPoints);

  //
  // We configure the title of the chart. In this demo chart design, we put the
  // company name as the top line of the title with left alignment.
  //
  m->addPlotAreaTitle(Chart::BottomLeft, m_tickerKey.ToUTF8());

  // A copyright message at the bottom left corner the title area
  m->addPlotAreaTitle(Chart::BottomRight, "<*font=Arial,size=8*>(c) Advanced Software Engineering");

  // Add the main chart
  m->addMainChart(mainHeight);

  // Set log or linear scale according to user preference
  m->setLogScale(m_logScale->IsChecked());

  // Set axis labels to show data values or percentage change to user preference
  if (m_percentageScale->IsChecked())
  {
    m->setPercentageAxis();
  }

  // Draw the main chart depending on the chart type the user has selected
  wxString selectedType = m_chartTypes[m_chartType->GetSelection()];
  if (selectedType == "Close")
    m->addCloseLine(0x000040);
  else if (selectedType == "TP")
    m->addTypicalPrice(0x000040);
  else if (selectedType == "WC")
    m->addWeightedClose(0x000040);
  else if (selectedType == "Median")
    m->addMedianPrice(0x000040);

  // Add comparison line if there is data for comparison
  if (compareData.len > 0)
    m->addComparison(compareData, 0x0000ff, m_compareKey.ToUTF8());

  // Add moving average lines.
  AddMovingAvg(m, m_movingAvgs[m_avgType1->GetSelection()], m_avgPeriod1, 0x663300);
  AddMovingAvg(m, m_movingAvgs[m_avgType2->GetSelection()], m_avgPeriod2, 0x9900ff);

  // Draw the main chart if the user has selected CandleStick or OHLC. We
  // draw it here to make sure it is drawn behind the moving average lines
  // (that is, the moving average lines stay on top.)
  if (selectedType == "CandleStick")
    m->addCandleStick(0x33ff33, 0xff3333);
  else if (selectedType == "OHLC")
    m->addHLOC(0x8000, 0x800000);

  // Add parabolic SAR if necessary
  if (m_parabolicSar->IsChecked())
    m->addParabolicSAR(0.02, 0.02, 0.2, Chart::DiamondShape, 5, 0x008800, 0x000000);

  // Add price band/channel/envelop to the chart according to user selection
  wxString selectedBand = m_priceBands[m_priceBand->GetSelection()];
  if (selectedBand == "BB")
    m->addBollingerBand(20, 2, 0x9999ff, 0xc06666ff);
  else if (selectedBand == "DC")
    m->addDonchianChannel(20, 0x9999ff, 0xc06666ff);
  else if (selectedBand == "Envelop")
    m->addEnvelop(20, 0.1, 0x9999ff, 0xc06666ff);

  // Add volume bars to the main chart if necessary
  if (m_volumeBars->IsChecked())
    m->addVolBars(indicatorHeight, 0x99ff99, 0xff9999, 0xc0c0c0);

  // Add additional indicators as according to user selection.
  m_indicator1 = m_techIndicator1->GetSelection();
  m_indicator2 = m_techIndicator2->GetSelection();
  AddIndicator(m, m_techIndicators[m_indicator1], indicatorHeight);
  AddIndicator(m, m_techIndicators[m_indicator2], indicatorHeight);

  // We may need to update the track line. If the mouse is moving on the chart, the track line
  // will be updated in MouseMovePlotArea. Otherwise, we need to update the track line here.
  if (!viewer->isInMouseMoveEvent())
  {
    TrackFinance(m, (0 == viewer->getChart()) ?
      ((XYChart*)m->getChart(0))->getPlotArea()->getRightX() : viewer->getPlotAreaMouseX());
  }

  // Set the chart to the viewer
  delete viewer->getChart();
  viewer->setChart(m);
  Refresh();
}

// Draw track cursor when mouse is moving over plotarea
void
FinanceDemo::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackFinance((MultiChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}

//
// Draw finance chart track line with legend
//
void FinanceDemo::TrackFinance(MultiChart* m, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = m->initDynamicLayer();

  // It is possible for a FinanceChart to be empty, so we need to check for it.
  if (m->getChartCount() == 0)
    return;

  // Get the data x-value that is nearest to the mouse
  int xValue = (int)(((XYChart*)m->getChart(0))->getNearestXValue(mouseX));

  // Iterate the XY charts (main price chart and indicator charts) in the FinanceChart
  XYChart* c = 0;
  for (int i = 0; i < m->getChartCount(); ++i) {
    c = (XYChart*)m->getChart(i);

    // Variables to hold the legend entries
    std::ostringstream ohlcLegend;
    std::vector<std::string> legendEntries;

    // Iterate through all layers to find the highest data point
    for (int j = 0; j < c->getLayerCount(); ++j) {
      Layer* layer = c->getLayerByZ(j);
      int xIndex = layer->getXIndexOf(xValue);
      int dataSetCount = layer->getDataSetCount();

      // In a FinanceChart, only layers showing OHLC data can have 4 data sets
      if (dataSetCount == 4) {
        double highValue = layer->getDataSet(0)->getValue(xIndex);
        double lowValue = layer->getDataSet(1)->getValue(xIndex);
        double openValue = layer->getDataSet(2)->getValue(xIndex);
        double closeValue = layer->getDataSet(3)->getValue(xIndex);

        if (closeValue != Chart::NoValue) {
          // Build the OHLC legend
          ohlcLegend << "      <*block*>";
          ohlcLegend << "Open: " << c->formatValue(openValue, "{value|P4}");
          ohlcLegend << ", High: " << c->formatValue(highValue, "{value|P4}");
          ohlcLegend << ", Low: " << c->formatValue(lowValue, "{value|P4}");
          ohlcLegend << ", Close: " << c->formatValue(closeValue, "{value|P4}");

          // We also draw an upward or downward triangle for up and down days and the %
          // change
          double lastCloseValue = layer->getDataSet(3)->getValue(xIndex - 1);
          if (lastCloseValue != Chart::NoValue) {
            double change = closeValue - lastCloseValue;
            double percent = change * 100 / closeValue;
            std::string symbol = (change >= 0) ?
              "<*font,color=008800*><*img=@triangle,width=8,color=008800*>" :
              "<*font,color=CC0000*><*img=@invertedtriangle,width=8,color=CC0000*>";

            ohlcLegend << "  " << symbol << " " << c->formatValue(change, "{value|P4}");
            ohlcLegend << " (" << c->formatValue(percent, "{value|2}") << "%)<*/font*>";
          }

          ohlcLegend << "<*/*>";
        }
      }
      else {
        // Iterate through all the data sets in the layer
        for (int k = 0; k < layer->getDataSetCount(); ++k) {
          DataSet* dataSet = layer->getDataSetByZ(k);

          std::string name = dataSet->getDataName();
          double value = dataSet->getValue(xIndex);
          if ((0 != name.size()) && (value != Chart::NoValue)) {

            // In a FinanceChart, the data set name consists of the indicator name and
            // its latest value. It is like "Vol: 123M" or "RSI (14): 55.34". As we are
            // generating the values dynamically, we need to extract the indictor name
            // out, and also the volume unit (if any).

            // The volume unit
            std::string unitChar;

            // The indicator name is the part of the name up to the colon character.
            int delimiterPosition = (int)name.find(':');
            if (name.npos != delimiterPosition) {

              // The unit, if any, is the trailing non-digit character(s).
              int lastDigitPos = (int)name.find_last_of("0123456789");
              if ((name.npos != lastDigitPos) && (lastDigitPos + 1 < (int)name.size()) &&
                (lastDigitPos > delimiterPosition))
                unitChar = name.substr(lastDigitPos + 1);

              name.resize(delimiterPosition);
            }

            // In a FinanceChart, if there are two data sets, it must be representing a
            // range.
            if (dataSetCount == 2) {
              // We show both values in the range in a single legend entry
              value = layer->getDataSet(0)->getValue(xIndex);
              double value2 = layer->getDataSet(1)->getValue(xIndex);
              name = name + ": " + c->formatValue((std::min)(value, value2), "{value|P3}");
              name = name + " - " + c->formatValue((std::max)(value, value2), "{value|P3}");
            }
            else {
              // In a FinanceChart, only the layer for volume bars has 3 data sets for
              // up/down/flat days
              if (dataSetCount == 3) {
                // The actual volume is the sum of the 3 data sets.
                value = layer->getDataSet(0)->getValue(xIndex) + layer->getDataSet(1
                )->getValue(xIndex) + layer->getDataSet(2)->getValue(xIndex);
              }

              // Create the legend entry
              name = name + ": " + c->formatValue(value, "{value|P3}") + unitChar;
            }

            // Build the legend entry, consist of a colored square box and the name (with
            // the data value in it).
            std::ostringstream legendEntry;
            legendEntry << "<*block*><*img=@square,width=8,edgeColor=000000,color="
              << std::hex << dataSet->getDataColor() << "*> " << name << "<*/*>";
            legendEntries.push_back(legendEntry.str());
          }
        }
      }
    }

    // Get the plot area position relative to the entire FinanceChart
    PlotArea* plotArea = c->getPlotArea();
    int plotAreaLeftX = plotArea->getLeftX() + c->getAbsOffsetX();
    int plotAreaTopY = plotArea->getTopY() + c->getAbsOffsetY();

    // The legend begins with the date label, then the ohlcLegend (if any), and then the
    // entries for the indicators.
    std::ostringstream legendText;
    legendText << "<*block,valign=top,maxWidth=" << (plotArea->getWidth() - 5)
      << "*><*font=Arial Bold*>[" << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
      << "]<*/font*>" << ohlcLegend.str();
    for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i) {
      legendText << "      " << legendEntries[i];
    }
    legendText << "<*/*>";

    // Draw a vertical track line at the x-position
    d->vline(plotAreaTopY, plotAreaTopY + plotArea->getHeight(), c->getXCoor(xValue) +
      c->getAbsOffsetX(), d->dashLineColor(0x000000, 0x0101));

    // Display the legend on the top of the plot area
    TTFText* t = d->text(legendText.str().c_str(), "Arial", 8);
    t->draw(plotAreaLeftX + 5, plotAreaTopY + 3, 0x000000, Chart::TopLeft);
    t->destroy();
  }
}

void
FinanceDemo::OnViewPortChanged(wxCommandEvent& event)
{
  // Update the chart if necessary
  if (m_chartViewer->needUpdateChart())
    DrawChart(m_chartViewer);
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
FinanceDemo::OnPointerClick(wxCommandEvent& event)
{
  m_pointerButton->SetValue(true);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(false);
  OnMouseUsageChanged(Chart::MouseUsageScroll);
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ZOOM_IN
 */

void
FinanceDemo::OnZoomInClick(wxCommandEvent& event)
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(true);
  m_zoomoutButton->SetValue(false);
  OnMouseUsageChanged(Chart::MouseUsageZoomIn);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
FinanceDemo::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
 */

void
FinanceDemo::OnZoomOutClick(wxCommandEvent& event)
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

//
// Convert std::vector to a DoubleArray
//
DoubleArray
FinanceDemo::vectorToArray(const std::vector<double>& v, int startIndex, int length)
{
  if ((length < 0) || (length + startIndex > (int)v.size()))
    length = ((int)v.size()) - startIndex;
  return (length <= 0) ? DoubleArray() : DoubleArray(&(v[startIndex]), length);
}

//
// Convert DoubleArray to std::vector
//
std::vector<double>
FinanceDemo::arrayToVector(DoubleArray a)
{
  return std::vector<double>(a.data, a.data + a.len);
}
