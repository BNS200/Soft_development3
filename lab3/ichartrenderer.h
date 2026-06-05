#ifndef ICHARTRENDERER_H
#define ICHARTRENDERER_H

#include <QChartView>
#include <memory>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QDebug>
#include <QPieSeries>
#include "idataprovider.h"


class IChartRenderer {

public:
    virtual ~IChartRenderer() = default;
    virtual QChart* createChart(const DataSet& data) = 0;
    virtual QString getChartType() const = 0;
    virtual void customizeChart(QChart* chart) = 0;
};

#endif // ICHARTRENDERER_H
