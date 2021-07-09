#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the bar chart
    double data[] = {85, 156, 179.5, 211, 123};
    const int data_size = (int)(sizeof(data)/sizeof(*data));

    // The labels for the bar chart
    const char* labels[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    const int labels_size = (int)(sizeof(labels)/sizeof(*labels));

    // Create a XYChart object of size 250 x 250 pixels
    XYChart* c = new XYChart(250, 250);

    // Set the plotarea at (30, 20) and of size 200 x 200 pixels
    c->setPlotArea(30, 20, 200, 200);

    // Add a bar chart layer using the given data
    c->addBarLayer(DoubleArray(data, data_size));

    // Set the labels on the x axis.
    c->xAxis()->setLabels(StringArray(labels, labels_size));

    // Output the chart
    c->makeChart("simplebar.png");

    //free up resources
    delete c;

    return 0;
}

