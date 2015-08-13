#include "informationdialog.h"
#include "ui_informationdialog.h"
#include<QTextCodec>
#include<QDebug>
#include<QBuffer>
#include<QMessageBox>
#include <QSqlError>
extern QString userid;
informationDialog::informationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::informationDialog)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    ui->setupUi(this);
    setFixedSize(this->width(), this->height()); // 禁止改变窗口大小
    QString runPath =QCoreApplication::applicationDirPath();
    db=QSqlDatabase::addDatabase("QMYSQL3","infordriver");
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


    getinfor();
    showdata();
    ui->groupBox_xiugai->hide();

}
void informationDialog::showdata()
{
    model= new QSqlTableModel(this,db);
    model->setTable("readertype");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, ("Reader type"));
    model->setHeaderData(1, Qt::Horizontal, ("Borrow Limit"));
    model->setHeaderData(2, Qt::Horizontal,("Renew"));
    ui->tableView_rtype->setModel(model);
    ui->tableView_rtype->setEditTriggers(false);
    ui->tableView_rtype->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_rtype->show();

}
void informationDialog::getinfor()
{
    QSqlQuery query;
    query.exec("select name,status,images,qrcode from admin where rid='"+userid+"'");
    query.next();
    QString name=query.value(0).toString();
    QString status=query.value(1).toString();
    ui->lineEdit_1->setText(userid);
    ui->lineEdit_2->setText(name);
    ui->lineEdit_3->setText(status);
    ui->lineEdit_1->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    ui->lineEdit_4->setReadOnly(true);
    QPixmap photo;
    photo.loadFromData(query.value(2).toByteArray(), "PNG");
    ui->label_pic->setScaledContents(true);
    ui->label_pic->setPixmap(photo);

    QString url=query.value(3).toString();
    ui->webView->setUrl(url);
    ui->webView->setDisabled(true);
    query.exec("select count(*) from borrow where rid='"+userid+"'");
    query.next();
    QString number=query.value(0).toString();
    ui->lineEdit_4->setText(number);


}

informationDialog::~informationDialog()
{
    delete ui;
}
//上传头像
void informationDialog::on_pushButton_upoad_clicked()
{
     //通过路径获取图片
//  QString takepicName =QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选到文件"),QDir::currentPath(),tr("png(*.png)"));
// // ui->lineEdit->setText(takepicName);
//  if(!takepicName.isEmpty())
//  {
//  QSqlQuery query;
//  query.exec("UPDATE READER SET PIC='"+takepicName+"'WHERE RID='"+userid+"'");
//  query.exec("select pic from reader where rid ='"+userid+"'");
//  query.next();
//  takepicName=query.value(0).toString();
// // ui->lineEdit->setText(userid);

//  QImage image(takepicName);
// // ui->lineEdi_test->setText(filename);
//  ui->label_pic->setScaledContents(true);
//  ui->label_pic->setPixmap(QPixmap::fromImage(image));
// }
    openPicture();
}
//close()
void informationDialog::on_pushButton_quit_clicked()
{
    close();

}

