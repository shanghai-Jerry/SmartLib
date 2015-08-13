#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork>
#include "thread.h"
class server: public QTcpServer
{
     Q_OBJECT

public:
    server(QObject *parent = 0);
    ~server();
    void deleteConn(QString name);
    void sendMessage(QString message, QString name);

signals:
    void newConnect(QString connAddr);
    void closeConn(QString name);
    void finishConn(QString name);
    void bytesArrived(QString message, QString name);
    void sendMessages(QString message, QString name);

private slots:
    void incomingConnection();
    void disConnect(QString name);

private:
};

#endif // SERVER_H
