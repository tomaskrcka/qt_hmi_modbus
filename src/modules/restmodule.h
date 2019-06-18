#ifndef RESTMODULE_H
#define RESTMODULE_H

#include <QString>
#include <QObject>
#include <QByteArray>

#include "../http/httprestclient.h"
#include "../tankitem.h"

class RestModule : public QObject
{
    Q_OBJECT
public:
    explicit RestModule(HttpRestClient * const httpclient);

    virtual int UserExists(QString rfid_code);
    virtual int UserExistsHigherLevel(QString rfid_code);
    virtual bool CreateTank(TankItem item);
    // camera take picture ??

private:
    HttpRestClient * const httpclient_;
};

#endif // RESTMODULE_H
