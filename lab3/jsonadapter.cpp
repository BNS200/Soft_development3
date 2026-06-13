#include "jsonadapter.h"

JSONDataAdapter::JSONDataAdapter(const QString& filePath, const QString& dateFormat) : m_filePath(filePath), m_dateFormat(dateFormat) {}

QString JSONDataAdapter::getSourceName() const
{
    return "JSON File";
}

QString JSONDataAdapter::getDescription() const
{
    return QString("JSON: %1").arg(m_filePath);
}

DataSet JSONDataAdapter::getData() const
{
    DataSet result;

    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return result;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isNull()) {
        file.close();
        return result;
    }

    QJsonArray array = doc.array();

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        DataPoint point;

        if (obj.contains("date")) {
            point.date = QDateTime::fromString(obj["date"].toString(), m_dateFormat);
        }
        if (obj.contains("value")) {
            point.value = obj["value"].toDouble();
        }

        if (obj.contains("label")) {
            point.label = obj["label"].toString();
        } else {
            point.label = point.date.toString("dd.MM.yyyy");
        }

        result.append(point);
    }

    file.close();
    return result;
}
