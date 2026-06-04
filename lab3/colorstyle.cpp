#include "colorstyle.h"
#include <QBrush>
#include <QFont>

void ColorStyle::configurePrinter(QPrinter& printer)
{
    printer.setColorMode(QPrinter::Color);
    printer.setResolution(300);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Landscape);
}

void ColorStyle::configurePainter(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
}

void ColorStyle::customizeChart(QChart* chart)
{
    if (!chart) return;

    chart->setTheme(QChart::ChartThemeLight);

    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setBackgroundVisible(true);

    chart->legend()->setVisible(true);
    chart->legend()->setBrush(QBrush(Qt::black));
    chart->legend()->setAlignment(Qt::AlignBottom);

    QFont titleFont = chart->titleFont();
    titleFont.setBold(true);
    titleFont.setPointSize(14);
    chart->setTitleFont(titleFont);
}

QString ColorStyle::getStyleName() const
{
    return "Color";
}
