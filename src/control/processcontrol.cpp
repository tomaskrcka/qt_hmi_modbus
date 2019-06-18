#include "processcontrol.h"

#include <QtCore>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <QCoreApplication>

#include "../appcontext.h"
#include "../settings/constants.h"


ProcessControl::ProcessControl(CameraInterface * const camera_module,
                            Modbus * const modbus, InternalRegisters * const internalregister,
                            QObject *parent) :
        QThread(parent), camera_module_(camera_module), modbus_(modbus),
        register_(internalregister)
{
    actual_state_ = ProcessStates::STATES::START;
    running_ = true;
    default_position_ = false;
    pressure1 = 0.0;
    pressure2 = 0.0;
    //manual_filter_ = manual_lights_ = false;
    last_picture_path_ = "";
    //tmp_map_.clear();
    connect(register_, SIGNAL(registerChanged(InternalRegisters::REGISTER,bool)),
                         this, SLOT(registerChanged(InternalRegisters::REGISTER,bool)));
    // connect(timer_, SIGNAL(timeout()), this, SLOT(timeout()));

    connect(modbus_, SIGNAL(registerChanged(int,quint16)), this, SLOT(registerModbusChanged(int,quint16)));
    connect(modbus_, SIGNAL(registerBitChanged(int,quint8,bool)),
            this, SLOT(registerBitChanged(int,quint8,bool)));

    connect(modbus_, SIGNAL(errorModbusOccurred()),
            this, SLOT(errorModbusOccurred()));

    // timer_->start(1000);
    modbus->addRegisterCheck(RegistersRead::oiPressure40002);
    modbus->addRegisterCheck(RegistersRead::oiPressure40005);
    modbus->addRegisterCheck(RegistersRead::oiTime1_40003);
    modbus->addRegisterCheck(RegistersRead::oiTime2_40004);

    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxDefaultPosition);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxPressingInProgress);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxPressingNOK);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxPressingOK);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxSecurity);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxService);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxTakePicture);
    modbus->addBitRegisterCheck(RegistersRead::oiRegister40001,
                                RegisterRead40001_Bits::oxPushBtns);
}

ProcessControl::~ProcessControl() {
    changeState(ProcessStates::STATES::END);
    this->wait();
}

void ProcessControl::errorModbusOccurred() {
    qInfo() << "errorModbusOccurred";
    register_->setRegister(InternalRegisters::REGISTER::CONN_PROBLEM, true);
    changeState(ProcessStates::STATES::CONN_PROBLEM);
}

void ProcessControl::registerModbusChanged(int addr, quint16 value) {
    if (addr == RegistersRead::oiPressure40002) {
        pressure1 = value;
    }

    if (addr == RegistersRead::oiPressure40005) {
        pressure2 = value;
    }

    if (addr == RegistersRead::oiTime1_40003) {
        time1 = value;
    }

    if (addr == RegistersRead::oiTime2_40004) {
        time1 = value;
    }
}

