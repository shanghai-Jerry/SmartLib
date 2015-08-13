#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "addreaderdialog.h"
#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QMessageBox>
#include<QSqlTableModel>
#include<QTimer>
#include<QList>
#include <QtNetwork>
#include<QDateTime>
#include<QTextCodec>
#include<QNetworkInterface>
#include<QListWidgetItem>
#include "logindialog.h"
#include "modifyreaderdialog.h"
#include "informationdialog.h"
#include "newrecorddialog.h"
#include "modifybookdialog.h"
namespace Ui {
class MainWindow;
class server;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void sendMessage(QString);//发送数据反馈信息
    void sleep(int msec);
    ~MainWindow();
private slots:
    void on_pushButton_addbook_clicked();

    void on_pushButton_booklist_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_clicked();

    void on_BookAction_triggered();

    void on_Readaction_triggered();

    void on_action_triggered();

    void on_Systemaction_triggered();

    void on_pushButton_readlist_clicked();

    void on_pushButton_radeadd_clicked();

    void on_pushButton_readdel_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_datacopy_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_deleterecord_clicked();

    void timerUpDate();
    void on_pushButton_continue_clicked();
    void on_pushButton_changepwd_clicked();
    void license();
    void on_pushButton__perinfo_clicked();
    void getbookitemvalue(QModelIndex index);
    void getreaderitemvalue(QModelIndex index);
    void mouseDoubleClickEvent(QMouseEvent * ev);
    void getip();
       //客户端数据发送
       void processConnect();//发送连接信息
       void displayError(QAbstractSocket::SocketError); //显示链接错误
       void receiveData();
       void on_pushButton_listen_clicked();
       void on_pushButton_close_clicked();
       void on_BorrowAction_triggered();
       void on_pushButton_modifyreader_clicked();

       void on_pushButton_modifybook_clicked();



private:

     Ui::MainWindow *ui;
     QSqlDatabase db;
     QSqlTableModel *model;
     QSqlTableModel *modelreader;
     QSqlTableModel *modelrecord;
     QTimer *timer;
     QAction *borrow;
     int perstatus;
     QList<QHostAddress> ipAddressesList;
     QTcpSocket  *clientSocket;//创建一个tcp连接
     QString sendtext;
     quint16 m_blockSize;


};

#endif // MAINWINDOW_H
