#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the bar chart
    double data[] = {85, 156, 179, 211, 123, 189, 166};
    const int data_size = (int)(sizeof(data)/sizeof(*data));

    // The labels for the bar chart
    const char* labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    const int labels_size = (int)(sizeof(labels)/sizeof(*labels));

    // The colors for the bars
    int colors[] = {0x5588bb, 0x66bbbb, 0xaa6644, 0x99bb55, 0xee9944, 0x444466, 0xbb5555};
    const int colors_size = (int)(sizeof(colors)/sizeof(*colors));

    // Create a XYChart object of size 600 x 360 pixels
    XYChart* c = new XYChart(600, 360);

    // Set the plotarea at (70, 20) and of size 500 x 300 pixels, with transparent background and
    // border and light grey (0xcccccc) horizontal grid lines
    c->setPlotArea(70, 20, 500, 300, Chart::Transparent, -1, Chart::Transparent, 0xcccccc);

    // Set the x and y axis stems to transparent and the label font to 12pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("Arial", 12);
    c->yAxis()->setLabelStyle("Arial", 12);

    // Add a multi-color bar chart layer using the given data
    BarLayer* layer = c->addBarLayer(DoubleArray(data, data_size), IntArray(colors, colors_size));

    // Use bar gradient lighting with the light intensity from 0.8 to 1.15
    layer->setBorderColor(Chart::Transparent, Chart::barLighting(0.8, 1.15));

    // Set rounded corners for bars
    layer->setRoundedCorners();

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, labels_size));

    // For the automatic y-axis labels, set the minimum spacing to 40 pixels.
    c->yAxis()->setTickDensity(40);

    // Add a title to the y axis using dark grey (0x555555) 14pt Arial font
    c->yAxis()->setTitle("Y-Axis Title Placeholder", "Arial", 14, 0x555555);

    // Output the chart
    c->makeChart("colorbar2.png");

    //free up resources
    delete c;

    return 0;
}

