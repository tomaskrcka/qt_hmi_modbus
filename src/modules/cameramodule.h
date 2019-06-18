#ifndef CAMERAMODULE_H
#define CAMERAMODULE_H

#include "camerainterface.h"

#include <QString>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class CameraModule : public CameraInterface
{
public:
    explicit CameraModule(QUrl imageUrl, QString path, QString file_ext, QObject *parent = nullptr);
    virtual QString TakeSnapshot(QString postfix);

private:
    QNetworkAccessManager web_ctrl_;
    QByteArray downloaded_data_;
    QUrl imageurl_;
    QString path_;
    QString file_ext_;
};

#endif // CAMERAMODULE_H
