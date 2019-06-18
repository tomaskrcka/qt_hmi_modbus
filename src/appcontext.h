#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QSettings>
#include <QScopedPointer>
#include <QVariantList>
#include <QList>

#include "storage/storageinterface.h"
#include "tankitem.h"
#include "settings/settings.h"
#include "control/processcontrol.h"
#include "control/internalregisters.h"

class AppContext
{
public:
    static AppContext& GetInstance()
    {
        static AppContext instance;
        return instance;
    }

    bool InitContext(InternalRegisters * const internalreg = nullptr);
    void SetProcessControl(ProcessControl * const process_control);
    void SetRegister(InternalRegisters * const internalreg);
    bool ReinitDB();
    AppContext(AppContext const&)      = delete;
    void operator=(AppContext const&)  = delete;

    StorageInterface * GetStorage() {
        return storage_interface_;
    }

    ProcessControl * GetProcessControl() {
        return process_control_;
    }

    InternalRegisters * GetRegister() {
        return register_;
    }

    int GetAppStatus() {
        return status_;
    }

    QVariantList getPrograms();
    void copyProgram(TankItem &tankitem, const QString program);

    QList<TankItem> tankitems_;
    int num_tanks_;
    Settings settings_;
    QString user_rfid_;

private:
    AppContext() : process_control_(nullptr), status_(0) {
    }

    ~AppContext() {
        if (storage_interface_ != nullptr) {
            delete storage_interface_;
        }
    }

    StorageInterface * storage_interface_;
    ProcessControl * process_control_;
    InternalRegisters * register_;

    int status_;

};

#endif // APPCONTEXT_H
