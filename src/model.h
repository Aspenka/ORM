#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMap>
#include <QString>
#include "tableschema.h"

/*========================================================================
 Класс представляет собой модель одной записи из таблицы БД.
 Поля класса содержат следующий набор информации:
 - схему данных, соответвтвующую текущей записи;
 - перечень значений полей записи;
 - перечень данных по связям с другими таблицами;
 - информация о существовании данной записи в таблице БД.

 Методы класса позволяют выполнять следующший набор действий:
 - устанавливать значения записи в соответствии с полем таблицы БД;
 - устанавливать связи с другими таблицами;
 - устанавливать флаг существования записи в таблице БД;
 - получать значение записи по соответствующему полю;
 - получать записи, связанные с текущей по имени связи;
 - определять существование текущей записи в БД;
 - сохранять текущую запись в БД;
 - удалять текущую запись из БД;
 - очищать объект класса от всех данных.
========================================================================*/
class Model : public QObject
{
    Q_OBJECT
private:
    TableSchema                 schema;         //схема данных
    QMap <QString, QVariant>    record;         //имя поля и его значение
    QMap <QString, Model *>     relationData;   //данные по связям с другими таблицами
    bool                        exists = false; //флаг существования данной записи в БД

    QString     generateInsert  (); //генерация запроса на добавление новой записи
    QString     generateUpdate  (); //генерация запроса на обновление текущей записи
    QString     generateWhere   (); //генерация условия для выполнения запроса

    void        copy            (const Model & obj);    //вспомогательный метод, копирующий данные в текущий обхект
    bool        checkExistence  ();                     //проверка существования записи в БД
    bool        execQuery       (const QString & sql);  //выполнение SQL-запроса
public:
    explicit    Model           (QString tableName, QObject *parent = 0);   //конструктор по умолчанию
                Model           (const Model & obj, QObject);               //конструктор копирования
                ~Model          ();                                         //деструктор

    Model &     operator =      (Model const & obj);    //перегрузка оператора =
    bool        operator ==     (const Model & right);  //перегрузка оператора ==
    bool        operator !=     (const Model & right);  //перегрузка оператора !=

    void        setRecord       (const QString & fieldName, const QVariant & value);            //установка значения поля по его имени
    void        setRelationData (const QString & relationName, Model *relationModel);           //установка связи
    void        setExists       (bool value);                                                   //установка флага существования записи в БД
    void        setSchema       (const QString & tableName);                                    //установка нвой схемы данных

    QVariant    getRecord       (const QString & fieldName);    //получение записи по имени ее поля
    Model *     getRelationData (const QString & relationName); //получение связанной модели
    bool        isExists        ();                             //получение флага существования записи в БД

    bool        save            (); //сохранение записи в БД
    bool        remove          (); //удаление записи из БД
    void        clear           (); //очистка модели
signals:

public slots:
};

#endif // MODEL_H
