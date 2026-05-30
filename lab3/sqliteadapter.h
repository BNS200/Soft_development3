#ifndef SQLITEADAPTER_H
#define SQLITEADAPTER_H
#include "idataprovider.h"


class SQLiteDataAdapter : public IDataProvider {
private:
    QString m_dbPath;
    QString m_tableName;
    QString m_dateColumn;
    QString m_valueColumn;
    QString m_dateFormat;

public:
    SQLiteDataAdapter(const QString& dbPath, const QString& tableName, const QString& dateColumn, const QString& valueColumn, const QString& dateFormat);

    DataSet getData() const override;
    QString getSourceName() const override;
    QString getDescription() const override;
};

#endif // SQLITEADAPTER_H
