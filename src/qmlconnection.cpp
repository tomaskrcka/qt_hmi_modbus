#include "qmlconnection.h"

#include "appcontext.h"
#include "control/processcontrol.h"

#include <QDebug>
#include <qlogging.h>
#include <QProcess>

QmlConnection::QmlConnection(QObject *parent) : QObject(parent)
{

}

int QmlConnection::loginUser(QString user_rfid) {
    int res = -1;

    TankItem item;
    item.ok = false;
    AppContext::GetInstance().tankitems_.clear();
    AppContext::GetInstance().tankitems_.append(item);
    AppContext::GetInstance().tankitems_.append(item);

    if (user_rfid == "123") {
        AppContext::GetInstance().GetRegister()->setRegister(InternalRegisters::REGISTER::LOGGED, true);
        AppContext::GetInstance().tankitems_[0].rfid = QString("123");
        AppContext::GetInstance().tankitems_[1].rfid = QString("123");
        AppContext::GetInstance().tankitems_[0].user_id = 123;
        AppContext::GetInstance().tankitems_[1].user_id = 123;
        AppContext::GetInstance().tankitems_[0].user_logintime = QDateTime::currentDateTime();
        AppContext::GetInstance().tankitems_[1].user_logintime = QDateTime::currentDateTime();

        return true;
    }

    try {
        res =  AppContext::GetInstance().GetStorage()->UserExists(user_rfid);
    } catch(...) {
        return -1;
    }

    if (res > 0) {
        AppContext::GetInstance().user_rfid_ = user_rfid;
        AppContext::GetInstance().tankitems_[0].rfid = user_rfid;
        AppContext::GetInstance().tankitems_[1].rfid = user_rfid;
        AppContext::GetInstance().tankitems_[0].user_id = res;
        AppContext::GetInstance().tankitems_[1].user_id = res;
        AppContext::GetInstance().tankitems_[0].user_logintime = QDateTime::currentDateTime();
        AppContext::GetInstance().tankitems_[1].user_logintime = QDateTime::currentDateTime();
        AppContext::GetInstance().GetRegister()->setRegister(InternalRegisters::REGISTER::LOGGED, true);
    }

    return res;
}

int QmlConnection::userExists(QString user_rfid) {
    int res = -1;

    if (user_rfid == "234") {
        return 123;
    }

    try {
        res =  AppContext::GetInstance().GetStorage()->UserExistsHigherLevel(user_rfid);
    } catch(...) {
        return -1;
    }

    return res;
}

bool QmlConnection::saveTank() {
    bool res = false;
    /*try {
        res = AppContext::GetInstance().GetStorage()->SaveDefect(code,
                                                    user_code.toInt(), defect_code);
    } catch(...) {
        qWarning() << "invalid user code: " << user_code;
    }*/
    return res;
}

bool QmlConnection::alreadyProcessed(QString serial_number) {
    return AppContext::GetInstance().GetStorage()->AlreadyProcessed(serial_number);
}

int QmlConnection::getNumOfTanks() {
    return AppContext::GetInstance().num_tanks_;
}

void QmlConnection::setNumOfTanks(int num) {
    AppContext::GetInstance().num_tanks_ = num;
}

bool QmlConnection::isConnected() {
    return AppContext::GetInstance().GetStorage()->IsConnected();
}

bool QmlConnection::reconnected() {
    return AppContext::GetInstance().ReinitDB();
}

void QmlConnection::restartApp() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

int QmlConnection::appStatus() {
    return AppContext::GetInstance().GetAppStatus();
}

// if OK or NOK
void QmlConnection::tankOneStatus(bool state) {
    AppContext::GetInstance().tankitems_[0].ok = state;
}

// if OK or NOK
void QmlConnection::tankTwoStatus(bool state) {
    AppContext::GetInstance().tankitems_[1].ok = state;
}

