#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QStringList>
#include "tableschema.h"
#include "model.h"

enum parameter_e
{
    SELECTIONS,
    WHERE,
    ORDER,
    LIMIT,
    INNER_JOIN,
    OUTER_JOIN
};

class Query : public QObject
{
    Q_OBJECT
private:
    TableSchema         schema;         //схема запрашиваемых данных
    QStringList         selectedFields; //перечень полей для выборки
    QMap
    <QString, QString>  where;          //строка с условием выборки
    QPair <bool, bool>  order;          //порядок сортировки выборанных данных
    bool                countFlag;      //флаг формирования строки подсчета выбранных полей
    int                 limit,          //рганичение на количество выводимых записей
                        count;          //количество записей по запрошенной выборке
    QString             outerJoin,      //выборка с внешним объединением таблиц
                        innerJoin,      //выорка с внутренним объединением таблиц
                        sqlString;      //строка SQL-запроса

    QString     generateSql         ();                             //генерация строки SQL-запроса
    QString     generateSelections  (QStringList list);             //генерация строки выборанных полей
    QString     generateWhere       (QMap <QString, QString> map);  //генерация строки условий
    QString     generateOrder       (QPair <bool, bool> pair);      //генерация строки с условиями сортировки выборки
    QString     generateLimit       (int value);                    //генерация строки с ограничением по выборке
    QString     generateCount       (bool flag);                    //генерация строки с подсчетом количества выборанных записей
    QString     generateOuterJoin   (QString relatedTable);         //генерация выборки с внешним объединением
    QString     generateInnerJoin   (QString relatedTable);         //генерация выборки с внутренним объединением таблиц
public:
    explicit    Query               (QString tableName, QObject *parent = 0);   //конструктор по умолчанию

    void        setSchema           (QString tableName);                    //установка новой схемы данных
    void        setSelectedFields   (QStringList list);                     //установка полей для выборки
    void        setWhere            (QString fieldName, QVariant value);    //установка условия выборки
    void        setOrder            (bool desc = false);                    //установка флага сортировки записей
    void        setLimit            (int limit);                            //установка ограничения на количество выбранных записей
    void        withOuterJoin       (QString relatedTable);                 //установка выборки с внешним объединением
    void        withInnerJoin       (QString relatedTable);                 //установка выборки с внутренним объединением

    Model *     getOne              (); //получить одну запись по выборке
    QList
    <Model *>   getAll              (); //получить перечень записей по выборке
    int         getCount            (); //полсчитать количество записей по выборке

    void        removeParameter     (parameter_e name); //удалить параметр из запроса
    void        clearAll            ();                 //очистка всех параметров объекта

signals:

public slots:
};

#endif // QUERY_H
