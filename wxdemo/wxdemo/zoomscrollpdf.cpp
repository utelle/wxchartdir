/*
** Name:        zoomscrollpdf.cpp
** Purpose:     PDF Report Demonstration
** Author:      Ulrich Telle
** Created:     2021-05-25
** Copyright:   (C) 2021, Ulrich Telle
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

#include "zoomscrollpdf.h"

#include <sstream>
#include <vector>

using namespace std;

#include "mondrian.xpm"

/*
 * ZoomScrollPdf type definition
 */

IMPLEMENT_CLASS( ZoomScrollPdf, wxDialog )

/*
 * ZoomScrollTrack event table definition
 */

BEGIN_EVENT_TABLE( ZoomScrollPdf, wxDialog )

  EVT_TOGGLEBUTTON( ID_POINTER, ZoomScrollPdf::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, ZoomScrollPdf::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, ZoomScrollPdf::OnZoomOutClick )
  EVT_BUTTON(wxID_SAVE, ZoomScrollPdf::OnSave)
  EVT_BUTTON(ID_PDF, ZoomScrollPdf::OnPdf)
  EVT_COMMAND_SCROLL(ID_SCROLLBAR, ZoomScrollPdf::OnScrollbarUpdated)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, ZoomScrollPdf::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, ZoomScrollPdf::OnViewPortChanged)
  EVT_CHARTVIEWER_MOUSEWHEEL(ID_CHARTVIEWER, ZoomScrollPdf::OnMouseWheelChart)

END_EVENT_TABLE()

/*
 * ZoomScrollPdf constructors
 */

  ZoomScrollPdf::ZoomScrollPdf()
{
  Init();
}

ZoomScrollPdf::ZoomScrollPdf( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * ZoomScrollPdf creator
 */

bool
ZoomScrollPdf::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();

  return true;
}

/*
 * ZoomScrollPdf destructor
 */

