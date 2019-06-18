#ifndef STORAGEINTERFACE_H
#define STORAGEINTERFACE_H

#include <QString>
#include <QtCore>
#include <QtGlobal>

#include "../tankitem.h"

class StorageInterface {
public:
    virtual bool Connect(QString username, QString pwd, QString dbname,
                         QString address, int port) = 0;
    virtual void Disconnect() = 0;
    virtual bool AlreadyProcessed(QString serial_number) = 0;
    virtual int UserExists(QString rfid_code) = 0;
    virtual int UserExistsHigherLevel(QString rfid_code) = 0;
    virtual bool IsConnected() = 0;
    virtual bool CreateTank(TankItem item) = 0;
    virtual ~StorageInterface(){}
};

#endif // STORAGEINTERFACE_H
