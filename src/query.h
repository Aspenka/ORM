#ifndef QUERY_H
#define QUERY_H

#include <QObject>

class Query : public QObject
{
    Q_OBJECT
public:
    explicit Query(QObject *parent = 0);

signals:

public slots:
};

#endif // QUERY_H
