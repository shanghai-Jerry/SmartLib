#include <QApplication>
#include "mainwindow.h"
#include "logindialog.h"
#include<QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog *logl;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    logl=new LoginDialog(NULL);
    if(logl->exec()==QDialog::Accepted)
    {
        MainWindow w;
        w.show();
        return a.exec();

    }
    return 0;

}
