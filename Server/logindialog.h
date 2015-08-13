#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include<QFile>
#include<QFileDialog>
#include<QByteArray>
#include <QDialog>
#include<QSqlTableModel>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include "addreaderdialog.h"
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QMessageBox>
#include<QtWebView/QtWebView>
#include<QWebElement>
#include<QWebFrame>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
signals:
     void setdata(QString data);

private slots:
    void on_pushButton_login_clicked();
    void timerUpDate();
    void on_pushButton_2_clicked();
    void on_pushButton_getpicture_clicked();
    void on_pushButton_clicked();
    void on_pushButton_qrcorelogin_clicked();
    void replayFinished(QNetworkReply *replay);
private:
     Ui::LoginDialog *ui;
     QSqlDatabase db;
     QString filename;
      QTimer *timer;

};

#endif // LOGINDIALOG_H
