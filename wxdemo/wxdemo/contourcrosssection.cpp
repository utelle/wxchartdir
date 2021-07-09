/*
** Name:        contourcrosssection.cpp
** Purpose:     Contour Plot with Cross Section
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

#include "contourcrosssection.h"
#include "wxchartviewer.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

// ContourCrossSection type definition

IMPLEMENT_DYNAMIC_CLASS(ContourCrossSection, wxDialog )

// CrossHair event table definition

BEGIN_EVENT_TABLE( ContourCrossSection, wxDialog )

  EVT_MENU(wxID_EXIT, ContourCrossSection::OnExitClick)
  EVT_CLOSE(ContourCrossSection::OnCloseWindow)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, ContourCrossSection::OnMouseMovePlotArea)

END_EVENT_TABLE()

// ContourCrossSection constructors

ContourCrossSection::ContourCrossSection()
{
  Init();
}

ContourCrossSection::ContourCrossSection( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

// CrossHair creator
bool
ContourCrossSection::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();

  return true;
}

// ContourCrossSection destructor
ContourCrossSection::~ContourCrossSection()
{
  delete m_chartViewer->getChart();
  delete m_crossSectionViewerX->getChart();
  delete m_crossSectionViewerY->getChart();
}

// Member initialisation
void
ContourCrossSection::Init()
{
  m_chartViewer = NULL;
}

// Control creation for CrossHair
void
ContourCrossSection::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  ContourCrossSection* itemDialog1 = this;

  SetBackgroundColour(*wxWHITE);

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer1, 0, wxALIGN_LEFT | wxLEFT, FromDIP(15));

  // Main Chart Viewer
  m_chartViewer = new wxChartViewer(this, ID_CHARTVIEWER, wxDefaultPosition, FromDIP(wxSize(575, 525)));
  itemBoxSizer1->Add(m_chartViewer, 0, wxALIGN_CENTER_VERTICAL | wxTOP, FromDIP(5));

  // Chart Viewer for X Cross Section
  m_crossSectionViewerX = new wxChartViewer(this, ID_CROSSSECTION_CHARTVIEWER_X, wxDefaultPosition, FromDIP(wxSize(100, 525)));
  itemBoxSizer1->Add(m_crossSectionViewerX, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxRIGHT, FromDIP(5));

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_LEFT | wxLEFT, FromDIP(15));

  // Chart Viewer for Y Cross Section
  m_crossSectionViewerY = new wxChartViewer(this, ID_CROSSSECTION_CHARTVIEWER_X, wxDefaultPosition, FromDIP(wxSize(570, 100)));
  itemBoxSizer4->Add(m_crossSectionViewerY, 0, wxALIGN_CENTER_VERTICAL | wxBOTTOM, FromDIP(10));

  m_contourLayer = 0;
  m_xTrackPosition = 0;
  m_yTrackPosition = 0;
  m_isDragging = Drag_None;
  m_dragOffset = 0;

  // Draw the Chart
  DrawChart(m_chartViewer);
}

// Draw chart
void
ContourCrossSection::DrawChart(wxChartViewer* viewer)
{
  // The random data used in this example
  double dataX[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const int dataX_len = (int)(sizeof(dataX) / sizeof(*dataX));
  double dataY[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const int dataY_len = (int)(sizeof(dataX) / sizeof(*dataX));
  std::vector<double> dataZ(dataX_len * dataY_len);

  double pi = acos(-1);
  for (int iY = 0; iY < dataY_len; ++iY)
  {
    double y = dataY[iY];
    double ay = 11 * cos(iY * pi * 2 / (dataY_len - 1) + 1.1673);
    for (int iX = 0; iX < dataX_len; ++iX)
    {
      double x = dataX[iX];
      double ax = 11 * cos(iX * pi * 2 / (dataX_len - 1) + 0.8629);
      dataZ[iY * dataX_len + iX] = 20 + ((ax + 0.43) * sin(y) + (ay + 0.9166) * sin(x)
        + 0.0998) * sin((1 + fabs(x) / 10.0) * (1 + fabs(y) / 10.0) * 0.6667);
    }
  }

  // Create a XYChart object of size 575 x 525 pixels
  XYChart* c = new XYChart(575, 525);

  // Set the plotarea at (75, 40) and of size 400 x 400 pixels. Use semi-transparent black
  // (80000000) dotted lines for both horizontal and vertical grid lines
  PlotArea* p = c->setPlotArea(75, 30, 450, 450, -1, -1, -1, c->dashLineColor((int)0xaf000000,
    Chart::DotLine), -1);

  // Set the chart and axis titles
  c->addTitle("     <*block,bgcolor=FFFF00*> *** Drag Crosshair to Move Cross Section *** <*/*>",
    "Arial Bold", 15);
  c->xAxis()->setTitle("X-Axis Title Place Holder", "Arial Bold Italic", 10);
  c->yAxis()->setTitle("Y-Axis Title Place Holder", "Arial Bold Italic", 10);

  // In this example, we put the y-axis on the right side of the chart
  c->setYAxisOnRight();

  // Set x-axis and y-axis labels to use 10pt Arial font
  c->xAxis()->setLabelStyle("Arial", 10);
  c->yAxis()->setLabelStyle("Arial", 10);

  // When auto-scaling, use tick spacing of 40 pixels as a guideline
  c->yAxis()->setTickDensity(40);
  c->xAxis()->setTickDensity(40);

  // Add a contour layer using the given data
  m_contourLayer = c->addContourLayer(DoubleArray(dataX, dataX_len),
    DoubleArray(dataY, dataY_len), DoubleArray(&dataZ[0], (int)dataZ.size()));

  // Enable contour label using 10pt Arial Bold font
  m_contourLayer->setContourLabelFormat("<*font=Arial Bold,size=10*>{value}<*/font*>");

  // Move the grid lines in front of the contour layer
  c->getPlotArea()->moveGridBefore(m_contourLayer);

  // Add a vertical color axis (the legend) at x = 0 at the same y-position as the plot area.
  ColorAxis* cAxis = m_contourLayer->setColorAxis(0, p->getTopY(), Chart::TopLeft,
    p->getHeight(), Chart::Right);
  // Use continuous gradient coloring (as opposed to step colors)
  cAxis->setColorGradient(true);

  // Add a title to the color axis using 12 points Arial Bold Italic font
  cAxis->setTitle("Color Legend Title Place Holder", "Arial Bold Italic", 10);

  // Set color axis labels to use Arial Bold font
  cAxis->setLabelStyle("Arial", 10);

  // Set the chart image to the WinChartViewer
  viewer->setChart(c);

  // Tooltip for the contour chart
  viewer->setImageMap(c->getHTMLImageMap("", "",
    "title='<*cdml*><*font=Arial Bold*>X={x|2}\nY={y|2}\nZ={z|2}'"));

  // Draw the cross section
  DrawCrossSectionX(m_xTrackPosition = (p->getLeftX() + p->getRightX()) / 2);
  DrawCrossSectionY(m_yTrackPosition = (p->getTopY() + p->getBottomY()) / 2);
  DrawCrossHair(viewer);
}

