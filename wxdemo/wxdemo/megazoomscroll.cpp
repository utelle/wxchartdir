/*
** Name:        megazoomscroll.cpp
** Purpose:     Mega Chart Zoom/Scroll with Track Line
** Author:      Ulrich Telle
** Created:     2021-06-25
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

#include <wx/thread.h>

#include "megazoomscroll.h"

#include <sstream>
#include <vector>
#include <array>

using namespace std;

#include "mondrian.xpm"

// In this example, we plot 3 data series, each with 10,000,000 data points.
// So the total is 30,000,000 data points.
static int bufferSize = 10000000;

// A thread task used for random number generator. Because of the large number of data
// points, we will create 3 random number generators running in 3 threads to speed up
// random number generation.
class RanSeriesTask : public wxThread
{
private:
  int seed;            // random number seed
  double* buffer;      // buffer to store the result
  int count;           // the count of random numbers

public:
  RanSeriesTask(int seed, double* buffer, int count)
    : wxThread(wxTHREAD_JOINABLE), seed(seed), buffer(buffer), count(count)
  {
  }
  virtual ExitCode Entry() wxOVERRIDE
  {
    // Generate random numbers
    RanSeries r(seed);
    r.fillSeries(buffer, count, 2500, -1, 1);
    return NULL;
  }
  virtual void OnExit() wxOVERRIDE
  {
  }

};

// A thread task used for processing the data series in the data accelerator. We will
// create 3 threads so that 3 data series can be processed concurrently.
class FastSeriesTask : public wxThread
{
private:
  DataAccelerator* fastDB;
  const char* id;
  const double* data;
  int len;

public:
  FastSeriesTask(DataAccelerator* fastDB, const char* id, const double* data, int len)
    : wxThread(wxTHREAD_JOINABLE), fastDB(fastDB), id(id), data(data), len(len)
  {
  }
  virtual ExitCode Entry() wxOVERRIDE
  {
    // Process the data series
    fastDB->addDataSeries(id, data, len);
    return NULL;
  }
};


/*
 * MegaZoomScroll type definition
 */

IMPLEMENT_CLASS(MegaZoomScroll, wxDialog )

/*
 * MegaZoomScroll event table definition
 */

BEGIN_EVENT_TABLE(MegaZoomScroll, wxDialog )

  EVT_TOGGLEBUTTON( ID_POINTER, MegaZoomScroll::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, MegaZoomScroll::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, MegaZoomScroll::OnZoomOutClick )
  EVT_BUTTON(ID_START, MegaZoomScroll::OnPlotChart)
  EVT_COMMAND_SCROLL(ID_SCROLLBAR, MegaZoomScroll::OnScrollbarUpdated)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, MegaZoomScroll::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, MegaZoomScroll::OnViewPortChanged)
  EVT_CHARTVIEWER_MOUSEWHEEL(ID_CHARTVIEWER, MegaZoomScroll::OnMouseWheelChart)

END_EVENT_TABLE()

/*
 * MegaZoomScroll constructors
 */

  MegaZoomScroll::MegaZoomScroll()
{
  Init();
}

