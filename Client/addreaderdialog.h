#ifndef ADDREADERDIALOG_H
#define ADDREADERDIALOG_H
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include <QDialog>
#include<QMessageBox>
#include<QtGui>
#include<QCamera>
#include<QCameraImageCapture>
#include<QCameraViewfinder>
#include<QFileDialog>
namespace Ui {
class addreaderDialog;
}

class addreaderDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit addreaderDialog(QWidget *parent = 0);
    ~addreaderDialog();
    
private slots:
    void on_pushButton_2_clicked();
    void addreaderchange();
    void on_pushButton_ok_clicked();
    void on_checkBoxs_clicked();
    //
    void captureImage();
    void displayImage(int,QImage);
    //void saveImage();
    void on_pushButton_takepicture_clicked();
    void setdate();

    void openPicture();
    void on_pushButton_upload_clicked();

private:
     Ui::addreaderDialog *ui;
     QSqlDatabase db;
     //
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imageCapture;
     QString takepicName;
     QString takepicNamevaild;
     QString rid;
     QString pwd ;
     QString name ;
     QString sex;
     QString birth ;
     QString major;
     QString rtype ;
     QString status ;

};

#endif // ADDREADERDIALOG_H
