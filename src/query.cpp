#include <QtSql>
#include "query.h"
#include "table.h"

/*==============================================================================
 Конструктор по умолчанию
 tableName - имя таблицы БД, в соответствии с которой создается схема данных для
             объекта
==============================================================================*/
Query::Query(const QString & tableName, QObject *parent) : QObject(parent),
    schema(tableName)
{
    clearAll();
    setSchema(tableName);
}

/*==============================================================================
 Метод устанавливает новую схему данных для объекта
 tableName - имя таблицы БД, в соответствии с которой  будет  установлена  новая
             схема данных
==============================================================================*/
void Query::setSchema(const QString &tableName)
{
    if(databases::checkTable(tableName, QString("base.db")))
    {
        schema = Table::instance().get(tableName);
    }
    else
    {
        errors::printError(errors::EXISTS_TABLE, tableName);
        schema.clear();
    }
}

/*==============================================================================
 Метод устанавливает перечень полей для выборки данных из БД
 list - перечень полей для выборки данных из БД
==============================================================================*/
void Query::setSelectedFields(const QStringList &list)
{
    selectedFields = list;
}

/*==============================================================================
 Метод генерирует строку условий для SQL-запроса
 fieldName - имя поля
 value - значение поля
==============================================================================*/
void Query::setWhere(const QString &fieldName, const QVariant & value)
{
    where.insert(fieldName, value);
}

/*==============================================================================
 Метод генерирует строку с условиями сортировки для SQL-запроса
 fieldName - имя поля, по которому будет осуществляться сортировка
 desc - флаг, указывающий на то, что в запросе будет осуществлена сортировка по
        убыванию
==============================================================================*/
void Query::setOrder(QString fieldName, bool desc)
{
    if(desc != false)
    {
        order = QPair <QString, bool> (fieldName, true);
    }
    else
    {
        order = QPair <QString, bool> (fieldName, false);
    }
}

/*==============================================================================
 Метод генерирует строку с указанием предельного количества записей по выборке
 limit - количество записей по выборке из БД
==============================================================================*/
void Query::setLimit(int lim)
{
    limit = lim;
}

/*==============================================================================
 Метод генерирует строку с указанием подсчета записей по выборке
 fieldName - имя поля, по которому будет осуществляться выборка
==============================================================================*/
void Query::setCount(QString fieldName)
{
    count = generateCount(fieldName);
}

/*==============================================================================
 Метод генерирует строку запроса для выборки с внешним объединением данных
 relatedTable - имя связанной таблицы
==============================================================================*/
void Query::withOuterJoin(const QString &relatedTable)
{
    outerJoin = generateJoin(QString(" LEFT OUTER JOIN "), relatedTable);
    relatedTName = relatedTable;
}

/*==============================================================================
 Метод генерирует строку запроса для выорки с внутренним объединением данных
 relatedTable - имя связанной таблицы
==============================================================================*/
void Query::withInnerJoin(const QString &relatedTable)
{
    innerJoin = generateJoin(QString(" INNER JOIN "), relatedTable);
    relatedTName = relatedTable;
}

/*==============================================================================
 Метод возвращает одну запись по запросу
==============================================================================*/
Model *Query::getOne()
{    
    return handleResult().at(0);
}

/*==============================================================================
 Метод возвращает все записи по запросу
==============================================================================*/
QList<Model *> Query::getAll()
{
    return handleResult(false);
}

/*==============================================================================
 Метод обрабатывает результаты выполнения запроса и возвращает их в удобном  для
 дальнейшей обработки виде
 isOne - флаг, показывающий, сколько записей нужно обработать
==============================================================================*/
QList<Model *> Query::handleResult(bool isOne)
{
    if(isOne)
    {
        limit = 1;
    }
    sqlString = generateSql();
    //qDebug() << "SQL: " <<  sqlString;  //!!!выпилить позже!!!
    QList <Model *> modelList;
    matrix_t result;
    QString name = schema.getTableName();

    if( (!innerJoin.isEmpty()) || (!outerJoin.isEmpty()) )
    {
        name = relatedTName;
    }

    result = dbRequest(sqlString, name);
    if(!result.isEmpty())
    {
        for(int i = 0; i < result.size(); ++i)
        {
            Model *model = createModel(result.at(i), name);
            modelList.append(model);
        }
    }
    else
    {
        errors::printError(errors::EMPTY_QUERY_RESULT, QString(""));
    }

    return modelList;
}

