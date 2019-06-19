#include "settings.h"

#include "tanksetting.h"

#include <qlogging.h>

constexpr auto SETTINGS_TIME1 = "time1";
constexpr auto SETTINGS_TIME2 = "time2";
constexpr auto SETTINGS_PRESSURE = "pressure";
constexpr auto SETTINGS_TYPE = "type";

Settings::Settings() : settings_(QString("config.ini"), QSettings::IniFormat)
{

}

void Settings::ReadConfig() {
    db_enabled_ = settings_.contains("db/hostname");
    username_ = settings_.value("db/username", "test");
    password_ = settings_.value("db/password", "test");
    hostname_ = settings_.value("db/hostname", "localhost");
    dbname_ = settings_.value("db/dbname", "test");
    port_ = settings_.value("db/port", 27017);
    restapi_ = settings_.value("server/url", "http://127.0.0.1");
    camera_url_ = settings_.value("cam/url", "http://127.0.0.1:8080/cgi-bin/net_video.cgi?channel=1");
    camera_path_ = settings_.value("cam/path", "/cam/directory");
    camera_ext_ = settings_.value("cam/ext", "jpg");
    plc_addr_ =  settings_.value("plc/addr", "127.0.0.1");
    plc_port_ =  settings_.value("plc/port", "5002");

    ReadTankSettings();
}

void Settings::ReadTankSettings() {

    settings_.beginGroup("tanks");
    QStringList groups = settings_.childGroups();

    QString group;
    foreach (group, groups) {
        TankSetting item;
        bool ok;

        settings_.beginGroup(group);
        if (!settings_.contains(SETTINGS_TIME1)) {
            qWarning() << group << ": Not set " << SETTINGS_TIME1;
            continue;
        }
        QVariant q = settings_.value(SETTINGS_TIME1);

        item.time1 = q.toInt(&ok);
        if (!ok) {
            qWarning() << group << ": Invalid format of " << SETTINGS_TIME1;
            continue;
        }

        if (!settings_.contains(SETTINGS_TIME2)) {
            qWarning() << group << ": Not set " << SETTINGS_TIME2;
            continue;
        }
        q = settings_.value(SETTINGS_TIME2);
        item.time2 = q.toInt(&ok);
        if (!ok) {
            qWarning() << group << ": Invalid format of " << SETTINGS_TIME2;
            continue;
        }

        if (!settings_.contains(SETTINGS_PRESSURE)) {
            qWarning() << group << ": Not set " << SETTINGS_PRESSURE;
            continue;
        }
        q = settings_.value(SETTINGS_PRESSURE);

        item.pressure = q.toDouble(&ok);
        qInfo() << item.pressure;
        if (!ok) {
            qWarning() << group << ": Invalid format of " << SETTINGS_PRESSURE;
            continue;
        }

        if (!settings_.contains(SETTINGS_TYPE)) {
            qWarning() << group << ": Not set " << SETTINGS_TYPE;
            continue;
        }
        q = settings_.value(SETTINGS_TYPE);
        item.tank_type = q.toString();
        tank_items.append(item);
        settings_.endGroup();
    }

    settings_.endGroup();
}