MegaZoomScroll::MegaZoomScroll( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * MegaZoomScroll creator
 */

bool
MegaZoomScroll::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * MegaZoomScroll destructor
 */

MegaZoomScroll::~MegaZoomScroll()
{
  delete m_chartViewer->getChart();
  delete m_fastData;
}

/*
 * Member initialisation
 */

void
MegaZoomScroll::Init()
{
  m_fastData = 0;
  m_hasFinishedInitialization = false;
}

/*
 * Control creation for ZoomScrollTrack
 */

void
MegaZoomScroll::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  MegaZoomScroll* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer1);

  wxPanel* itemPanel2 = new wxPanel( itemFrame1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer1->Add(itemPanel2, 1, wxGROW|wxALL, 0);
  m_bgColour = itemPanel2->GetBackgroundColour();

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemPanel2->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW | wxALL, FromDIP(3));

  m_pointerButton = new wxToggleButton(itemPanel2, ID_POINTER, wxT(""), wxDefaultPosition, FromDIP(wxSize(32,32)), wxBU_EXACTFIT);
  m_pointerButton->SetBitmap(GetBitmapResource("pointer.png"));
  m_pointerButton->SetBitmapMargins(0, 0);
  // Initially set the mouse to drag to scroll mode
  m_pointerButton->SetValue(true);
  itemBoxSizer3->Add(m_pointerButton, 0, wxLEFT | wxALL, FromDIP(3));

  m_zoominButton = new wxToggleButton(itemPanel2, wxID_ZOOM_IN, wxT(""), wxDefaultPosition, FromDIP(wxSize(32,32)), wxBU_EXACTFIT);
  m_zoominButton->SetBitmap(GetBitmapResource("zoomin.png"));
  m_zoominButton->SetBitmapMargins(0, 0);
  m_zoominButton->SetValue(false);
  itemBoxSizer3->Add(m_zoominButton, 0, wxLEFT | wxALL, FromDIP(3));

  m_zoomoutButton = new wxToggleButton(itemPanel2, wxID_ZOOM_OUT, wxT(""), wxDefaultPosition, FromDIP(wxSize(32,32)), wxBU_EXACTFIT);
  m_zoomoutButton->SetBitmap(GetBitmapResource("zoomout.png"));
  m_zoomoutButton->SetBitmapMargins(0, 0);
  m_zoomoutButton->SetValue(false);
  itemBoxSizer3->Add(m_zoomoutButton, 0, wxGROW | wxALL, FromDIP(3));

  itemBoxSizer3->Add(3, 3, 1, wxALIGN_CENTER_VERTICAL | wxALL, FromDIP(3));

  m_startButton = new wxButton(itemPanel2, ID_START, _("Plot Chart"), wxDefaultPosition, FromDIP(wxSize(200,32)), wxBU_AUTODRAW);
  itemBoxSizer3->Add(m_startButton, 0, wxRIGHT | wxALL, 3);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(FromDIP(wxSize(650, 350)));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer2->Add(m_chartViewer, 1, wxGROW | wxALL, FromDIP(3));

  m_scrollBar = new wxScrollBar(itemPanel2, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
  m_scrollBar->SetScrollbar(0, 0, 1000000000, 200000000);
  itemBoxSizer2->Add(m_scrollBar, 0, wxGROW | wxALL, FromDIP(3));

  // Load the data
  LoadData();

  // Display initial Message
  PieChart* c = new PieChart(800, 400, 0xd0e0ff);
  c->addTitle(Chart::Center, "<*block,halign=left,maxwidth=500*>"
    "<*font=Arial Bold,size=18,underline=2*>Mega Chart Zoom/Scroll with Track Line<*/font*>"
    "<*br*><*br*>This example demonstrates a chart with huge amount of data. We limit "
    "it to 3 lines, each with 10 million points, so that it uses less than 1G of RAM "
    "(320M to store the data, 600M to plot the data and handle the GUI)."
    "<*br*><*br*><*br*>Press the Plot Chart button to plot the chart.");

  m_chartViewer->setChart(c);
  OnMouseUsageChanged(Chart::MouseUsageScroll);
}

