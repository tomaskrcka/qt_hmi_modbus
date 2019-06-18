#include "internalregisters.h"

#include <qlogging.h>
#include <QDebug>

/*
 * Holding the internal state of the application
 */

InternalRegisters::InternalRegisters(QObject *parent) : QObject(parent)
{
    setDefault();
}

void InternalRegisters::setRegister(REGISTER reg, bool value) {
    bool e_mit = true;
    if (reg == REGISTER::LOGGED) {
        logged_ = value;
    } else if (reg == REGISTER::PRODUCT_READ) {
        product_read_ = value;
    } else if (reg == REGISTER::PRODUCT_SN) {
        product_sn_ = value;
    } else if (reg == REGISTER::CONFIRM) {
        confirm_ = value;
    } else if (reg == REGISTER::EMERGENCY) {
        emergency_ = value;
    } else if (reg == REGISTER::INIT_OK) {
        init_ok_ = value;
    } else if (reg == REGISTER::CONTROL_END) {
        control_end_ = value;
    } else if (reg == REGISTER::CONTROL_OK) {
        control_ok_ = value;
    } else if (reg == REGISTER::PROCEDURE_STARTED) {
        procedure_started_ = value;
    } else if (reg == REGISTER::PROCEDURE_ENDED) {
        procedure_end_ = value;
    } else if (reg == REGISTER::BTN_CONTROL) {
        btn_control_ = value;
    } else if (reg == REGISTER::WAITING_START) {
        waiting_start_ = value;
    } else if (reg == REGISTER::WAITING_PRESS) {
        waiting_press_ = value;
    } else if (reg == REGISTER::CONFIRM_END) {
        confirm_end_ = value;
    } else if (reg == REGISTER::CONN_PROBLEM) {
        conn_error_ = value;
    } else if (reg == REGISTER::KEY_MANUAL) {
        key_manual_ = value;
    } else if (reg == REGISTER::SECURITY) {
        security_ = value;
    } else if (reg == REGISTER::SWITCH_MANUAL) {
        switch_manual_ = value;
    } else if (reg == REGISTER::LOGIN) {
        login_ = value;
    } else if (reg == REGISTER::CAMERROR) {
        cam_error_ = value;
    } else if (reg == REGISTER::DEFAULT_POSITION) {
        default_position_ = value;
    } else if (reg == REGISTER::STORED) {
        stored_ = value;
    } else {
        qWarning() << "non existing register";
        e_mit = false;
    }

    if (e_mit)
        emit registerChanged(reg, value);
}

bool InternalRegisters::getRegister(REGISTER reg) {
    bool value = false;

    if (reg == REGISTER::LOGGED) {
        value = logged_;
    } else if (reg == REGISTER::PRODUCT_READ) {
        value = product_read_;
    } else if (reg == REGISTER::PRODUCT_SN) {
        value = product_sn_;
    } else if (reg == REGISTER::CONFIRM) {
        value = confirm_;
    } else if (reg == REGISTER::EMERGENCY) {
        value = emergency_;
    } else if (reg == REGISTER::INIT_OK) {
        value = init_ok_;
    } else if (reg == REGISTER::CONTROL_END) {
        value = control_end_;
    } else if (reg == REGISTER::CONTROL_OK) {
        value = control_ok_;
    } else if (reg == REGISTER::PROCEDURE_STARTED) {
        value = procedure_started_;
    } else if (reg == REGISTER::PROCEDURE_ENDED) {
        value = procedure_end_;
    } else if (reg == REGISTER::CONFIRM_END) {
        value = confirm_end_;
    } else if (reg == REGISTER::BTN_CONTROL) {
        value = btn_control_;
    } else if (reg == REGISTER::WAITING_START) {
        value = waiting_start_;
    } else if (reg == REGISTER::WAITING_PRESS) {
        value = waiting_press_;
    } else if (reg == REGISTER::CONN_PROBLEM) {
        value = conn_error_;
    } else if (reg == REGISTER::KEY_MANUAL) {
        value = key_manual_;
    } else if (reg == REGISTER::SECURITY) {
        value = security_;
    } else if (reg == REGISTER::SWITCH_MANUAL) {
        value = switch_manual_;
    } else if (reg == REGISTER::LOGIN) {
        value = login_;
    } else if (reg == REGISTER::CAMERROR) {
        value = cam_error_;
    } else if (reg == REGISTER::DEFAULT_POSITION) {
        value = default_position_;
    } else if (reg == REGISTER::STORED) {
        value = stored_;
    } else {
        qWarning() << "non existing register";
       // throw
    }

    return value;
}

void InternalRegisters::setDefault() {
    logged_ = false;
    login_ = false;
    product_read_ = false;
    product_sn_ = false;
    confirm_ = false;
    emergency_ = false;
    stored_ = false;
    init_ok_ = false;
    switch_manual_ = false;
    control_end_ = control_ok_ = false;
    procedure_end_ = false;
    confirm_end_ = false;
    btn_control_ = false;
    procedure_started_ = false;
    waiting_start_ = false;
    waiting_press_ = false;
    conn_error_ = false;
    key_manual_ = false;
    security_ = false;
    master_conn_error_ = false;
    logged_otherdev = false;
    cam_error_ = false;
    default_position_ = false;

}
