#include "chartdir.h"

// Use a bubble chart as the surface texture
void addBubbleTexture(SurfaceChart* sc)
{
    // Set the original surface color to be from grey (0xbbbbbb) to light grey (0xeeeeee) based on
    // the z-value. This will become the background color of the surface.
    int colorGradient[] = {0xbbbbbb, 0xeeeeee};
    sc->colorAxis()->setColorGradient(true, IntArray(colorGradient, 2));

    // The data values for the red bubbles
    double dataX0[] = {-5, -2, 1, 7};
    const int dataX0_size = (int)(sizeof(dataX0)/sizeof(*dataX0));
    double dataY0[] = {2, 4, -2, -6};
    const int dataY0_size = (int)(sizeof(dataY0)/sizeof(*dataY0));
    double dataZ0[] = {20, 60, 50, 85};
    const int dataZ0_size = (int)(sizeof(dataZ0)/sizeof(*dataZ0));

    // The data values for the blue bubbles
    double dataX1[] = {5, -5, -3};
    const int dataX1_size = (int)(sizeof(dataX1)/sizeof(*dataX1));
    double dataY1[] = {3, -4, 7};
    const int dataY1_size = (int)(sizeof(dataY1)/sizeof(*dataY1));
    double dataZ1[] = {100, 85, 95};
    const int dataZ1_size = (int)(sizeof(dataZ1)/sizeof(*dataZ1));

    // Create a bubble chart of the same size as the surface XY size
    XYChart* c = new XYChart(sc->getPlotRegionWidth(), sc->getPlotRegionDepth(), Chart::Transparent)
        ;
    c->setPlotArea(0, 0, c->getWidth(), c->getHeight(), -1, -1, Chart::Transparent,
        Chart::Transparent);

    // Synchronize the bubble chart axis scale to the surface axis scale. As the surface axes are
    // visible, we can hide the bubble chart axes.
    c->yAxis()->syncAxis(sc->yAxis());
    c->xAxis()->syncAxis(sc->xAxis());
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);

    // Add bubbles with the semi-transparent red color (0x7fff3333). Also add a matching legend
    // entry to the surface chart.
    c->addScatterLayer(DoubleArray(dataX0, dataX0_size), DoubleArray(dataY0, dataY0_size), "",
        Chart::CircleSymbol, 9, 0x7fff3333, 0x7fff3333)->setSymbolScale(DoubleArray(dataZ0,
        dataZ0_size));
    sc->getLegend()->addKey("Hot Zone", 0x7fff3333);

    // Add bubbles with the semi-transparent blue color (0x7f3333ff). Also add a matchine legend
    // entry to the surface chart
    c->addScatterLayer(DoubleArray(dataX1, dataX1_size), DoubleArray(dataY1, dataY1_size), "",
        Chart::CircleSymbol, 9, 0x7f3333ff, 0x7f3333ff)->setSymbolScale(DoubleArray(dataZ1,
        dataZ1_size));
    sc->getLegend()->addKey("Wet Zone", 0x7f3333ff);

    // Before we generate the bubble chart as texture, we must layout the surface chart first. It is
    // because the bubble chart axis scale depends on the surface chart axis scale.
    sc->layout();

    // Output the bubble chart and save it as a resource
    sc->setResource("texture", c->makeChart());
    // Use the resource as the texture
    sc->setSurfaceTexture(sc->patternColor("@/texture"));

    //free up resources
    delete c;
}

// Use a polar chart as the surface texture
void addPolarTexture(SurfaceChart* sc)
{
    sc->setSurfaceAxisGrid(Chart::Transparent, Chart::Transparent);

    PolarChart* c = new PolarChart(sc->getPlotRegionWidth(), sc->getPlotRegionDepth(),
        Chart::Transparent);

    c->setPlotArea(c->getWidth() / 2, c->getHeight() / 2, c->getWidth() / 2);

    // Use alternative light grey/dark grey circular background color
    c->setPlotAreaBg(0xaf000000, Chart::Transparent);

    // Set the grid style to circular grid
    c->setGridStyle(false);

    // Set angular axis as 0 - 360, with a spoke every 30 units
    c->angularAxis()->setLinearScale(0, 360, 30);
    c->angularAxis()->setLabelStyle("normal", 8, Chart::Transparent);
    c->radialAxis()->setLinearScale(0, 10, 2);
    c->radialAxis()->setColors(Chart::Transparent, Chart::Transparent);

    // Output the polar chart and save it as a resource
    sc->setResource("texture", c->makeChart());
    // Use the resource as the texture
    sc->setSurfaceTexture(sc->patternColor("@/texture"));

    //free up resources
    delete c;
}