// Load the data
void
MegaZoomScroll::LoadData()
{
  // Allocate space for the data arrays
  m_dataSeriesA.resize(bufferSize);
  m_dataSeriesB.resize(bufferSize);
  m_dataSeriesC.resize(bufferSize);
  m_timeStamps.resize(bufferSize);

  // To speed up random number generation, we use 3 threads to generate the random data
  // for the 3 data series. The current thread is used for generating the timestamps.
  std::array<wxThread*,3> threads;
  threads[0] = new RanSeriesTask(109, &m_dataSeriesA[0], (int)m_dataSeriesA.size());
  threads[1] = new RanSeriesTask(110, &m_dataSeriesB[0], (int)m_dataSeriesB.size());
  threads[2] = new RanSeriesTask(111, &m_dataSeriesC[0], (int)m_dataSeriesC.size());
  for (int j = 0; j < 3; ++j)
  {
    threads[j]->Run();
  }

  for (int i = 0; i < (int)m_timeStamps.size(); ++i)
  {
    m_timeStamps[i] = i;
  }

  if (!threads[0]->Wait() && !threads[1]->Wait() && !threads[2]->Wait())
  {
    for (int j = 0; j < 3; ++j)
    {
      delete threads[j];
    }
  }
}

// Initialize the wxChartViewer
void
MegaZoomScroll::InitChartViewer(wxChartViewer* viewer)
{
  // Set the full x range to be the duration of the data
  viewer->setFullRange("x", m_timeStamps[0], m_timeStamps[m_timeStamps.size() - 1]);

  // Initialize the view port to show the latest 20% of the time range
  viewer->setViewPortWidth(0.2);
  viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

  // Set the maximum zoom to 10 points
  viewer->setZoomInWidthLimit(10.0 / m_timeStamps.size());

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  viewer->setMouseWheelZoomRatio(1.1);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void MegaZoomScroll::OnViewPortChanged(wxCommandEvent& event)
{
  if (!m_hasFinishedInitialization)
  {
    return;
  }

  // In addition to updating the chart, we may also need to update other controls that
  // changes based on the view port.
  UpdateControls(m_chartViewer);

  // Update chart if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
}

// Update controls in the user interface when the view port changed
void
MegaZoomScroll::UpdateControls(wxChartViewer* viewer)
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
MegaZoomScroll::DrawChart(wxChartViewer* viewer)
{
  // Get the start date and end date that are visible on the chart.
  double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
  double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortRight());
  m_fastData->setSubsetRange(viewPortStartDate, viewPortEndDate);

  //
  // At this stage, we have extracted the visible data. We can use those data to plot the chart.
  //

  //================================================================================
  // Configure overall chart appearance.
  //================================================================================

  XYChart* c = new XYChart(800, 400);

  // Set the plotarea at (0, 0) with width 1 pixel less than chart width, and height 20 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(0, 0, c->getWidth() - 1, c->getHeight() - 20, c->linearGradientColor(0, 0, 0,
    c->getHeight() - 20, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // In our code, we can overdraw the line slightly, so we clip it to the plot area.
  c->setClipping();

  // Add a legend box at the right side using horizontal layout. Use 10pt Arial Bold as font. Set
  // the background and border color to Transparent and use line style legend key.
  LegendBox* b = c->addLegend(c->getWidth() - 1, 10, false, "Arial Bold", 10);
  b->setBackground(Chart::Transparent);
  b->setAlignment(Chart::Right);
  b->setLineStyleKey();

  // Set the x and y axis stems to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("Arial", 10);
  c->yAxis()->setLabelStyle("Arial", 10, 0x336699);

  // Configure the y-axis label to be inside the plot area and above the horizontal grid lines
  c->yAxis()->setLabelGap(-1);
  c->yAxis()->setMargin(20);
  c->yAxis()->setLabelAlignment(1);

  // Configure the x-axis labels to be to the left of the vertical grid lines
  c->xAxis()->setLabelAlignment(1);

  //================================================================================
  // Add data to chart
  //================================================================================

  //
  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).
  //

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer* layer = c->addLineLayer(m_fastData, "mA", 0xff0000, "Alpha");
  layer->setLineWidth(2);

  LineLayer* layer2 = c->addLineLayer(m_fastData, "mB", 0x00cc00, "Beta");
  layer2->setLineWidth(2);

  LineLayer* layer3 = c->addLineLayer(m_fastData, "mC", 0x0000ff, "Gamma");
  layer3->setLineWidth(2);

  //================================================================================
  // Configure axis scale and labelling
  //================================================================================

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  viewer->syncLinearAxisWithViewPort("x", c->xAxis());

  // For the automatic axis labels, set the minimum spacing to 75/40 pixels for the x/y axis.
  c->xAxis()->setTickDensity(75);
  c->yAxis()->setTickDensity(40);

  // Set the auto-scale margin to 0.05, and the zero affinity to 0.2
  c->yAxis()->setAutoScale(0.05, 0.05, 0.2);

  //================================================================================
  // Output the chart
  //================================================================================

  // We need to update the track line too. If the mouse is moving on the chart (eg. if
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if ((!viewer->isInMouseMoveEvent()) && viewer->isMouseOnPlotArea())
    TrackLineLabel(c, viewer->getPlotAreaMouseX());

  delete viewer->getChart();
  viewer->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
