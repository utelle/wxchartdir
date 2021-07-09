/*
** Name:        threedchartrotation.cpp
** Purpose:     3D Chart Interactive Rotation
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

#include "threedchartrotation.h"
#include "wxchartviewer.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

// ThreeDChartRotation type definition

IMPLEMENT_DYNAMIC_CLASS(ThreeDChartRotation, wxDialog )

// CrossHair event table definition

BEGIN_EVENT_TABLE(ThreeDChartRotation, wxDialog )

  EVT_MENU(wxID_EXIT, ThreeDChartRotation::OnExitClick)
  EVT_CLOSE(ThreeDChartRotation::OnCloseWindow)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER, ThreeDChartRotation::OnClickChart)
  EVT_CHARTVIEWER_MOUSEMOVE_CHART(ID_CHARTVIEWER, ThreeDChartRotation::OnMouseMoveChart)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, ThreeDChartRotation::OnViewPortChanged)

END_EVENT_TABLE()

// ThreeDChartRotation constructors

ThreeDChartRotation::ThreeDChartRotation()
{
  Init();
}

ThreeDChartRotation::ThreeDChartRotation( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

// ThreeDChartRotation creator
bool
ThreeDChartRotation::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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

// ThreeDChartRotation destructor
ThreeDChartRotation::~ThreeDChartRotation()
{
  delete m_chartViewer->getChart();
}

// Member initialisation
void
ThreeDChartRotation::Init()
{
  m_chartViewer = NULL;
}

// Control creation for ThreeDChartRotation
void
ThreeDChartRotation::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  ThreeDChartRotation* itemDialog1 = this;

  SetBackgroundColour(*wxWHITE);

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_LEFT | wxLEFT, FromDIP(15));

  m_drawFrameOnRotate = new wxCheckBox(this, ID_DRAWFRAME, _("Draw Frame on Rotate"), wxDefaultPosition, wxDefaultSize, 0);
  m_drawFrameOnRotate->SetValue(true);
  itemBoxSizer3->Add(m_drawFrameOnRotate, 0, wxALIGN_LEFT | wxALL, FromDIP(3));

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer1, 0, wxALIGN_LEFT | wxLEFT, FromDIP(15));

  // Main Chart Viewer
  m_chartViewer = new wxChartViewer(this, ID_CHARTVIEWER, wxDefaultPosition, FromDIP(wxSize(720, 600)));
  itemBoxSizer1->Add(m_chartViewer, 0, wxALIGN_CENTER_VERTICAL | wxTOP, FromDIP(5));

  // 3D view angles
  m_elevationAngle = 30;
  m_rotationAngle = 45;

  // Keep track of mouse drag
  m_isDragging = false;
  m_lastMouseX = -1;
  m_lastMouseY = -1;

  // Update the viewport to display the chart
  m_chartViewer->updateViewPort(true, false);
}

// Draw chart
void
ThreeDChartRotation::DrawChart(wxChartViewer* viewer)
{
  // The x and y coordinates of the grid
  double dataX[] = { -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const int dataXLength = (int)(sizeof(dataX) / sizeof(*dataX));

  double dataY[] = { -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const int dataYLength = (int)(sizeof(dataY) / sizeof(*dataY));

  // The values at the grid points. In this example, we will compute the values using the
  // formula z = x * sin(y) + y * sin(x).
  double dataZ[dataXLength * dataYLength];
  for (int yIndex = 0; yIndex < dataYLength; ++yIndex)
  {
    double y = dataY[yIndex];
    for (int xIndex = 0; xIndex < dataXLength; ++xIndex)
    {
      double x = dataX[xIndex];
      dataZ[yIndex * dataXLength + xIndex] = x * sin(y) + y * sin(x);
    }
  }

  // Create a SurfaceChart object of size 720 x 600 pixels
  SurfaceChart* c = new SurfaceChart(720, 600);

  // Set the center of the plot region at (330, 290), and set width x depth x height to
  // 360 x 360 x 270 pixels
  c->setPlotRegion(330, 290, 360, 360, 270);

  // Set the data to use to plot the chart
  c->setData(DoubleArray(dataX, dataXLength), DoubleArray(dataY, dataYLength),
    DoubleArray(dataZ, dataXLength * dataYLength));

  // Spline interpolate data to a 80 x 80 grid for a smooth surface
  c->setInterpolation(80, 80);

  // Set the view angles
  c->setViewAngle(m_elevationAngle, m_rotationAngle);

  // Check if draw frame only during rotation
  if (m_isDragging && m_drawFrameOnRotate->IsChecked())
    c->setShadingMode(Chart::RectangularFrame);

  // Add a color axis (the legend) in which the left center is anchored at (660, 270). Set
  // the length to 200 pixels and the labels on the right side.
  c->setColorAxis(650, 270, Chart::Left, 200, Chart::Right);

  // Set the x, y and z axis titles using 10 points Arial Bold font
  c->xAxis()->setTitle("X", "Arial Bold", 15);
  c->yAxis()->setTitle("Y", "Arial Bold", 15);

  // Set axis label font
  c->xAxis()->setLabelStyle("Arial", 10);
  c->yAxis()->setLabelStyle("Arial", 10);
  c->zAxis()->setLabelStyle("Arial", 10);
  c->colorAxis()->setLabelStyle("Arial", 10);

  // Output the chart
  delete viewer->getChart();
  viewer->setChart(c);

  //include tool tip for the chart
  viewer->setImageMap(c->getHTMLImageMap("", "",
    "title='<*cdml*>X: {x|2}<*br*>Y: {y|2}<*br*>Z: {z|2}'"));
}

void
ThreeDChartRotation::OnViewPortChanged(wxCommandEvent& event)
{
  // Update chart if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
}

void
ThreeDChartRotation::OnClickChart(wxCommandEvent& event)
{
  
  if (m_isDragging && !wxGetMouseState().ButtonIsDown(wxMOUSE_BTN_ANY))
  {
    // mouse up means not dragging
    m_isDragging = false;
    m_chartViewer->updateViewPort(true, false);
  }
}

// Draw track cursor when mouse is moving over plotarea
void
ThreeDChartRotation::OnMouseMoveChart(wxCommandEvent& evt)
{
  int mouseX = m_chartViewer->getChartMouseX();
  int mouseY = m_chartViewer->getChartMouseY();

  // Drag occurs if mouse is moving and mouse button is down
  if (wxGetMouseState().ButtonIsDown(wxMOUSE_BTN_ANY))
  {
    if (m_isDragging)
    {
      // The chart is configured to rotate by 90 degrees when the mouse moves from
      // left to right, which is the plot region width (360 pixels). Similarly, the
      // elevation changes by 90 degrees when the mouse moves from top to buttom,
      // which is the plot region height (270 pixels).
      m_rotationAngle += (m_lastMouseX - mouseX) * 90.0 / 360;
      m_elevationAngle += (mouseY - m_lastMouseY) * 90.0 / 270;
      m_chartViewer->updateViewPort(true, false);
    }

    // Keep track of the last mouse position
    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;
    m_isDragging = true;
  }
  else if (m_isDragging)
  {
    // mouse up means not dragging
    m_isDragging = false;
    m_chartViewer->updateViewPort(true, false);
  }
}

void
ThreeDChartRotation::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

// wxEVT_CLOSE_WINDOW event handler for ID_CROSSHAIR
void
ThreeDChartRotation::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}
