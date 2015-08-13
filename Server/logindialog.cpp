#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
#include<QDebug>
#include<QPicture>
QString usernamewhole;
QString userid;
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->webView_2->hide();
    ui->webView->hide();
    setFixedSize(this->width(), this->height());
    setWindowIcon(QIcon(":/img/welcome.png"));
    setWindowTitle(QString::fromLocal8Bit("Login-Server"));
    //ui->lineEdit_user->setText(QString::fromLocal8Bit("Please Type ID"));
    ui->lineEdit_user->selectedText();
    ui->lineEdit_user->setText("114831037");
    ui->lineEdit_passwd->setText("1037");
    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);

    connect(ui->lineEdit_user,SIGNAL(textEdited(QString)),ui->pushButton_getpicture,SLOT(click()));

    //QString runPath =QCoreApplication::applicationDirPath();

    db=QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName("localhost");
    db.setDatabaseName("mydb"); //
    db.setUserName("root"); //
    db.setPassword("!aq8813797769101!&)"); //
    if(db.open())
       {
           qDebug()<<"database is established!";
       }
       else
       {
           qDebug()<<"database is established error!";
       }

}

LoginDialog::~LoginDialog()
{
    delete ui;
    db.close();
}
//登陆
void LoginDialog::on_pushButton_login_clicked()
{

    QString runPath =QCoreApplication::applicationDirPath();
    QString user = ui->lineEdit_user->text();
    QString pass = ui->lineEdit_passwd->text();

   if(!user.isEmpty()&& !pass.isEmpty())
   {

        QSqlQuery q;
        q.exec("select count(*) from admin where rid='"+user+"'");
        q.next();
        int rowcnt1 = q.value(0).toInt();
        if(rowcnt1>0)
        {
          QSqlQuery query;
          query.exec("select name,status,pwd,rid from admin where rid='"+user+"'");
          query.next();
          QString username=query.value(0).toString();
          usernamewhole=username;
          userid=user;
          QString status=query.value(1).toString();
          QString pwd =query.value(2).toString();
          QString rid =query.value(3).toString();
             if(user == ui->lineEdit_user->text() && pwd ==ui->lineEdit_passwd->text())
             {

                // QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"),QString::fromLocal8Bit("欢迎：")+status+tr(" ")+username+QString::fromLocal8Bit(" 身份"));
                 accept();
                 close();
             }
             else
             {
              QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"), QString::fromLocal8Bit("User or Password Error..."));
             }

       }
        else
        {
         QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"), QString::fromLocal8Bit("User Error!"));
        }

    }
   else
   {
    QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"), QString::fromLocal8Bit("Please Complete"));
   }

}

//
void LoginDialog::on_pushButton_2_clicked()
{
    close();
}
//
void LoginDialog::on_pushButton_getpicture_clicked()
{
//    qDebug() << "Available drivers:";
//        QStringList drivers = QSqlDatabase::drivers();
//        foreach(QString driver, drivers)
//           qDebug() << driver;
  QString user = ui->lineEdit_user->text();
  QString pass = ui->lineEdit_passwd->text();
  QSqlQuery query;
  query.exec("select count(*) from admin where rid='"+user+"'");
  query.next();
  int rowcnt1 = query.value(0).toInt();
  if(rowcnt1>0)
  {
      ui->pushButton_getpicture->hide();
      //
      QSqlQuery q("SELECT images FROM admin WHERE rid ='"+user+"'");
      q.next();
      QPixmap photo;
      photo.loadFromData(q.value(0).toByteArray(), "PNG");
      ui->label_test->setScaledContents(true);
      ui->label_test->setPixmap(photo);

      //提取图片
//      filename=query.value(0).toString();
//      QImage image(filename);
//     // ui->lineEdi_test->setText(filename);
//      ui->label_test->setScaledContents(true);
//      ui->label_test->setPixmap(QPixmap::fromImage(image));
//      ui->pushButton_getpicture->hide();

  }
  else
  {
    ui->label_test->clear();
    ui->pushButton_getpicture->setVisible(true);
    ui->label_test->setStyleSheet("border-image: url(:/img/logininback.png);");
  }
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                    QDir::currentPath(),
//                                                    tr("Images (*.png *.xpm *.jpg)"));


}


void LoginDialog::on_pushButton_clicked()
{
    addreaderDialog adlg;
       if(adlg.exec()==QDialog::Accepted)
        {
         //QMessageBox::information(NULL, ("Tips"),("success"));
        }
}
//qrcode  登陆
void LoginDialog::timerUpDate()
{
    QSqlQuery query;
    query.exec("select login from admin where rid='"+ui->lineEdit_user->text()+"'");
    query.next();
    if(query.value(0).toInt()==1)
    {
        query.exec("update admin set login=0 where rid='"+ui->lineEdit_user->text()+"'");
        query.next();
        userid=ui->lineEdit_user->text();
        accept();
    }
}
void LoginDialog::on_pushButton_qrcorelogin_clicked()
{
    if(!ui->lineEdit_user->text().isEmpty())
     {
    ui->groupBox_logininte->hide();
    QSqlQuery query;
    query.exec("select count(*) from admin where rid='"+ui->lineEdit_user->text()+"'");
    query.next();
     int cout=query.value(0).toInt();
     if(cout>0)
         {
          query.exec("select qrcode from admin where rid='"+ui->lineEdit_user->text()+"'");
          query.next();
          ui->webView_2->show();
          QString url=query.value(0).toString();
          if(url.isEmpty())
          {
              ui->groupBox_logininte->show();
          }
          else
          {
          ui->webView_2->setUrl(url);
          ui->webView_2->setDisabled(true);
          ui->pushButton_qrcorelogin->setEnabled(false);
          //开始搜数据
          timer = new QTimer(this);
          connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
          timer->start(10);
          }

//          QString urltest="http://192.168.1.106/Default.aspx?rid="+ui->lineEdit_user->text();
//          QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//          connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replayFinished(QNetworkReply*)));
//          manager->get(QNetworkRequest(urltest));
        }
        else
        {
           QMessageBox::information(NULL, ("Tips"),("User ERROR"));
        }
    }
    else
    {
        QMessageBox::information(NULL, ("Tips"),("User NULL"));
    }

}
void LoginDialog::replayFinished(QNetworkReply *replay)
{
     QString html = tr(replay->readAll());
     html.toLower();
     QWebFrame *wef=ui->webView->page()->mainFrame();
     ui->webView->setHidden(true);
     wef->setHtml(html);
    QWebElement douc=wef->documentElement();
    QWebElement firstinpu=douc.findFirst("input[id=rid]");
    QString idvalue=firstinpu.attribute("value");//获取得到验证状态
    if(idvalue.compare(ui->lineEdit_user->text())==0)//状态返回
    {
        accept();

    }
    else
    {
        QMessageBox::information(NULL, ("Tips"),("Failed"));
    }


    //ui->lineEdit_9->setText(value);
}
