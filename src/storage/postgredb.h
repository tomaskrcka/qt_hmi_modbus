#ifndef POSTGREDB_H
#define POSTGREDB_H

#include <QSqlDatabase>

#include "storageinterface.h"
#include "../tankitem.h"

class PostgreDB : public StorageInterface
{
public:
    PostgreDB();
    virtual bool Connect(QString username, QString pwd, QString dbname,
                         QString address, int port);
    virtual void Disconnect();
    virtual bool AlreadyProcessed(QString serial_number);
    virtual int UserExists(QString rfid_code);
    virtual int UserExistsHigherLevel(QString rfid_code) {return -1;}
    virtual bool IsConnected();
    virtual ~PostgreDB();

    virtual bool CreateTank(TankItem item) {return true;}

private:
    qint64 CreateTank(long user_code, QString sn, QDateTime start_time,
                                long pressing_time, long pressure, bool isOK);
};

#endif // POSTGREDB_H
