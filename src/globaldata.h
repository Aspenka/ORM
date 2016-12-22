#ifndef GLOBALDATA
#define GLOBALDATA

#include <QObject>
#include <QDateTime>
#include <QtSql>
#include <QDebug>
#include <QString>

/*=============================================================
 * Пространство имен globalData содержит переменные  и функции,
 * использование которых необходимо во многих модулях программы
 * во избежание получения избыточного кода.
=============================================================*/
namespace globalData
{
//перечислитель, содержащий коды ошибок
    enum errors_e
    {
        EXISTS_TABLE,   //отсутствие таблицы в БД
        DB_ERROR,       //ошибка БД
        EXISTS_RELATION,//отсутствие отношения между таблицами
        QUERY_ERROR     //ошибка выполнения запроса
    };

//функция возвращает значение текущей даты и времени
    QString getCurrentDate ()
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }

//функция, возвращающая результат подключения к БД, где:
//*name - имя подключаемой БД
    bool connectToDB (QSqlDatabase &database, QString name = "")
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

//отображение сообщения об ошибке в зависимости от ее кода, где:
//*errors_e code - код ошибки
//*const QString & value - параметр сообщения
    void printError (const errors_e code, const QString & value)
    {
        switch(code)
        {
            case EXISTS_TABLE:
            {
                qDebug() << QObject::tr(" [!][TableSchema][%1][%2] Table name %3 does not exists!").
                            arg(QString::number(code), getCurrentDate(), value);
                break;
            }
            case DB_ERROR:
            {

                qDebug() << QObject::tr(" [!][TableSchema][%1][%2] Database connecting error: %3").
                            arg(QString::number(code), getCurrentDate(), value);
                break;
            }
            case EXISTS_RELATION:
            {
                qDebug() << QObject::tr(" [!][TableSchema][%1][%2] Relation %3 does not exists!").
                            arg(QString::number(code), getCurrentDate(), value);
                break;
            }
            case QUERY_ERROR:
            {
                qDebug() << QObject::tr(" [!][TableSchema][%1][%2] SQL query error: %3").
                            arg(QString::number(code), getCurrentDate(), value);
                break;
            }
        }
    }

}
#endif // GLOBALDATA

