#include "modifybookdialog.h"
#include "ui_modifybookdialog.h"
extern QString getbookitems;
extern QString connAddr;
modifybookDialog::modifybookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifybookDialog)
{
    ui->setupUi(this);
    ui->pushButton_4->hide();
    ui->lineEdit->setReadOnly(true);
    db=QSqlDatabase::addDatabase("QMYSQL3","mod");
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
           ui->webView->load(url);
           ui->webView->setDisabled(true);
           if(!url.isEmpty())
           {
            //ui->pushButton_3->setEnabled(false);

           }

          q.exec("select images from book where bookid='"+getbookitems+"'");
          q.next();
          QPixmap photo;
          photo.loadFromData(q.value(0).toByteArray(), "jpg");//图片类型
          ui->label_headpict_2->setScaledContents(true);
          ui->label_headpict_2->setPixmap(photo);


}

modifybookDialog::~modifybookDialog()
{
    delete ui;
}

void modifybookDialog::on_pushButton_clicked()
{
    QSqlQuery q;
            q.exec("update book set isbn='"+ui->lineEdit_2->text()+ "' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book set bookname='"+ui->lineEdit_3->text()+ "' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book set author='"+ui->lineEdit_4->text()+ "' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book set publish='"+ui->lineEdit_5->text()+ "' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book set  pubtime='"+ui->lineEdit_6->text()+ "' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book setprice='"+ui->lineEdit_7->text()+ "' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book set storenum='"+ui->lineEdit_8->text()+"' where bookid='"+ui->lineEdit->text()+"'");
            q.exec("update book set introduction='"+ui->textEdit_intro->toPlainText()+"' where bookid='"+ui->lineEdit->text()+"'");
    q.next();
    savePicture();
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
    QString str("    Information    \nBookID:"+ui->lineEdit->text()+"\nBookName："
                 +ui->lineEdit_3->text()+"\nAuthor："+ui->lineEdit_4->text()+"\nStore："+ui->lineEdit_8->text()+"\n");//注意二维码的编码问题
    //在线生成Qrcode的url
    QString url ="http://tool.oschina.net/action/qrcode/generate?data="+weburl
            +"&output=image%2Fpng&error=L&type=6&margin=0&size=4";

    //服务器生成
    url="http://localhost:8886/WebForm1.aspx?id="+getbookitems+"&url="+str;
    ui->webView_2->load(url);

    //ui->pushButton_3->setEnabled(false);

    url="http://"+connAddr+":8886/image/"+getbookitems+".jpg";
    QSqlQuery q;
    q.exec("update book set qrcode='"+url+"'where bookid='"+getbookitems+"'");
    q.next();
    ui->pushButton_4->show();

//    q.exec("select qrcode from book where bookid='"+getbookitems+"'");
//    q.next();
//    QString url2=q.value(0).toString();
//    ui->webView->load(url2);
//    ui->webView->setDisabled(true);
}
//upload pic
void modifybookDialog::on_pushButton_upload_clicked()
{
    openPicture();
}
void modifybookDialog::openPicture()
{

    QString g_strCurrentDir;
    QString strImage = QFileDialog::getOpenFileName(
    this,
   "Please Select image file",
    g_strCurrentDir,
   "Image Format (*.jpg)");
    if (strImage.isNull())
    {
    return;
    }
    g_strCurrentDir = QDir(strImage).absolutePath();
    if(!strImage.isEmpty())
    {
    takepicName = strImage;
    ui->label_headpict_2->setPixmap(QPixmap(strImage).scaled(ui->label_headpict_2->size()));
    }


}
void modifybookDialog::savePicture()
{

    //保存图片到数据库中：
     QByteArray data;
     QSqlQuery query;
     QFile* file;
     if(takepicName.isEmpty())
     {
         return;
     }
     else
     {
     file=new QFile(takepicName); //使用拍照功能用此语句
     }
     file->open(QIODevice::ReadOnly);
     data = file->readAll();
     file->close();
     QString sql;
     QVariant var(data);
       sql="update book set images =? where bookid='"+ui->lineEdit->text()+"'";
     query.prepare(sql);
     query.addBindValue(var);
     if(!query.exec())
     {
         qDebug()<<query.lastError().text().toLocal8Bit().data();
     }
     else

     {
         qDebug()<<"Insert success";
     }
}
//show qrc
void modifybookDialog::on_pushButton_4_clicked()
{
    QSqlQuery q;
    q.exec("select qrcode from book where bookid='"+getbookitems+"'");
    q.next();
    QString url2=q.value(0).toString();
    ui->webView->load(url2);
    ui->webView->setDisabled(true);
}
