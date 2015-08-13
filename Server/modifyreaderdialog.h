#ifndef MODIFYREADERDIALOG_H
#define MODIFYREADERDIALOG_H
#include <QSqlError>
#include <QDialog>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QMessageBox>
#include<QtGui>
#include<QCamera>
#include<QCameraImageCapture>
#include<QCameraViewfinder>
#include<QFileDialog>
#include<QDebug>
#include<QtWebView/QtWebView>
#include<QFileDialog>
#include <QtNetwork>
#include <QtWebKit>
#include <QWebFrame>
namespace Ui {
class modifyreaderDialog;
}

class modifyreaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifyreaderDialog(QWidget *parent = 0);
    ~modifyreaderDialog();
    void openPicture();


private slots:
    void on_pushButton_ok_clicked();
    void captureImage();
    void displayImage(int,QImage);
    void on_pushButton_2_clicked();
    void setdate();
    void on_pushButton_takepicture_2_clicked();

    void on_pushButton_qrcode_clicked();

    void on_pushButton_clicked();

    void on_pushButton_showqrc_clicked();

    void on_pushButton_dataset_3_clicked();

private:
    Ui::modifyreaderDialog *ui;
    QSqlDatabase db;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
    QString takepicName;
    QString pwd;
    QString name;
    QString sex;
    QString birth;
    QString major;
    QString status;
    QString rtype;
   //



};

#endif // MODIFYREADERDIALOG_H