void informationDialog::on_pushButton_ok_clicked()
{
    close();
}
void informationDialog::openPicture()
{
    QString g_strCurrentDir;
    QString strImage = QFileDialog::getOpenFileName(
    this,
   "Please Select image file",
    g_strCurrentDir,
   "Image Format (*.png *.jpg *.bmp *.gif)");
    if (strImage.isNull())
    {
    return;
    }
    g_strCurrentDir = QDir(strImage).absolutePath();
    ui->label_pic->setPixmap(QPixmap(strImage).scaled(ui->label_pic->size()));
   //保存图片到数据库中：
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    ui->label_pic->pixmap()->save(&buffer,"PNG");
    QByteArray data;
    QSqlQuery query;
    QString path = strImage;
    QFile* file=new QFile(path); //fileName为二进制数据文件名
    file->open(QIODevice::ReadOnly);
    data = file->readAll();
    file->close();
    QVariant var(data);
//    QString sql = "insert into images(rid,images) values(?,?)";
//    query.prepare(sql);
//    query.addBindValue(userid.toInt());
//    query.addBindValue(var);
     QString sql ="update admin set images =? where rid='"+userid+"'";
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

void informationDialog::displayImage()
{
//   QSqlQuery q("SELECT * FROM images WHERE rid ='"+userid+"'");
//    while (q.next())
//     {
//        if (q.isNull(0) ==false)
//        {
//        QPixmap photo;
//        ui->label_pic->setText(q.value(0).toString());
//        photo.loadFromData(q.value(1).toByteArray(), "PNG");
//        ui->label_pic->setPixmap(photo);
//         qDebug()<<"Query success";
//        }
//        else
//        {
//            qDebug()<<"Query failed";
//        }
//    }
         QSqlQuery q("SELECT images FROM admin WHERE rid ='"+userid+"'");
         q.next();
         QPixmap photo;
         photo.loadFromData(q.value(1).toByteArray(), "PNG");
         ui->label_pic->setPixmap(photo);
          qDebug()<<"Query success";
}

void informationDialog::on_pushButton_add_clicked()
{
   ui->groupBox_xiugai->show();
   ui->pushButton_finish->setText("增加");

   ui->lineEdit_type->clear();
   ui->lineEdit_type->setReadOnly(false);
   ui->lineEdit_borr->clear();
   ui->lineEdit_conti->clear();

   ui->pushButton_add->setEnabled(false);
   ui->pushButton_delete->setEnabled(false);
   ui->pushButton_modfiy->setEnabled(false);
   status=0;

}

void informationDialog::on_pushButton_modfiy_clicked()
{
    QTableView *view=ui->tableView_rtype;
    int n=view->currentIndex().row();
    if(n<0)
    {
       QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"),("Select one row Please"));
       return;
    }
  ui->groupBox_xiugai->show();
  ui->pushButton_finish->setText("修改");

  ui->pushButton_add->setEnabled(false);
  ui->pushButton_delete->setEnabled(false);
  ui->pushButton_modfiy->setEnabled(false);
//  QTableView *view=ui->tableView_rtype;
//  int n=view->currentIndex().row();
  QString rtype=model->data(model->index(n,0)).toString();
  QString se=model->data(model->index(n,1)).toString();
  QString th=model->data(model->index(n,2)).toString();
  ui->lineEdit_type->setText(rtype);
  ui->lineEdit_type->setReadOnly(true);
  ui->lineEdit_borr->setText(se);
  ui->lineEdit_conti->setText(th);
  status=1;

}

void informationDialog::on_pushButton_delete_clicked()
{
  QTableView *view=ui->tableView_rtype;
  int n=view->currentIndex().row();
  if(n<0)
  {
     QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"),("Select one row Please"));
     return;
  }
  ui->groupBox_xiugai->show();
  ui->pushButton_finish->setText("删除");
  ui->pushButton_add->setEnabled(false);
  ui->pushButton_delete->setEnabled(false);
  ui->pushButton_modfiy->setEnabled(false);
  QString rtype=model->data(model->index(n,0)).toString();
  QString se=model->data(model->index(n,1)).toString();
  QString th=model->data(model->index(n,2)).toString();
  ui->lineEdit_type->setText(rtype);
  ui->lineEdit_type->setReadOnly(true);
  ui->lineEdit_borr->setText(se);
  ui->lineEdit_conti->setText(th);
  status=2;

}

void informationDialog::on_pushButton_finish_clicked()
{
     ui->groupBox_xiugai->hide();

     if(status==0)
     {
          QSqlQuery q;
          QString sql;
          sql="insert into readertype values(?,?,?)";
          q.prepare(sql);
          q.addBindValue(ui->lineEdit_type->text());
          q.addBindValue(ui->lineEdit_borr->text());
          q.addBindValue(ui->lineEdit_conti->text());
          if(!q.exec())
          {
              qDebug()<<q.lastError().text().toLocal8Bit().data();
          }
          else

          {
              qDebug()<<"Insert success";
          }
     }
     else if(status==1)
     {
         QSqlQuery q;
         QString sql;
         sql="update readertype set borrlimit=?,conlimit=? where rtype='"+ui->lineEdit_type->text()+"'";
         q.prepare(sql);
         q.addBindValue(ui->lineEdit_borr->text());
         q.addBindValue(ui->lineEdit_conti->text());
         q.exec();
     }

     else
     {
         QSqlQuery q;
         q.exec("select count(*) from reader where rtype='"+ui->lineEdit_type->text()+"'");
         q.next();
         int num=q.value(0).toInt();
         if(num>0)
         {
         QMessageBox::information(NULL, QString::fromLocal8Bit("Tips"),("Type used,Please check"));
         }
         else
         {
         q.exec("delete from readertype where rtype='"+ui->lineEdit_type->text()+"'");
         q.next();
         }
     }

     showdata();
     ui->pushButton_add->setEnabled(true);
     ui->pushButton_delete->setEnabled(true);
     ui->pushButton_modfiy->setEnabled(true);

}

void informationDialog::on_pushButton_back_clicked()
{
    ui->groupBox_xiugai->hide();
    status=0;
    ui->pushButton_add->setEnabled(true);
    ui->pushButton_delete->setEnabled(true);
    ui->pushButton_modfiy->setEnabled(true);

}
