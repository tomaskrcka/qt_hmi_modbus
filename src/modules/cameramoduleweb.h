#ifndef CAMERAMODULEWEB_H
#define CAMERAMODULEWEB_H

#include "camerainterface.h"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class CameraModuleWeb : public CameraInterface
{
public:
    explicit CameraModuleWeb(QUrl imageurl, QString path, QString file_ext, QObject *parent = nullptr);
    virtual QString TakeSnapshot(QString postfix);

private slots:
    void fileDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager web_ctrl_;
    QByteArray downloaded_data_;
    QUrl imageurl_;
    QString path_;
    QString postfix_;
    QString file_ext_;
    QString img_path_;
};

#endif // CAMERAMODULEWEB_H
