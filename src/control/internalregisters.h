#ifndef INTERNALREGISTERS_H
#define INTERNALREGISTERS_H

#include <QObject>

class InternalRegisters : public QObject
{
    Q_OBJECT
public:
    InternalRegisters(QObject *parent = nullptr);

    enum class REGISTER {
        INIT_OK,
        LOGIN,
        LOGGED,
        PRODUCT_READ,
        PRODUCT_SN,
        CONFIRM,
        CONTROL_OK,
        CONTROL_END,       // The control has been finished
        WAITING_START,     // The pause before the pressing procedure
        WAITING_PRESS,     // Waiting for the pressure
        PROCEDURE_STARTED, // The procedure has been started
        PROCEDURE_ENDED, // The end of the procedure
        BTN_CONTROL,  // An user must confirm the procedure
        CONFIRM_END,
        CONN_PROBLEM,
        KEY_MANUAL,
        SECURITY,
        EMERGENCY,
        SWITCH_MANUAL,
        CAMERROR,
        DEFAULT_POSITION,
        STORED
    };

    void setRegister(REGISTER reg, bool value);
    bool getRegister(REGISTER reg);
    void setDefault();

signals:
    void registerChanged(InternalRegisters::REGISTER reg, bool value);

private:
    bool init_ok_;
    bool login_;
    bool logged_;
    bool product_read_;
    bool product_sn_;
    bool confirm_;
    bool emergency_;
    bool security_;
    bool control_ok_;
    bool control_end_;
    bool procedure_end_;
    bool confirm_end_;
    bool procedure_started_;
    bool btn_control_;
    bool waiting_start_;
    bool waiting_press_;
    bool conn_error_;
    bool key_manual_;
    bool switch_manual_;
    bool master_conn_error_;
    bool logged_otherdev;
    bool cam_error_;
    bool default_position_;
    bool stored_;

};
Q_DECLARE_METATYPE(InternalRegisters::REGISTER)

#endif // INTERNALREGISTERS_H
