#ifndef MODIFYBOOKDIALOG_H
#define MODIFYBOOKDIALOG_H
#include<QDebug>
#include <QSqlError>
#include <QDialog>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtGui>
#include <QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>

//#include<QtWebView>

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

   void replayFinished(QNetworkReply *replay);
private:
    Ui::modifybookDialog *ui;
    QSqlDatabase db;
    QString urltest;
     QNetworkAccessManager *manager;


};

#endif // MODIFYBOOKDIALOG_H
