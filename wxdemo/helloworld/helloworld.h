/*
** Name:        helloworld.h
** Purpose:     Hello World sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2021, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef HELLOWORLD_H_
#define HELLOWORLD_H_

/*!
 * Includes
 */

#include "chartdir.h"

/*!
 * Control identifiers
 */

#define SYMBOL_HELLOWORLD_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_HELLOWORLD_TITLE _("Hello World")
#define SYMBOL_HELLOWORLD_IDNAME wxID_ANY
#define SYMBOL_HELLOWORLD_SIZE wxDefaultSize
#define SYMBOL_HELLOWORLD_POSITION wxDefaultPosition

/*!
 * HelloWorld class declaration
 */

class HelloWorld: public wxFrame
{    
  DECLARE_DYNAMIC_CLASS( HelloWorld )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  HelloWorld();
  HelloWorld( wxWindow* parent, wxWindowID id = SYMBOL_HELLOWORLD_IDNAME, const wxString& caption = SYMBOL_HELLOWORLD_TITLE, const wxPoint& pos = SYMBOL_HELLOWORLD_POSITION, const wxSize& size = SYMBOL_HELLOWORLD_SIZE, long style = SYMBOL_HELLOWORLD_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_HELLOWORLD_IDNAME, const wxString& caption = SYMBOL_HELLOWORLD_TITLE, const wxPoint& pos = SYMBOL_HELLOWORLD_POSITION, const wxSize& size = SYMBOL_HELLOWORLD_SIZE, long style = SYMBOL_HELLOWORLD_STYLE );

  /// Destructor
  ~HelloWorld();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Event handler declarations

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
  void OnCloseWindow(wxCloseEvent& event);

  // Member variables

  XYChart* m_chart;
};

#endif
    // HELLOWORLD_H_
