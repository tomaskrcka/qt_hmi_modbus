#ifndef HTTPRESTCLIENT_H
#define HTTPRESTCLIENT_H

#include <QObject>
#include <QPair>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimerEvent>
#include <QEventLoop>
#include <QTimer>

class HttpRestClient : public QObject
{
    Q_OBJECT
public:
    HttpRestClient();
    virtual ~HttpRestClient();

    QPair<QString, bool> getQuery(QString url);
    QPair<QString, bool> postQuery(QString url, QString postdata);

signals:
    void dataReceived(QString data, bool error);

/*
private slots:
    void fileDownloaded(QNetworkReply* pReply);
*/

private:
    QNetworkAccessManager * networkManager;
    QEventLoop connection_loop;
    QTimer timer_;
    QString result;
    bool done;
    bool res_ok;
};

#endif // HTTPRESTCLIENT_H