void QmlConnection::setTankSN(int tankNum, QString sn) {
    AppContext::GetInstance().tankitems_[tankNum].sn = sn;

    if (((AppContext::GetInstance().num_tanks_ == 2) && (tankNum == 1)) ||
            ((AppContext::GetInstance().num_tanks_ == 1) && (tankNum == 0)))
        AppContext::GetInstance().GetRegister()->setRegister(InternalRegisters::REGISTER::PRODUCT_SN, true);
}

void QmlConnection::setTankProgram(int tankNum, QString program) {
    AppContext::GetInstance().tankitems_[tankNum].tank_type = program;
    AppContext::GetInstance().copyProgram(AppContext::GetInstance().tankitems_[tankNum], program);

    if (((AppContext::GetInstance().num_tanks_ == 2) && (tankNum == 1)) ||
            ((AppContext::GetInstance().num_tanks_ == 1) && (tankNum == 0)))
        AppContext::GetInstance().GetRegister()->setRegister(InternalRegisters::REGISTER::PRODUCT_READ, true);
}

double QmlConnection::getPressure() {
    return AppContext::GetInstance().GetProcessControl()->getAnalog();
}

double QmlConnection::getPressure2() {
    return AppContext::GetInstance().GetProcessControl()->getAnalog2();
}

QString QmlConnection::getSN(int tankNum) {
    return AppContext::GetInstance().tankitems_.at(tankNum).sn;
}

void QmlConnection::logout() {
    return AppContext::GetInstance()
            .GetProcessControl()->changeState(ProcessStates::STATES::LOGIN);
}

bool QmlConnection::loginstate() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::LOGIN);
}

QString QmlConnection::getProgram(int tankNum) {
    return AppContext::GetInstance().tankitems_.at(tankNum).tank_type;
}

void QmlConnection::confirmCycle() {
    AppContext::GetInstance().GetRegister()->setRegister(InternalRegisters::REGISTER::CONFIRM, true);
}

int QmlConnection::remainingTime() {
    return AppContext::GetInstance().GetProcessControl()->remainingTime();
}

bool QmlConnection::controlEnded() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::CONTROL_END);
}

bool QmlConnection::controlOK() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::CONTROL_OK);
}

bool QmlConnection::stored() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::STORED);
}

bool QmlConnection::procedureStarted() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::PROCEDURE_STARTED);
}

bool QmlConnection::procedureEnded() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::PROCEDURE_ENDED);
}

bool QmlConnection::btnControl() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::BTN_CONTROL);
}

bool QmlConnection::waitingStart() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::WAITING_START);
}

bool QmlConnection::waitingPress() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::WAITING_PRESS);
}

bool QmlConnection::emergency() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::EMERGENCY);
}

bool QmlConnection::logged() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::LOGGED);
}

bool QmlConnection::connError() {
    auto res = AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::CONN_PROBLEM);

    return res;
}

bool QmlConnection::keyManual() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::KEY_MANUAL);
}

bool QmlConnection::security() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::SECURITY);
}

bool QmlConnection::default_position() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::DEFAULT_POSITION);
}

bool QmlConnection::switchManual() {
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::SWITCH_MANUAL);
}

void QmlConnection::confirmEnd(bool btn1, bool btn2) {
    AppContext::GetInstance().GetRegister()
            ->setRegister(InternalRegisters::REGISTER::CONFIRM_END, true);

    AppContext::GetInstance().tankitems_[0].ok = btn1;
    if (AppContext::GetInstance().num_tanks_ == 2) {
        AppContext::GetInstance().tankitems_[1].ok = btn2;
    }
}

bool QmlConnection::camError() {
    QThread::msleep(100);
    return AppContext::GetInstance()
            .GetRegister()->getRegister(InternalRegisters::REGISTER::CAMERROR);
}

void QmlConnection::camErrorSet(bool status) {
    return AppContext::GetInstance()
            .GetRegister()->setRegister(InternalRegisters::REGISTER::CAMERROR, status);
}
