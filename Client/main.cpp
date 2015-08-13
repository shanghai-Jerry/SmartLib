#include <QApplication>
#include "mainwindow.h"
#include "logindialog.h"
#include<QTextCodec>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog *logl;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    logl=new LoginDialog(NULL);
    QSize size =a.desktop( )->screenGeometry( ).size( );
    logl->resize(size);
    if(logl->exec()==QDialog::Accepted)
    {

        MainWindow w;
        w.show();
        return a.exec();

    }
    return 0;


    

}
