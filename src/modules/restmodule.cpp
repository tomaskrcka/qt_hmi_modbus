#include "restmodule.h"

RestModule::RestModule(HttpRestClient * const httpclient)
    : httpclient_(httpclient)
{

}

int RestModule::UserExists(QString rfid_code) {
    return 1;
}

int RestModule::UserExistsHigherLevel(QString rfid_code) {
    return 1;
}

bool RestModule::CreateTank(TankItem item) {
    return true;
}
