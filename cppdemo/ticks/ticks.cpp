#include "chartdir.h"

void createChart(int chartIndex, const char *filename)
{
    // The data for the chart
    double data[] = {100, 125, 265, 147, 67, 105};
    const int data_size = (int)(sizeof(data)/sizeof(*data));
    const char* labels[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun"};
    const int labels_size = (int)(sizeof(labels)/sizeof(*labels));

    // Create a XYChart object of size 250 x 250 pixels
    XYChart* c = new XYChart(250, 250);

    // Set the plot area at (27, 25) and of size 200 x 200 pixels
    c->setPlotArea(27, 25, 200, 200);

    if (chartIndex == 1) {
        // High tick density, uses 10 pixels as tick spacing
        c->addTitle("Tick Density = 10 pixels");
        c->yAxis()->setTickDensity(10);
    } else {
        // Normal tick density, just use the default setting
        c->addTitle("Default Tick Density");
    }

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, labels_size));

    // Add a color bar layer using the given data. Use a 1 pixel 3D border for the bars.
    c->addBarLayer(DoubleArray(data, data_size), IntArray(0, 0))->setBorderColor(-1, 1);

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;

}

int main(int argc, char *argv[])
{
    createChart(0, "ticks0.png");
    createChart(1, "ticks1.png");
    return 0;
}

