#ifndef TANKITEM_H
#define TANKITEM_H

#include <QtCore>
#include <QDateTime>

class TankItem {
public:

    QString tank_type;
    // pressing time 1 (seconds)
    quint32 time1;
    // pressing time 2 (seconds)
    quint32 time2;

    double pressure;

    // serial number
    QString sn;
    // user code
    QString rfid;
    // path to the picture from camera
    QString picture_path;
    int user_id;
    QDateTime user_logintime;
    // Celkový čas cyklu tlakování
    quint64 time_of_pressing;
    // Tlak v době ukončení první části tlakování.
    double end_pressure_;
    // Čas a datum ukončení cyklu kontroly
    QDateTime end_of_control;
    bool ok;
};

#endif // TANKITEM_H

