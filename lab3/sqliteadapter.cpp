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
        return result;
    }

    QSqlQuery columnsCheck(db);
    columnsCheck.exec(QString("PRAGMA table_info(%1)").arg(m_tableName));
    bool hasDateColumn = false;
    bool hasValueColumn = false;
    while (columnsCheck.next()) {
        QString colName = columnsCheck.value(1).toString();
        if (colName == m_dateColumn)
            hasDateColumn = true;
        if (colName == m_valueColumn)
            hasValueColumn = true;
    }

    if (!hasDateColumn) {
        db.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        throw std::runtime_error(QString("Колонка '%1' не найдена в таблице '%2'").arg(m_dateColumn, m_tableName).toStdString());
    }

    if (!hasValueColumn) {
        db.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        throw std::runtime_error(QString("Колонка '%1' не найдена в таблице '%2'").arg(m_valueColumn, m_tableName).toStdString());
    }

    QString queryValue = QString("SELECT %1, %2 FROM %3 ORDER BY %1 LIMIT 10").arg(m_dateColumn, m_valueColumn, m_tableName);

    QSqlQuery query(db);

    if (!query.exec(queryValue)) {
        db.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        return result;
    }

    int rowCount = 0;
    while (query.next()) {
        DataPoint point;

        if (query.value(0).isNull()) {
            db.close();
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
            throw std::runtime_error(QString("NULL значение в колонке '%1' в строке %2").arg(m_dateColumn).arg(rowCount + 1).toStdString());
        }

        QString dateStr = query.value(0).toString();
        point.date = QDateTime::fromString(dateStr, m_dateFormat);

        if (!point.date.isValid()) {
            db.close();
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
            throw std::runtime_error(QString("Неверный формат даты в строке %1:").arg(rowCount + 1).arg(dateStr).toStdString());
        }

        if (query.value(1).isNull()) {
            db.close();
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
            throw std::runtime_error(QString("NULL значение в колонке '%1' в строке %2").arg(m_valueColumn).arg(rowCount + 1).toStdString());
        }

        bool ok;
        double val = query.value(1).toDouble(&ok);
        if (!ok) {
            db.close();
            QSqlDatabase::removeDatabase("qt_sql_default_connection");
            throw std::runtime_error(QString("Значение в колонке '%1' в строке %2 не является числом: '%3'").arg(m_valueColumn).arg(rowCount + 1).toStdString());
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

QStringList SQLiteAdapter::getTableList(const QString& filePath)
{
    QStringList tables;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath);

    if (!db.open()) {
        return tables;
    }

    QSqlQuery query("SELECT name FROM sqlite_master WHERE type='table'");

    while (query.next()) {
        tables << query.value(0).toString();
    }

    db.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    return tables;
}
