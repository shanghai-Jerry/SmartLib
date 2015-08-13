#include "server.h"

server::server(QObject *parent)
:QTcpServer(parent)
{
    //服务器接收到新的连接，则触发有新连接槽函数
    connect(this, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
}
void server::incomingConnection()
{
     qDebug()<<QObject::tr("helloworld-incomingconnect");
     QTcpSocket* socket = this->nextPendingConnection();//获取新连接的Socket

     QString connAddr(socket->peerAddress().toString());//获取对应用户地址
     connAddr.append(" - " + QString::number(socket->peerPort()));
     emit newConnect(connAddr);

     //创建多线程，设定线程Socket及对应用户地址
     Thread *thread = new Thread(this,this);
     thread->setSocket(socket);
     thread->setConnAddr(connAddr);
     connect(thread, SIGNAL(bytesArrived(QString, QString)), this, SIGNAL(bytesArrived(QString, QString)));
     connect(thread, SIGNAL(disConnect(QString)), this, SLOT(disConnect(QString)));
     thread->start();

}
server::~server()
{

}

/*
* 发送数据，发射发送数据信号
* 输入参数: QString message 数据信息
*			QString name	接收用户名
*/
void server::sendMessage(QString message, QString name)
{
    emit sendMessages(message,name);
}


/*
* 关闭用户连接，发射主动关闭用户连接信号
* 输入参数: QString name 用户名
*/
void server::deleteConn(QString name)
{
    emit closeConn(name);
}

/*
* 关闭用户连接，发射关闭用户连接信号
* 输入参数: QString name 关闭连接的用户名
*/
void server::disConnect(QString name)
{
    emit finishConn(name);
}




