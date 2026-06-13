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

    QStringList categories;

    for (int i = 0; i < data.size(); ++i) {
        const DataPoint& point = data[i];

        QBarSet* barSet = new QBarSet(point.label);
        *barSet << point.value;

        barSet->setColor(colors[i % colors.size()]);

        series->append(barSet);
    }

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
