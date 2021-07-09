/*
** Name:        wxdemo.cpp
** Purpose:     wxWidgets Demo sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
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

#include <wx/imaglist.h>

#include "wxdemo.h"
#include "wxchartviewer.h"
#include "democharts.h"
#include "hotspotdialog.h" 

#include "mondrian.xpm"

class DemoTreeItem : public wxTreeItemData
{
public:
  DemoTreeItem(int idDemo) : m_idDemo(idDemo) {};
  int GetDemoId() const { return m_idDemo; }
private:
  int      m_idDemo;
};

/*
 * WxDemo type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WxDemo, wxFrame )

/*
 * WxDemo event table definition
 */

BEGIN_EVENT_TABLE( WxDemo, wxFrame )

  EVT_MENU(wxID_EXIT, WxDemo::OnExitClick)
  EVT_CLOSE(WxDemo::OnCloseWindow)
  EVT_TREE_SEL_CHANGED(ID_TREECTRL, WxDemo::OnTreeCtrlSelected)
  EVT_TREE_ITEM_ACTIVATED(ID_TREECTRL, WxDemo::OnTreeCtrlSelected)
  EVT_BUTTON(ID_SHOWMODAL, WxDemo::OnShowModalDemo)

  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+1, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+2, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+3, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+4, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+5, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+6, WxDemo::OnChartClicked)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER+7, WxDemo::OnChartClicked)

END_EVENT_TABLE()

/*
 * WxDemo constructors
 */

WxDemo::WxDemo()
{
  Init();
}

WxDemo::WxDemo( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*
 * WxDemo creator
 */

bool
WxDemo::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  // Force a reasonable initial window size
  SetInitialSize(FromDIP(SYMBOL_WXDEMO_SIZE));

  Centre();
  return true;
}

/*
 * WxDemo destructor
 */

WxDemo::~WxDemo()
{
  for (int j = 0; j < noOfChartViewers; ++j)
  {
    if (m_chartViewer[j]->getChart() != NULL)
    {
      delete m_chartViewer[j]->getChart();
    }
  }
}

/*
 * Member initialisation
 */

void
WxDemo::Init()
{
}

/*
 * Control creation for WxDemo
 */

void
WxDemo::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  WxDemo* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer1, 1, wxGROW | wxALL, 0);

  m_demoTree = new wxTreeCtrl(itemDialog1, ID_TREECTRL, wxDefaultPosition, FromDIP(wxSize(270, -1)), wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);

  wxImageList* il = new wxImageList(FromDIP(16), FromDIP(16), true);
  for (DemoChart* d = demoCharts; 0 != d->name; ++d)
  {
    if (d->noOfCharts == 0)
    {
      wxImage groupImage;
      groupImage.LoadFile(wxString::FromUTF8(d->icon), wxBITMAP_TYPE_PNG);
      groupImage.Rescale(FromDIP(16), FromDIP(16));
      il->Add(wxBitmap(groupImage));
    }
  }
  m_demoTree->AssignImageList(il);

  wxTreeItemId root = m_demoTree->AddRoot("ChartDirector Demo");

  // Loop through all the demo charts to populate the tree view
  int item = 0;
  int demoGroup = -1;
  wxTreeItemId group;
  for (DemoChart* d = demoCharts; 0 != d->name; ++d)
  {
    if (d->noOfCharts == 0)
    {
      ++demoGroup;
      group = m_demoTree->AppendItem(root, wxString::FromUTF8(d->name), demoGroup);
    }
    else
    {
      DemoTreeItem* demoItem = new DemoTreeItem(item);
      wxTreeItemId demo = m_demoTree->AppendItem(group, wxString::FromUTF8(d->name), demoGroup, -1, demoItem);
    }
    ++item;
  }

  itemBoxSizer1->Add(m_demoTree, 0, wxGROW | wxALL, 0);

  m_scrolledWindow = new wxScrolledWindow(itemDialog1, ID_SCROLLEDWINDOW, wxDefaultPosition, FromDIP(wxSize(600, 450)), wxNO_BORDER | wxHSCROLL | wxVSCROLL);

  itemBoxSizer1->Add(m_scrolledWindow, 1, wxGROW | wxALL, FromDIP(3));
  m_scrolledWindow->SetScrollbars(1, 1, 0, 0);
  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
  m_scrolledWindow->SetSizer(itemBoxSizer3);

  m_gridSizer = new wxGridSizer(0, 3, 0, 0);
  itemBoxSizer3->Add(m_gridSizer, 0, wxALIGN_LEFT | wxALL, 0);

  for (int j = 0; j < noOfChartViewers; ++j)
  {
    m_chartViewer[j] = new wxChartViewer(m_scrolledWindow, ID_CHARTVIEWER + j);
    m_gridSizer->Add(m_chartViewer[j], 0, wxGROW | wxALIGN_TOP | wxALL, FromDIP(3));
  }

  m_scrolledWindow->FitInside();

  itemDialog1->SetSizerAndFit(itemBoxSizer2);
}

