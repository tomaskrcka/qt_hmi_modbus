#ifndef QMLCONNECTION_H
#define QMLCONNECTION_H

#include <QObject>

#include "control/processcontrol.h"

class QmlConnection : public QObject
{
    Q_OBJECT
public:
    explicit QmlConnection(QObject *parent = nullptr);

    Q_INVOKABLE bool isConnected();
    Q_INVOKABLE bool reconnected();
    Q_INVOKABLE int loginUser(QString user_rfid);
    Q_INVOKABLE int userExists(QString user_rfid);
    Q_INVOKABLE bool loginstate();
    Q_INVOKABLE bool saveTank();
    Q_INVOKABLE bool alreadyProcessed(QString serial_number);
    Q_INVOKABLE void restartApp();
    Q_INVOKABLE int appStatus();
    Q_INVOKABLE int getNumOfTanks();
    Q_INVOKABLE void setNumOfTanks(int num);
    Q_INVOKABLE void tankOneStatus(bool state);
    Q_INVOKABLE void tankTwoStatus(bool state);

    Q_INVOKABLE void setTankSN(int tankNum, QString sn);
    Q_INVOKABLE void setTankProgram(int tankNum, QString program);

    Q_INVOKABLE double getPressure();
    Q_INVOKABLE double getPressure2();
    Q_INVOKABLE QString getSN(int tankNum);
    Q_INVOKABLE QString getProgram(int tankNum);
    Q_INVOKABLE void confirmCycle();
    Q_INVOKABLE int remainingTime();

    Q_INVOKABLE bool controlEnded();
    Q_INVOKABLE bool controlOK();
    Q_INVOKABLE bool procedureStarted();
    Q_INVOKABLE bool procedureEnded();
    Q_INVOKABLE void confirmEnd(bool btn1, bool btn2);
    Q_INVOKABLE bool btnControl();
    Q_INVOKABLE bool waitingStart();
    Q_INVOKABLE bool waitingPress();
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool emergency();
    Q_INVOKABLE bool default_position();
    Q_INVOKABLE bool stored();
    Q_INVOKABLE bool logged();
    Q_INVOKABLE bool connError();
    Q_INVOKABLE bool keyManual();
    Q_INVOKABLE bool security();
    Q_INVOKABLE bool switchManual();
    Q_INVOKABLE bool camError();
    Q_INVOKABLE void camErrorSet(bool status);

signals:

public slots:
};

#endif // QMLCONNECTION_H
