#include "cameramoduleweb.h"

#include <QDateTime>
#include <QDir>
#include <QFile>

CameraModuleWeb::CameraModuleWeb(QUrl imageurl, QString path, QString file_ext, QObject *parent) :
    CameraInterface(parent), imageurl_(imageurl), path_(path), file_ext_(file_ext) {
    connect(&web_ctrl_, SIGNAL (finished(QNetworkReply*)),
    SLOT (fileDownloaded(QNetworkReply*)));
    last_picture_path_ = "";
    img_path_ = "";
}

void CameraModuleWeb::fileDownloaded(QNetworkReply* pReply) {
    QByteArray data = pReply->readAll();
    //emit a signal
    pReply->deleteLater();

    if ((QDir(img_path_).exists()) ||(QDir().mkpath(img_path_))) {
        QFile file(img_path_);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
    }
    last_picture_path_ = img_path_;
}

QString CameraModuleWeb::TakeSnapshot(QString postfix) {
    QDateTime local(QDateTime::currentDateTime());
    QDate date(QDate::currentDate());
    QString tmppath = QString("%1_%2").arg(date.weekNumber()).arg(date.year());

    QString retrn = tmppath + QDir::separator() + postfix + "_" + local.toString(Qt::ISODate)
            + "." + file_ext_;

    img_path_ = path_ + QDir::separator() + tmppath;

    QNetworkRequest request(imageurl_);
    web_ctrl_.get(request);

    return retrn;
}
