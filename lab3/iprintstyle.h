#ifndef IPRINTSTYLE_H
#define IPRINTSTYLE_H
#include <QPainter>
#include <QPrinter>
#include <QChart>
#include <QChartView>



class IPrintStyle {
public:
    virtual ~IPrintStyle() = default;
    virtual void configurePrinter(QPrinter& printer) = 0;
    virtual void configurePainter(QPainter& painter) = 0;
    virtual void customizeChart(QChart* chart) = 0;
    virtual QString getStyleName() const = 0;
};


#endif // IPRINTSTYLE_H
