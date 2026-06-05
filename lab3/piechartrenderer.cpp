#include "piechartrenderer.h"

QChart* PieChartRenderer::createChart(const DataSet& data)
{
    if (data.isEmpty()) {
        return nullptr;
    }

    QChart* chart = new QChart();
    chart->setTitle("Pie Chart");

    QPieSeries* series = new QPieSeries();

    for (const DataPoint& point : data) {
        series->append(point.label, point.value);
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
