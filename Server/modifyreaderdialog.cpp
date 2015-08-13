#include "modifyreaderdialog.h"
#include "ui_modifyreaderdialog.h"
#include<QTextCodec>
extern QString userid;
extern QString connAddr;
modifyreaderDialog::modifyreaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifyreaderDialog)
{

    ui->setupUi(this);
    ui->lineEdit_10->setEchoMode(QLineEdit::Password);
    ui->lineEdit_photopath_2->setReadOnly(true);
    ui->lineEdit_12->addItem(QString::fromLocal8Bit("Male"));
    ui->lineEdit_12->addItem(QString::fromLocal8Bit("Female"));
//    ui->lineEdit_14->addItem(QString::fromLocal8Bit("学生"));
//    ui->lineEdit_14->addItem(QString::fromLocal8Bit("老师"));
    ui->lineEdit_14->addItem(QString::fromLocal8Bit("Admin"));
    ui->lineEdit_9->setReadOnly(true);
    ui->calendarWidget_2->hide();
    ui->pushButton_showqrc->setEnabled(false);
    db=QSqlDatabase::addDatabase("QMYSQL3","modifyr");
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

    QList<QString> qresult;
    QSqlQuery q;
    int  rtypenum=0;
    q.exec("select rtype from readertype");
    while(q.next())
    {
        ui->comboBox_rtype->addItem(q.value(0).toString());
    }
    q.exec("select rid,pwd,name,sex,birth,major,status,images,qrcode,rtype from admin where rid='"+userid+"'");
    q.next();
    for(int index=1; index<7; index++){
              qresult.append(q.value(index).toString());
          }
              QList<QString>::iterator i;
              i = qresult.begin();
              ui->lineEdit_9->setText(userid);
              ui->lineEdit_10->setText(*i);
              ui->lineEdit_11->setText(*(i+1));

              ui->lineEdit_12->setCurrentText(*(i+2));
              ui->lineEdit_8->setText(*(i+3));
              ui->lineEdit_13->setText(*(i+4));
              ui->lineEdit_14->setCurrentText(*(i+5));

              QPixmap photo;
              photo.loadFromData(q.value(7).toByteArray(), "PNG");
              ui->label_headpict_2->setScaledContents(true);
              ui->label_headpict_2->setPixmap(photo);
              QString url =q.value(8).toString();
              if(!url.isEmpty())
              {
              ui->webView->load(url);
              ui->webView->setDisabled(true);
              }
//              if(!url.isEmpty())
//              ui->pushButton_qrcode->setEnabled(false);
              QString rtype =q.value(9).toString();
              ui->comboBox_rtype->setCurrentText(rtype);

              camera=new QCamera(this);
              viewfinder=new QCameraViewfinder(this);
              imageCapture=new QCameraImageCapture(camera);
              ui->horizontalLayout_capture_2->addWidget(viewfinder);
              ui->label_headpict_2->setScaledContents(true);
              //ui->pushButton_Save->setEnabled(false);
              connect(ui->pushButton_Capture_2, SIGNAL(clicked()), this, SLOT(captureImage()));
              connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));
              connect(ui->pushButton_dataset_2,SIGNAL(clicked()),ui->calendarWidget_2,SLOT(show()));
              connect(ui->pushButton_dataset_2,SIGNAL(clicked()),ui->pushButton_dataset_2,SLOT(hide()));
              connect(ui->calendarWidget_2,SIGNAL(clicked(const QDate &)),this,SLOT(setdate()));
}

void modifyreaderDialog::setdate()
{
  QDate date=ui->calendarWidget_2->selectedDate();
  QString dtstr=date.toString("yyyy-MM-dd");
  ui->lineEdit_8->setText(dtstr);
  ui->calendarWidget_2->hide();
  ui->pushButton_dataset_2->setVisible(true);

}

void modifyreaderDialog::captureImage()
{

    //ui->label_picstatus->setText(tr("获取图片..."));
    imageCapture->capture();
    //ui->label_picstatus->setHidden(true);


}

