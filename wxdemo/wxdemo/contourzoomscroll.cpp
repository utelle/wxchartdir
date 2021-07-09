/*
** Name:        contourzoomscroll.cpp
** Purpose:     Contour Plot Zooming and Scrolling
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

#include <wx/mstream.h>
#include <wx/wfstream.h>

#include "contourzoomscroll.h"

#include <sstream>
#include <vector>

using namespace std;

#include "mondrian.xpm"

/*
 * ContourZoomScroll type definition
 */

IMPLEMENT_CLASS(ContourZoomScroll, wxDialog)

/*
 * ContourZoomScroll event table definition
 */

BEGIN_EVENT_TABLE( ContourZoomScroll, wxDialog )

  EVT_TOGGLEBUTTON( ID_POINTER, ContourZoomScroll::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, ContourZoomScroll::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, ContourZoomScroll::OnZoomOutClick )
  EVT_BUTTON( wxID_SAVE, ContourZoomScroll::OnSave )
  EVT_SLIDER( ID_SLIDER, ContourZoomScroll::OnSliderUpdated )
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, ContourZoomScroll::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * ContourZoomScroll constructors
 */

ContourZoomScroll::ContourZoomScroll()
{
  Init();
}

ContourZoomScroll::ContourZoomScroll( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*
 * ContourZoomScroll creator
 */

bool
ContourZoomScroll::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * ContourZoomScroll destructor
 */

ContourZoomScroll::~ContourZoomScroll()
{
  // Delete the old chart in m_ChartViewer unless it is the first chart
  if (m_chartViewer->getChart() != firstChart)
  {
    delete m_chartViewer->getChart();
  }

  // Delete the first chart
  delete firstChart;

  // Delete the chart in the m_ViewPortControl
  delete m_viewPortControl->getChart();
}

/*
 * Member initialisation
 */

void
ContourZoomScroll::Init()
{
}

/*
 * Control creation for XyZoomScroll
 */

void
ContourZoomScroll::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  ContourZoomScroll* itemFrame1 = this;

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

  m_saveButton = new wxButton(itemPanel2, wxID_SAVE, _(" &Save"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_saveButton->SetBitmap(GetBitmapResource("save.png"));
  m_saveButton->SetBitmapMargins(FromDIP(10), FromDIP(0));
  itemStaticBoxSizer1->Add(m_saveButton, 0, wxGROW | wxALL, FromDIP(3));

  itemStaticBoxSizer1->Add(3, 3, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(3));

  wxStaticText* itemStaticText2 = new wxStaticText(itemPanel2, wxID_STATIC, _("Zoom Level"), wxDefaultPosition, wxDefaultSize, 0);
  itemStaticBoxSizer1->Add(itemStaticText2, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT | wxTOP, FromDIP(3));

  m_zoomBar = new wxSlider(itemPanel2, ID_SLIDER, 100, 1, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_TOP);
  itemStaticBoxSizer1->Add(m_zoomBar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(3));

  itemStaticBoxSizer1->Add(3, 3, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(3));

  m_viewPortControl = new wxViewPortControl(itemPanel2, ID_VIEWPORT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER);
  m_viewPortControl->SetMinSize(FromDIP(wxSize(120, 120)));

  // Configure the CViewPortControl to use transparent black for region outside the viewport,
  // that is, to darken the outside region.
  m_viewPortControl->setViewPortExternalColor(0x7f000000);
  m_viewPortControl->setViewPortBorderColor(0x7fffffff);
  m_viewPortControl->setSelectionBorderColor(0x7fffffff);

  m_viewPortControl->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemStaticBoxSizer1->Add(m_viewPortControl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

  itemStaticBoxSizer1->Add(3, 3, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, FromDIP(3));

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, FromDIP(3));

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(FromDIP(wxSize(650, 350)));

  // Initialize member variables
  firstChart = 0;
  firstColorAxis = 0;

  // Set initial zoom/scroll direction
  m_chartViewer->setScrollDirection(Chart::DirectionHorizontalVertical);
  m_chartViewer->setZoomDirection(Chart::KeepAspectRatio);

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  m_chartViewer->setMouseWheelZoomRatio(1.1);

  // Configure CDML tooltip to use a semi-transparent rounded rectangle as container and
  // Arial Bold as font
  m_chartViewer->setCDMLToolTipPrefix("<*block,bgColor=60FFFFDD,roundedCorners=5,margin=5,edgeColor=000000*><*font=Arial Bold*>");

  m_chartViewer->setMouseUsage(Chart::MouseUsageScroll);

  // Load the data
  LoadData();

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);

  // Configure the CViewPortControl to use transparent black for region outside the viewport,
  // that is, to darken the outside region.
  m_viewPortControl->setViewPortExternalColor(0x7f000000);
  m_viewPortControl->setViewPortBorderColor(0x7fffffff);
  m_viewPortControl->setSelectionBorderColor(0x7fffffff);

  // Draw and display the full chart in the wxViewPortControl
  DrawFullChart(m_viewPortControl, m_chartViewer);

  // Bind the wxChartViewer to the wxViewPortControl
  m_viewPortControl->setViewer(m_chartViewer);

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, FromDIP(3));
}

