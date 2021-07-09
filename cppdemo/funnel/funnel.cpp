#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The data for the pyramid chart
    double data[] = {156, 123, 211, 179};
    const int data_size = (int)(sizeof(data)/sizeof(*data));

    // The labels for the pyramid chart
    const char* labels[] = {"Corporate Tax", "Working Capital", "Re-investment", "Dividend"};
    const int labels_size = (int)(sizeof(labels)/sizeof(*labels));

    // The colors for the pyramid layers
    int colors[] = {0x66aaee, 0xeebb22, 0xcccccc, 0xcc88ff};
    const int colors_size = (int)(sizeof(colors)/sizeof(*colors));

    // Create a PyramidChart object of size 500 x 400 pixels
    PyramidChart* c = new PyramidChart(500, 400);

    // Set the funnel center at (200, 210), and width x height to 150 x 300 pixels
    c->setFunnelSize(200, 210, 150, 300);

    // Set the elevation to 5 degrees
    c->setViewAngle(5);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, data_size), StringArray(labels, labels_size));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, colors_size));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Add labels at the right side of the pyramid layers using Arial Bold font. The labels will
    // have 3 lines showing the layer name, value and percentage.
    c->setRightLabel("{label}\nUS ${value}K\n({percent}%)", "Arial Bold");

    // Output the chart
    c->makeChart("funnel.png");

    //free up resources
    delete c;

    return 0;
}

