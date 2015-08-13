#ifndef THREAD_H
#define THREAD_H
#include <QtNetwork>
#include<QtGui>
#include "server.h"
#include "mainwindow.h"
class Thread :public QThread
{
    Q_OBJECT

public:
    server* th;
    Thread(QObject* parent, server* th);
    ~Thread();
    void run();
    void setSocket(QTcpSocket* socket);
    void setConnAddr(QString connAddr);

signals:
    void error(QTcpSocket::SocketError socketError);
    void bytesArrived(QString message, QString m_connAddr);
    void disConnect(QString m_connAddr);

private slots:
    void closeConn(QString);
    void disConnect();
    void receiveData();
    void sendMessage(QString message, QString name);

private:
    QTcpSocket* m_tcpSocket;
    QString m_connAddr;
    quint16 m_blockSize;

};

#endif // THREAD_H
