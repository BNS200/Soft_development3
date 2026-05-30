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

