/*
** Name:        democharts.h
** Purpose:     wxWidgets Demo sample application
** Copyright:   (C) 2016, Advanced Software Engineering Limited (QT based implementation)
**              (C) 2018-2021, Ulrich Telle (Adjustments for wxWidgets)
** License:     ChartDirector license
*/

#ifndef WXCHARTS_HDR
#define WXCHARTS_HDR

//////////////////////////////////////////////////////////////////////////////////
//
//	This demo program contains a number of demo chart functions. Each function 
//	can produce a number of demo charts. We store all the functions and their
//	descriptions into an array to facilitate processing
//
//////////////////////////////////////////////////////////////////////////////////

//  Forward declaration - represents a ChartDirector chart 
class wxChartViewer;

//	Structure representing a single demo chart function
struct DemoChart
{
	const char* name;	//name of the demo function
	int noOfCharts;		//the number of charts generated by this demo function
  void (*createChart)(wxWindow* parent, wxChartViewer* viewer, int chartIndex);	//the demo function entry point
  const char *icon;   //icon for the demo
};

//	An array storing all demo chart functions. An empty entry signals the end 
//	of the array. 
extern DemoChart demoCharts[];

#endif