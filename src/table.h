#ifndef TABLE
#define TABLE

#include <QObject>
#include "tableschema.h"
#include "globaldata.h"
/*========================================================================
 Класс Table является синглтоном и генерирует перечень схем для всех  таб-
 лиц, входящих в состав БД
========================================================================*/
class Table
{

private:
    QMap <QString, TableSchema>        tables;     //перечень сгенерированных таблиц

/*========================================================================
 конструктор по умолчанию
========================================================================*/
    explicit Table ()
    {
        generateTables();
    }
/*========================================================================
 метод генерируем схемы данных для каждой таблицы БД
========================================================================*/
    void generateTables ()
    {
        TableSchema schema("base_db");
        QStringList tableList = TableSchema::getTables("base.db");
        QMap <QString, TableSchema> map;

        foreach (QString i, tableList)
        {
            schema.generate(i);
            map.insert(i, schema);
            schema.clear();
        }
        tables = map;
    }

public:
/*========================================================================
 статический метод, создающий объект синглтон
========================================================================*/
    static Table & instance ()
    {
        static Table singleton;
        return singleton;
    }

/*========================================================================
 метод возвращает схему данных в зависимости от значения имени таблицы
 tableName
========================================================================*/
    TableSchema get (QString table)
    {
        TableSchema schema("base.db", table);
        if(!tables.contains(table))
        {
            printError(errors::EXISTS_TABLE, table);
        }
        else
        {
            schema = tables.value(table);
        }
        return schema;
    }
};
#endif // TABLE

