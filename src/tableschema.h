#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QList>
#include <QMap>
#include <QPair>
#include <QtSql>

/*========================================================================
 Тип данных Отношения. Данный тип определяет отношения между таблицами.
 Представляет собой карту, включающую в себя ключ и пару значений.
 Ключ карты - наименование отношения (в общем случае это имя связанной
              таблицы);
 Первое значение пары - имя поля, являющегося внешним ключом текущей
                        таблицы;
 Второе значение пары - имя связующего поля.
========================================================================*/
typedef QMap <QString, QPair <QString, QString>> relation_t;


/*========================================================================
 Класс TableSchema представляет собой схему данных для таблицы БД и  вклю-
 чает в себя набор методов по работе с этой схемой.

 Поля класса содержат информацию о таблице БД, а именно:
 -перечень полей;
 -перечень первичных ключей;
 -перечень таблиц, связанных с текущей посредством внешних ключей;
 -перечень таблиц, входящих в состав текущей БД;
 -имя текущей БД;
 -перечень всех связей текущей таблицы по внешним ключам;
 -текущее имя таблицы;
 -объект для взаимодействия с БД.

 Методы класса предназначены для работы со схемой данных и позволяют:
 -устанавливать текущее имя таблицы;
 -очистить текущую схему от всех данных;
 -сгенерировать новую схему данных;
 -получить имена всех таблиц, входящих в состав текущей БД;
 -получить перечень полей текущей таблицы;
 -получить перечень первичных ключей для текущей таблицы;
 -получить перечень талиц, связанных с текущей схемой данных;
 -получить имя текущей таблицы;
 -получить данные о связи с другой таблицей по ключу;
 -получить данные обо всех сгенерированных на данный момент связях;
 -проверить наличие поля в текущей таблице по его имени;
 -проверить наличие таблицы в текущей БД по его имени;
 -проверить связь данной схемы с другой таблицей по ее имени.
========================================================================*/
class TableSchema : public QObject
{
    Q_OBJECT
private:
    QStringList         fields,         //перечень полей
                        primaryKeys,     //перечень первичных ключей
                        relatedTables;  //перечень связанных таблиц
    QStringList         dbTables;       //перечень таблиц, сходящих в состав БД
    QString             dbName;         //имя БД
    relation_t          relations;      //перечень связей
    QString             tableName;      //имя таблицы
    QSqlDatabase        base;           //база данных


    void                copy                (const TableSchema &obj);   //копирование данных в текущий объект
    void                setFields           (const QSqlRecord &record); //Установить данные о полях
    void                setPrimaryKeys      (QSqlIndex index);          //Установить первичные ключи
    void                setRelations        ();                         //установить отношения между таблицами

public:
    explicit            TableSchema         (const QString & baseName, QString table = "", QObject *parent = 0);    //конутрктор по умолчанию
                        TableSchema         (QObject *parent = 0);
                        TableSchema         (const TableSchema & obj, QObject *parent = 0);                         //конструктор копирования

    TableSchema &       operator =          (TableSchema const & obj);      //перегрузка оператора =
    bool                operator ==         (const TableSchema & right);    //перегрузка оператора ==
    bool                operator !=         (const TableSchema & right);    //перегрузка оператора !=

    void                setTableName        (const QString & name);         //установить имя таблицы
    void                clear               ();                             //очистить схему
    void                generate            (const QString &table = "");    //сгенерировать схему

    static QStringList  getTables           (const QString & databaseName); //получить имена таблиц БД
    QStringList         getFields           (); //получить перечень полей
    QStringList         getPrimaryKeys      (); //получить перечень первичных ключей
    QStringList         getRelatedTables    (); //получить перечень связанных таблиц
    QString             getTableName        (); //получить имя таблицы
    QPair
    <QString, QString>  getRelation         (const QString & tableName);    //получить данные о конкретном отношении по его имени
    relation_t          getRelations        ();                             //получить все сгенерированные отношения

    bool                checkField          (const QString & fieldName);    //проверить наличие поля в таблице
    bool                checkTable          (const QString &tableName);     //проверить наличие таблицы в БД
    bool                checkRelation       (const QString & relationName); //проверить наличие связанной таблицы
signals:

public slots:
};

#endif // TABLESCHEMA_H
