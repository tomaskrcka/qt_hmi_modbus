#include "cameramodule.h"

#include <QDateTime>
#include <QDir>
#include <QProcess>
#include <QString>
#include <QThread>
#include <QFile>

CameraModule::CameraModule(QUrl imageurl, QString path, QString file_ext, QObject *parent) :
    CameraInterface(parent), imageurl_(imageurl), path_(path), file_ext_(file_ext) {
    last_picture_path_ = "";
}

QString CameraModule::TakeSnapshot(QString name_ext) {
    QString img_path;
    QDateTime local(QDateTime::currentDateTime());
    QDate date(QDate::currentDate());
    QString tmppath = QString("%1_%2").arg(date.weekNumber()).arg(date.year());

    QString retrn = tmppath + QDir::separator() + name_ext + "_" + local.toString(Qt::ISODate)
            + "." + file_ext_;

    img_path = path_ + QDir::separator() + tmppath;

    if ((QDir(img_path).exists()) ||(QDir().mkpath(img_path))) {
        img_path = path_ + QDir::separator() + retrn;

        QString cmd = QString("ffmpeg -f MJPEG -y -i %1 -r 1 -vframes 1 -q:v 1 %2").arg(imageurl_.toString()).arg(img_path);
        QProcess::startDetached(cmd);
    }
    last_picture_path_ = img_path;
    return retrn;
}

/*
bool CameraModule::TakeSnapshotLastOne() {

    if (last_picture_path_.length() == 0)
        return false;

    QString cmd = QString("ffmpeg -f MJPEG -y -i %1 -r 1 -vframes 1 -q:v 1 %2").arg(imageurl_.toString()).arg(last_picture_path_);
    QProcess::startDetached(cmd);
    int attemp = 5;
    while (attemp-- > 0 && !QFile::exists(last_picture_path_)) {
        QThread::msleep(1000);
    }

    return QFile::exists(last_picture_path_);
}
*/
