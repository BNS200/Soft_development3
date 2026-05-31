#ifndef JSONADAPTER_H
#define JSONADAPTER_H

#include "idataprovider.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


class JSONDataAdapter : public IDataProvider {
private:
    QString m_filePath;
    QString m_dateFormat;

public:
    JSONDataAdapter(const QString& filePath, const QString& dateFormat);

    DataSet getData() const override;
    QString getSourceName() const override;
    QString getDescription() const override;
};

#endif // JSONADAPTER_H
