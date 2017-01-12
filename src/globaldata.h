#ifndef GLOBALDATA
#define GLOBALDATA

#include <QObject>
#include <QDateTime>
#include <QtSql>
#include <QDebug>
#include <QString>

/*=============================================================
 Данный заголовочный файл содержит  набор  пространств  имен, в
 состав которых входят функции, часто  использующиеся в  разных
 модулях программы
=============================================================*/

/*=============================================================
 Пространство имен errors содержит поля и методы, предназначен-
 ные для обработки ошибок
=============================================================*/
namespace messages
{
    //перечислитель, содержащий коды ошибок
    enum messages_e
    {
        //================Коды ошибок===================
        EXISTS_TABLE,       //отсутствие таблицы в БД
        EXISTS_RELATION,    //отсутствие отношения между таблицами
        EXISTS_FIELD,       //отсутствие поля в таблице
        DB_ERROR,           //ошибка БД
        QUERY_ERROR,        //ошибка выполнения запроса
        EMPTY_QUERY_RESULT,  //запрос к БД не дал результатов

        //=======Коды информационных сообщений==========
        RECORD_SAVED,       //запись сохранена в БД
        RECORD_REMOVED,     //запись удалена из БД
    };

    extern  QString getCurrentDate  ();                                                                             //функция возвращает значение текущей даты и времени
    extern  void    printMessage    (const messages_e code, const QString className, const QString & value = "");   //отображение сообщения об событии в зависимости от его кода
}

/*=============================================================
 Пространство имен databases содержит методы по работе с БД
=============================================================*/
namespace databases
{
    extern  bool        connectToDB     (QSqlDatabase &database, QString name = "");            //функция, возвращающая результат подключения к БД
    extern  bool        checkTable      (const QString & tableName, const QString & dbName);    //функция, проверяющая наличие таблицы в БД
    extern  QStringList getTables       (const QString & dbName);                               //функция, получающая перечень таблиц, входящих в состав БД
}

/*=============================================================
 Пространство имен SQL содержит методы, упрощающие генерацию
 SQL-запросов
=============================================================*/
namespace SQL
{
    extern  QString convertToSQLString  (const QVariant & value);   //конвертирование value в строковый формат
}

#endif // GLOBALDATA