/*==============================================================================
 Метод запускает SQL-запрос на выполнение и заполняет модель полученными данными
 record - одна запись, полученная в результате выборки
 tableName - имя таблицы, из которой производилась выборка
==============================================================================*/
Model *Query::createModel(const list_t &record, const QString & tableName)
{
    Q_UNUSED(record);
    Q_UNUSED(tableName);

    /*qDebug() << "Requesr result:\n";
    for(int i = 0; i < record.size(); ++i)
    {
        qDebug() << record.at(i).first << ": " << record.at(i).second;
    }*/

    Model *model = new Model(schema.getTableName());


    return model;
}

/*==============================================================================
 Метод возвращает количество записей, полученных в результате выполнения запроса
==============================================================================*/
int Query::getCount()
{
    sqlString = generateSql();
    matrix_t result = dbRequest(sqlString, QString("COUNT"));
    if(!result.isEmpty())
    {
        return result.at(0).at(0).second.toInt();
    }
    return  0;
}

/*==============================================================================
 Метод удаляет один из параметров запроса
 name - имя параметра запроса
==============================================================================*/
void Query::removeParameter(parameter_e name)
{
    switch (name)
    {
        case SELECTIONS:
        {
            selectedFields.clear();
            break;
        }
        case WHERE:
        {
            where.clear();
            break;
        }
        case ORDER:
        {
            order = QPair <QString, bool> ("", false);
            break;
        }
        case LIMIT:
        {
            limit = 0;
            break;
        }
        case COUNT:
        {
            count.clear();
            break;
        }
        case INNER_JOIN:
        {
            innerJoin.clear();
            relatedTName.clear();
            break;
        }
        case OUTER_JOIN:
        {
            outerJoin.clear();
            relatedTName.clear();
            break;
        }
    }
}

/*==============================================================================
 Метод очищает все данные, связанные с генерацией SQL-запросов и схемами данных
==============================================================================*/
void Query::clearAll()
{
    selectedFields.clear();
    where.clear();
    order = QPair <QString, bool> ("", false);
    limit = 0;
    count.clear();
    outerJoin.clear();
    innerJoin.clear();
    sqlString.clear();
    relatedTName.clear();
}

/*==============================================================================
 Метод генерирует строку SQL-запроса
==============================================================================*/
QString Query::generateSql()
{
    QString query = "";
    query += generateSelections(selectedFields);
    if(!outerJoin.isEmpty())
    {
        query += outerJoin;
    }
    else if(!innerJoin.isEmpty())
    {
        query += innerJoin;
    }
    query += generateWhere(where);
    query += generateOrder(order);
    query += generateLimit(limit);

    return query;
}

/*==============================================================================
 Метод генерирует строку, содержащую набор полей для выборки
 list - перечень полей для выборки
==============================================================================*/
QString Query::generateSelections(const QStringList & list)
{
    QString selections = "SELECT ";
    if(!count.isEmpty())
    {
        selections += count;
    }
    else if(!list.isEmpty())
    {
        selections += schema.getTableName() + "." + list.at(0);
        for (int i = 1; i < list.size(); ++i)
        {
            selections += ", " + schema.getTableName() + "." + list.at(i);
        }
    }
    else
    {
        selections += "*";
    }
    selections += " FROM " + schema.getTableName();
    return selections;
}

