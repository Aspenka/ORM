#include "globaldata.h"

QString errors::getCurrentDate ()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

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
    }
}

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
