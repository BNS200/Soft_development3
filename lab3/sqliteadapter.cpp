#include "sqliteadapter.h"

SQLiteAdapter::SQLiteAdapter(const QString& dbPath, const QString& tableName, const QString& dateColumn, const QString& valueColumn, const QString& dateFormat)
    : m_dbPath(dbPath), m_tableName(tableName), m_dateColumn(dateColumn), m_valueColumn(valueColumn), m_dateFormat(dateFormat)
{}

QString SQLiteAdapter::getSourceName() const
{
    return "SQLite Database";
}

QString SQLiteAdapter::getDescription() const
{
    return QString("SQLite: %1\nTable: %2\nDate column: %3\nValue column: %4").arg(m_dbPath, m_tableName, m_dateColumn, m_valueColumn);
}

DataSet SQLiteAdapter::getData() const
{
    DataSet result;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_dbPath);

    if (!db.open()) {
        qDebug() << "Cannot open database:" << m_dbPath;
        return result;
    }

    QString queryValue = QString("SELECT %1, %2 FROM %3 ORDER BY %1").arg(m_dateColumn, m_valueColumn, m_tableName);

    QSqlQuery query(db);

    if (!query.exec(queryValue)) {
        db.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        return result;
    }

    int rowCount = 0;
    while (query.next()) {
        DataPoint point;

        QString dateStr = query.value(0).toString();
        point.date = QDateTime::fromString(dateStr, m_dateFormat);

        if (!point.date.isValid()) {
            qDebug() << "Failed to parse date";
        }

        point.value = query.value(1).toDouble();

        point.label = point.date.toString("dd.MM.yyyy HH:mm");

        result.append(point);
        rowCount++;
    }
    db.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    return result;
}
