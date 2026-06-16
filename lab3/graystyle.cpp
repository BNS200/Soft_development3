#include "graystyle.h"
#include <QBrush>
#include <QFont>
#include <QBarSeries>
#include <QBarSet>
#include <QPieSeries>

void GrayStyle::configurePrinter(QPrinter& printer)
{
    printer.setColorMode(QPrinter::GrayScale);
    printer.setResolution(300);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);
}

void GrayStyle::configurePainter(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
}

void GrayStyle::customizeChart(QChart* chart)
{
    if (!chart) return;

    // chart->setTheme(QChart::ChartThemeDark);

    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setBackgroundVisible(true);

    chart->legend()->setVisible(true);
    chart->legend()->setBrush(QBrush(Qt::black));
    chart->legend()->setAlignment(Qt::AlignBottom);

    QFont titleFont = chart->titleFont();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    chart->setTitleFont(titleFont);


    QList<QAbstractSeries*> seriesList = chart->series();
    for (QAbstractSeries* series : seriesList) {
        if (QBarSeries* barSeries = qobject_cast<QBarSeries*>(series)) {
            for (QBarSet* barSet : barSeries->barSets()) {
                barSet->setColor(Qt::darkGray);
                barSet->setBorderColor(Qt::black);
                barSet->setLabelColor(Qt::black);
            }
        } else if (QPieSeries* pieSeries = qobject_cast<QPieSeries*>(series)) {
            for (QPieSlice* slice : pieSeries->slices()) {
                slice->setBrush(QBrush(Qt::darkGray));
                slice->setLabelColor(Qt::black);
                slice->setBorderColor(Qt::black);
            }
        }
    }

    if (!chart->axes().isEmpty()) {
        for (QAbstractAxis* axis : chart->axes()) {
            axis->setLabelsVisible(true);
            axis->setLabelsColor(Qt::black);
        }
    }
}

QString GrayStyle::getStyleName() const
{
    return "Gray";
}