// Draw the X cross section
void ContourCrossSection::DrawCrossSectionX(int x)
{
  XYChart* c = (XYChart*) m_chartViewer->getChart();
  DrawCrossSectionX(m_crossSectionViewerX, (XYChart*) m_chartViewer->getChart(), m_contourLayer, x);
  m_crossSectionViewerX->updateDisplay();
}

void ContourCrossSection::DrawCrossSectionX(wxChartViewer* viewer, XYChart* mainChart,
  ContourLayer* contourLayer, int x)
{
  // Get the vertical cross section data at the given x coordinate
  PlotArea* p = mainChart->getPlotArea();
  DoubleArray z = contourLayer->getCrossSection(x, p->getBottomY(), x, p->getTopY());

  // Create XYChat of the same height as the main chart. Align the plot area with that of the
  // main chart.
  XYChart* c = new XYChart(100, mainChart->getHeight());
  c->setPlotArea(10, p->getTopY(), c->getWidth() - 22, p->getHeight(), -1, -1, -1,
    c->dashLineColor((int)0xaf000000, Chart::DotLine), -1);

  // The vertical chart will have the x-axis vertical and y-axis horizontal. Synchroinze the
  // vertical axis (x-axis) with the y-axis of the main chart. Synchroinze the horizontal
  // axis (y-axis) with the color axis.
  c->swapXY();
  c->xAxis()->syncAxis(mainChart->yAxis());
  c->yAxis()->syncScale(contourLayer->colorAxis());

  // The vertical axis (x-axis) does not need labels as it is aligned with the main chart y-axis.
  c->xAxis()->setLabelStyle("normal", 8, Chart::Transparent);

  // Rotate the horizontal axis (y-axis) labels by 270 degrees
  c->yAxis()->setLabelStyle("normal", 8, Chart::TextColor, 270);

  // Add an area layer using the cross section data and the color scale of the color axis.
  AreaLayer* layer = c->addAreaLayer(z, c->yScaleColor(contourLayer->colorAxis()->getColorScale()));
  layer->setBorderColor(Chart::SameAsMainColor);
  layer->setXData(mainChart->getYValue(p->getBottomY()), mainChart->getYValue(p->getTopY()));

  // Display the chart
  delete viewer->getChart();
  viewer->setChart(c);
}

