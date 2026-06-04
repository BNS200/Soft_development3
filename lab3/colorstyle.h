#ifndef COLORSTYLE_H
#define COLORSTYLE_H
#include "iprintstyle.h"


class ColorStyle : public IPrintStyle {
public:
    ColorStyle() = default;

    void configurePrinter(QPrinter& printer) override;
    void configurePainter(QPainter& painter) override;
    void customizeChart(QChart* chart) override;
    QString getStyleName() const override;
};

#endif // COLORSTYLE_H
x