ZoomScrollPdf::~ZoomScrollPdf()
{
  delete m_ranSeries;
  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

/*
 * Member initialisation
 */

void
ZoomScrollPdf::Init()
{
}

/*
 * Control creation for ZoomScrollPdf
 */

void
ZoomScrollPdf::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  ZoomScrollPdf* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer1);

  wxPanel* itemPanel2 = new wxPanel( itemFrame1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer1->Add(itemPanel2, 1, wxGROW|wxALL, 0);
  m_bgColour = itemPanel2->GetBackgroundColour();

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel2->SetSizer(itemBoxSizer3);

  wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(itemPanel2, wxID_ANY, wxEmptyString);
  wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer1, 0, wxGROW | wxALL, FromDIP(3));

  m_pointerButton = new wxToggleButton( itemPanel2, ID_POINTER, _(" &Scroll"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
  m_pointerButton->SetBitmap(GetBitmapResource("pointer.png"));
  m_pointerButton->SetBitmapMargins(FromDIP(10), FromDIP(0));
  // Initially set the mouse to drag to scroll mode
  m_pointerButton->SetValue(true);
  itemStaticBoxSizer1->Add(m_pointerButton, 0, wxGROW|wxALL, FromDIP(3));

  m_zoominButton = new wxToggleButton( itemPanel2, wxID_ZOOM_IN, _(" Zoom &In"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_zoominButton->SetBitmap(GetBitmapResource("zoomin.png"));
  m_zoominButton->SetBitmapMargins(FromDIP(10), FromDIP(0));
  m_zoominButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoominButton, 0, wxGROW|wxALL, FromDIP(3));

  m_zoomoutButton = new wxToggleButton( itemPanel2, wxID_ZOOM_OUT, _(" Zoom &Out"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_zoomoutButton->SetBitmap(GetBitmapResource("zoomout.png"));
  m_zoomoutButton->SetBitmapMargins(FromDIP(10), FromDIP(0));
  m_zoomoutButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoomoutButton, 0, wxGROW|wxALL, FromDIP(3));

  itemStaticBoxSizer1->Add(3, 3, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(3));

  m_saveButton = new wxButton(itemPanel2, wxID_SAVE, _(" &Save Chart Image"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  itemStaticBoxSizer1->Add(m_saveButton, 0, wxGROW | wxALL, FromDIP(3));

  m_pdfButton = new wxButton(itemPanel2, ID_PDF, _(" &Create PDF Report"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  itemStaticBoxSizer1->Add(m_pdfButton, 0, wxGROW | wxALL, FromDIP(3));

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, FromDIP(3));

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(FromDIP(wxSize(650, 350)));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, FromDIP(3));

  m_scrollBar = new wxScrollBar(itemPanel2, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
  m_scrollBar->SetScrollbar(0, 0, 1000000000, 200000000);
  itemBoxSizer8->Add(m_scrollBar, 0, wxGROW | wxALL, FromDIP(3));

  // Load the data
  LoadData();

  // Initialize the wxChartViewer
  InitChartViewer(m_chartViewer);

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);
}

// Load the data
void
ZoomScrollPdf::LoadData()
{
  // In this example, we just use random numbers as data.
  m_ranSeries = new RanSeries(127);
  m_timeStamps = m_ranSeries->getDateSeries(1827, Chart::chartTime(2007, 1, 1), 86400);
  m_dataSeriesA = m_ranSeries->getSeries(1827, 150, -10, 10);
  m_dataSeriesB = m_ranSeries->getSeries(1827, 200, -10, 10);
  m_dataSeriesC = m_ranSeries->getSeries(1827, 250, -8, 8);
}

// Initialize the wxChartViewer
void
ZoomScrollPdf::InitChartViewer(wxChartViewer* viewer)
{
  // Set the full x range to be the duration of the data
  viewer->setFullRange("x", m_timeStamps[0], m_timeStamps[m_timeStamps.len - 1]);

  // Initialize the view port to show the latest 20% of the time range
  viewer->setViewPortWidth(0.2);
  viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

  // Set the maximum zoom to 10 points
  viewer->setZoomInWidthLimit(10.0 / m_timeStamps.len);

  viewer->setMouseUsage(Chart::MouseUsageScroll);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void ZoomScrollPdf::OnViewPortChanged(wxCommandEvent& event)
{
  // In addition to updating the chart, we may also need to update other controls that
  // changes based on the view port.
  UpdateControls(m_chartViewer);

  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
  m_scrollBar->Enable(m_chartViewer->getViewPortWidth() < 1);
}

// Update controls in the user interface when the view port changed
void
ZoomScrollPdf::UpdateControls(wxChartViewer* viewer)
{
  // The logical length of the scrollbar. It can be any large value. The actual value does
  // not matter.
  const int scrollBarLen = 1000000000;

  // Update the horizontal scroll bar
  int position = (int)(0.5 + viewer->getViewPortLeft() * scrollBarLen);
  int thumbSize = (int) ceil(viewer->getViewPortWidth() * scrollBarLen);
  int range = scrollBarLen /*- m_scrollBar->GetPageSize()*/;
  int pageSize = thumbSize;
  m_scrollBar->SetScrollbar(position, thumbSize, range, pageSize);
  m_scrollBar->Enable(viewer->getViewPortWidth() < 1);
}

// Draw the chart and display it in the given viewer
void
ZoomScrollPdf::DrawChart(wxChartViewer* viewer)
{
  // Get the start date and end date that are visible on the chart.
  double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
  double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() +
    viewer->getViewPortWidth());

  // Draw the XYChart
  XYChart* c = DrawXYChart(viewPortStartDate, viewPortEndDate);

  // Add a title to the chart using 18 pts Times New Roman Bold Italic font
  c->addTitle("   PDF Report Demonstration", "Times New Roman Bold Italic", 18);

  // We need to update the track line too. If the mouse is moving on the chart (eg. if
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if ((!viewer->isInMouseMoveEvent()) && viewer->isMouseOnPlotArea())
    TrackLineLabel(c, viewer->getPlotAreaMouseX());

  delete viewer->getChart();
  viewer->setChart(c);
}

// Draw an XYChart using data from startX to endX
XYChart* ZoomScrollPdf::DrawXYChart(double startX, double endX)
{
  // Get the array indexes that corresponds to the visible start and end dates
  int startIndex = (int)floor(Chart::bSearch(m_timeStamps, startX));
  int endIndex = (int)ceil(Chart::bSearch(m_timeStamps, endX));
  int noOfPoints = endIndex - startIndex + 1;

  // Extract the part of the data array that are visible.
  DoubleArray viewPortTimeStamps = DoubleArray(m_timeStamps.data + startIndex, noOfPoints);
  DoubleArray viewPortDataSeriesA = DoubleArray(m_dataSeriesA.data + startIndex, noOfPoints);
  DoubleArray viewPortDataSeriesB = DoubleArray(m_dataSeriesB.data + startIndex, noOfPoints);
  DoubleArray viewPortDataSeriesC = DoubleArray(m_dataSeriesC.data + startIndex, noOfPoints);

  //
  // At this stage, we have extracted the visible data. We can use those data to plot the chart.
  //

  ///////////////////////////////////////////////////////////////////////////////////////
  // Configure overall chart appearance.
  ///////////////////////////////////////////////////////////////////////////////////////

  // Create an XYChart object of size 650 x 350 pixels, with a white (ffffff) background and grey
  // (aaaaaa) border
  XYChart* c = new XYChart(650, 350, 0xffffff, 0xaaaaaa);

  // Set the plotarea at (55, 55) with width 90 pixels less than chart width, and height 90 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(55, 55, c->getWidth() - 90, c->getHeight() - 90, c->linearGradientColor(0, 55, 0,
    c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
  c->setClipping();

  // Add a legend box at (55, 30) using horizontal layout. Use 8pts Arial Bold as font. Set the
  // background and border color to Transparent and use line style legend key.
  LegendBox* b = c->addLegend(55, 30, false, "Arial Bold", 8);
  b->setBackground(Chart::Transparent);
  b->setLineStyleKey();

  // Set legend icon style to use line style icon, sized for 8pt font
  c->getLegend()->setLineStyleKey();
  c->getLegend()->setFontSize(8);

  // Set the axis stem to transparent
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);

  // Add axis title using 10pts Arial Bold Italic font
  c->yAxis()->setTitle("Ionic Temperature (C)", "Arial Bold Italic", 10);

  ///////////////////////////////////////////////////////////////////////////////////////
  // Add data to chart
  ///////////////////////////////////////////////////////////////////////////////////////

  //
  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).
  //

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer* layer = c->addLineLayer();
  layer->setLineWidth(2);

  // In this demo, we do not have too many data points. In real code, the chart may contain a lot
  // of data points when fully zoomed out - much more than the number of horizontal pixels in this
  // plot area. So it is a good idea to use fast line mode.
  layer->setFastLineMode();

  // Now we add the 3 data series to a line layer, using the color red (ff0000), green
  // (00cc00) and blue (0000ff)
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0xff3333, "Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x008800, "Beta");
  layer->addDataSet(viewPortDataSeriesC, 0x3333CC, "Gamma");

  ///////////////////////////////////////////////////////////////////////////////////////
  // Configure axis scale and labelling
  ///////////////////////////////////////////////////////////////////////////////////////

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  //viewer->syncDateAxisWithViewPort("x", c->xAxis());
  c->xAxis()->setDateScale(startX, endX);

  //
  // In this demo, the time range can be from a few years to a few days. We demonstrate
  // how to set up different date/time format based on the time range.
  //

  // If all ticks are yearly aligned, then we use "yyyy" as the label format.
  c->xAxis()->setFormatCondition("align", 360 * 86400);
  c->xAxis()->setLabelFormat("{value|yyyy}");

  // If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first
  // label of a year, and "mmm" for other labels.
  c->xAxis()->setFormatCondition("align", 30 * 86400);
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|mmm yyyy}",
    Chart::AllPassFilter(), "{value|mmm}");

  // If all ticks are daily algined, then we use "mmm dd<*br*>yyyy" in bold font as the
  // first label of a year, and "mmm dd" in bold font as the first label of a month, and
  // "dd" for other labels.
  c->xAxis()->setFormatCondition("align", 86400);
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(),
    "<*block,halign=left*><*font=bold*>{value|mmm dd<*br*>yyyy}",
    Chart::StartOfMonthFilter(), "<*font=bold*>{value|mmm dd}");
  c->xAxis()->setMultiFormat(Chart::AllPassFilter(), "{value|dd}");

  // For all other cases (sub-daily ticks), use "hh:nn<*br*>mmm dd" for the first label of
  // a day, and "hh:nn" for other labels.
  c->xAxis()->setFormatCondition("else");
  c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(),
    "<*font=bold*>{value|hh:nn<*br*>mmm dd}", Chart::AllPassFilter(), "{value|hh:nn}");

  return c;
}

