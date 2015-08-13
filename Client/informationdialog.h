#ifndef INFORMATIONDIALOG_H
#define INFORMATIONDIALOG_H
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QSqlTableModel>
#include <QDialog>
#include<QFileDialog>
#include <QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
namespace Ui {
class informationDialog;
}

class informationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit informationDialog(QWidget *parent = 0);
    ~informationDialog();

private slots:
    void on_pushButton_upoad_clicked();
    void on_pushButton_quit_clicked();
    void getinfor();
    void on_pushButton_ok_clicked();
    void displayImage();
    void openPicture();
    void on_pushButton_add_clicked();
    void on_pushButton_modfiy_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_finish_clicked();
    void showdata();
    void on_pushButton_back_clicked();
    void replayFinished(QNetworkReply *replay);

private:
    Ui::informationDialog *ui;
     QSqlDatabase db;
     QSqlTableModel *model;
     int status;
};

#endif // INFORMATIONDIALOG_H