// Load the data
void
ContourZoomScroll::LoadData()
{
  // For simplicity, in this demo, we just use hard coded data.
    // The x and y coordinates of the grid
  double x[] = { -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  double y[] = { -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  dataX.assign(x, x + sizeof(x) / sizeof(*x));
  dataY.assign(y, y + sizeof(y) / sizeof(*y));

  // The values at the grid points. In this example, we will compute the values using the
  // formula z = x * sin(y) + y * sin(x).
  dataZ.resize(dataX.size() * dataY.size());
  for (int yIndex = 0; yIndex < (int)dataY.size(); ++yIndex)
  {
    double y = dataY[yIndex];
    for (int xIndex = 0; xIndex < (int)dataX.size(); ++xIndex)
    {
      double x = dataX[xIndex];
      dataZ[yIndex * dataX.size() + xIndex] = x * sin(y) + y * sin(x);
    }
  }
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
ContourZoomScroll::OnViewPortChanged(wxCommandEvent& event)
{
  // In addition to updating the chart, we may also need to update other controls that
  // changes based on the view port.
  UpdateControls(m_chartViewer);

  // Update chart and image map if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
}

// Update other controls that may be affected by the viewport.
void
ContourZoomScroll::UpdateControls(wxChartViewer* viewer)
{
  // Update the Zoom slider to reflect the current zoom level of the view port
  double smallerSide = viewer->getViewPortWidth() > viewer->getViewPortHeight()
                     ? viewer->getViewPortHeight() : viewer->getViewPortWidth();
  m_zoomBar->SetValue((int)(smallerSide * m_zoomBar->GetMax() + 0.5));
}

// Draw the chart and display it in the given viewer
void
ContourZoomScroll::DrawChart(wxChartViewer* viewer)
{
  // Create an XYChart object 500 x 480 pixels in size, with the same background color
  // as the container
  XYChart* c = new XYChart(540, 490);

  // Set the plotarea at (75, 40) and of size 400 x 400 pixels. Use semi-transparent black
  // (80000000) dotted lines for both horizontal and vertical grid lines
  c->setPlotArea(50, 40, 400, 400, -1, -1, -1, c->dashLineColor(0x80000000, Chart::DotLine), -1);

  // Enable clipping mode to clip the part of the data that is outside the plot area.
  c->setClipping();

  // Add a title to the chart using 15 points Arial Bold Italic font
  c->addTitle("z = x * sin(y) + y * sin(x)      ", "Arial Bold Italic", 15);

  // Set x-axis and y-axis title using 12 points Arial Bold Italic font
  c->xAxis()->setTitle("X-Axis Title Place Holder", "Arial Bold Italic", 12);
  c->yAxis()->setTitle("Y-Axis Title Place Holder", "Arial Bold Italic", 12);

  // Set x-axis and y-axis labels to use Arial Bold font
  c->xAxis()->setLabelStyle("Arial Bold");
  c->yAxis()->setLabelStyle("Arial Bold");

  // When auto-scaling, use tick spacing of 40 pixels as a guideline
  c->yAxis()->setTickDensity(40);
  c->xAxis()->setTickDensity(40);

  // Add a contour layer using the given data
  ContourLayer* layer = c->addContourLayer(vectorToArray(dataX), vectorToArray(dataY),
    vectorToArray(dataZ));

  // Move the grid lines in front of the contour layer
  c->getPlotArea()->moveGridBefore(layer);

  // Add a color axis (the legend) in which the top left corner is anchored at (505, 40). Set the
  // length to 400 pixels and the labels on the right side.
  ColorAxis* cAxis = layer->setColorAxis(470, 40, Chart::TopLeft, 400, Chart::Right);

  // Add a title to the color axis using 12 points Arial Bold Italic font
  cAxis->setTitle("Color Legend Title Place Holder", "Arial Bold Italic", 12)
    ->setAlignment(Chart::Right);

  // Set color axis labels to use Arial Bold font
  cAxis->setLabelStyle("Arial Bold");

  // In this example, we have not explicitly configured the full x and y range. In this case,
  // the first time syncLinearAxisWithViewPort is called, ChartDirector will auto-scale the axis
  // and assume the resulting range is the full range. In subsequent calls, ChartDirector will
  // set the axis range based on the view port and the full range.
  viewer->syncLinearAxisWithViewPort("x", c->xAxis());
  viewer->syncLinearAxisWithViewPort("y", c->yAxis());

  // To ensure the color is consistent, we must keep the color axis the same scale when the
  // user zooms or scorlls the chart. To do this, we save the color axis of the first chart.
  // and synchronize all future charts with this color axis.
  if (0 == firstChart)
  {
    // We need to save the first chart as well as the first color axis. Otherwise the first
    // chart may get deleted, and this will delete the first color axis as well.
    firstChart = c;
    firstColorAxis = cAxis;
  }
  else
    cAxis->syncAxis(firstColorAxis);

  // Delete the old chart unless it is the first chart
  if (viewer->getChart() != firstChart)
    delete viewer->getChart();

  // Display chart
  viewer->setChart(c);

  // Tooltip for the contour chart
  viewer->setImageMap(c->getHTMLImageMap("", "", "title='<*cdml*>X={x|2}\nY={y|2}\nZ={z|2}'"));
}

// Draw the full thumbnail chart and display it in the given CViewPortControl
void
ContourZoomScroll::DrawFullChart(wxViewPortControl* vpc, wxChartViewer* viewer)
{
  // Create an XYChart object 110 x 110 pixels in size
  XYChart* c = new XYChart(110, 110);

  // Set the plotarea to cover the entire chart. Disable grid lines by setting their colors
  // to transparent.
  c->setPlotArea(0, 0, c->getWidth() - 1, c->getHeight() - 1, -1, -1, -1, Chart::Transparent,
    Chart::Transparent);

  // Add a contour layer using the given data
  ContourLayer* layer = c->addContourLayer(vectorToArray(dataX), vectorToArray(dataY),
    vectorToArray(dataZ));
  layer->setContourColor(0xcf000000);

  //layer->colorAxis()->syncAxis(firstColorAxis);

  // The x and y axis scales reflect the full range of the view port
  c->xAxis()->setLinearScale(viewer->getValueAtViewPort("x", 0), viewer->getValueAtViewPort("x", 1),
    Chart::NoValue);
  c->yAxis()->setLinearScale(viewer->getValueAtViewPort("y", 0), viewer->getValueAtViewPort("y", 1),
    Chart::NoValue);

  // Set the chart image to the WinChartViewer
  delete vpc->getChart();
  vpc->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
ContourZoomScroll::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

/*
 * Get bitmap resources
 */

wxBitmap
ContourZoomScroll::GetBitmapResource( const wxString& name )
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
  else if (name == wxT("save.png"))
  {
    image.LoadFile(wxT("./icons/save_icon.png"), wxBITMAP_TYPE_PNG);
    image.Rescale(FromDIP(16), FromDIP(16));
    return wxBitmap(image);
  }
  return wxNullBitmap;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
ContourZoomScroll::OnPointerClick( wxCommandEvent& event )
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
ContourZoomScroll::OnZoomInClick( wxCommandEvent& event )
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
ContourZoomScroll::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

// The Save button is pressed
void
ContourZoomScroll::OnSave(wxCommandEvent& event)
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

/*
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
 */

void
ContourZoomScroll::OnSliderUpdated( wxCommandEvent& event )
{
  int value = m_zoomBar->GetValue();
  OnZoomBarChanged(value);
}

// User moves the Zoom slider control
void
ContourZoomScroll::OnZoomBarChanged(int value)
{
  if (!m_chartViewer->isInViewPortChangedEvent())
  {
    // Remember the center point
    double centerX = m_chartViewer->getViewPortLeft() + m_chartViewer->getViewPortWidth() / 2;
    double centerY = m_chartViewer->getViewPortTop() + m_chartViewer->getViewPortHeight() / 2;

    // Aspect ratio and zoom factor
    double aspectRatio = m_chartViewer->getViewPortWidth() / m_chartViewer->getViewPortHeight();
    double zoomTo = ((double)value) / m_zoomBar->GetMax();

    // Zoom by adjusting ViewPortWidth and ViewPortHeight while maintaining the aspect ratio
    m_chartViewer->setViewPortWidth(zoomTo * ((aspectRatio < 1) ? 1 : aspectRatio));
    m_chartViewer->setViewPortHeight(zoomTo * ((aspectRatio > 1) ? 1 : (1 / aspectRatio)));

    // Adjust ViewPortLeft and ViewPortTop to keep center point unchanged
    m_chartViewer->setViewPortLeft(centerX - m_chartViewer->getViewPortWidth() / 2);
    m_chartViewer->setViewPortTop(centerY - m_chartViewer->getViewPortHeight() / 2);

    // Update the chart image only, but no need to update the image map.
    m_chartViewer->updateViewPort(true, false);
  }
}

// Convert std::vector to a DoubleArray
DoubleArray
ContourZoomScroll::vectorToArray(std::vector<double> &v)
{
  return (v.size() == 0) ? DoubleArray() : DoubleArray(&(v[0]), v.size());
}
