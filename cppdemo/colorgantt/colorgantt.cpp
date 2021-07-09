#include "chartdir.h"

int main(int argc, char *argv[])
{
    // The tasks for the gantt chart
    const char* labels[] = {"Market Research", "Define Specifications", "Overall Archiecture",
        "Project Planning", "Detail Design", "Software Development", "Test Plan", "Testing and QA",
        "User Documentation"};
    const int labels_size = (int)(sizeof(labels)/sizeof(*labels));

    // The task index, start date, end date and color for each bar
    double taskNo[] = {0, 0, 1, 2, 3, 4, 5, 6, 6, 7, 8, 8};
    const int taskNo_size = (int)(sizeof(taskNo)/sizeof(*taskNo));
    double startDate[] = {Chart::chartTime(2018, 8, 16), Chart::chartTime(2018, 10, 4),
        Chart::chartTime(2018, 8, 30), Chart::chartTime(2018, 9, 13), Chart::chartTime(2018, 9, 20),
        Chart::chartTime(2018, 9, 27), Chart::chartTime(2018, 10, 4), Chart::chartTime(2018, 10, 4),
        Chart::chartTime(2018, 10, 25), Chart::chartTime(2018, 11, 1), Chart::chartTime(2018, 10, 18
        ), Chart::chartTime(2018, 11, 8)};
    const int startDate_size = (int)(sizeof(startDate)/sizeof(*startDate));
    double endDate[] = {Chart::chartTime(2018, 8, 30), Chart::chartTime(2018, 10, 18),
        Chart::chartTime(2018, 9, 13), Chart::chartTime(2018, 9, 27), Chart::chartTime(2018, 10, 4),
        Chart::chartTime(2018, 10, 11), Chart::chartTime(2018, 11, 8), Chart::chartTime(2018, 10, 18
        ), Chart::chartTime(2018, 11, 8), Chart::chartTime(2018, 11, 22), Chart::chartTime(2018, 11,
        1), Chart::chartTime(2018, 11, 22)};
    const int endDate_size = (int)(sizeof(endDate)/sizeof(*endDate));
    int colors[] = {0x00cc00, 0x00cc00, 0x00cc00, 0x0000cc, 0x0000cc, 0xcc0000, 0xcc0000, 0x0000cc,
        0xcc0000, 0xcc0000, 0x00cc00, 0xcc0000};
    const int colors_size = (int)(sizeof(colors)/sizeof(*colors));

    // Create a XYChart object of size 620 x 325 pixels. Set background color to light red
    // (0xffcccc), with 1 pixel 3D border effect.
    XYChart* c = new XYChart(620, 325, 0xffcccc, 0x000000, 1);

    // Add a title to the chart using 15 points Times Bold Itatic font, with white (ffffff) text on
    // a dark red (800000) background
    c->addTitle("Mutli-Color Gantt Chart Demo", "Times New Roman Bold Italic", 15, 0xffffff
        )->setBackground(0x800000);

    // Set the plotarea at (140, 55) and of size 460 x 200 pixels. Use alternative white/grey
    // background. Enable both horizontal and vertical grids by setting their colors to grey
    // (c0c0c0). Set vertical major grid (represents month boundaries) 2 pixels in width
    c->setPlotArea(140, 55, 460, 200, 0xffffff, 0xeeeeee, Chart::LineColor, 0xc0c0c0, 0xc0c0c0
        )->setGridWidth(2, 1, 1, 1);

    // swap the x and y axes to create a horziontal box-whisker chart
    c->swapXY();

    // Set the y-axis scale to be date scale from Aug 16, 2018 to Nov 22, 2018, with ticks every 7
    // days (1 week)
    c->yAxis()->setDateScale(Chart::chartTime(2018, 8, 16), Chart::chartTime(2018, 11, 22), 86400 *
        7);

    // Set multi-style axis label formatting. Month labels are in Arial Bold font in "mmm d" format.
    // Weekly labels just show the day of month and use minor tick (by using '-' as first character
    // of format string).
    c->yAxis()->setMultiFormat(Chart::StartOfMonthFilter(), "<*font=Arial Bold*>{value|mmm d}",
        Chart::StartOfDayFilter(), "-{value|d}");

    // Set the y-axis to shown on the top (right + swapXY = top)
    c->setYAxisOnRight();

    // Set the labels on the x axis
    c->xAxis()->setLabels(StringArray(labels, labels_size));

    // Reverse the x-axis scale so that it points downwards.
    c->xAxis()->setReverse();

    // Set the horizontal ticks and grid lines to be between the bars
    c->xAxis()->setTickOffset(0.5);

    // Add some symbols to the chart to represent milestones. The symbols are added using scatter
    // layers. We need to specify the task index, date, name, symbol shape, size and color.
    double mileStone1Task[] = {1};
    double mileStone1Date[] = {Chart::chartTime(2018, 9, 13)};
    c->addScatterLayer(DoubleArray(mileStone1Task, 1), DoubleArray(mileStone1Date, 1),
        "Milestone 1", Chart::Cross2Shape(), 13, 0xffff00);
    double mileStone2Task[] = {3};
    double mileStone2Date[] = {Chart::chartTime(2018, 10, 4)};
    c->addScatterLayer(DoubleArray(mileStone2Task, 1), DoubleArray(mileStone2Date, 1),
        "Milestone 2", Chart::StarShape(5), 15, 0xff00ff);
    double mileStone3Task[] = {5};
    double mileStone3Date[] = {Chart::chartTime(2018, 11, 8)};
    c->addScatterLayer(DoubleArray(mileStone3Task, 1), DoubleArray(mileStone3Date, 1),
        "Milestone 3", Chart::TriangleSymbol, 13, 0xff9933);

    // Add a multi-color box-whisker layer to represent the gantt bars
    BoxWhiskerLayer* layer = c->addBoxWhiskerLayer2(DoubleArray(startDate, startDate_size),
        DoubleArray(endDate, endDate_size), DoubleArray(), DoubleArray(), DoubleArray(), IntArray(
        colors, colors_size));
    layer->setXData(DoubleArray(taskNo, taskNo_size));
    layer->setBorderColor(Chart::SameAsMainColor);

    // Divide the plot area height ( = 200 in this chart) by the number of tasks to get the height
    // of each slot. Use 80% of that as the bar height.
    layer->setDataWidth((int)(200 * 0.8 / labels_size));

    // Add a legend box at (140, 265) - bottom of the plot area. Use 8pt Arial Bold as the font with
    // auto-grid layout. Set the width to the same width as the plot area. Set the backgorund to
    // grey (dddddd).
    LegendBox* legendBox = c->addLegend2(140, 265, Chart::AutoGrid, "Arial Bold", 8);
    legendBox->setWidth(461);
    legendBox->setBackground(0xdddddd);

    // The keys for the scatter layers (milestone symbols) will automatically be added to the legend
    // box. We just need to add keys to show the meanings of the bar colors.
    legendBox->addKey("Market Team", 0x00cc00);
    legendBox->addKey("Planning Team", 0x0000cc);
    legendBox->addKey("Development Team", 0xcc0000);

    // Output the chart
    c->makeChart("colorgantt.png");

    //free up resources
    delete c;

    return 0;
}

