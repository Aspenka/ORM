#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QList>
#include <QMap>
#include <QtSql>

typedef QMap <QString, QPair <QString, QString>> relation_t;

class TableSchema : public QObject
{
    Q_OBJECT
private:
/*==========================Поля=========================================*/
    QStringList         fields,         //перечень полей
                        primaryKey,     //перечень первичных ключей
                        relatedTables;  //перечень связанных таблиц
    QStringList         dbTables;       //перечень таблиц, сходящих в состав БД
    QString             dbName;         //имя БД
    relation_t          relations;      //перечень связей
    QString             tableName;      //имя таблицы
    static QSqlDatabase database;       //база данных


/*=========================Методы========================================*/
    void                copy                (const TableSchema &obj);   //копирование данных в текущий объект
    void                setFields           (const QSqlRecord &record); //Установить данные о полях
    void                setPrimaryKey       (QSqlIndex index);          //Установить первичные ключи
    void                setRelations        ();                         //установить отношения между таблицами

public:
    explicit            TableSchema         (const QString & baseName, QString table = "", QObject *parent = 0);    //конутрктор по умолчанию
                        TableSchema         (const TableSchema & obj, QObject *parent = 0);                         //конструктор копирования

    TableSchema &       operator =          (TableSchema const & obj);      //перегрузка оператора =
    bool                operator ==         (const TableSchema & right);    //перегрузка оператора ==
    bool                operator !=         (const TableSchema & right);    //перегрузка оператора !=

    void                setTableName        (const QString & name);         //установить имя таблицы
    void                clear               ();                             //очистить схему
    void                generate            (const QString &table = "");    //сгенерировать схему

    static QStringList  getTables           (const QString & databaseName); //получить имена таблиц БД
    QStringList         getFields           (); //получить перечень полей
    QStringList         getPrimaryKey       (); //получить перечень первичных ключей
    QStringList         getRelatedTables    (); //получить перечень связанных таблиц
    QString             getTableName        (); //получить имя таблицы
    QPair
    <QString, QString>  getRelation         (const QString & tableName);    //получить данные о конкретном отношении по его имени

    bool                checkField          (const QString & fieldName);    //проверить наличие поля в таблице
    bool                checkTable          (const QString &tableName);     //static  и добавить перечень таблиц ЬД?
    bool                checkRelation       (const QString & relationName);    //проверить наличие связанной таблицы
signals:

public slots:
};

#endif // TABLESCHEMA_H