MegaZoomScroll::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// User clicks on the the horizontal scroll bar
void
MegaZoomScroll::OnScrollBarChanged(int value)
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
MegaZoomScroll::OnMouseWheelChart(wxCommandEvent& event)
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
MegaZoomScroll::OnMouseMovePlotArea(wxCommandEvent& event)
{
  if (!m_hasFinishedInitialization)
    return;

  TrackLineLabel((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}

// Draw track line with data labels
void
MegaZoomScroll::TrackLineLabel(XYChart* c, int mouseX)
{
  // Obtain the dynamic layer of the chart
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);
  if (xCoor < plotArea->getLeftX())
    return;

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

  // Draw a label on the x-axis to show the track line position.
  std::ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->formatValue(xValue, "{value}") << " <*/font*>";
  TTFText* t = d->text(xlabel.str().c_str(), "Arial Bold", 10);

  // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
  int xLabelPos = (std::max)(0, (std::min)(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
  t->draw(xLabelPos, plotArea->getBottomY() + 2, 0xffffff);
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
        t = d->text(label.str().c_str(), "Arial Bold", 10);

        // Draw the label on the right side of the dot if the mouse is on the left side the
        // chart, and vice versa. This ensures the label will not go outside the chart image.
        if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
          t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
        else
          t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);

        t->destroy();
      }
    }
  }
}

/*
 * Get bitmap resources
 */

wxBitmap
MegaZoomScroll::GetBitmapResource( const wxString& name )
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
MegaZoomScroll::OnPointerClick( wxCommandEvent& event )
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
MegaZoomScroll::OnZoomInClick( wxCommandEvent& event )
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
MegaZoomScroll::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_START
 */

void
MegaZoomScroll::OnPlotChart(wxCommandEvent& event)
{
  // Has already initialized ??
  if (m_hasFinishedInitialization)
    return;

  // Use the DataAccerlerate the accelerate the rendering. To speed up, we create two
  // threads to process two of the data series, and use the current thread to process
  // the third series.

  m_fastData = new DataAccelerator(&m_timeStamps[0], (int)m_timeStamps.size());

  wxThread* threadA = new FastSeriesTask(m_fastData, "mA", &m_dataSeriesA[0], (int)m_dataSeriesA.size());
  threadA->Run();

  wxThread* threadB = new FastSeriesTask(m_fastData, "mB", &m_dataSeriesB[0], (int)m_dataSeriesB.size());
  threadB->Run();

  m_fastData->addDataSeries("mC", &m_dataSeriesC[0], (int)m_dataSeriesC.size());

  threadA->Wait();
  delete threadA;

  threadB->Wait();
  delete threadB;

  // Initialize the CChartViewer
  InitChartViewer(m_chartViewer);
  m_hasFinishedInitialization = true;

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);
}

/*
* wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
*/

void
MegaZoomScroll::OnScrollbarUpdated(wxScrollEvent& event)
{
  int value = event.GetPosition();
  bool enabled = m_scrollBar->IsEnabled();
  OnScrollBarChanged(value);
}