void modifyreaderDialog::displayImage(int /*id*/, QImage image)
{
         ui->label_headpict_2->setPixmap(QPixmap::fromImage(image));
         takepicName =QDir::currentPath()+"/"+userid+".png";
         ui->lineEdit_photopath_2->setText(takepicName);
        //ui->lineEdit_5->setText(takepicName);
        if(takepicName.isEmpty())
        {
           //ui->label_picstatus->setText(tr("Failure"));
            return;
        }
        const QPixmap *pixmap=ui->label_headpict_2->pixmap();
        if(pixmap)
        {
            pixmap->save(takepicName);
    //        QByteArray data;
    //        QSqlQuery query;
    //        QFile* file=new QFile(takepicName);
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
modifyreaderDialog::~modifyreaderDialog()
{
    delete ui;
}

void modifyreaderDialog::on_pushButton_ok_clicked()
{

    pwd=ui->lineEdit_10->text();
    name=ui->lineEdit_11->text();
    sex=ui->lineEdit_12->currentText();
    birth=ui->lineEdit_8->text();
    major=ui->lineEdit_13->text();
    status=ui->lineEdit_14->currentText();
    rtype=ui->comboBox_rtype->currentText();
    QByteArray data;
    QSqlQuery query;
    QString sql;
    QFile* file;
    if(ui->lineEdit_photopath_2->text().isEmpty())
    {
        sql ="update admin set pwd=?,name=?,sex=?,birth=?,major=?,status=?,rtype=? where rid='"+userid+"'";
        query.prepare(sql);
        query.addBindValue(pwd);
        query.addBindValue(name);
        query.addBindValue(sex);
        query.addBindValue(birth);
        query.addBindValue(major);
        query.addBindValue(status);
        query.addBindValue(rtype);
        if(!query.exec())
        {
            qDebug()<<query.lastError().text().toLocal8Bit().data();
        }
        else

        {
            qDebug()<<"update success";
        }
        accept();
        close();
    }
    else
    {
    takepicName=ui->lineEdit_photopath_2->text();
    file=new QFile(takepicName);
    file->open(QIODevice::ReadOnly);
    data = file->readAll();
    file->close();
    QVariant var(data);
    sql ="update admin set pwd=?,name=?,sex=?,birth=?,major=?,status=?,images =?,rtype=? where rid='"+userid+"'";
    query.prepare(sql);
    query.addBindValue(pwd);
    query.addBindValue(name);
    query.addBindValue(sex);
    query.addBindValue(birth);
    query.addBindValue(major);
    query.addBindValue(status);
    query.addBindValue(var);
    query.addBindValue(rtype);
    if(!query.exec())
    {
        qDebug()<<query.lastError().text().toLocal8Bit().data();
    }
    else

    {
        qDebug()<<"update success";
    }
    accept();
    close();
    }

}

void modifyreaderDialog::on_pushButton_2_clicked()
{
    close();
}
//
void modifyreaderDialog::on_pushButton_takepicture_2_clicked()
{
    camera->setViewfinder(viewfinder);
    camera->start();
    ui->pushButton_Capture_2->setEnabled(true);
    ui->pushButton_takepicture_2->setText(QString::fromLocal8Bit("ReTake"));

}

//Qrcode
void modifyreaderDialog::on_pushButton_qrcode_clicked()
{
    //QString str=(QString::fromLocal8Bit("基本信息")+"\n"+QString::fromLocal8Bit("读者编号：")+userid+"\n"+QString::fromLocal8Bit("读者姓名：")+ui->lineEdit_11->text());
    QString weburl="http%3A%2F%2F192.168.1.106%2FDefault2.aspx%3Fbid%3D1%26rid%3D"+userid;

    //在线生成qrcode的url
    QString url ="http://tool.oschina.net/action/qrcode/generate?data="+weburl
            +"&output=image%2Fpng&error=L&type=6&margin=0&size=4";

    //服务器生成（此bid用作标记）
    url="http://localhost:8886/WebForm1.aspx?id="+userid+"&url=http://"+connAddr+"/Default2.aspx?bid=1";
    ui->webView_2->load(url);
    //ui->pushButton_qrcode->setEnabled(false);
    //QThread::sleep(10);
    ui->pushButton_showqrc->setEnabled(true);
    url="http://"+connAddr+":8886/image/"+userid+".jpg";
    QSqlQuery q;
    q.exec("update admin set qrcode='"+url+"'where rid='"+userid+"'");
    q.next();

}
//show qrc
void modifyreaderDialog::on_pushButton_showqrc_clicked()
{
   // QString url="http://"+connAddr+":8886/image/"+userid+".jpg";
    QSqlQuery q;
    q.exec("select qrcode from admin where rid='"+userid+"'");
    q.next();
    QString url=q.value(0).toString();
    ui->webView->load(url);
    ui->webView->setDisabled(true);

}
void modifyreaderDialog::openPicture()
{
    QString g_strCurrentDir;
    QString strImage = QFileDialog::getOpenFileName(
    this,
   "Please Select image file",
    g_strCurrentDir,
   "Image Format (*.png)");
    if (strImage.isNull())
    {
    return;
    }
    g_strCurrentDir = QDir(strImage).absolutePath();
    ui->lineEdit_photopath_2->setText(g_strCurrentDir);
    ui->label_headpict_2->setPixmap(QPixmap(strImage).scaled(ui->label_headpict_2->size()));
    takepicName=g_strCurrentDir;
}
//本地图片
void modifyreaderDialog::on_pushButton_clicked()
{
   openPicture();
}


void modifyreaderDialog::on_pushButton_dataset_3_clicked()
{

}
