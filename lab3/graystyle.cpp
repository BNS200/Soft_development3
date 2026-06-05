#include "graystyle.h"

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

    chart->setTheme(QChart::ChartThemeDark);

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

QString GrayStyle::getStyleName() const
{
    return "Gray";
}
