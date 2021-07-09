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

    // Set the pyramid center at (200, 180), and width x height to 150 x 300 pixels
    c->setPyramidSize(200, 180, 150, 300);

    // Set the elevation to 15 degrees
    c->setViewAngle(15);

    // Set the pyramid data and labels
    c->setData(DoubleArray(data, data_size), StringArray(labels, labels_size));

    // Set the layer colors to the given colors
    c->setColors(Chart::DataColor, IntArray(colors, colors_size));

    // Leave 1% gaps between layers
    c->setLayerGap(0.01);

    // Add labels at the center of the pyramid layers using Arial Bold font. The labels will show
    // the percentage of the layers.
    c->setCenterLabel("{percent}%", "Arial Bold");

    // Add labels at the right side of the pyramid layers using Arial Bold font. The labels will
    // have two lines showing the layer name and value.
    c->setRightLabel("{label}\nUS$ {value}M", "Arial Bold");

    // Output the chart
    c->makeChart("threedpyramid.png");

    //free up resources
    delete c;

    return 0;
}

