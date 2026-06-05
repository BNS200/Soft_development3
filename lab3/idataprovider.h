#ifndef IDATAPROVIDER_H
#define IDATAPROVIDER_H

#include <QList>
#include <QDateTime>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

struct DataPoint {
    QDateTime date;
    double value;
    QString label;

    DataPoint() : date(QDateTime()), value(0.0), label("") {}

    DataPoint(QDateTime d, double v, QString l) : date(d), value(v), label(l) {}
};

using DataSet = QList<DataPoint>;

class IDataProvider {
public:
    virtual ~IDataProvider() = default;
    virtual DataSet getData() const = 0;
    virtual QString getSourceName() const = 0;
    virtual QString getDescription() const = 0;
};

#endif // IDATAPROVIDER_H