/*==============================================================================
 Метод генерирует строку условий для выборки данных
 map - карта, содержащая набор условий для формирования запроса. Ключ  карты со-
       держит имя поля, значение карты - значение поля
==============================================================================*/
QString Query::generateWhere(const QMap<QString, QVariant> &map)
{
    QString wheres = "";
    if(!map.isEmpty())
    {
        QMap <QString, QVariant>::const_iterator i = map.begin();
        wheres += " WHERE " + i.key() + " = " + SQL::convertToSQLString(i.value());
        ++i;
        if(map.size() > 1)
        {
            while(i != map.end())
            {
                wheres += " AND " + i.key() + " = " + SQL::convertToSQLString(i.value());
                ++i;
            }
        }
    }
    return wheres;
}

/*==============================================================================
 Метод генерирует строку, содержащую данные по сортировке данных
 pair - пара значений. Первое значение пары - наличие сортировки в SQL-запросе,
        второе значение - флаг сортировки(false - по возрастанию, true - по убы-
        ванию)
==============================================================================*/
QString Query::generateOrder(const QPair<QString, bool> &pair)
{
    QString orders = "";
    if(pair.first != "")
    {
        orders += " ORDER BY " + pair.first;
        if(pair.second != false)
        {
            orders += " DESC";
        }
    }
    return orders;
}

/*==============================================================================
 Метод генерироует строку, содержащую ограничение по выборке данных
 value - количество записей по выборке
==============================================================================*/
QString Query::generateLimit(int value)
{
    QString limits = "";
    if(value != 0)
    {
        limits += " LIMIT " + QString::number(value);
    }
    return limits;
}

/*==============================================================================
 Метод генерирует строку для получения количества записей, полученных в  резуль-
 тате выполнения запроса
 flag - флаг наличия данной строки в SQL-запросе
==============================================================================*/
QString Query::generateCount(QString fieldName)
{
    QString counts = "";
    if(fieldName != "")
    {
        counts += "COUNT(" + fieldName + ")";
    }
    return counts;
}

/*==============================================================================
 Метод генерирует строку для выборки данных с объединением таблиц
 joinType - тип объединения таблиц
 relatedTable - имя связанной таблицы
==============================================================================*/
QString Query::generateJoin (const QString & joinType, const QString & relatedTable)
{
    QString res = "";
    if(schema.checkRelation(relatedTable))
    {
        res += joinType + relatedTable + " ON ";
        QPair <QString, QString> relation = schema.getRelation(relatedTable);
        res += relation.first + " = " + relation.second;
    }
    else
    {
        errors::printError(errors::EXISTS_RELATION, relatedTable);
    }
    return res;
}

/*==============================================================================
 Метод возвращает все записи, полученные в результате выполнения SQL-запроса
 sql - строка SQL-запроса
 schem - имя таблицы
==============================================================================*/
matrix_t Query::dbRequest(const QString &sql, const QString &tableName)
{
    matrix_t result;
    QSqlDatabase base;
    if(databases::connectToDB(base, QString("base.db")))
    {
        QSqlQuery *query = new QSqlQuery(base);
        query->prepare(sql);
        if(query->exec())
        {
            while(query->next())
            {
                list_t list;
                for(int i = 0; i < query->record().count(); ++i)
                {
                    QPair <QString, QVariant> pair;
                    TableSchema sch = Table::instance().get(tableName);
                    if( sch.checkField(query->record().fieldName(i)) || tableName == "COUNT")
                    {
                        pair.first = query->record().fieldName(i);
                        pair.second = query->value(i);
                        if(!list.contains(pair))
                        {
                            list.append(pair);
                        }
                    }
                }
                if(!result.contains(list))
                {
                    result.append(list);
                    list.clear();
                }
                else
                {
                    if(outerJoin != "")
                    {
                        result.append(list);
                        list.clear();
                    }
                }
            }
        }
        else
        {
            errors::printError(errors::QUERY_ERROR, query->lastError().text());
        }
    }
    else
    {
        errors::printError(errors::DB_ERROR, base.lastError().text());
    }
    return result;
}

