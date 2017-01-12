#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QPair>
#include "tableschema.h"
#include "model.h"

//Перечисление содержит информацию о параметрах SQL-запроса:
enum parameter_e
{
    SELECTIONS, //выбранные поля
    WHERE,      //условия выборки
    ORDER,      //сортировка выборки
    LIMIT,      //ограниченик по количеству выбираемых записей
    COUNT,      //подсчет количества записей, полученных в результате вборки
    INNER_JOIN, //выборка с внутренним объединением таблиц
    OUTER_JOIN  //выборка с внешним объединением таблиц
};

typedef QList <QList <QPair <QString, QVariant>>>   matrix_t;   //тип данных, позволяющий хранить все записи, полученные в результате выборки
typedef QList <QPair <QString, QVariant>>           list_t;     //тип данных, позволяющий хранить одну запись, полученную в результате выборки

/*========================================================================
 Класс Query представлят собой объект, генерирующий и выполняющий SQL-зап-
 росы по выборке данных, а так же возвращающий  данные, полученные  в  ре-
 зультате  выполнения этих SQL-запросов.

 Класс включает в себя перечень полей, содержащих следующую информацию:
 - схема запрашиваемых данных;
 - перечень полей для выборки данных;
 - условия для формирования выборки данных;
 - порядок сортировки данных;
 - флаг осуществления подсчета количества записей,  полученных в результа-
   те выполнения SQL-запроса;
 - количество записей по запрошенной выборке;
 - данные по внешнему объединениею таблиц;
 - данные по внутреннему объединению таблиц;
 - сгенерированный SQL-запрос

 Методы класса подволяют выполнять следующий набор действий:
 - генерация отдельных компонентов SQL-запроса и самого SQL-запроса;
 - установка новой схемы данных для генерации запроса;
 - установка данных, необходимых для генерации SQL-запроса, а именно:
   - поля, по которым осуществляется выборка данных;
   - условия для формирования выборки;
   - информация по сортировке записей;
   - установка ограничений по выборке записей;
   - установка информации по внешним и внутренним объединениям таблиц;
 - получение одной записи по запросу;
 - получение всех записей по запросу;
 - получение количества записей, получившихся в результате выполнения зап-
   роса;
 - удаление одного из компонентов SQL-запроса;
 - очищение данных текущего объекта.
========================================================================*/
class Query : public QObject
{
    Q_OBJECT
private:
    TableSchema         schema;         //схема запрашиваемых данных
    QStringList         selectedFields; //перечень полей для выборки
    QMap
    <QString, QVariant> where;          //строка с условием выборки
    QPair
    <QString, bool>     order;          //порядок сортировки выборанных данных
    int                 limit;          //ограничение на количество выводимых записей
    QString             outerJoin,      //выборка с внешним объединением таблиц
                        innerJoin,      //выорка с внутренним объединением таблиц
                        count,          //количество записей по запрошенной выборке
                        relatedTName,   //имя связанной таблицы
                        sqlString;      //строка SQL-запроса

    QString     generateSql         ();                                                         //генерация строки SQL-запроса
    QString     generateSelections  (const QStringList & list);                                 //генерация строки выборанных полей
    QString     generateWhere       (const QMap<QString, QVariant> & map);                      //генерация строки условий
    QString     generateOrder       (const QPair<QString, bool> &pair);                         //генерация строки с условиями сортировки выборки
    QString     generateLimit       (int value = 0);                                            //генерация строки с ограничением по выборке
    QString     generateCount       (QString fieldName);                                        //генерация строки с подсчетом количества выборанных записей
    QString     generateJoin        (const QString & joinType, const QString & relatedTable);   //генерация выборки с объединением таблиц

    matrix_t    dbRequest           (const QString & sql, const QString & tableName);   //выполнение сгенерированного SQL-запроса
    Model *     createModel         (const list_t & record, const QString &tableName);  //создание модели данными
    Model *&    fillModel           (Model *& model, const list_t &record);             //заполнение модели данными
    QList
    <Model*>    handleResult        (bool isOne = true);                                //метод обрабатывает результаты SQL-запроса

public:
    explicit    Query               (const QString & tableName, QObject *parent = 0);   //конструктор по умолчанию

    void        setSchema           (const QString & tableName);                            //установка новой схемы данных
    void        setSelectedFields   (const QStringList & list);                             //установка полей для выборки???
    void        setWhere            (const QString & fieldName, const QVariant & value);    //установка условия выборки
    void        setOrder            (QString fieldName, bool desc = false);                 //установка флага сортировки записей
    void        setLimit            (int lim);                                              //установка ограничения на количество выбранных записей
    void        setCount            (QString fieldName = "*");                              //установка подсчета количества полученных записей
    void        withOuterJoin       (const QString & relatedTable);                         //установка выборки с внешним объединением
    void        withInnerJoin       (const QString & relatedTable);                         //установка выборки с внутренним объединением

    Model *     getOne              ();                     //получить одну запись по выборке
    QList
    <Model *>   getAll              ();                     //получить перечень записей по выборке
    int         getCount            ();    //подсчитать количество записей по выборке

    void        removeParameter     (parameter_e name); //удалить параметр из запроса
    void        clearAll            ();                 //очистка всех параметров объекта

signals:

public slots:
};

#endif // QUERY_H