void ProcessControl::registerBitChanged(int addr, quint8 bit, bool value) {
    qInfo() << "addr: " << addr << "bit: " << bit << " value: " << value;

    if (addr == RegistersRead::oiRegister40001) {
        switch (bit) {
        case RegisterRead40001_Bits::oxSecurity:
            if ((actual_state_ != ProcessStates::STATES::EMERGENCY) && (value)) {
               changeState(ProcessStates::STATES::EMERGENCY);
            } else if ((actual_state_ == ProcessStates::STATES::EMERGENCY) && (!value)) {
               changeState(ProcessStates::STATES::LOGIN);
            }
            break;
        case RegisterRead40001_Bits::oxPressingInProgress:
            /*
            if ((!value) && (register_->getRegister(InternalRegisters::REGISTER::PROCEDURE_STARTED)))
                register_->setRegister(InternalRegisters::REGISTER::BTN_CONTROL, true);
            */
            // register_->setRegister(InternalRegisters::REGISTER::PROCEDURE_STARTED, value);

            if ((actual_state_ == ProcessStates::STATES::WAITING_BTN_START) && (value)) {
                changeState(ProcessStates::STATES::PRESS_CYCLE);
            }

            break;
        case RegisterRead40001_Bits::oxPushBtns:
            // register_->setRegister(InternalRegisters::REGISTER::BTN_CONTROL, value);
            if ((actual_state_ == ProcessStates::STATES::PRESS_CYCLE) && (value)){
                changeState(ProcessStates::STATES::WAITING_BTNS);
            }
            break;
        case RegisterRead40001_Bits::oxPressingOK:
            // register_->setRegister(InternalRegisters::REGISTER::PROCEDURE_ENDED, value);
            if ((actual_state_ == ProcessStates::STATES::WAITING_BTNS) && (value)) {
                changeState(ProcessStates::STATES::CONFIRM_STATE);
            }
            break;
        case RegisterRead40001_Bits::oxTakePicture:
            if (value && AppContext::GetInstance().tankitems_.length() > 0) {
               QString pict = QString("%1_%2").arg(AppContext::GetInstance().tankitems_[0].tank_type)
                    .arg(AppContext::GetInstance().tankitems_[0].sn);
                QString resp = camera_module_->TakeSnapshot(pict);
                AppContext::GetInstance().tankitems_[0].picture_path = resp;
                last_picture_path_ = resp;
                if ((AppContext::GetInstance().num_tanks_ == 2)
                        && (AppContext::GetInstance().tankitems_.length() > 1)) {
                    QString pict = QString("%1_%2").arg(AppContext::GetInstance().tankitems_[1].tank_type)
                        .arg(AppContext::GetInstance().tankitems_[1].sn);
                    QString resp = camera_module_->TakeSnapshot(pict);
                    AppContext::GetInstance().tankitems_[1].picture_path = resp;
                }
            }
            break;
        case RegisterRead40001_Bits::oxService:
            if (value)
                changeState(ProcessStates::STATES::MANUAL_KEY);
            else if ((actual_state_ == ProcessStates::STATES::MANUAL_KEY) && (!value)) {
                changeState(ProcessStates::STATES::LOGIN);
            }
            break;
        case RegisterRead40001_Bits::oxManualBtn:
            if (value)
                changeState(ProcessStates::STATES::MANUAL_KEY);
            else if ((actual_state_ == ProcessStates::STATES::MANUAL_KEY) && (!value)) {
                changeState(ProcessStates::STATES::LOGIN);
            }
            break;
        case RegisterRead40001_Bits::oxDefaultPosition:
            // default position
            register_->setRegister(InternalRegisters::REGISTER::DEFAULT_POSITION, value);
            break;
        default:
            break;
        }
    }
}

double ProcessControl::getAnalog() {
    return pressure1;
}

double ProcessControl::getAnalog2() {
    return pressure2;
}

int ProcessControl::remainingTime() {
    return time1;
}

void ProcessControl::registerChanged(InternalRegisters::REGISTER reg, bool value) {
    if ((actual_state_ == ProcessStates::STATES::START)
            && (reg == InternalRegisters::REGISTER::INIT_OK) && (value)) {
       changeState(ProcessStates::STATES::LOGIN);
    }

    if ((actual_state_ == ProcessStates::STATES::LOGIN)
            && (reg == InternalRegisters::REGISTER::LOGGED) && (value)) {
       changeState(ProcessStates::STATES::MANUAL_AUTO);
    }

    if ((actual_state_ == ProcessStates::STATES::CAMERROR)
            && (reg == InternalRegisters::REGISTER::CAMERROR) && (!value)) {
       changeState(ProcessStates::STATES::LOGIN);
    }

    if ((actual_state_ == ProcessStates::STATES::MANUAL_AUTO)
            && (reg == InternalRegisters::REGISTER::PRODUCT_READ) && (value)) {
       changeState(ProcessStates::STATES::CONTROL);
    }

    if ((actual_state_ == ProcessStates::STATES::CONTROL)
            && (reg == InternalRegisters::REGISTER::CONFIRM) && (value)) {
       changeState(ProcessStates::STATES::WAITING_BTN_START);
    }

    if ((actual_state_ == ProcessStates::STATES::CONFIRM_STATE)
            && (reg == InternalRegisters::REGISTER::CONFIRM_END) && (value)) {
       changeState(ProcessStates::STATES::STORE_ITEM);
    }

    if ((actual_state_ != ProcessStates::STATES::CONN_PROBLEM) &&
            (reg == InternalRegisters::REGISTER::CONN_PROBLEM) && (value)) {
       changeState(ProcessStates::STATES::CONN_PROBLEM);
    }

    /*if ((actual_state_ != ProcessStates::STATES::LOGIN) && (reg == InternalRegisters::REGISTER::LOGIN)
            && value) {
        changeState(ProcessStates::STATES::LOGIN);
    }*/

    // emergency -- nouzove vyjeti kdyz probiha proces
    // security -- zastaveni
}