void
WxDemo::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

/*
* wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
*/

void
WxDemo::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}

void
WxDemo::OnShowModalDemo(wxCommandEvent& event)
{
  wxDialog* demo = (wxDialog*) event.GetClientData();
  if (demo)
  {
    for (int i = 0; i < noOfChartViewers; ++i)
    {
      m_chartViewer[i]->Show(false);
    }
    demo->ShowModal();
    demo->Destroy();
  }
}

/*
* wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_TREECTRL
*/

void
WxDemo::OnTreeCtrlSelected(wxTreeEvent& event)
{
  wxEventType eventType = event.GetEventType();
  wxTreeItemId id = event.GetItem();
  DemoTreeItem* demoItem = (DemoTreeItem*) m_demoTree->GetItemData(id);

  if (demoItem)
  {
    Freeze();
    // Get the item selected
    int item = demoItem->GetDemoId();

    // The hidden column should contain the index to the demo module. The index is 0 if the item
    // is just a category heading.
    int nCharts = 0;
    m_gridSizer->Clear();
    int demoIndex = item;

    if (demoIndex > 0)
    {
      // Get the demo module
      DemoChart* d = demoCharts + demoIndex;

      // Hide and clear the old charts
      for (int i = 0; i < noOfChartViewers; ++i)
      {
        m_chartViewer[i]->Hide();
        delete m_chartViewer[i]->getChart();
        m_chartViewer[i]->setChart(0);
      }

      // Each demo module can display a number of charts. We display all of them.
      nCharts = d->noOfCharts;
      m_gridSizer->SetCols(nCharts < 3 ? nCharts > 0 ? nCharts : 1 : 3);

      for (int i = 0; i < d->noOfCharts; ++i)
      {
        d->createChart(this, m_chartViewer[i], i);
        m_gridSizer->Add(m_chartViewer[i], 0, wxALIGN_TOP | wxALL, FromDIP(3));
      }

      // We hide unused chart viewers.
      for (int i = 0; i < noOfChartViewers; ++i)
      {
        m_chartViewer[i]->Show(i < d->noOfCharts);
      }
    }

    // Layout the charts
    m_gridSizer->FitInside(m_scrolledWindow);
    m_scrolledWindow->GetSizer()->Layout();
    SendSizeEvent();
    Thaw();
    event.Skip();
  }
  else if (eventType == wxEVT_TREE_ITEM_ACTIVATED)
  {
    if (!m_demoTree->IsExpanded(id))
    {
      m_demoTree->Expand(id);
    }
    else
    {
      m_demoTree->Collapse(id);
    }
  }
}

/*
 * Should we show tooltips?
 */

bool
WxDemo::ShowToolTips()
{
  return true;
}

// User clicks on the wxChartViewer
void
WxDemo::OnChartClicked(wxCommandEvent& event)
{
  wxChartViewer* viewer = (wxChartViewer*) event.GetEventObject();
  ImageMapHandler* handler = viewer->getImageMapHandler();
  if (0 != handler)
  {
    // Query the ImageMapHandler to see if the mouse is on a clickable hot spot. We
    // consider the hot spot as clickable if its href ("path") parameter is not empty.
    const char* path = handler->getValue("path");
    if ((0 != path) && (0 != *path))
    {
      // In this sample code, we just show all hot spot parameters.
      HotspotDialog hs(this);
      hs.SetData(handler);
      hs.ShowModal();
    }
  }
}
