#ifndef BARCHARTRENDERER_H
#define BARCHARTRENDERER_H

#include "ichartrenderer.h"



class BarChartRenderer : public IChartRenderer {

public:
    BarChartRenderer() = default;

    QChart* createChart(const DataSet& data) override;
    QString getChartType() const override;
    void customizeChart(QChart* chart) override;
};

#endif // BARCHARTRENDERER_H