//
// Draw the Y cross section
//
void ContourCrossSection::DrawCrossSectionY(int y)
{
  DrawCrossSectionY(m_crossSectionViewerY, (XYChart*) m_chartViewer->getChart(), m_contourLayer, y);
  m_crossSectionViewerY->updateDisplay();
}

void ContourCrossSection::DrawCrossSectionY(wxChartViewer* viewer, XYChart* mainChart,
  ContourLayer* contourLayer, int y)
{
  // Get the vertical horizontal section data at the given y coordinate
  PlotArea* p = mainChart->getPlotArea();
  DoubleArray z = contourLayer->getCrossSection(p->getLeftX(), y, p->getRightX(), y);

  // Create XYChat of the same width as the main chart. Align the plot area with that of the
  // main chart.
  XYChart* c = new XYChart(mainChart->getWidth(), 100);
  c->setPlotArea(p->getLeftX(), 10, p->getWidth(), c->getHeight() - 22, -1, -1, -1,
    c->dashLineColor((int)0xaf000000, Chart::DotLine), -1);

  // Synchroinze the x-axis with the x-axis of the main chart. Synchroinze the y-axis with the
  // color axis.
  c->xAxis()->syncAxis(mainChart->xAxis());
  c->yAxis()->syncScale(contourLayer->colorAxis());

  // The x-axis does not need labels as it is aligned with the main chart x-axis.
  c->xAxis()->setLabelStyle("normal", 8, Chart::Transparent);

  AreaLayer* layer = c->addAreaLayer(z, c->yScaleColor(contourLayer->colorAxis()->getColorScale()));
  layer->setBorderColor(Chart::SameAsMainColor);
  layer->setXData(mainChart->getXValue(p->getLeftX()), mainChart->getXValue(p->getRightX()));

  // Display the chart
  delete viewer->getChart();
  viewer->setChart(c);
}

//
// Draw the crosshair, which is just two straight lines
//
void ContourCrossSection::DrawCrossHair(wxChartViewer* viewer)
{
  // Get the chart to draw the crosshair on.
  XYChart* c = (XYChart*) viewer->getChart();

  // The crosshair will be drawn on the dynamic layer of the chart.
  DrawArea* d = c->initDynamicLayer();

  // Add two lines across the plot area of the chart
  PlotArea* p = c->getPlotArea();
  d->line(m_xTrackPosition, p->getTopY(), m_xTrackPosition, p->getBottomY(), 0x000000, 2);
  d->line(p->getLeftX(), m_yTrackPosition, p->getRightX(), m_yTrackPosition, 0x000000, 2);

  // Update the display
  viewer->updateDisplay();
}


