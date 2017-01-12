#include "model.h"
#include "table.h"
#include "query.h"

/*==============================================================================
 Конструктор по умолчанию.
 tableName - имя таблицы, для которой создается модель записи
==============================================================================*/
Model::Model(QString tableName, QObject *parent) : QObject(parent),
    schema(tableName)
{
    setSchema(tableName);
}

/*==============================================================================
 Конструктор копирования
 obj - объект, данные которого предназначены для копирования в текущий объект
==============================================================================*/
Model::Model(const Model &obj, QObject)
{
    copy(obj);
}

/*==============================================================================
 Деструктор класса
==============================================================================*/
Model::~Model()
{
    clear();
}

/*==============================================================================
 Перегрузка оператора =
==============================================================================*/
Model &Model::operator =(const Model &obj)
{
    copy(obj);
    return *(this);
}

/*==============================================================================
 Перегрузка оператора ==
==============================================================================*/
bool Model::operator ==(const Model &right)
{
    if( (schema  == right.schema) &&
        (record == right.record) &&
        (relationData == right.relationData) &&
        (exists == right.exists))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*==============================================================================
 Перегрузка оператора !=
==============================================================================*/
bool Model::operator !=(const Model &right)
{
    return !(this == & right);
}

/*==============================================================================
 Метод генерирует строку SQL-запроса на добавление записи в БД
==============================================================================*/
QString Model::generateInsert()
{
    QString sql = "INSERT INTO " + schema.getTableName() + " (";
    QStringList fieldList = schema.getFields();
    QString fields, values;
    fields = fieldList.at(0);
    values = SQL::convertToSQLString(getRecord(fieldList.at(0)));

    for(int i = 1; i < fieldList.size(); ++i)
    {
        fields += ", " + fieldList.at(i);
        values += ", " + SQL::convertToSQLString(getRecord(fieldList.at(i)));
    }
    sql += fields + ") VALUES (";
    sql += values + ")";
    return sql;
}

/*==============================================================================
 Метод генерирует строку SQL-запроса на удаление записи в БД
==============================================================================*/
QString Model::generateUpdate()
{
    QString sql = "UPDATE " + schema.getTableName();
    sql += " SET ";
    QMap <QString, QVariant>::const_iterator i = record.begin();
    ++i;
    sql += i.key() + " = " + SQL::convertToSQLString(i.value());
    ++i;
    for(; i != record.end(); ++i)
    {
        sql += ", " + i.key() + " = " + SQL::convertToSQLString(i.value());
    }
    sql += generateWhere();
    return sql;
}

/*==============================================================================
 Метод генерирует строку условий, необходимых для успешного выполнения  SQL-зап-
 роса
==============================================================================*/
QString Model::generateWhere()
{
    QString sql = " WHERE ";
    QStringList pkList = schema.getPrimaryKeys();
    sql += pkList.at(0) + " = " + SQL::convertToSQLString(getRecord(pkList.at(0)));
    for(int i = 1; i < pkList.size(); ++i)
    {
        sql += " AND ";
        sql += pkList.at(i) + " = " + SQL::convertToSQLString(getRecord(pkList.at(i)));
    }
    return sql;
}

/*==============================================================================
 Метод копирует данные из объекта obj в текущий объект
==============================================================================*/
void Model::copy(const Model &obj)
{
    schema = obj.schema;
    record = obj.record;
    relationData == obj.relationData;
    exists = obj.exists;
}

/*==============================================================================
 Метод проверяет наличие существования текущей записи в соответствующей таблице
 в БД (!!наличие записи подтверждается при совпадении только первичного ключа!!)
==============================================================================*/
bool Model::checkExistence()
{
    Query query(schema.getTableName());
    query.setCount();

    QStringList primaryList = schema.getPrimaryKeys();
    for(int i = 0; i < primaryList.size(); ++i)
    {
        QString fieldName = primaryList.at(i);
        query.setWhere(fieldName, getRecord(fieldName));
    }

    if(query.getCount() != 0)
    {
        return true;
    }
    return false;
}

/*==============================================================================
 Метод выполняет SQL-запрос и возвращает результаты его выполнения
==============================================================================*/
bool Model::execQuery(const QString &sql)
{
    QSqlDatabase base;
    if(databases::connectToDB(base, "base.db"))
    {
        QSqlQuery *query = new QSqlQuery(base);
        query->prepare(sql);
        if(query->exec())
        {
            return true;
        }
        else
        {
            messages::printMessage(messages::QUERY_ERROR, QString("Model"), query->lastError().text());
        }
        delete query;
    }
    else
    {
        messages::printMessage(messages::DB_ERROR, QString("Model"), base.lastError().text());
    }
    return false;
}

/*==============================================================================
 Метод устанавливает значение записи
 fieldName - имя поля
 value - значение поля
==============================================================================*/
void Model::setRecord(const QString &fieldName, const QVariant &value)
{
    if(schema.checkField(fieldName))
    {
        record.insert(fieldName, value);
    }
    else
    {
        QString message = fieldName + " doesn't exists in table " + schema.getTableName();
        messages::printMessage(messages::EXISTS_FIELD, QString("Model"), message);
    }
}

/*==============================================================================
 Метод устанавливает данные по конкретной связи
 relationName - имя связи
 relationModel - связанная модель
==============================================================================*/
void Model::setRelationData(const QString &relationName, Model *relationModel)
{
    relationData.insert(relationName, relationModel);
}

/*==============================================================================
 Метод устанавливает флаг существование записи в БД
 value - значение флага
==============================================================================*/
void Model::setExists(bool value)
{
    exists = value;
}

/*==============================================================================
 Метод устанавливает новую схему данных в соответствии с именем таблицы
 tableName
==============================================================================*/
void Model::setSchema(const QString &tableName)
{
    if(databases::checkTable(tableName, QString("base.db")))
    {
        schema = Table::instance().get(tableName);
    }
    else
    {
        messages::printMessage(messages::EXISTS_TABLE, QString("Model"), tableName);
        schema.clear();
    }
}

/*==============================================================================
 Метод возвращает значение конкретной записи по имени поля fieldName
==============================================================================*/
QVariant Model::getRecord(const QString &fieldName)
{
    if(record.contains(fieldName))
    {
        return record.value(fieldName);
    }
    return QVariant("");
}

/*==============================================================================
 Метод возвращает связанную модель по имени связи relationName
==============================================================================*/
Model *Model::getRelationData(const QString &relationName)
{
    if(relationData.contains(relationName))
    {
        return relationData.value(relationName);
    }
    return new Model("");
}

/*==============================================================================
 Метод возвращает флаг существования текущей модели в БД
==============================================================================*/
bool Model::isExists()
{
    if(!exists)
    {
        exists = checkExistence();
    }
    return exists;
}

/*==============================================================================
 Метод сохраняет модель в БД
==============================================================================*/
bool Model::save()
{
    if(isExists())
    {
        QString res = generateUpdate();
        return execQuery(res);
    }
    QString res = generateInsert();
    setExists(true);
    return execQuery(res);
}

/*==============================================================================
 Метод удаляет модель из БД
==============================================================================*/
bool Model::remove()
{
    QString sql = "DELETE FROM " + schema.getTableName();
    sql += generateWhere();
    return execQuery(sql);
}

/*==============================================================================
 Метод очищает данные о модели
==============================================================================*/
void Model::clear()
{
    schema.clear();
    record.clear();
    qDeleteAll(relationData.begin(), relationData.end());
    relationData.clear();
    exists = false;
}
