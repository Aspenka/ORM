#include "model.h"

Model::Model(QString tableName, QObject *parent) : QObject(parent)
{
    Q_UNUSED(tableName);
}

Model::Model(const Model &obj, QObject)
{
    Q_UNUSED(obj);
}

Model &Model::operator =(const Model &obj)
{
    Q_UNUSED(obj);
    return *(this);
}

bool Model::operator ==(const Model &right)
{
    Q_UNUSED(right);
    return true;
}

bool Model::operator !=(const Model &right)
{
    Q_UNUSED(right);
    return true;
}

void Model::copy(const Model &obj)
{
    Q_UNUSED(obj);
}

void Model::setRecord(const QString &fieldName, const QVariant &value)
{
    Q_UNUSED(fieldName);
    Q_UNUSED(value);
}

void Model::setRelationData(const QString &relationName, const Model *relationModel)
{
    Q_UNUSED(relationName);
    Q_UNUSED(relationModel);
}

void Model::setExists(bool value)
{
    Q_UNUSED(value);
}

QString Model::getRecord(const QString &fieldName)
{
    Q_UNUSED(fieldName);
    return QString("");
}

Model *Model::getRelationData(const QString &relationName)
{
    Q_UNUSED(relationName);
    return this;
}

bool Model::isExists()
{
    return true;
}

bool Model::save()
{
    return true;
}

bool Model::remove()
{
    return true;
}

void Model::clear()
{

}

