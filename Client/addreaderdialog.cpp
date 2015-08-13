#include "addreaderdialog.h"
#include "ui_addreaderdialog.h"
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QDebug>
#include <QSqlError>
//QString statuswhole;
addreaderDialog::addreaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addreaderDialog)
{
     ui->setupUi(this);
     setWindowIcon(QIcon(":/img/welcome.png"));
     setWindowTitle(("SmartLib"));
     ui->label_message->setText(("Checking..."));
     ui->lineEdit_2->setEchoMode(QLineEdit::Password);
     ui->pushButton_ok->setEnabled(false);
     ui->pushButton_takepicture->setText(("Take"));
     ui->pushButton_Capture->setEnabled(false);
     ui->lineEdit_4->addItem(("Male"));
     ui->lineEdit_4->addItem(("Female"));
     ui->lineEdit_7->addItem(("Student"));
     ui->lineEdit_7->addItem(("Teacher"));
//     ui->lineEdit_7->addItem(("Admin"));
     ui->comboBox_rt->addItem(("Normal"));
     ui->comboBox_rt->addItem(("Special"));
     ui->comboBox_rt->addItem(("Other"));

     ui->lineEdit_5->setText(("Birthday"));
     ui->lineEdit_5->setReadOnly(true);

     rid=ui->lineEdit_1->text();
     pwd = ui->lineEdit_2->text();
     name = ui->lineEdit_3->text();
     sex = ui->lineEdit_4->currentText();
     birth = ui->lineEdit_5->text();
     major = ui->lineEdit_6->text();
     rtype=ui->comboBox_rt->currentText();
     status = ui->lineEdit_7->currentText();
     //dateset
     ui->calendarWidget->hide();
     connect(ui->lineEdit_1,SIGNAL(textChanged(QString)),this,SLOT(on_checkBoxs_clicked()));
     connect(ui->pushButton_dataset,SIGNAL(clicked()),ui->calendarWidget,SLOT(show()));
     connect(ui->pushButton_dataset,SIGNAL(clicked()),ui->pushButton_dataset,SLOT(hide()));
     connect(ui->calendarWidget,SIGNAL(clicked(const QDate &)),this,SLOT(setdate()));
     //picture
     camera=new QCamera(this);
     viewfinder=new QCameraViewfinder(this);
     imageCapture=new QCameraImageCapture(camera);
     ui->horizontalLayout_capture->addWidget(viewfinder);
     ui->label_headpict->setScaledContents(true);
     //ui->pushButton_Save->setEnabled(false);
     connect(ui->pushButton_Capture, SIGNAL(clicked()), this, SLOT(captureImage()));
     connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));

     //connect(ui->pushButton_Save, SIGNAL(clicked()), this, SLOT(saveImage()));

     //databaseconnect
     QString runPath =QCoreApplication::applicationDirPath();
     db=QSqlDatabase::addDatabase("QMYSQL3","addr");
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
     ui->comboBox_rt->clear();

     QSqlQuery q;
     q.exec("select rtype from readertype");
     while(q.next())
     {
         ui->comboBox_rt->addItem(q.value(0).toString());

     }

//     ui->comboBox_rt->addItem(("普通"));
//     ui->comboBox_rt->addItem(("特殊"));


}

