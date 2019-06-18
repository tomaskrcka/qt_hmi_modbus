#include "appcontext.h"

#include <QSettings>
#include <qlogging.h>
#include "tankitem.h"
#include "storage/mongodb.h"

bool AppContext::InitContext(InternalRegisters * internalreg) {
    if (storage_interface_ == nullptr) {
        storage_interface_ = new MongoDB;
    }

    if (internalreg != nullptr)
        register_ = internalreg;

    Settings & settings_ = AppContext::GetInstance().settings_;

    bool res = false;
    if (settings_.db_enabled_)
        res = storage_interface_->Connect(settings_.username_.toString(), settings_.password_.toString(),
                                settings_.dbname_.toString(), settings_.hostname_.toString(),
                                settings_.port_.toInt());
    else
        res = true;

    // sleep(2);
    register_->setRegister(InternalRegisters::REGISTER::INIT_OK, true);

#ifdef DEBUG
    status_ = (res ? 1 : 2);
#else
    status_ = (res ? 1 : 2);
#endif

    return res;
}

void AppContext::SetProcessControl(ProcessControl * process_control) {
    process_control_ = process_control;
}

void AppContext::SetRegister(InternalRegisters * internalreg) {
    register_ = internalreg;
}

bool AppContext::ReinitDB() {
    return InitContext();
}

QVariantList AppContext::getPrograms() {
    QVariantList dataList;
    foreach (auto item, settings_.tank_items) {
        QVariantMap itemmap;
        itemmap.insert("name", item.tank_type);
        dataList.append(itemmap);
    }

    return dataList;
}

void AppContext::copyProgram(TankItem &tankitem, const QString program) {
    foreach (auto item, settings_.tank_items) {
        if (program == item.tank_type) {
            tankitem.time1 = item.time1;
            tankitem.time2 = item.time2;
            tankitem.pressure = item.pressure;
        }
    }
}
