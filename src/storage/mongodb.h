#ifndef MONGODB_H
#define MONGODB_H

#include "storageinterface.h"
#include "../tankitem.h"

extern "C"
{
#include <bson.h>
#include <mongoc.h>
}

class MongoDB: public StorageInterface
{
public:
    MongoDB();
    virtual ~MongoDB();
    virtual bool Connect(QString username, QString pwd, QString dbname,
                         QString address, int port);
    virtual void Disconnect();
    virtual bool AlreadyProcessed(QString serial_number);
    virtual int UserExists(QString rfid_code);
    virtual int UserExistsHigherLevel(QString rfid_code);
    virtual bool IsConnected();

    virtual bool CreateTank(TankItem item);
private:
    int ParseUser(QByteArray json, bool check_admin = false);

    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_collection_t *coll_user;
    QString db_name_;
    bool connected;
};

#endif // MONGODB_H
