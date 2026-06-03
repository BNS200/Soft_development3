#ifndef ICHARTRENDERER_H
#define ICHARTRENDERER_H

#include <QChart>
#include <QChartView>
#include <memory>
#include "idataprovider.h"


class IChartRenderer {

public:
    virtual ~IChartRenderer() = default;
    virtual QChart* createChart(const DataSet& data) = 0;
    virtual QString getChartType() const = 0;
    virtual void customizeChart(QChart* chart) = 0;
};

#endif // ICHARTRENDERER_H
