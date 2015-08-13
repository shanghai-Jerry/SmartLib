#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include<QDir>
#include<QDebug>
addbookDialog::addbookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addbookDialog)
{
    ui->setupUi(this);
    ui->checkBox->setHidden(true);
    ui->label_message->setText(QString::fromLocal8Bit("Checking..."));
    setWindowIcon(QIcon(":/img/welcome.png"));
    setWindowTitle(QString::fromLocal8Bit("SmartLibrary"));
    connect(ui->lineEdit_1,SIGNAL(textChanged(QString)),ui->checkBox,SLOT(click()));
    connect(this,SIGNAL(close()),this,SLOT(accept()));
    ui->pushButt_ok->setEnabled(false);
    QString runPath =QCoreApplication::applicationDirPath();
     db=QSqlDatabase::addDatabase("QMYSQL3","addbookdriver");
     db.setHostName("localhost");
     db.setDatabaseName("mydb"); // ï¿½ï¿½ï¿½ï¿½Ö®Ç°ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý¿ï¿½
     db.setUserName("root"); // ï¿½ï¿½ï¿½Ç´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã»ï¿½ï¿½ï¿½
     db.setPassword("!aq8813797769101!&)"); //ï¿½Ã»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
     //db.setDatabaseName(runPath+"/mydb.mdb");
     if(db.open())
        {
            qDebug()<<"database is established!";
        }
        else
        {
            qDebug()<<"database is established error!";
        }



}
void addbookDialog::closeEvent(QCloseEvent *event)
{
    accept();
}
void addbookDialog::addbookchange()
{

      QSqlQuery q;
      q.exec("select count(*) from Book where bookid='"+ui->lineEdit_1->text()+"'");
      q.next();
      int rowcnt = q.value(0).toInt();
      if(rowcnt>0)
      {

        QList<QString> qresult;
        ui->label_message->setText(QString::fromLocal8Bit("Already Have"));
        q.exec("select bookid,isbn,bookname,author,publish,pubtime,price,storenum from Book where bookid='"+ui->lineEdit_1->text()+"'");
        q.next();
        for(int index= 1; index<8; index++) {
                  qresult.append(q.value(index).toString());
              }
                  QList<QString>::iterator i;
                  i = qresult.begin();
                  ui->lineEdit_2->setText(*i);
                  ui->lineEdit_3->setText(*(i+1));
                  ui->lineEdit_4->setText(*(i+2));
                  ui->lineEdit_5->setText(*(i+3));
                  ui->lineEdit_6->setText(*(i+4));
                  ui->lineEdit_7->setText(*(i+5));
                  ui->lineEdit_8->setText(*(i+6));

      }
      else
      {
        ui->label_message->setText(QString::fromLocal8Bit("OK"));
      }
}

addbookDialog::~addbookDialog()
{

    db.close();
    delete ui;

}
//Í¼ï¿½ï¿½ï¿½ï¿½ï¿½Ó²ï¿½ï¿½ï¿½
void addbookDialog::on_pushButt_ok_clicked()
{

    QString bookid=ui->lineEdit_1->text();
    QString isbn = ui->lineEdit_2->text();
    QString bookname = ui->lineEdit_3->text();
    QString author = ui->lineEdit_4->text();
    QString publish = ui->lineEdit_5->text();
    QString pubtime = ui->lineEdit_6->text();
    QString price = ui->lineEdit_7->text();
    QString storenum = ui->lineEdit_8->text();
    if(storenum.toInt()<=0)
    {
        QMessageBox::information(NULL, tr("Tips"),QString::fromLocal8Bit("Wrong number!"));
        return;
    }
  if(!bookid.isEmpty()&&!isbn.isEmpty()&&!bookname.isEmpty()&&!author.isEmpty()&&!publish.isEmpty()&&!pubtime.isEmpty()&&!price.isEmpty()&&!storenum.isEmpty())
   {
       QSqlQuery q;
       q.exec("select count(*) from book where bookid='"+bookid+"'");
       q.next();
       int rowcnt = q.value(0).toInt();
       if(rowcnt>0)
       {

           QMessageBox msgBox;
           msgBox.setWindowTitle("Tips:");
           msgBox.setText(QString::fromLocal8Bit("Update now"));
           QPushButton *connectButton = msgBox.addButton(tr("Update"), QMessageBox::ActionRole);
           QPushButton *abortButton = msgBox.addButton(tr("Cancel"),QMessageBox::ActionRole);

           msgBox.exec();

           if (msgBox.clickedButton() == connectButton)
           {
               // connect

               QSqlQuery q;
               q.exec("select storenum from book where bookid='"+bookid+"'");
               q.next();
               int store=q.value(0).toInt();
               store++;
               storenum = QString::number(store);
               q.exec("update book set storenum = '"+storenum+"'where bookid ='"+bookid+"'");
               q.next();
               QMessageBox::information(NULL, tr("Tips"),QString::fromLocal8Bit("Number Update!"));
               accept();

           }
           else if (msgBox.clickedButton() == abortButton)
           {
               close();
           }
       }

       else
       {

             q.exec("insert into book(bookid,isbn,bookname,author,publish,pubtime,price,storenum)values('"+bookid+"','"+isbn+"','"+bookname+"','"+author+"','"+publish+"','"+pubtime+"','"+price+"','"+storenum+"')");
             savePicture();
             QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("Add Success!"));
             accept();
       }

  }
  else
  {
      QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("Please complete!"));

  }
   //accept();
}
//È¡ï¿½ï¿½ï¿½ï¿½Å¥
void addbookDialog::on_pushButton_cancel_clicked()
{
    accept();
    close();
}

//checkbox
void addbookDialog::on_checkBox_clicked()
{
    if(!ui->lineEdit_1->text().isEmpty())
    {
        if(ui->checkBox->isChecked())
        {
          ui->pushButt_ok->setEnabled(true);
           addbookchange();
        }
       else
       {
       ui->lineEdit_2->clear();
       ui->lineEdit_3->clear();
       ui->lineEdit_4->clear();
       ui->lineEdit_5->clear();
       ui->lineEdit_6->clear();
       ui->lineEdit_7->clear();
       ui->lineEdit_8->clear();
       ui->label_message->setText(QString::fromLocal8Bit(""));

       }
    }

 }
//UPLOAD PIC
void addbookDialog::on_pushButton_upload_clicked()
{
    openPicture();
}
void addbookDialog::openPicture()
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
void addbookDialog::savePicture()
{

    //±£´æÍ¼Æ¬µ½Êý¾Ý¿âÖÐ£º

     QByteArray data;
     QSqlQuery query;
     QFile* file;
     if(takepicName.isEmpty())
     {
        return;
     }
     else
     {
     file=new QFile(takepicName); //Ê¹ÓÃÅÄÕÕ¹¦ÄÜÓÃ´ËÓï¾ä
     }
     file->open(QIODevice::ReadOnly);
     data = file->readAll();
     file->close();
     QString sql;
     QVariant var(data);
       sql="insert into book(images)values(?)";
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
