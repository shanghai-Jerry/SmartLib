#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
#include<QDebug>
#include<QPicture>
QString usernamewhole;
QString userid;
QString serverip;
QString statuswhole;
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->label_qrc->hide();
    ui->label_status->setText("DisConnected");
//    ui->webView->hide();
//    ui->webView_2->hide();
    setFixedSize(this->width(),this->height()); // 禁止改变窗口大小
    setWindowIcon(QIcon(":/img/welcome.png"));
    setWindowTitle(QString::fromLocal8Bit("Login-Client"));
    ui->lineEdit_user->setText(QString::fromLocal8Bit("Please Register"));
    ui->lineEdit_user->selectedText();
    ui->lineEdit_user->setText("114831036");
    ui->lineEdit_passwd->setText("1036");
    ui->lineEdit_passwd->setEchoMode(QLineEdit::Password);

    //connect(ui->lineEdit_user,SIGNAL(textEdited(QString)),ui->pushButton_getpicture,SLOT(click()));
    //QString runPath =QCoreApplication::applicationDirPath();

}
void LoginDialog::databaseconnection(QString ipaddress)
{

    db=QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName(ipaddress);
    db.setPort(3306);
    db.setDatabaseName("mydb"); // 我们之前建立的数据库
    db.setUserName("root"); // 我们创建的用户名
    db.setPassword("!aq8813797769101!&)"); //用户的密码
    if(db.open())
       {
           ui->label_status->setText("Connected");
           qDebug()<<"database is established-client!";
           QMessageBox::information(NULL, QString::fromLocal8Bit("TIPS"), QString::fromLocal8Bit("Connnect success"));
       }
       else
       {
           qDebug()<<"database is established error!";
           QMessageBox::information(NULL, QString::fromLocal8Bit("tips"), QString::fromLocal8Bit("Connnect failed,please type ip and port"));
       }

}
LoginDialog::~LoginDialog()
{
    delete ui;
    db.close();
}

void LoginDialog::on_pushButton_login_clicked()
{

    QString runPath =QCoreApplication::applicationDirPath();
    QString user = ui->lineEdit_user->text();
    QString pass = ui->lineEdit_passwd->text();
   if(!user.isEmpty()&& !pass.isEmpty())
   {

        QSqlQuery q;
        if(ui->radioButton->isChecked())
        q.exec("select count(*) from admin where rid='"+user+"'");
        else
        q.exec("select count(*) from reader where rid='"+user+"'");
        q.next();
        int rowcnt1 = q.value(0).toInt();
        if(rowcnt1>0)
        {
          QSqlQuery query;
          if(ui->radioButton->isChecked())
          query.exec("select name,status,pwd,rid from admin where rid='"+user+"'");
          else
          query.exec("select name,status,pwd,rid from Reader where rid='"+user+"'");
          query.next();
          QString username=query.value(0).toString();
          usernamewhole=username;
          userid=user;
          QString status=query.value(1).toString();
          statuswhole=status;
          QString pwd =query.value(2).toString();
          QString rid =query.value(3).toString();
             if(user == ui->lineEdit_user->text() && pwd ==ui->lineEdit_passwd->text())
             {

                // QMessageBox::information(NULL, QString::fromLocal8Bit("登陆成功"),QString::fromLocal8Bit("欢迎")+status+tr(" ")+username+QString::fromLocal8Bit(" 登陆"));
                 accept();
                 close();
             }
             else
             {
                 QMessageBox::information(NULL, QString::fromLocal8Bit("TIPS"), QString::fromLocal8Bit("USER ERROR OR PWD ERROR!"));
             }

       }
        else
        {
         QMessageBox::information(NULL, QString::fromLocal8Bit("TIPS"), QString::fromLocal8Bit("User Error!"));
        }
 //  }

    }//输入不为空
   else
   {
    QMessageBox::information(NULL, QString::fromLocal8Bit("TIPS"), QString::fromLocal8Bit("Please Complete!"));
   }

}

