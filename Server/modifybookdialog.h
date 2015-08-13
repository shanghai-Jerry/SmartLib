#ifndef MODIFYBOOKDIALOG_H
#define MODIFYBOOKDIALOG_H
#include<QDebug>
#include <QSqlError>
#include <QDialog>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtGui>
#include<QtWebView/QtWebView>
#include<QWebElement>
#include<QWebFrame>
#include<QFileDialog>
#include<QDebug>
#include <QSqlError>
namespace Ui {
class modifybookDialog;
}

class modifybookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifybookDialog(QWidget *parent = 0);
    ~modifybookDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
   void savePicture();
   void openPicture();

    void on_pushButton_upload_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::modifybookDialog *ui;
    QSqlDatabase db;
    QString urltest;
    QString takepicName;


};

#endif // MODIFYBOOKDIALOG_H