void ProcessControl::writeRegisterBit(int addr, quint8 bit, bool value) {
    modbus_->writeRegisterBit(addr, bit, value);
    // register_->setRegister(InternalRegisters::REGISTER::CONN_PROBLEM, !res);
}

void ProcessControl::changeState(ProcessStates::STATES state) {
    if (state == actual_state_) {
        return;
    }

    mutex.lock();
    qInfo() << "Change state: "<< static_cast<int>(state);
    actual_state_ = state;
    state_hanged.wakeAll();
    mutex.unlock();
}

void ProcessControl::run() {
    ProcessStates::STATES processed = ProcessStates::STATES::START;
    QTime start_time;

    sleep(3);
    if (register_->getRegister(InternalRegisters::REGISTER::INIT_OK)
            && actual_state_ == ProcessStates::STATES::START) {
        changeState(ProcessStates::STATES::LOGIN);
    }

    while (running_) {
        mutex.lock();
        if (processed == actual_state_) {
            state_hanged.wait(&mutex);
        }
        processed = actual_state_;
        mutex.unlock();

        if (processed != ProcessStates::STATES::LOGIN) {
            register_->setRegister(InternalRegisters::REGISTER::LOGIN, false);
            QMetaObject::invokeMethod(this, "writeRegisterBit",
                  Qt::QueuedConnection, Q_ARG(int, iiRegister40006),
                  Q_ARG(quint8, RegisterWrite40006_Bits::ixLoginScreen), Q_ARG(bool, false));
        }

        if (processed == ProcessStates::STATES::LOGIN) {
            qInfo() << "state login";
            QMetaObject::invokeMethod(this, "writeRegisterBit",
                  Qt::QueuedConnection, Q_ARG(int, iiRegister40006),
                  Q_ARG(quint8, RegisterWrite40006_Bits::ixReady), Q_ARG(bool, false));
            register_->setDefault();
            register_->setRegister(InternalRegisters::REGISTER::LOGIN, true);
            time1 = 0;
            if (!camera_module_->IsLastPictureOK()) {
                qInfo() << "cam error";
                register_->setRegister(InternalRegisters::REGISTER::CAMERROR, true);
            } else {
                QMetaObject::invokeMethod(this, "writeRegisterBit",
                      Qt::QueuedConnection, Q_ARG(int, iiRegister40006),
                      Q_ARG(quint8, RegisterWrite40006_Bits::ixLoginScreen), Q_ARG(bool, true));
            }
            camera_module_->ClearLastPicture();
        } else if (processed == ProcessStates::STATES::MANUAL_AUTO) {
            // ???
            qInfo() << "state manual/auto";
            start_time.setHMS(0, 0, 0);
        } else if (processed == ProcessStates::STATES::CONTROL) {
            qInfo() << "state control";
            // waiting for the pressure ??
            register_->setRegister(InternalRegisters::REGISTER::CONTROL_OK, true);
            register_->setRegister(InternalRegisters::REGISTER::CONTROL_END, true);
        } else if (processed == ProcessStates::STATES::CONTROL_FAILED) {
            qInfo() << "state control failed";
            // TBD
            register_->setRegister(InternalRegisters::REGISTER::CONTROL_OK, false);
            register_->setRegister(InternalRegisters::REGISTER::CONTROL_END, true);
        } else if (processed == ProcessStates::STATES::WAITING_BTN_START) {
            qInfo() << "state waiting btn1";
            QMetaObject::invokeMethod(this, "writeRegisterBit",
                  Qt::QueuedConnection, Q_ARG(int, iiRegister40006),
                  Q_ARG(quint8, RegisterWrite40006_Bits::ixReady), Q_ARG(bool, true));

        } else if (processed == ProcessStates::STATES::PRESS_CYCLE) {
            qInfo() << "Press cycle";
        } else if (processed == ProcessStates::STATES::WAITING_BTNS) {
            qInfo() << "state waiting btns";
            register_->setRegister(InternalRegisters::REGISTER::BTN_CONTROL, true);
            AppContext::GetInstance().tankitems_[0].end_pressure_ = getAnalog();
            if (AppContext::GetInstance().num_tanks_ == 2)
                AppContext::GetInstance().tankitems_[1].end_pressure_
                        = getAnalog();
        } else if (processed == ProcessStates::STATES::CONFIRM_STATE) {
            qInfo() << "confirm state ";
            register_->setRegister(InternalRegisters::REGISTER::PROCEDURE_ENDED, true);
            AppContext::GetInstance().tankitems_[0].end_of_control = QDateTime::currentDateTime();
            AppContext::GetInstance().tankitems_[0].time_of_pressing = start_time.elapsed();
            if (AppContext::GetInstance().num_tanks_ == 2) {
                AppContext::GetInstance().tankitems_[1].time_of_pressing = start_time.elapsed();
                AppContext::GetInstance().tankitems_[1].end_of_control = QDateTime::currentDateTime();
            }
            QMetaObject::invokeMethod(this, "writeRegisterBit",
                  Qt::QueuedConnection, Q_ARG(int, iiRegister40006),
                  Q_ARG(quint8, RegisterWrite40006_Bits::ixReady), Q_ARG(bool, false));
        } else if (processed == ProcessStates::STATES::STORE_ITEM) {
            qInfo() << "state storeitem";

            qInfo() << AppContext::GetInstance().tankitems_[0].end_of_control;
            qInfo() << AppContext::GetInstance().tankitems_[0].end_pressure_;
            qInfo() << AppContext::GetInstance().tankitems_[0].ok;
            qInfo() << AppContext::GetInstance().tankitems_[0].pressure;
            qInfo() << AppContext::GetInstance().tankitems_[0].sn;
            qInfo() << AppContext::GetInstance().tankitems_[0].tank_type;
            qInfo() << AppContext::GetInstance().tankitems_[0].time_of_pressing;
            qInfo() << AppContext::GetInstance().tankitems_[0].rfid;

            AppContext::GetInstance().GetStorage()->CreateTank(AppContext::GetInstance().tankitems_[0]);
            if (AppContext::GetInstance().num_tanks_ == 2) {
                AppContext::GetInstance().GetStorage()->CreateTank(AppContext::GetInstance().tankitems_[1]);
            }
            register_->setRegister(InternalRegisters::REGISTER::STORED, true);

            // must be here to inform gui
           // sleep(1);
            //changeState(ProcessStates::STATES::LOGIN);
        } else if (processed == ProcessStates::STATES::MANUAL_KEY) {
            qInfo() << "manual key";
            register_->setRegister(InternalRegisters::REGISTER::KEY_MANUAL, true);
        } else if (processed == ProcessStates::STATES::EMERGENCY) {
            qInfo() << "state emergency";
            register_->setRegister(InternalRegisters::REGISTER::EMERGENCY, true);
        } else if (processed == ProcessStates::STATES::SECURITY) {
            qInfo() << "state security";
            register_->setRegister(InternalRegisters::REGISTER::SECURITY, true);
        } else if (processed == ProcessStates::STATES::CONN_PROBLEM) {
            qInfo() << "communication problem";
            register_->setRegister(InternalRegisters::REGISTER::CONN_PROBLEM, true);
        } else if (processed == ProcessStates::STATES::END) {
            running_ = false;
            break;
        }
    }
}

