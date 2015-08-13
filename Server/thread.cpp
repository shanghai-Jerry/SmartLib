#include "thread.h"

Thread::Thread(QObject *parent, server *th)
    : QThread(parent), th(th)
{
    m_blockSize = 0;//初始化其为0
}

Thread::~Thread()
{

}
/*
* 线程运行函数，设定数据通信信号与槽，启动事件循环
*/
void Thread::run()
{
    qDebug()<<QObject::tr("run");
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));				//接收数据
    connect(th, SIGNAL(sendMessages(QString, QString)),this, SLOT(sendMessage(QString, QString)));//发送数据
    connect(th, SIGNAL(closeConn(QString)), this, SLOT(closeConn(QString)));			//主动关闭连接
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disConnect()));				//被动关闭连接

    exec();//启动事件循环
}

/*
* 设定线程Socket
*/
void Thread::setSocket(QTcpSocket* sock)
{
    this->m_tcpSocket = sock;
}

/*
* 设定线程Socket对应用户地址
*/
void Thread::setConnAddr(QString connAddr)
{
    this->m_connAddr = connAddr;
}

/*
* 发送数据槽函数，通过用户对应的Socket将数据发送出去
* 输入参数: QString message 要发送的数据
*			QString name    接收用户名
*/
void Thread::sendMessage(QString message,QString name)
{
    if (message.isEmpty())
    {
        return;
    }

    //当前Socket对应用户地址与接收数据用户一致，则发送数据
    if (m_connAddr == name)
    {
        QByteArray block;								//用于暂存要发送的数据
        QDataStream out(&block,QIODevice::WriteOnly);   //使用数据流写入数据
        //out.setVersion(QDataStream::Qt_4_6);
        out << (quint16)0;
        out << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        m_tcpSocket->write(block);
    }
}

/*
* 读取数据槽函数，发射有数据到来信号
*/
void Thread::receiveData()
{
    QDataStream in(m_tcpSocket);
    //in.setVersion(QDataStream::Qt_4_6);				//设置数据流版本
    //有数据，读取
    QString message;
    if (m_tcpSocket->bytesAvailable() > 0)
    {
        in >> m_blockSize;
        in >> message;								//将接收到的数据存放到变量中
        m_tcpSocket->flush();
        emit bytesArrived(message, m_connAddr);	//发射有数据到来信号
    }
}

/*
* 主动关闭连接槽函数
* 输入参数: QString name 要关闭连接的用户名
*/
void Thread::closeConn(QString name)
{
    //当前Socket对应地址与用户名相同，则关闭
    if (m_connAddr == name)
    {
        m_tcpSocket->close();
    }
}

/*
* 关闭连接槽函数，发射关闭连接信号
*/
void Thread::disConnect()
{
    emit disConnect(m_connAddr);
}