//取消
void LoginDialog::on_pushButton_2_clicked()
{
    close();
}
//获取头像
void LoginDialog::on_pushButton_getpicture_clicked()
{
//    qDebug() << "Available drivers:";
//        QStringList drivers = QSqlDatabase::drivers();
//        foreach(QString driver, drivers)
//           qDebug() << driver;
  QString user = ui->lineEdit_user->text();
  QString pass = ui->lineEdit_passwd->text();
  QSqlQuery query;
  if(ui->radioButton->isChecked())
      query.exec("select count(*) from admin where rid='"+user+"'");
  else
  query.exec("select count(*) from reader where rid='"+user+"'");
  query.next();
  int rowcnt1 = query.value(0).toInt();
  if(rowcnt1>0)
  {
      //ui->pushButton_getpicture->hide();
      //通过存储的图片获取
      QSqlQuery q;
      if(ui->radioButton->isChecked())
        q.exec("SELECT images FROM admin WHERE rid ='"+user+"'");
      else
         q.exec("SELECT images FROM reader WHERE rid ='"+user+"'");
      q.next();
      QPixmap photo;
      photo.loadFromData(q.value(0).toByteArray(), "PNG");
      ui->label_test->setScaledContents(true);
      ui->label_test->setPixmap(photo);

      //通过路径获取图片
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
    QMessageBox::information(NULL, QString::fromLocal8Bit("TIPS"), QString::fromLocal8Bit("User Error!"));
  }
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                    QDir::currentPath(),
//                                                    tr("Images (*.png *.xpm *.jpg)"));


}
//连接服务器
void LoginDialog::on_pushButton_connect_clicked()
{
    serverip=ui->hostLineEdit->text();

    databaseconnection(serverip);



}
//注册
void LoginDialog::on_pushButton_resigter_clicked()
{
    addreaderDialog adlg;
    if(adlg.exec()==QDialog::Accepted)
    {
     //QMessageBox::information(NULL, QString::fromLocal8Bit("读者注册"), QString::fromLocal8Bit("读者注册成功，可以登录了"));
    }
}
//密码重置
void LoginDialog::on_pushButton_forget_clicked()
{

}
//qrcode   登陆
void LoginDialog::timerUpDate()
{
    QSqlQuery query;
    if(ui->radioButton->isChecked())
    query.exec("select login from admin where rid='"+ui->lineEdit_user->text()+"'");
    else
    query.exec("select login from reader where rid='"+ui->lineEdit_user->text()+"'");
    query.next();
    if(query.value(0).toInt()==1)
    {
        if(ui->radioButton->isChecked())
        query.exec("update admin set login=0 where rid='"+ui->lineEdit_user->text()+"'");
        else
        query.exec("update reader set login=0 where rid='"+ui->lineEdit_user->text()+"'");
        query.next();
        userid=ui->lineEdit_user->text();

        accept();
    }
}
void LoginDialog::on_pushButton_qrcodelohin_clicked()
{
    if(!ui->lineEdit_user->text().isEmpty())
     {
        QString url;
        QSqlQuery query;
        if(ui->radioButton->isChecked())
        query.exec("select count(*) from admin where rid='"+ui->lineEdit_user->text()+"'");
        else
        query.exec("select count(*) from reader where rid='"+ui->lineEdit_user->text()+"'");
        query.next();
        int cout=query.value(0).toInt();
       if(cout>0)
         {
          if(ui->radioButton->isChecked())
          query.exec("select qrcode from admin where rid='"+ui->lineEdit_user->text()+"'");
          else
          query.exec("select qrcode from reader where rid='"+ui->lineEdit_user->text()+"'");
          query.next();

          url=query.value(0).toString();
          if(url.isEmpty())
          {
               ui->groupBox_logbox->show();
          }
          else
          {
           //QMessageBox::information(NULL, ("Tips"),("qrcode ERROR"));
           ui->groupBox_logbox->hide();
           ui->label_qrc->show();
           QNetworkAccessManager *manager = new QNetworkAccessManager(this);
           connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replayFinished(QNetworkReply*)));
           manager->get(QNetworkRequest(url));
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
   //解析html
      // QMessageBox::information(NULL, ("Tips"),("qrcodeutl ERROR"));
   QByteArray html=(replay->readAll());
   QPixmap photo;
   photo.loadFromData(html, "PNG");
   ui->label_qrc->setScaledContents(true);
   ui->label_qrc->setPixmap(photo);

//QWebFrame *wef=ui->webView_2->page()->mainFrame();
//wef->setHtml(html);
//    QWebElement douc=wef->documentElement();
//    QWebElement firstinpu=douc.findFirst("input[id=bookid]");
//    QString value=firstinpu.attribute("value");//获取得到的bookid

   //ui->lineEdit_9->setText(value);
}

//return
void LoginDialog::on_pushButton_back_clicked()
{
    ui->groupBox_logbox->show();
    ui->label_qrc->hide();
}
