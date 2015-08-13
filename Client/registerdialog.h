
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
class registerdialog;
}

class registerdialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit registerdialog(QWidget *parent = 0);
    ~registerdialog();
    
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

    void on_pushButton_Save_clicked();
    void setdate();
private:
     Ui::registerdialog *ui;
     QSqlDatabase db;
     //
     QCamera *camera;
     QCameraViewfinder *viewfinder;
     QCameraImageCapture *imageCapture;
     QString takepicName;
     QString takepicNamevaild;

};

#endif // ADDREADERDIALOG_H
