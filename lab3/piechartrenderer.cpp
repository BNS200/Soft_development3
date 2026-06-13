#include "piechartrenderer.h"
#include <QDebug>

QChart* PieChartRenderer::createChart(const DataSet& data)
{
    if (data.isEmpty()) {
        return nullptr;
    }

    QChart* chart = new QChart();
    chart->setTitle("Pie Chart");

    QPieSeries* series = new QPieSeries();

    QList<QColor> colors = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::cyan,
        Qt::magenta,
        Qt::yellow,
        Qt::darkRed,
        Qt::darkGreen,
        Qt::darkBlue,
        Qt::darkCyan,
        Qt::darkMagenta,
        Qt::darkYellow,
        Qt::gray,
        Qt::lightGray,
        Qt::darkGray
    };

    int i = 0;
    for (const DataPoint& point : data) {
        QPieSlice* slice = series->append(point.label, point.value);
        slice->setColor(colors[i % colors.size()]);
        i++;
    }

    chart->addSeries(series);
    customizeChart(chart);
    return chart;
}

QString PieChartRenderer::getChartType() const
{
    return "Pie Chart";
}

void PieChartRenderer::customizeChart(QChart* chart)
{
    chart->setTheme(QChart::ChartThemeLight);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
}
