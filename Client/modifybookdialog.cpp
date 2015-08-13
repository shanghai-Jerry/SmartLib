#include "modifybookdialog.h"
#include "ui_modifybookdialog.h"
extern QString getbookitems;
extern QString serverip;
modifybookDialog::modifybookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifybookDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    ui->lineEdit_4->setReadOnly(true);
    ui->lineEdit_5->setReadOnly(true);
    ui->lineEdit_6->setReadOnly(true);
    ui->lineEdit_7->setReadOnly(true);
    ui->lineEdit_8->setReadOnly(true);
    ui->textEdit_intro->setReadOnly(true);
    db=QSqlDatabase::addDatabase("QMYSQL3","modifyboo");
    db.setHostName("localhost");
    db.setDatabaseName("mydb"); // 我们之前建立的数据库
    db.setUserName("root"); // 我们创建的用户名
    db.setPassword("!aq8813797769101!&)"); //用户的密码
    //db.setDatabaseName(runPath+"/mydb.mdb");
    if(db.open())
       {
           qDebug()<<"database is established!";
       }
       else
       {
           qDebug()<<"database is established error!";
       }
    QSqlQuery q;
    QList<QString> longerList;
    QString str;
    q.exec("select * from book where bookid='"+getbookitems+"'");
    q.next();
    for(int i=0;i<=9;i++)
    {

         str=q.value(i).toString();
         longerList.append(str);
    }
    ui->lineEdit->setText(longerList.at(0));
    ui->lineEdit_2->setText(longerList.at(1));
    ui->lineEdit_3->setText(longerList.at(2));
    ui->lineEdit_4->setText(longerList.at(3));
    ui->lineEdit_5->setText(longerList.at(4));
    ui->lineEdit_6->setText(longerList.at(5));
    ui->lineEdit_7->setText(longerList.at(6));
    ui->lineEdit_8->setText(longerList.at(7));
    ui->textEdit_intro->setText(longerList.at(8));

           QString url =longerList.at(9);
          if(!url.isEmpty())
              //ui->pushButton_3->setEnabled(false);
//           ui->webView->load(url);
//           ui->webView->setDisabled(true);

              q.exec("select images from book where bookid='"+getbookitems+"'");
              q.next();
              QPixmap photo;
              photo.loadFromData(q.value(0).toByteArray(), "jpg");//图片类型
              ui->label_headpict_2->setScaledContents(true);
              ui->label_headpict_2->setPixmap(photo);
            manager = new QNetworkAccessManager(this);
               connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replayFinished(QNetworkReply*)));
            manager->get(QNetworkRequest(url));


}

modifybookDialog::~modifybookDialog()
{
    delete ui;
}

void modifybookDialog::on_pushButton_clicked()
{
    QSqlQuery q;
    q.exec("update book set introduction='"+ui->textEdit_intro->toPlainText()+"'where bookid='"+ui->lineEdit->text()+"'");
    accept();
    close();
}

void modifybookDialog::on_pushButton_2_clicked()
{
    accept();;
    close();
}
//qrcode
void modifybookDialog::on_pushButton_3_clicked()
{
    QString weburl="http%3A%2F%2F192.168.1.106%2FDefault.aspx%3Fbid%3D"+ui->lineEdit->text()+"%26rid%3D";
    //http%3A%2F%2F192.168.1.106%3A59950%2FWebSite1%2FDefault.aspx%3Fbid%3D你%26rid%3D额
    QString str("     Information     \nBookID:"+ui->lineEdit->text()+"\nBookName："
                 +ui->lineEdit_3->text()+"\nAuthor："+ui->lineEdit_4->text()+"\nStore："+ui->lineEdit_8->text()+"\nIntroduction"+ui->textEdit_intro->toPlainText()+"\n");

    //在线生成Qrcode的url
    QString url ="http://tool.oschina.net/action/qrcode/generate?data="+weburl
            +"&output=image%2Fpng&error=L&type=6&margin=0&size=4";
    //服务器生成
    url="http://"+serverip+":8886/WebForm1.aspx?id="+getbookitems+"&url="+str;
    ui->webView_2->load(url);
    QThread::msleep(20);
    url="http://"+serverip+":8886/image/"+getbookitems+".jpg";
    manager->get(QNetworkRequest(url));
    QSqlQuery q;
    q.exec("update book set qrcode='"+url+"'where bookid='"+ui->lineEdit->text()+"'");
    q.next();
}
void modifybookDialog::replayFinished(QNetworkReply *replay)
{
   //解析html
   QByteArray html=(replay->readAll());

   QPixmap photo;
   photo.loadFromData(html, "PNG");
   ui->label_qrc->setScaledContents(true);
   ui->label_qrc->setPixmap(photo);
}

