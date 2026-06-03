#ifndef PIECHARTRENDERER_H
#define PIECHARTRENDERER_H

#include "ichartrenderer.h"
#include <QChart>
#include <QPieSeries>


class PieChartRenderer : public IChartRenderer {
public:
    PieChartRenderer() = default;

    QChart* createChart(const DataSet& data) override;
    QString getChartType() const override;
    void customizeChart(QChart* chart) override;
};

#endif // PIECHARTRENDERER_H
