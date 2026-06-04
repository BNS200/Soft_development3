#ifndef GRAYSTYLE_H
#define GRAYSTYLE_H
#include "iprintstyle.h"


class GrayStyle : public IPrintStyle {
public:
    GrayStyle() = default;

    void configurePrinter(QPrinter& printer) override;
    void configurePainter(QPainter& painter) override;
    void customizeChart(QChart* chart) override;
    QString getStyleName() const override;
};

#endif // GRAYSTYLE_H
