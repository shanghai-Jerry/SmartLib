#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QDialog>

namespace Ui {
class recordDialog;
}

class recordDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit recordDialog(QWidget *parent = 0);
    ~recordDialog();
    
private:
    Ui::recordDialog *ui;
};

#endif // RECORDDIALOG_H
