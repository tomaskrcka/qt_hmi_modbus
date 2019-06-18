#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QtCore>
#include <QtConcurrent/QtConcurrent>
#include <qqmlcontext.h>
#include <qqml.h>

#include "src/appcontext.h"
#include "src/qmlconnection.h"
#include "src/control/processcontrol.h"
#include "src/modules/cameramoduleweb.h"
#include "src/modules/modbus.h"
#include "src/modules/restmodule.h"
#include "src/http/httprestclient.h"

void initDB(InternalRegisters * internal_register) {

    if (!AppContext::GetInstance().InitContext(internal_register)) {
        qWarning() << "Error inititalization";
    }
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<QmlConnection>("cz.nkgroup", 1, 0, "QmlConnection");
    qmlRegisterType<ProcessStates>("cz.nkgroup", 1, 0, "States");

    InternalRegisters internal_register;
    qRegisterMetaType<InternalRegisters::REGISTER>();

    Settings & settings = AppContext::GetInstance().settings_;
    settings.ReadConfig();
    QtConcurrent::run(initDB, &internal_register);

    HttpRestClient httpclient;
    // RestModule restModule(&httpclient);


    CameraInterface * camera_module = new CameraModuleWeb(settings.camera_url_.toUrl(), settings.camera_path_.toString(), settings.camera_ext_.toString());

    Modbus modbus(settings.plc_addr_.toString(), settings.plc_port_.toInt());
    modbus.InitModule();

    ProcessControl pr(camera_module, &modbus,
                      &internal_register);
    pr.start();
    AppContext::GetInstance().SetProcessControl(&pr);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("myModel", QVariant::fromValue(AppContext::GetInstance().getPrograms()));

    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
