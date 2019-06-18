#ifndef PROCESSCONTROL_H
#define PROCESSCONTROL_H

#include <QtCore>
#include <QTimer>

#include "../modules/modbus.h"
#include "../modules/camerainterface.h"
#include "../tankitem.h"
#include "internalregisters.h"
#include <atomic>

class ProcessStates : public QObject {
    Q_OBJECT
public:
    enum class STATES {
        START,
        LOGIN,
        MANUAL_AUTO,
        CONTROL,
        CONTROL_FAILED,
        WAITING_BTN_START,
        DOWN_VALVE_BTN_PRESS,
        WAITING_BEFORE_START,
        WAITING_PRESSURE,
        PRESS_CYCLE,
        WAITING_BTNS,
        UP_VALVE,
        CONFIRM_STATE,
        STORE_ITEM,
        EMERGENCY,
        SECURITY,
        MANUAL_KEY,
        CONN_PROBLEM,
        END,
        LOGIN_OTHERDEV,
        CAMERROR
    };
    Q_ENUM(STATES)

};

class ProcessControl : public QThread
{
    Q_OBJECT

public:
    explicit ProcessControl(CameraInterface * const camera_module,
                            Modbus * const modbus, InternalRegisters * const internalregister,
                            QObject *parent = nullptr);

    virtual ~ProcessControl();

    void changeState(ProcessStates::STATES state);
    double getAnalog();
    double getAnalog2();
    int remainingTime();

private slots:
    void registerChanged(InternalRegisters::REGISTER reg, bool value);
    void registerModbusChanged(int addr, quint16 value);
    void registerBitChanged(int addr, quint8 bit, bool value);
    void errorModbusOccurred();


protected:
    void run();

private:
    Q_INVOKABLE void writeRegisterBit(int addr, quint8 bit, bool value);

    CameraInterface * const camera_module_;
    Modbus * const modbus_;
    InternalRegisters * const register_;

    ProcessStates::STATES  actual_state_;
    bool running_;
    QWaitCondition state_hanged;
    QMutex mutex;

    quint32 time1;
    double pressure1;
    double pressure2;
    QString last_picture_path_;
    bool default_position_;
};

#endif // PROCESSCONTROL_H
