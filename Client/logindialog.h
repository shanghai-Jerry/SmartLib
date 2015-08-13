#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<QFile>
#include<QFileDialog>
#include<QByteArray>
#include <QDialog>
#include<QMessageBox>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include "addreaderdialog.h"
#include<QMessageBox>
#include<QtWidgets>
#include <QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
//#include <QtWebKit>
//#include<QtWebView/QtWebView>


namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);

    void databaseconnection(QString ipaddress);

    ~LoginDialog();
signals:
     void setdata(QString data);

private slots:

    void on_pushButton_login_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_getpicture_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_resigter_clicked();

    void on_pushButton_forget_clicked();

    void on_pushButton_qrcodelohin_clicked();

    void timerUpDate();

    void replayFinished(QNetworkReply *replay);

    void on_pushButton_back_clicked();

private:
     Ui::LoginDialog *ui;
     QSqlDatabase db;
     QString filename;
     QTimer *timer;
};

#endif // LOGINDIALOG_H
