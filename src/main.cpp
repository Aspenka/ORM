#include <QCoreApplication>
#include <QPair>

#include "query.h"
#include "model.h"
#include "globaldata.h"

Model *& fillModel(Model *& model);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Model *model = new Model("gateway_task");
    fillModel(model);
    if(!model->save())
    {
        return 0;
    }
    messages::printMessage(messages::RECORD_SAVED, QString("main"), QString("gateway_task"));
    if(!model->remove())
    {
        return 0;
    }
    messages::printMessage(messages::RECORD_REMOVED, QString("main"), QString("gateway_task"));
    return 0;
}

Model *& fillModel(Model *&model)
{
    model->setRecord("gateway_task_uid", "rtyhh65");
    model->setRecord("cron", "3,4,5 */2 * * * *");
    model->setRecord("gateway_uid", "545");
    model->setRecord("status", 0);
    return model;
}
