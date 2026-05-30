#include "sqliteadapter.h"

SQLiteDataAdapter::SQLiteDataAdapter(const QString& dbPath, const QString& tableName, const QString& dateColumn, const QString& valueColumn, const QString& dateFormat)
    : m_dbPath(dbPath), m_tableName(tableName), m_dateColumn(dateColumn), m_valueColumn(valueColumn), m_dateFormat(dateFormat)
{}

QString SQLiteDataAdapter::getSourceName() const
{
    return "SQLite Database";
}

QString SQLiteDataAdapter::getDescription() const
{
    return QString("SQLite: %1\nTable: %2\nDate column: %3\nValue column: %4").arg(m_dbPath, m_tableName, m_dateColumn, m_valueColumn);
}
