#ifndef BARCHARTRENDERER_H
#define BARCHARTRENDERER_H

#include "ichartrenderer.h"
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>


class BarChartRenderer : public IChartRenderer {

public:
    BarChartRenderer() = default;

    QChart* createChart(const DataSet& data) override;
    QString getChartType() const override;
    void customizeChart(QChart* chart) override;
};

#endif // BARCHARTRENDERER_H
