#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>

namespace Ui {
class deleteDialog;
}

class deleteDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit deleteDialog(QWidget *parent = 0);
    ~deleteDialog();
    
private:
    Ui::deleteDialog *ui;
};

#endif // DELETEDIALOG_H