void createChart(int chartIndex, const char *filename)
{
    // The x and y coordinates of the grid
    double dataX[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const int dataX_size = (int)(sizeof(dataX)/sizeof(*dataX));
    double dataY[] = {-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const int dataY_size = (int)(sizeof(dataY)/sizeof(*dataY));

    // The values at the grid points. In this example, we will compute the values using the formula
    // z = x * y.
    const int dataZ_size = dataX_size * dataY_size;
    double dataZ[dataZ_size];
    for(int yIndex = 0; yIndex < dataY_size; ++yIndex) {
        for(int xIndex = 0; xIndex < dataX_size; ++xIndex) {
            dataZ[yIndex * dataX_size + xIndex] = dataX[xIndex] * dataY[yIndex];
        }
    }

    // Create a SurfaceChart object of size 480 x 450 pixels
    SurfaceChart* c = new SurfaceChart(480, 450);

    // Set the center of the plot region at (240, 210), and set width x depth x height to 240 x 240
    // x 200 pixels
    c->setPlotRegion(240, 210, 240, 240, 200);

    // Set the elevation and rotation angles to 30 and 15 degrees
    c->setViewAngle(30, 15);

    // Set the data to use to plot the chart
    c->setData(DoubleArray(dataX, dataX_size), DoubleArray(dataY, dataY_size), DoubleArray(dataZ,
        dataZ_size));

    // Spline interpolate data to a 80 x 80 grid for a smooth surface
    c->setInterpolation(80, 80);

    // Reserve 50 pixels at the bottom of the z-axis to allow for the XY projection
    c->zAxis()->setMargin(0, 50);

    // Add XY projection
    c->addXYProjection();

    // Use semi-transparent black (0xc0000000) for x and y major surface grid lines. Use dotted
    // style for x and y minor surface grid lines.
    int majorGridColor = 0xc0000000;
    int minorGridColor = c->dashLineColor(majorGridColor, Chart::DotLine);
    c->setSurfaceAxisGrid(majorGridColor, majorGridColor, minorGridColor, minorGridColor);

    // Get the surface texture
    BaseChart* textureChart = 0;
    if (chartIndex == 0) {
        // Surface texture is a bubble chart
        c->addTitle("<*underline=2*>Bubble Chart Texture", "Arial Bold", 16);

        // This texture uses the legend box, so we need to add a legend box to the chart at (490,
        // 200).
        LegendBox* b = c->addLegend(490, 200, true, "Arial Bold", 10);
        b->setBackground(Chart::Transparent, Chart::Transparent);
        b->setMaxWidth(c->getWidth() - 490 - 1);

        // Add the texture
        addBubbleTexture(c);
    } else if (chartIndex == 1) {
        // Surface Texture is a polar chart
        c->addTitle("<*underline=2*>Polar Chart Texture", "Arial Bold", 16);

        // This texture uses the color axis, so we add the color axis at (420, 75)
        ColorAxis* cAxis = c->setColorAxis(420, 75, Chart::TopLeft, 200, Chart::Right);

        // By default, the color axis synchronizes with the z-axis. We cancel the synchronization so
        // that the color axis will auto-scale independently.
        cAxis->syncAxis(0);

        // Add the texture
        addPolarTexture(c);
    } else {
        // Surface Texture is an image
        c->addTitle("<*underline=2*>Image Texture", "Arial Bold", 16);

        // Use a DrawArea to load the image
        DrawArea* d = new DrawArea();
        d->load("maptexture.png");
        d->resize(c->getPlotRegionWidth(), c->getPlotRegionDepth());

        // Set the DrawArea as a resource
        c->setResource("texture", d);
        // Use the resource as the texture
        c->setSurfaceTexture(c->patternColor("@/texture"));
        delete d;
    }

    // Set contour lines to semi-transparent white (80ffffff)
    c->setContourColor(0x80ffffff);

    // Set the x, y and z axis titles using 10 pt Arial Bold font
    c->xAxis()->setTitle("X Title<*br*>Placeholder", "Arial Bold", 10);
    c->yAxis()->setTitle("Y Title<*br*>Placeholder", "Arial Bold", 10);
    c->zAxis()->setTitle("Z Title Placeholder", "Arial Bold", 10);

    // Output the chart
    c->makeChart(filename);

    //free up resources
    delete c;

}

int main(int argc, char *argv[])
{
    createChart(0, "surfacetexture0.png");
    createChart(1, "surfacetexture1.png");
    createChart(2, "surfacetexture2.png");
    return 0;
}