// The Pointer, Zoom In or Zoom out button is pressed
void
ZoomScrollPdf::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// User clicks on the the horizontal scroll bar
void
ZoomScrollPdf::OnScrollBarChanged(int value)
{
  if (!m_chartViewer->isInViewPortChangedEvent())
  {
    // Set the view port based on the scroll bar
    int scrollBarLen = m_scrollBar->GetRange();
    m_chartViewer->setViewPortLeft(value / (double)scrollBarLen);

    // Update the chart display without updating the image maps. (We can delay updating
    // the image map until scrolling is completed and the chart display is stable.)
    m_chartViewer->updateViewPort(true, false);
  }
}

// When the mouse enters the chart, we will generate an image map for hot spots and tooltips
// support if it has not already been generated.
void
ZoomScrollPdf::OnMouseWheelChart(wxCommandEvent& event)
{
  // Process the mouse wheel only if the mouse is over the plot area
  if (!m_chartViewer->isMouseOnPlotArea())
  {
    event.Skip();
    return;
  }

  // We zoom in or out by 10% depending on the mouse wheel direction.
  double newVpWidth = m_chartViewer->getViewPortWidth() * (event.GetInt() > 0 ? 0.9 : 1 / 0.9);
  double newVpHeight = m_chartViewer->getViewPortHeight() * (event.GetInt() > 0 ? 0.9 : 1 / 0.9);

  // We do not zoom beyond the zoom width or height limits.
  if (newVpWidth > m_chartViewer->getZoomOutWidthLimit())
    newVpWidth = m_chartViewer->getZoomOutWidthLimit();
  if (newVpWidth < m_chartViewer->getZoomInWidthLimit())
    newVpWidth = m_chartViewer->getZoomInWidthLimit();

  if (newVpHeight, m_chartViewer->getZoomOutHeightLimit())
    newVpHeight, m_chartViewer->getZoomOutHeightLimit();
  if (newVpHeight < m_chartViewer->getZoomInHeightLimit())
    newVpHeight = m_chartViewer->getZoomInHeightLimit();

  if ((newVpWidth != m_chartViewer->getViewPortWidth()) ||
      (newVpHeight != m_chartViewer->getViewPortHeight()))
  {
    // Set the view port position and size so that the point under the mouse remains under
    // the mouse after zooming.

    double deltaX = (m_chartViewer->getPlotAreaMouseX() - m_chartViewer->getPlotAreaLeft()) *
      (m_chartViewer->getViewPortWidth() - newVpWidth) / m_chartViewer->getPlotAreaWidth();
    m_chartViewer->setViewPortLeft(m_chartViewer->getViewPortLeft() + deltaX);
    m_chartViewer->setViewPortWidth(newVpWidth);

    double deltaY = (m_chartViewer->getPlotAreaMouseY() - m_chartViewer->getPlotAreaTop()) *
      (m_chartViewer->getViewPortHeight() - newVpHeight) / m_chartViewer->getPlotAreaHeight();
    m_chartViewer->setViewPortTop(m_chartViewer->getViewPortTop() + deltaY);
    m_chartViewer->setViewPortHeight(newVpHeight);

    m_chartViewer->updateViewPort(true, false);
  }
}

