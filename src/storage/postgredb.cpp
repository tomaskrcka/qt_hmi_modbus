#include "postgredb.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <qlogging.h>

constexpr auto DBNAME = "ELKAMET";


PostgreDB::PostgreDB()
{
}

PostgreDB::~PostgreDB() {
}

bool PostgreDB::Connect(QString username, QString pwd, QString dbname,
                   QString address, int port) {

    QSqlDatabase db = QSqlDatabase::database(DBNAME);
    if (db.isValid()) {
        QSqlDatabase::removeDatabase(db.connectionName());
        db.close();
    }
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setConnectOptions("connect_timeout=5");
    db.setDatabaseName(DBNAME);
    db.setHostName(address);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(pwd);
    db.setPort(port);
    bool ok = db.open();

    if (ok)
    {
        qDebug() << "OK";
    } else {
        qWarning() << "NOK";
        qDebug() << db.lastError().text();
    }

    return ok;
}

bool PostgreDB::IsConnected() {
    QSqlDatabase db = QSqlDatabase::database (DBNAME);

    return db.isValid() && db.isOpen();
}

void PostgreDB::Disconnect() {
    QSqlDatabase db = QSqlDatabase::database (DBNAME);
    db.close();
}

bool PostgreDB::AlreadyProcessed(QString serial_number) {
    QString q_str;
    q_str = QString("SELECT * FROM \"tank\" where \"serialNumber\" ilike \"%1\" and \"isOK\"=true")
            .arg(serial_number);
    QSqlQuery query(q_str);

    bool res = false;
    while (query.next())
    {
        res = true;
    }

    return res;
}

int PostgreDB::UserExists(QString rfid_code) {
    QString q_str;
    q_str = QString("SELECT * FROM \"user\" where \"rfid\" ilike \"%1\"")
            .arg(rfid_code);
    QSqlQuery query(q_str);

    int res = -1;
    while (query.next())
    {
        res = 1;
    }

    return res;
}

// tank - id, SN, user, whole time of pressing, pressure on the beginning, date of the end, OK/NOK
qint64 PostgreDB::CreateTank(long user_code, QString sn, QDateTime start_time,
                             long pressing_time, long pressure, bool isOK) {
    QSqlQuery query;

    query.prepare("INSERT INTO tank (\"serialNumber\", \"userId\", \"pressingTime\", \"pressure\", \"dateStart\", \"dateEnd\", \"isOK\") "
                  "VALUES (:serialNumber, :userId, :pressingTime, :pressure, :dateStart, CURRENT_TIMESTAMP, :isOK)");

    query.bindValue(":serialNumber", sn);
    query.bindValue(":userId", QVariant((qlonglong) user_code));
    query.bindValue(":pressingTime", QVariant((qlonglong) pressing_time));
    query.bindValue(":pressure", QVariant((qlonglong) pressure));
    query.bindValue(":dateStart", start_time);
    query.bindValue(":isOK", isOK);
    query.exec();

    QVariant res = query.lastInsertId();

    qWarning() << query.lastError();
    if (!res.isValid()) {
        qWarning() << query.lastError();
        return -1;
    }

    return res.toLongLong();
}
