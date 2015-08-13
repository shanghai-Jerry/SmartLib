#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include <QDialog>
#include<QMessageBox>
#include<QFileDialog>
#include<QDebug>
#include <QSqlError>
namespace Ui {
class addbookDialog;
}

class addbookDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit addbookDialog(QWidget *parent = 0);
    ~addbookDialog();

private slots:

    void on_pushButt_ok_clicked();
    void addbookchange();
    void on_pushButton_cancel_clicked();
    void on_checkBox_clicked();
    void openPicture();
    void savePicture();
    void on_pushButton_upload_clicked();

private:
    Ui::addbookDialog *ui;
    QSqlDatabase db;
    QString takepicName;
protected:
    void closeEvent(QCloseEvent *event);

};

#endif // ADDBOOKDIALOG_H