// Draw track cursor when mouse is moving over plotarea
void
ZoomScrollPdf::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackLineLabel((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();

  // Hide the track cursor when the mouse leaves the plot area
  m_chartViewer->removeDynamicLayer("mouseLeavePlotArea");
}

// Draw track line with data labels
void
ZoomScrollPdf::TrackLineLabel(XYChart* c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor,
    d->dashLineColor(0x000000, 0x0101));

  // Draw a label on the x-axis to show the track line position.
  std::ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
    << " <*/font*>";
  TTFText* t = d->text(xlabel.str().c_str(), "Arial Bold", 8);

  // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
  int xLabelPos = (std::max)(0, (std::min)(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
  t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
  t->destroy();

  // Iterate through all layers to draw the data labels
  for (int i = 0; i < c->getLayerCount(); ++i) {
    Layer* layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for (int j = 0; j < layer->getDataSetCount(); ++j)
    {
      DataSet* dataSet = layer->getDataSetByZ(j);
      const char* dataSetName = dataSet->getDataName();

      // Get the color, name and position of the data label
      int color = dataSet->getDataColor();
      int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

      // Draw a track dot with a label next to it for visible data points in the plot area
      if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
        Chart::Transparent) && dataSetName && *dataSetName)
      {
        d->circle(xCoor, yCoor, 4, 4, color, color);

        std::ostringstream label;
        label << "<*font,bgColor=" << std::hex << color << "*> "
          << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
        t = d->text(label.str().c_str(), "Arial Bold", 8);

        // Draw the label on the right side of the dot if the mouse is on the left side the
        // chart, and vice versa. This ensures the label will not go outside the chart image.
        if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
          t->draw(xCoor + 5, yCoor, 0xffffff, Chart::Left);
        else
          t->draw(xCoor - 5, yCoor, 0xffffff, Chart::Right);

        t->destroy();
      }
    }
  }
}

/*
 * Get bitmap resources
 */

