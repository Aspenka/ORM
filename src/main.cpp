#include <QCoreApplication>
#include <QPair>
#include "tableschema.h"
#include "table.h"

TableSchema *   getSchema   (const QString & tableName);
void            printSchema (TableSchema schema);
void            printList   (const QStringList & list, const QString &text);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*QStringList tables = TableSchema::getTables("base.db");
    printList(tables, QString("Database tablesinfo:"));

    TableSchema *DLRT_sch = getSchema("gateway_target");
    qDebug() << "\nSchema of \"gateway_target\":";
    printSchema(DLRT_sch);
    delete DLRT_sch;

    TableSchema *wrong_sch = getSchema("wrong");
    qDebug() << "\nSchema of \"wrong\":";
    printSchema(wrong_sch);
    delete wrong_sch;

    TableSchema DLRT_sch = Table::instance().get("gateway_target");
    qDebug() << "\nSchema of \"gateway_target\":";
    printSchema(DLRT_sch);*/

    qDebug() << "\nPlease press any key...";

    return a.exec();
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
