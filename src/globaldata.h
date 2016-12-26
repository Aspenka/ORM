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
namespace errors
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
    extern QString getCurrentDate ();

//отображение сообщения об ошибке в зависимости от ее кода, где:
//*errors_e code - код ошибки
//*const QString & value - параметр сообщения
    extern void printError (const errors_e code, const QString & value);
}

namespace databases
{
//функция, возвращающая результат подключения к БД, где:
//*name - имя подключаемой БД
    extern bool connectToDB (QSqlDatabase &database, QString name = "");
}


#endif // GLOBALDATA

