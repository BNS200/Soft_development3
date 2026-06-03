#include "barchartrenderer.h"
#include <QDebug>

QChart* BarChartRenderer::createChart(const DataSet& data)
{
    if (data.isEmpty()) {
        return nullptr;
    }

    QChart* chart = new QChart();
    chart->setTitle(QString("Bar Chart "));

    QBarSeries* series = new QBarSeries();
    QBarSet* barSet = new QBarSet("Values");

    QStringList categories;
    for (const DataPoint& point : data) {
        *barSet << point.value;
        categories << point.label;
    }

    series->append(barSet);
    chart->addSeries(series);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Categories");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Values");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    customizeChart(chart);
    return chart;
}

QString BarChartRenderer::getChartType() const
{
    return "Bar Chart";
}

void BarChartRenderer::customizeChart(QChart* chart)
{
    chart->setTheme(QChart::ChartThemeLight);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}
