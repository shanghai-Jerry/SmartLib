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
#include "thread.h"
#include "server.h"
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
    void setQRData(QString data);
    void paintEvent(QPaintEvent *pe);
    ~MainWindow();
private slots:
    void itemClicked(QListWidgetItem* );

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

    // 服务器数据接受
       void displayError(QAbstractSocket::SocketError); //显示错误
       void processConnectSlot(QString name);
       void finishConn(QString );
       void processDataSlot(QString, QString);

       void on_pushButton_listen_clicked();

       void on_pushButton_close_clicked();

       void on_BorrowAction_triggered();

       void on_pushButton_modifyreader_clicked();

       void on_pushButton_modifybook_clicked();

       void on_pushButton_listuser_clicked();

//       void on_pushButton_restore_clicked();

private:

     Ui::MainWindow *ui;
     QSqlDatabase db;
     QSqlTableModel *model;
     QSqlTableModel *modelreader;
     QSqlTableModel *modelrecord;
     QSqlTableModel *modeluser;
     QTimer *timer;
     QAction *info;
     class server* m_server;				//服务器
     QMap<QString, QString>* m_nameMap;	//用户通信信息map
     QList<QHostAddress> ipAddressesList;
     QString data;


};

#endif // MAINWINDOW_H