//获取图片
void addreaderDialog::captureImage()
{

    //ui->label_picstatus->setText(tr("此处获取图片..."));
    imageCapture->capture();
    //ui->label_picstatus->setHidden(true);


}
//显示图片
void addreaderDialog::displayImage(int /*id*/, QImage image)
{
    ui->label_headpict->setPixmap(QPixmap::fromImage(image));
    rid=ui->lineEdit_1->text();
    pwd = ui->lineEdit_2->text();
    name = ui->lineEdit_3->text();
    sex = ui->lineEdit_4->currentText();
    birth = ui->lineEdit_5->text();
    major = ui->lineEdit_6->text();
    status = ui->lineEdit_7->currentText();
    rtype=ui->comboBox_rt->currentText();
    if(!rid.isEmpty()&&!pwd.isEmpty()&&!name.isEmpty()&&!birth.isEmpty()&&!major.isEmpty())
    {
        takepicName =QDir::currentPath()+"/"+rid+".png";
         ui->lineEdit_photopath->setText(takepicName);
        //ui->lineEdit_5->setText(takepicName);
        if(takepicName.isEmpty())
        {
           //ui->label_picstatus->setText(tr("Failure"));
            return;
        }
        const QPixmap *pixmap=ui->label_headpict->pixmap();
        if(pixmap)
        {
            pixmap->save(takepicName);
    //        QByteArray data;
    //        QSqlQuery query;
    //        QFile* file=new QFile(takepicName); //fileName为二进制数据文件名
    //        file->open(QIODevice::ReadOnly);
    //        data = file->readAll();
    //        file->close();
    //        QVariant var(data);
    //        QString sql = "insert into images(rid,images) values(?,?)";
    //       // QString sql = "insert into images(images)values(?)";
    //        query.prepare(sql);
    //        query.addBindValue(rid.toInt());
    //        query.addBindValue(var);
    //        if(!query.exec())
    //        {
    //            qDebug()<<query.lastError().text().toLocal8Bit().data();
    //        }
    //        else
    //        {
    //            qDebug()<<"Insert success";
    //        }
        }
      ui->pushButton_ok->setEnabled(true);
    }
    else
    {
       QMessageBox::information(NULL, ("Tips"), ("Please complete"));
       ui->pushButton_ok->setEnabled(false);
    }

}
//日历设置时间
void addreaderDialog::setdate()
{
  QDate date=ui->calendarWidget->selectedDate();
  QString dtstr=date.toString("yyyy-MM-dd");

  ui->lineEdit_5->setText(dtstr);
  ui->calendarWidget->hide();
  ui->pushButton_dataset->setVisible(true);

}
void addreaderDialog::addreaderchange()
{

      QSqlQuery q;
      q.exec("select count(*) from admin where rid='"+ui->lineEdit_1->text()+"'");
      q.next();
      int rowcnt = q.value(0).toInt();
      if(rowcnt>0)
      {
        QList<QString> qresult;
        ui->label_message->setText("Exists!!");
//        q.exec("select rid,pwd,name,sex,birth,major,status,images from reader where rid='"+ui->lineEdit_1->text()+"'");
//        q.next();
//        for(int index= 1; index<7; index++){
//                  qresult.append(q.value(index).toString());
//              }
//                  QList<QString>::iterator i;
//                  i = qresult.begin();
//                  ui->lineEdit_2->setText(*i);
//                  ui->lineEdit_3->setText(*(i+1));
//                  ui->lineEdit_4->setCurrentText(*(i+2));
//                  ui->lineEdit_5->setText(*(i+3));
//                  ui->lineEdit_6->setText(*(i+4));
//                  ui->lineEdit_7->setCurrentText(*(i+5));
//                  QPixmap photo;
//                  photo.loadFromData(q.value(7).toByteArray(), "PNG");
//                  ui->label_headpict->setScaledContents(true);
//                  ui->label_headpict->setPixmap(photo);
      }
      else
      {
        q.exec("select count(*) from reader where rid='"+ui->lineEdit_1->text()+"'");
        q.next();
          int rowcnt2 = q.value(0).toInt();
          if(rowcnt2>0)
              ui->label_message->setText(("Exists!!"));
        else
        ui->label_message->setText(("ok"));
        ui->lineEdit_5->setText(("Birthday"));

      }
}
addreaderDialog::~addreaderDialog()
{
    delete ui;
    db.close();
}

void addreaderDialog::on_pushButton_2_clicked()
{
    accept();
    close();
}

void addreaderDialog::on_pushButton_ok_clicked()
{
               //QSqlQuery query;
               QByteArray data;
               QSqlQuery query;
               QFile* file;
               if(takepicName.isEmpty())
                 file=new QFile(":/img/logininback.png"); //fileName为二进制数据文件名
               else
               {
               file=new QFile(takepicName); //使用拍照功能用此语句
               }
               file->open(QIODevice::ReadOnly);
               data = file->readAll();
               file->close();
               QVariant var(data);
               QString sql ;
               if(ui->lineEdit_7->currentText()==("管理员"))
                 sql= "insert into admin(rid,pwd,name,sex,birth,major,rtype,status,images)values(?,?,?,?,?,?,?,?,?)";
                else
                 sql= "insert into reader(rid,pwd,name,sex,birth,major,rtype,status,images)values(?,?,?,?,?,?,?,?,?)";
              // QString sql = "insert into images(images)values(?)";
               query.prepare(sql);
               query.addBindValue(rid);
               query.addBindValue(pwd);
               query.addBindValue(name);
               query.addBindValue(sex);
               query.addBindValue(birth);
               query.addBindValue(major);
               query.addBindValue(rtype);
               query.addBindValue(status);
               query.addBindValue(var);
               if(!query.exec())
               {
                   qDebug()<<query.lastError().text().toLocal8Bit().data();
               }
               else

               {
                   qDebug()<<"Insert success";
               }


    //db.close();
    accept();
    QMessageBox::information(NULL, ("Tips"), tr("Add Success"));

    close();


}

void addreaderDialog::on_checkBoxs_clicked()
{

       addreaderchange();

}
//take pict
void addreaderDialog::on_pushButton_takepicture_clicked()
{
    camera->setViewfinder(viewfinder);
    camera->start();
    ui->pushButton_Capture->setEnabled(true);
    ui->pushButton_takepicture->setText(("Retake"));


}
//上传图片
void addreaderDialog::on_pushButton_upload_clicked()
{
    openPicture();
}
void addreaderDialog::openPicture()
{
    rid=ui->lineEdit_1->text();
    pwd = ui->lineEdit_2->text();
    name = ui->lineEdit_3->text();
    sex = ui->lineEdit_4->currentText();
    birth = ui->lineEdit_5->text();
    major = ui->lineEdit_6->text();
    rtype=ui->comboBox_rt->currentText();
    status = ui->lineEdit_7->currentText();
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
    ui->label_headpict->setPixmap(QPixmap(strImage).scaled(ui->label_headpict->size()));

   //保存图片到数据库中：
    QString path = strImage;
    takepicName =path;
    ui->lineEdit_photopath->setText(path);
    if(!rid.isEmpty()&&!pwd.isEmpty()&&!name.isEmpty()&&!birth.isEmpty()&&!major.isEmpty())
    {
       ui->pushButton_ok->setEnabled(true);
    }
    else
    {
     QMessageBox::information(NULL, ("Tips"), ("Please complete"));

    }
}
