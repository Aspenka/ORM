#include "globaldata.h"

/*==============================================================================
 Метод возвращает текущую дату и время в формате yyyy-MM-dd hh:mm:ss
==============================================================================*/
QString errors::getCurrentDate ()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

/*==============================================================================
 Метод выводит на экран сообщение, в зависимости от кода ошибки
 code - код ошибки;
 value - часть сообщения об ошибке
==============================================================================*/
void errors::printError (const errors_e code, const QString & value)
{
    switch(code)
    {
        case errors::EXISTS_TABLE:
        {
            qDebug() << QObject::tr(" [!][TableSchema][%1][%2] Table name %3 does not exists!").
                        arg(QString::number(code), getCurrentDate(), value);
            break;
        }
        case errors::DB_ERROR:
        {

            qDebug() << QObject::tr(" [!][TableSchema][%1][%2] Database connecting error: %3").
                        arg(QString::number(code), getCurrentDate(), value);
            break;
        }
        case errors::EXISTS_RELATION:
        {
            qDebug() << QObject::tr(" [!][TableSchema][%1][%2] Relation %3 does not exists!").
                        arg(QString::number(code), getCurrentDate(), value);
            break;
        }
        case errors::QUERY_ERROR:
        {
            qDebug() << QObject::tr(" [!][TableSchema][%1][%2] SQL query error: %3").
                        arg(QString::number(code), getCurrentDate(), value);
            break;
        }
        case errors::EMPTY_QUERY_RESULT:
        {
            qDebug() << QObject::tr(" [!][TableSchema][%1][%2] SQL query result is empty!").
                        arg(QString::number(code), getCurrentDate());
            break;
        }
    }
}

/*==============================================================================
 Метод возвращает результат подключения к БД
 database - объект по работе с БД
 name - имя базы данных, к которой осуществляется подключение
==============================================================================*/
bool databases::connectToDB (QSqlDatabase &database, QString name)
{
    if(QSqlDatabase::contains("dbConnection"))
    {
        database = QSqlDatabase::database("dbConnection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE", "dbConnection");
        database.setDatabaseName(name);
    }
    if(!database.open())
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*==============================================================================
 Метод возвращает перечень таблиц, входящих в состав БД
 dbName - имя базы данных
==============================================================================*/
QStringList databases::getTables(const QString &dbName)
{
    QSqlDatabase base;
    if(databases::connectToDB(base, dbName))
    {
        return base.tables();
    }
    errors::printError(errors::DB_ERROR, base.lastError().text());
    return QStringList();
}

/*==============================================================================
 Метод проверяет наличие таблицы в базе данных
 tableName - имя таблицы
 dbName - имя базы данных
==============================================================================*/
bool databases::checkTable(const QString &tableName, const QString &dbName)
{
    return getTables(dbName).contains(tableName);
}

/*==============================================================================
 Метод конвертирует значение value в строку для  SQL-запроса  в  соответствующем
 формате
==============================================================================*/
QString SQL::convertToSQLString(const QVariant &value)
{
    QString res;
    if(value.type() == QVariant::String)
    {
        res = "'" + value.toString() + "'";
    }
    else
    {
        res = value.toString();
    }
    return res;
}
