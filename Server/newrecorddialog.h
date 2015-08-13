#ifndef NEWRECORDDIALOG_H
#define NEWRECORDDIALOG_H
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include <QDialog>
#include<QMessageBox>
#include<QSqlTableModel>
namespace Ui {
class newrecordDialog;
}

class newrecordDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit newrecordDialog(QWidget *parent = 0);
    ~newrecordDialog();
    
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void setdate();
    void on_toolButton_clicked();
    void on_toolButton_2_clicked();
    void getdoubleclickitem(QModelIndex);
    void getdoubleclickitembook(QModelIndex);
private:
    QSqlDatabase db;
    Ui::newrecordDialog *ui;
    QSqlTableModel *modelreader;
    QSqlTableModel *model;
};

#endif // NEWRECORDDIALOG_H