// Draw track cursor when mouse is moving over plotarea
void
ContourCrossSection::OnMouseMovePlotArea(wxCommandEvent& evt)
{
  wxChartViewer* viewer = m_chartViewer;
  XYChart* c = (XYChart*) viewer->getChart();
  PlotArea* p = c->getPlotArea();

  int mouseX = viewer->getChartMouseX();
  int mouseY = viewer->getChartMouseY();

  // Drag occurs if mouse is moving and mouse button is down
  if (wxGetMouseState().ButtonIsDown(wxMOUSE_BTN_ANY))
  {
    // If the mouse is near the crosshair while the mouse button is pressed, then it is drag
    // dragging the crosshair and we need to update the contour projection.

    if ((m_isDragging == Drag_Vertical) && (m_xTrackPosition != mouseX - m_dragOffset))
    {
      // Is dragging the vertical crosshair line
      m_xTrackPosition = (std::max)(p->getLeftX(), (std::min)(p->getRightX(), mouseX - m_dragOffset));
      DrawCrossSectionX(m_xTrackPosition);
      DrawCrossHair(viewer);
    }

    if ((m_isDragging == Drag_Horizontal) && (m_yTrackPosition != mouseY - m_dragOffset))
    {
      // Is dragging the horizontal crosshair line
      m_yTrackPosition = (std::max)(p->getTopY(), (std::min)(p->getBottomY(), mouseY - m_dragOffset));
      DrawCrossSectionY(m_yTrackPosition);
      DrawCrossHair(viewer);
    }
  }
  else
  {
    // If mouse is near the crosshair, it can drag it by pressing the mouse button.
    if (abs(m_dragOffset = mouseX - m_xTrackPosition) <= 8)
      m_isDragging = Drag_Vertical;   // Mouse is a position to drag the vertical line
    else if (abs(m_dragOffset = mouseY - m_yTrackPosition) <= 8)
      m_isDragging = Drag_Horizontal; // Mouse is a position to drag the horizontal line
    else
      m_isDragging = Drag_None;
  }
}

// Draw cross hair cursor with axis labels
void
ContourCrossSection::Crosshair(XYChart* c, int mouseX, int mouseY)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Draw a vertical line and a horizontal line as the cross hair
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), mouseX, d->dashLineColor(0x000000, 0x0101));
  d->hline(plotArea->getLeftX(), plotArea->getRightX(), mouseY, d->dashLineColor(0x000000, 0x0101));

  // Draw y-axis label
  ostringstream ylabel;
  ylabel << "<*block,bgColor=FFFFDD,margin=3,edgeColor=000000*>" 
         << c->formatValue(c->getYValue(mouseY, c->yAxis()), "{value|P4}") << "<*/*>";
  TTFText* t = d->text(ylabel.str().c_str(), "Arial Bold", 8);
  t->draw(plotArea->getLeftX() - 5, mouseY, 0x000000, Chart::Right);
  t->destroy();

  // Draw x-axis label
  ostringstream xlabel;
  xlabel << "<*block,bgColor=FFFFDD,margin=3,edgeColor=000000*>" 
         << c->formatValue(c->getXValue(mouseX), "{value|P4}") << "<*/*>";
  t = d->text(xlabel.str().c_str(), "Arial Bold", 8);
  t->draw(mouseX, plotArea->getBottomY() + 5, 0x000000, Chart::Top);
  t->destroy();
}

void
ContourCrossSection::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

// wxEVT_CLOSE_WINDOW event handler for ID_CROSSHAIR
void
ContourCrossSection::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}
