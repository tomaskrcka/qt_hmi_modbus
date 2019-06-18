#include "mongodb.h"

extern "C"
{
#include <bson.h>
#include <mongoc.h>
}
#include <stdio.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

MongoDB::MongoDB()
{
    mongoc_init();
    connected = false;
}
MongoDB::~MongoDB()
{
    mongoc_cleanup ();
}


bool MongoDB::Connect(QString username, QString pwd, QString dbname,
                      QString address, int port) {
    /*
     * Create a new client instance
     */
    QString uri = QString("mongodb://%1:%2@%3:%4")
            .arg(username)
            .arg(pwd)
            .arg(address)
            .arg(port);
    if (username.length() == 0) {
        uri = QString("mongodb://%1:%2")
                    .arg(address)
                    .arg(port);
    }


    client = mongoc_client_new (uri.toLatin1().data());

    /*
     * Register the application name so we can track it in the profile logs
     * on the server. This can also be done from the URI (see other examples).
     */
    mongoc_client_set_appname (client, "mdrp");

    /*
     * Get a handle on the database "db_name" and collection "coll_name"
     */
    collection = mongoc_client_get_collection (client, dbname.toLatin1().data(), "man_pressing");
    coll_user = mongoc_client_get_collection (client, dbname.toLatin1().data(), "users");

    db_name_ = dbname;

    bson_t *command = BCON_NEW ("ping", BCON_INT32 (1));

    bool retval
            = mongoc_client_command_simple(client, dbname.toLatin1().data(),
                                           command, NULL, NULL, NULL);

    bson_destroy (command);

    connected = retval;

    return retval;
}

void MongoDB::Disconnect() {
    mongoc_collection_destroy (collection);
    mongoc_collection_destroy(coll_user);
    mongoc_client_destroy (client);
}

bool MongoDB::AlreadyProcessed(QString serial_number) {
    return true;
}

int MongoDB::UserExists(QString rfid_code){
    if (!connected)
        return -1;

    const bson_t *doc;
    bson_t *query;

    query = bson_new();

    qDebug() << "userexists check " << rfid_code;
    BSON_APPEND_UTF8(query, "rfid", rfid_code.toLatin1().data());
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (coll_user, query, NULL, NULL);

    int res = -1;
    if (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json (doc, NULL);
        QByteArray bb(str);
        qDebug() << "parse user";
        res = ParseUser(bb);
    }
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);

    return res;
}

int MongoDB::UserExistsHigherLevel(QString rfid_code) {
    if (!connected)
        return -1;

    const bson_t *doc;
    bson_t *query;

    query = bson_new();

    qDebug() << "userexists check " << rfid_code;
    BSON_APPEND_UTF8(query, "rfid", rfid_code.toLatin1().data());
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (coll_user, query, NULL, NULL);

    int res = -1;
    if (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json (doc, NULL);
        QByteArray bb(str);
        qDebug() << "parse user";
        res = ParseUser(bb, true);
    }
    bson_destroy (query);
    mongoc_cursor_destroy (cursor);

    return res;
}

int MongoDB::ParseUser(QByteArray json, bool check_admin) {
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject item = doc.object();

    if (item.isEmpty() || !item.contains("userid")) {
        qDebug() << "not contains userid";
        return -1;
    }

    int res = -1;
    bool ok;


    // check rights
    if (!item["rightsMAN"].toObject().contains("bath") || !item["rightsMAN"].toObject()["bath"].toBool(false)) {
        return -1;
    }

    if (check_admin) {
        if (!item["rightsKtm"].toObject().contains("admin") || !item["rightsKtm"].toObject().contains("servis")) {
            return -1;
        }

        if (!item["rightsKtm"].toObject()["admin"].toBool(false)
                || !item["rightsKtm"].toObject()["servis"].toBool(false)) {
            return -1;
        }
    }

    if (item["userid"].toObject().contains("$numberInt")) {
        res = item["userid"].toObject()["$numberInt"].toString().toInt(&ok);
    } else if (item["userid"].toObject().contains("$numberDouble")) {
        double tmp = item["userid"].toObject()["$numberDouble"].toString().toDouble(&ok);
        res = (int) tmp;
    }

    if (!ok) {
        qDebug() << "coversion problem " << item["userid"];
        return -1;
    }

    return res;
}

bool MongoDB::IsConnected() {
    bson_t *command = BCON_NEW ("ping", BCON_INT32 (1));

    bool retval
            = mongoc_client_command_simple(client, db_name_.toLatin1().data(),
                                           command, NULL, NULL, NULL);

    bson_destroy (command);

    connected = retval;

    return retval;
}

bool MongoDB::CreateTank(TankItem item) {
    bson_t *insert;
    bson_error_t error;

    if (!connected)
        return false;

    insert = BCON_NEW (
          "userid", BCON_INT32(item.user_id),
          "sn", BCON_UTF8 (item.sn.toLatin1().data()),
          "pressingTime", BCON_INT64(item.time_of_pressing),
          "endPressure", BCON_DOUBLE(item.end_pressure_),
          "ok", BCON_BOOL(item.ok),
          "picturePath", BCON_UTF8 (item.picture_path.toLatin1().data()),
          "dateLogin", BCON_DATE_TIME (item.user_logintime.toMSecsSinceEpoch()),
          "dateEndControl", BCON_DATE_TIME (item.end_of_control.toMSecsSinceEpoch()),
          "settings", "{",
            "tankType", BCON_UTF8 (item.tank_type.toLatin1().data()),
            "time1", BCON_INT32(item.time1),
            "time2", BCON_INT32(item.time2),
            "pressure", BCON_DOUBLE(item.pressure),
          "}");

    if (!mongoc_collection_insert_one (collection, insert, NULL, NULL, &error)) {
        return false;
    }

    return true;
}
