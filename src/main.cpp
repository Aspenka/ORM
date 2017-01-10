#include <QCoreApplication>
#include <QPair>
#include "tableschema.h"
#include "table.h"
#include "query.h"
#include "model.h"

TableSchema *   getSchema   (const QString & tableName);
void            printSchema (TableSchema schema);
void            printList   (const QStringList & list, const QString &text);

Query &         tuneQuery   (Query &query);
void            getQuery    (Query & query);
Query &         tuneCount   (Query & query);
void            getCount    (Query & query);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Query q(QString("gateway_action"));
    tuneQuery(q);
    getQuery(q);

    q.clearAll();
    tuneCount(q);
    getCount(q);

    q.removeParameter(COUNT);
    q.setWhere("gateway_target_group_uid", 1818);
    q.setOrder("gateway_action_uid");
    getQuery(q);

    qDebug() << "\nPlease press any key...";

    return 0;
}

TableSchema * getSchema(const QString & tableName)
{
    TableSchema *sch = new TableSchema(QString("base.db"));
    sch->generate(tableName);
    return sch;
}

void printSchema(TableSchema schema)
{
    QString tableName = schema.getTableName();
    QStringList fields = schema.getFields();
    QStringList pk = schema.getPrimaryKeys();
    QStringList relatedTables = schema.getRelatedTables();

    printList(fields, QString(tableName + " fields:"));
    printList(pk, QString(tableName + " primary keys:"));
    printList(relatedTables, QString(tableName + " related tables:"));

    qDebug() << tableName << " relations:";
    if(!relatedTables.isEmpty())
    {
        foreach(QString i, relatedTables)
        {
            QPair <QString, QString> p(schema.getRelation(i));
            qDebug() << i << ": " << p.first
                     << "->" << p.second;
        }
    }
    else
    {
        qDebug() << "\tis empty.";
    }
}

void printList(const QStringList &list, const QString &text)
{
    qDebug() << text;
    if(!list.isEmpty())
    {
        foreach (QString i, list)
        {
            qDebug() << "\t" << i;
        }
    }
    else
    {
        qDebug() << "\tis empty.";
    }
}

Query & tuneQuery (Query & query)
{
    query.setLimit(3);
    query.setOrder(QString("priority"), true);
    query.setWhere(QString("gateway_target_group_uid"), QString("1818"));

    QStringList list;
    list << "gateway_action_uid" << "meas_type_uid" << "device_parametr_uid" << "priority";
    query.setSelectedFields(list);
    return query;
}

void getQuery (Query & query)
{
    //Model *model = query.getOne();
    QList <Model *> list = query.getAll();
}

Query & tuneCount (Query & query)
{
    query.setCount("gateway_action_uid");
    query.setWhere("device_parametr_uid", 51);
    return query;
}
void getCount (Query & query)
{
    int count = query.getCount();
    qDebug() << "COUNT: " << count;
}
