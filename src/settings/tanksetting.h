#ifndef TANKSETTING_H
#define TANKSETTING_H

#include <QtCore>

class TankSetting {
public:
    QString tank_type;
    // pressing time 1 (seconds)
    quint32 time1;
    // pressing time 2 (seconds)
    quint32 time2;
    double pressure;

};

#endif // TANKSETTING_H