wxBitmap
ZoomScrollPdf::GetBitmapResource( const wxString& name )
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
ZoomScrollPdf::OnPointerClick( wxCommandEvent& event )
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
ZoomScrollPdf::OnZoomInClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(true);
  m_zoomoutButton->SetValue(false);
  OnMouseUsageChanged(Chart::MouseUsageZoomIn);
}

/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
 */

void
ZoomScrollPdf::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

// The Save button is pressed
void ZoomScrollPdf::OnSave(wxCommandEvent& event)
{
  wxFileDialog saveFileDialog(this, _("Save graphics file"), wxS(""), wxS("chartdirector_demo"),
    wxS("PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...

  // save the current contents in the file;
  wxString fileName = saveFileDialog.GetPath();
  if (!fileName.IsEmpty())
  {
    BaseChart* c = m_chartViewer->getChart();
    if (0 != c)
    {
      c->makeChart(fileName.ToUTF8());
    }
  }
}

// User clicks on the Create PDF Report pushbutton
void ZoomScrollPdf::OnPdf(wxCommandEvent& event)
{
  wxFileDialog saveFileDialog(this, _("Save ChartDirector PDF Report"), wxS(""), wxS("chartdirector_report"),
    wxS("PDF (*.pdf)|*.pdf"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...

  // save the current contents in the file;
  wxString fileName = saveFileDialog.GetPath();
  if (!fileName.IsEmpty())
  {
    CreatePdfReport(fileName.ToUTF8());
  }
}

// Create a multi-page PDF Report
void
ZoomScrollPdf::CreatePdfReport(const char* filename)
{
  // The MultiPagePDF object can create PDF from multiple pages, each with one chart
  // object. Since a chart object can contain text (eg. using BaseChart.addText) and
  // other charts (eg. using MultiChart), that means each page can contain text and
  // multiple charts.
  MultiPagePDF doc;

  // Page configuration - A4 = 210 x 297mm. The PDF default is 96 dpi (dot per inch),
  // so the A4 size is equal to 794 x 1123 dots.
  const char* pageConfig = "pagewidth = 794; pageHeight = 1123";

  // In this example, we include a cover page with only text. This is by creating an
  // empty pie chart with text only.
  PieChart firstPage(720, 960);
  firstPage.addText(360, 320, "<*size=50*>ChartDirector<*br*><*size=30*>PDF Report Demonstration<*/*>",
    "Arial Bold", 30, 0x000000, Chart::Center);
  firstPage.setOutputOptions(pageConfig);
  doc.addPage(&firstPage);

  // We include 2 charts per page, with each chart showing one year of data. Each page
  // will also have a header and page number

  int startYear = Chart::getChartYMD(m_timeStamps[0]) / 10000;
  int endYear = Chart::getChartYMD(m_timeStamps[m_timeStamps.len - 1] - 1) / 10000;
  int pageNumber = 0;

  for (int yyyy = startYear; yyyy <= endYear; yyyy += 2)
  {
    // This chart is the page.
    MultiChart m(760, 920);

    // Use addTitle to add a header
    m.addTitle("ChartDirector PDF Report Demonstration", "Arial Bold", 20);

    // Create the first chart
    XYChart* c = DrawXYChart(Chart::chartTime(yyyy, 1, 1), Chart::chartTime(yyyy + 1, 1, 1));
    m.addChart((m.getWidth() - c->getWidth()) / 2, 100, c);
    c->addTitle(c->formatValue(yyyy, "Year {value}"));

    XYChart* c2 = 0;
    if (yyyy < endYear)
    {
      // Create the second chart
      c2 = DrawXYChart(Chart::chartTime(yyyy + 1, 1, 1), Chart::chartTime(yyyy + 2, 1, 1));
      c2->addTitle(c->formatValue(yyyy + 1, "Year {value}"));
      m.addChart((m.getWidth() - c2->getWidth()) / 2, 500, c2);
    }

    // Add the page number
    ++pageNumber;
    m.addTitle(Chart::BottomCenter, c->formatValue(pageNumber, "{value}"), "Arial Bold", 8);

    m.setOutputOptions(pageConfig);
    doc.addPage(&m);

    delete c;
    delete c2;
  }

  // Output the PDF report
  doc.outPDF(filename);
}

/*
* wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
*/

void
ZoomScrollPdf::OnScrollbarUpdated(wxScrollEvent& event)
{
  int value = event.GetPosition();
  bool enabled = m_scrollBar->IsEnabled();
  OnScrollBarChanged(value);
}
