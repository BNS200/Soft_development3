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

    if (array.isEmpty()) {
        file.close();
        throw std::runtime_error(QString("Файл не содержит данных: %1").arg(m_filePath).toStdString());
    }

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array[i].toObject();
        DataPoint point;

        if (!obj.contains("date")) {
            file.close();
            throw std::runtime_error(QString("Отсутствует поле 'date' в строке %1").arg(i + 1).toStdString());
        }

        QString dateStr = obj["date"].toString();
        point.date = QDateTime::fromString(dateStr, m_dateFormat);


        if (!point.date.isValid()) {
            file.close();
            throw std::runtime_error(QString("Неверный формат даты в строке %1: '%2'\n").arg(i + 1).arg(dateStr).toStdString());
        }

        if (!obj.contains("value")) {
            file.close();
            throw std::runtime_error(QString("Отсутствует поле 'value' в строке %1").arg(i + 1).toStdString());
        }

        if (obj["value"].isNull()) {
            file.close();
            throw std::runtime_error(QString("Поле 'value' в строке %1 равно null").arg(i + 1).toStdString());
        }

        if (!obj["value"].isDouble()) {
            file.close();
            throw std::runtime_error(QString("Поле 'value' в строке %1 не является числом").arg(i + 1).toStdString());
        }

        point.value = obj["value"].toDouble();

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
