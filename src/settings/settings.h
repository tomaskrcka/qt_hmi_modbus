#ifndef SETTINGS_H
#define SETTINGS_H

#include "tanksetting.h"

class Settings
{
public:
    Settings();

    void ReadConfig();
    void ReadTankSettings();

    bool db_enabled_;
    QVariant username_;
    QVariant password_;
    QVariant hostname_;
    QVariant dbname_;
    QVariant port_;
    QVariant restapi_;
    QVariant camera_url_;
    QVariant camera_path_;
    QVariant camera_ext_;
    QVariant plc_addr_;
    QVariant plc_port_;
    QList<TankSetting> tank_items;
private:
    QSettings settings_;
};

#endif // SETTINGS_H
