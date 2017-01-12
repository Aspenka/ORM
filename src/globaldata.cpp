#include "globaldata.h"

/*==============================================================================
 Метод возвращает текущую дату и время в формате yyyy-MM-dd hh:mm:ss
==============================================================================*/
QString messages::getCurrentDate ()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

/*==============================================================================
 Метод выводит на экран сообщение, в зависимости от кода ошибки
 code - код ошибки;
 value - часть сообщения об ошибке
==============================================================================*/
void messages::printMessage(const messages_e code, const QString className, const QString & value)
{
    switch(code)
    {
        case messages::EXISTS_TABLE:
        {
            qDebug() << QObject::tr(" [!][%1][%2][%3] Table name %4 does not exists!").
                        arg(className, QString::number(code), getCurrentDate(), value);
            break;
        }
        case messages::EXISTS_RELATION:
        {
            qDebug() << QObject::tr(" [!][%1][%2][%3] Relation %4 does not exists!").
                        arg(className, QString::number(code), getCurrentDate(), value);
            break;
        }
        case messages::EXISTS_FIELD:
        {
            qDebug() << QObject::tr(" [!][%1][%2][%3] Field %4").
                        arg(className, QString::number(code), getCurrentDate(), value);
            break;
        }
        case messages::DB_ERROR:
        {

            qDebug() << QObject::tr(" [!][%1][%2][%3] Database connecting error: %4").
                        arg(className, QString::number(code), getCurrentDate(), value);
            break;
        }

        case messages::QUERY_ERROR:
        {
            qDebug() << QObject::tr(" [!][%1][%2][%3] SQL query error: %4").
                        arg(className, QString::number(code), getCurrentDate(), value);
            break;
        }
        case messages::EMPTY_QUERY_RESULT:
        {
            qDebug() << QObject::tr(" [!][%1][%2][%3] SQL query result is empty!").
                        arg(className, QString::number(code), getCurrentDate());
            break;
        }
        case messages::RECORD_SAVED:
        {
            qDebug() << QObject::tr(" [ ][%1][%2][%3] Record saved into table %4!").
                        arg(className, QString::number(code), getCurrentDate(), value);
            break;
        }
    case messages::RECORD_REMOVED:
        {
            qDebug() << QObject::tr(" [ ][%1][%2][%3] Record removed from table %4!").
                        arg(className, QString::number(code), getCurrentDate(), value);
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
    messages::printMessage(messages::DB_ERROR, QString("databases"), base.lastError().text());
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
