#include "tableschema.h"
#include "globaldata.h"
#include <QDateTime>

/*=============================================================================
 Конутрктор по умолчанию.
 QString table - имя таблицы, по которой нужно сгенерировать схему
 QString dbName - имя БД, в которой хранится таблица
=============================================================================*/
TableSchema::TableSchema(const QString & baseName, QString table, QObject *parent) : QObject(parent)
{
    setTableName(table);
    dbName = baseName;
}

/*==============================================================================
 Конструктор копирования.
 const TableSchema &obj - объект, данные которого копируются в текущий объект
==============================================================================*/
TableSchema::TableSchema(const TableSchema &obj, QObject *parent) : QObject(parent)
{
    copy(obj);
}

/*==============================================================================
 Перегрузка оператора =
==============================================================================*/
TableSchema &TableSchema::operator =(const TableSchema &obj)
{
    copy(obj);
    return *(this);
}

/*==============================================================================
 Перегрузка оператора ==
==============================================================================*/
bool TableSchema::operator ==(const TableSchema &right)
{
    if ((fields == right.fields) &&
        (primaryKey == right.primaryKey) &&
        (relatedTables == right.relatedTables) &&
        (tableName == right.tableName) &&
        (relations == right.relations) &&
        (TableSchema::dbName == right.TableSchema::dbName) &&
        (TableSchema::dbTables == right.TableSchema::dbTables))
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
bool TableSchema::operator !=(const TableSchema &right)
{
    return !( this == & right );
}

/*==============================================================================
 Метод устанавливает имя таблицы
 const QString & name - новое имя таблицы
==============================================================================*/
void TableSchema::setTableName(const QString & name)
{
    tableName = name;
}

/*==============================================================================
 метод очищает данные о схеме
==============================================================================*/
void TableSchema::clear()
{
    fields.clear();
    primaryKey.clear();
    relatedTables.clear();
    tableName = "";
    relations.clear();
}

/*==============================================================================
 Метод генерирует схему
 const QString & table - имя таблицы
==============================================================================*/
void TableSchema::generate(const QString & table)
{
    using namespace globalData;
    clear();
    setTableName(table);
    if(connectToDB(database, dbName))
    {
        if(checkTable(table))
        {            
            dbTables = database.tables();
            QSqlRecord record = database.record(tableName);
            setFields(record);
            QSqlIndex index = database.primaryIndex(tableName);
            setPrimaryKey(index);
            setRelations();
        }
        else
        {
            printError(EXISTS_TABLE, table);
        }
    }
    else
    {
        printError(DB_ERROR, TableSchema::database.lastError().text());
    }
}

/*==============================================================================
 Метод возвращает перечень таблиц БД
==============================================================================*/
QStringList TableSchema::getTables(const QString & databaseName)
{
    using namespace globalData;
    QSqlDatabase database;
    if(connectToDB(database, databaseName))
    {
        return database.tables();
    }
    else
    {
        printError(DB_ERROR, database.lastError().text());
        return QStringList();
    }
}

/*==============================================================================
 Метод возвращает перечень полей таблицы
==============================================================================*/
QStringList TableSchema::getFields()
{
    return fields;
}

/*==============================================================================
 Метод возвращает перечень первичных ключей таблицы
==============================================================================*/
QStringList TableSchema::getPrimaryKey()
{
    return primaryKey;
}

/*==============================================================================
 Метод возвращает перечень связанных таблиц
==============================================================================*/
QStringList TableSchema::getRelatedTables()
{
    return relatedTables;
}

/*==============================================================================
 Метод возвращает имя таблицы
==============================================================================*/
QString TableSchema::getTableName()
{
    return tableName;
}

/*==============================================================================
 Метод возвращает конкретную пару связанных полей двух таблиц в соответствии
 с именем связи
==============================================================================*/
QPair<QString, QString> TableSchema::getRelation(const QString &tableName)
{
    using namespace globalData;
    if(checkRelation(tableName))
    {
        return relations.value(tableName);
    }
    else
    {
        printError(EXISTS_RELATION, tableName);       
    }
    return QPair <QString, QString>();
}

/*==============================================================================
 Метод проверяетналичие поля fieldName в схеме данных
==============================================================================*/
bool TableSchema::checkField(const QString &fieldName)
{
    return fields.contains(fieldName);
}

/*==============================================================================
 Метод проверяет наличие таблицы tableName в БД
==============================================================================*/
bool TableSchema::checkTable(const QString & tableName)
{
    if(dbTables.isEmpty())
    {
        dbTables = TableSchema::getTables(dbName);
    }
    return dbTables.contains(tableName);
}

/*==============================================================================
 Метод проверяет наличие отношения таблицы с именем tableName к текущей таблице
==============================================================================*/
bool TableSchema::checkRelation(const QString & relationName)
{
    using namespace globalData;
    if(relations.contains(relationName))
    {
        return true;
    }
    else
    {
        TableSchema rSchema(dbName, relationName);
        rSchema.generate();
        if(rSchema.getRelatedTables().contains(tableName))
        {
            relations.insert(relationName, rSchema.getRelation(tableName));
            relatedTables.append(relationName);
            return true;
        }
        else
        {
            printError(EXISTS_RELATION, relationName);
            return false;
        }
    }

}

/*==============================================================================
 Метод копирует данные из объекта obj в текущий объект
==============================================================================*/
void TableSchema::copy(const TableSchema & obj)
{
    fields = obj.fields;
    primaryKey = obj.primaryKey;
    relatedTables = obj.relatedTables;
    dbTables = obj.dbTables;
    relations = obj.relations;
    tableName = obj.tableName;
    dbName = obj.dbName;
}

/*==============================================================================
 Метод заполняет поля схемы данных, используя данные БД
 const QSqlRecord & record - информация о записях БД
==============================================================================*/
void TableSchema::setFields(const QSqlRecord & record)
{
    for(int i = 0; i < record.count(); i++)
    {
        fields.append(record.fieldName(i));
    }
}

/*==============================================================================
 Метод заполняет данные о первичных ключах записи, используя информацию из БД.
 QSqlIndex index - информация о первичных ключах таблицы
==============================================================================*/
void TableSchema::setPrimaryKey(QSqlIndex index)
{
    for(int i = 0; i < index.count(); i++)
    {
        primaryKey.append(index.fieldName(i));
    }
}

/*==============================================================================
 Метод устанавливает отношения между текущей таблице и остальными таблицами БД
==============================================================================*/
void TableSchema::setRelations()
{
    using namespace globalData;
    QSqlQuery *query = new QSqlQuery(database);
    QString str = "PRAGMA foreign_keys = ON";
    query->prepare(str);
    if(query->exec())
    {
        query->clear();
        str.clear();
        str = "pragma foreign_key_list(" + tableName + ")";
        query->prepare(str);
        if(query->exec())
        {
            while(query->next())
            {
                QString linkedTable = query->value(2).toString();
                QString boundField = query->value(3).toString();
                QString linkedField = query->value(4).toString();
                boundField = tableName + "." + boundField;
                linkedField = linkedTable + "." + linkedField;
                relations.insert(linkedTable, QPair <QString, QString> (boundField, linkedField));
                relatedTables.append(linkedTable);
            }
       }
       else
       {
            printError(QUERY_ERROR, query->lastError().text());
       }
    }
    else
    {
        printError(QUERY_ERROR, query->lastError().text());
    }
    delete query;
}
