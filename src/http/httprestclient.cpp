#include "httprestclient.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

#include <QtCore>

#include "../appcontext.h"

constexpr auto TIMEOUT = 5000;

HttpRestClient::HttpRestClient()
{
    networkManager = new QNetworkAccessManager(this);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                &connection_loop, SLOT(quit()));
    connect(&timer_, SIGNAL(timeout()),
                &connection_loop, SLOT(quit()));
}

HttpRestClient::~HttpRestClient() {
    delete networkManager;
}

QPair<QString, bool> HttpRestClient::getQuery(QString url) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    res_ok = false;
    timer_.start(TIMEOUT);
    QNetworkReply * reply = networkManager->get(request);
    connection_loop.exec();

    if (reply->error() || (!reply->isFinished())) {
        if (!reply->isFinished()) {
            reply->abort();
            qWarning() << "too long response";
        } else {
            qDebug() << "ERROR!";
            qDebug() << reply->errorString();
        }
        AppContext::GetInstance().GetRegister()
                ->setRegister(InternalRegisters::REGISTER::CONN_PROBLEM, true);
    } else {
        QByteArray arr = reply->readAll();
        result = QString::fromLatin1(arr.data());
        res_ok = true;
    }

    reply->deleteLater();

    QPair<QString, bool> res_pair;
    res_pair.first = result;
    res_pair.second = res_ok;

    return res_pair;
}

QPair<QString, bool> HttpRestClient::postQuery(QString url, QString postdata) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    res_ok = false;

    QTimer timer_;
    QEventLoop connection_loop;
    timer_.start(TIMEOUT);
    connect(&timer_, SIGNAL(timeout()),
                &connection_loop, SLOT(quit()));
    QNetworkAccessManager networkManager;
    QNetworkReply * reply = networkManager.post(request, postdata.toUtf8());
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
                &connection_loop, SLOT(quit()));
    connection_loop.exec();

    if (reply->error() || (!reply->isFinished())) {
        if (!reply->isFinished()) {
            reply->abort();
            qWarning() << "too long response post";
        } else {
            qDebug() << "ERROR!";
            qDebug() << reply->errorString();
        }
        AppContext::GetInstance().GetRegister()
                ->setRegister(InternalRegisters::REGISTER::CONN_PROBLEM, true);
    } else {
        QByteArray arr = reply->readAll();
        result = QString::fromLatin1(arr.data());
        res_ok = true;
    }

    reply->deleteLater();

    QPair<QString, bool> res_pair;
    res_pair.first = result;
    res_pair.second = res_ok;

    return res_pair;
}
