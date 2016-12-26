#include "query.h"

Query::Query(QString tableName, QObject *parent) : QObject(parent),
    schema(tableName)
{
    Q_UNUSED(tableName);
}

void Query::setSchema(QString tableName)
{
    Q_UNUSED(tableName);
}

void Query::setSelectedFields(QStringList list)
{
    Q_UNUSED(list);
}

void Query::setWhere(QString fieldName, QVariant value)
{
    Q_UNUSED(fieldName);
    Q_UNUSED(value);
}

void Query::setOrder(bool desc)
{
    Q_UNUSED(desc);
}

void Query::setLimit(int limit)
{
    Q_UNUSED(limit);
}

void Query::withOuterJoin(QString relatedTable)
{
    Q_UNUSED(relatedTable);
}

void Query::withInnerJoin(QString relatedTable)
{
    Q_UNUSED(relatedTable);
}

Model *Query::getOne()
{    
    return new Model("");
}

QList<Model *> Query::getAll()
{
    return QList <Model *>();
}

int Query::getCount()
{
    return 0;
}

void Query::removeParameter(parameter_e name)
{
    Q_UNUSED(name);
}

void Query::clearAll()
{

}

QString Query::generateSql()
{
    return QString("");
}

QString Query::generateSelections(QStringList list)
{
    Q_UNUSED(list);
    return QString("");
}

QString Query::generateWhere(QMap<QString, QString> map)
{
    Q_UNUSED(map);
    return QString("");
}

QString Query::generateOrder(QPair<bool, bool> pair)
{
    Q_UNUSED(pair);
    return QString("");
}

QString Query::generateLimit(int value)
{
    Q_UNUSED(value);
    return QString("");
}

QString Query::generateCount(bool flag)
{
    Q_UNUSED(flag);
    return QString("");
}

QString Query::generateOuterJoin(QString relatedTable)
{
    Q_UNUSED(relatedTable);
    return QString("");
}

QString Query::generateInnerJoin(QString relatedTable)
{
    Q_UNUSED(relatedTable);
    return QString("");
}
