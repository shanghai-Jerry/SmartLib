#include "modifyreaderdialog.h"
#include "ui_modifyreaderdialog.h"
#include<QTextCodec>
extern QString userid;
extern QString serverip;
extern QString statuswhole;
modifyreaderDialog::modifyreaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifyreaderDialog)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replayFinished(QNetworkReply*)));
    ui->pushButton_showqrc->setEnabled(false);
    ui->lineEdit_10->setEchoMode(QLineEdit::Password);
    ui->lineEdit_12->addItem(QString::fromLocal8Bit("Male"));
    ui->lineEdit_12->addItem(QString::fromLocal8Bit("Female"));
    ui->lineEdit_14->addItem(QString::fromLocal8Bit("Student"));
    ui->lineEdit_14->addItem(QString::fromLocal8Bit("Teacher"));
    ui->lineEdit_14->addItem(QString::fromLocal8Bit("Admin"));

    ui->lineEdit_9->setReadOnly(true);
    ui->calendarWidget_2->hide();
    db=QSqlDatabase::addDatabase("QMYSQL3","modifyr");
    db.setHostName(serverip);
    db.setDatabaseName("mydb"); // 我们之前建立的数据库
    db.setUserName("root"); // 我们创建的用户名
    db.setPassword("!aq8813797769101!&)"); //用户的密码
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
 //   ui->comboBox_rtype->addItem(QString::fromLocal8Bit("Normal"));
//    ui->comboBox_rtype->addItem(QString::fromLocal8Bit("特殊"));
//    ui->comboBox_rtype->addItem(QString::fromLocal8Bit("其他"));
    q.exec("select rtype from readertype");
    while(q.next())
    {
        ui->comboBox_rtype->addItem(q.value(0).toString());
    }
    if(statuswhole==("Admin"))
        {

    q.exec("select rid,pwd,name,sex,birth,major,status,rtype,images,qrcode from admin where rid='"+userid+"'");
        }
    else
    {
     q.exec("select rid,pwd,name,sex,birth,major,status,rtype,images,qrcode from reader where rid='"+userid+"'");
    }
    q.next();
    for(int index=1; index<8; index++){
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
              ui->comboBox_rtype->setCurrentText(*(i+6));

              QPixmap photo;
              photo.loadFromData(q.value(8).toByteArray(), "PNG");
              ui->label_headpict_2->setScaledContents(true);
              ui->label_headpict_2->setPixmap(photo);

              QString url=q.value(9).toString();
//              ui->webView->load(url);
//              ui->webView->setDisabled(true);
              manager->get(QNetworkRequest(url));
              camera=new QCamera(this);
              viewfinder=new QCameraViewfinder(this);
              imageCapture=new QCameraImageCapture(camera);
              ui->horizontalLayout_capture_2->addWidget(viewfinder);
              ui->label_headpict_2->setScaledContents(true);
              //ui->pushButton_Save->setEnabled(false);
              connect(ui->pushButton_Capture_2, SIGNAL(clicked()), this, SLOT(captureImage()));
              connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));
              connect(ui->pushButton_dataset_3,SIGNAL(clicked()),ui->calendarWidget_2,SLOT(show()));
              connect(ui->pushButton_dataset_3,SIGNAL(clicked()),ui->pushButton_dataset_3,SLOT(hide()));
              connect(ui->calendarWidget_2,SIGNAL(clicked(const QDate &)),this,SLOT(setdate()));
}
//日历设置时间
void modifyreaderDialog::setdate()
{
  QDate date=ui->calendarWidget_2->selectedDate();
  QString dtstr=date.toString("yyyy-MM-dd");
  ui->lineEdit_8->setText(dtstr);
  ui->calendarWidget_2->hide();
  ui->pushButton_dataset_3->setVisible(true);

}
//获取图片
void modifyreaderDialog::captureImage()
{

    //ui->label_picstatus->setText(tr("此处获取图片..."));
    imageCapture->capture();
    //ui->label_picstatus->setHidden(true);


}
//显示图片
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
modifyreaderDialog::~modifyreaderDialog()
{
    delete ui;
}
//确定
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
        if(statuswhole==("Admin"))
            {
             sql ="update admin set pwd=?,name=?,sex=?,birth=?,major=?,status=?,rtype=? where rid='"+userid+"'";
            }
        else{
             sql ="update reader set pwd=?,name=?,sex=?,birth=?,major=?,status=?,rtype=? where rid='"+userid+"'";
        }

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
    if(statuswhole==("Admin"))
        {
         sql ="update admin set pwd=?,name=?,sex=?,birth=?,major=?,status=?,images =?,rtype=? where rid='"+userid+"'";
        }
    else
    {
    sql ="update reader set pwd=?,name=?,sex=?,birth=?,major=?,status=?,images =?,rtype=? where rid='"+userid+"'";
    }
    query.prepare(sql);
    query.addBindValue(pwd);
    query.addBindValue(name);
    query.addBindValue(sex);
    query.addBindValue(birth);
    query.addBindValue(major);
    query.addBindValue(status);
    query.addBindValue(rtype);
    query.addBindValue(var);
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
//取消
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
    ui->pushButton_takepicture_2->setText(QString::fromLocal8Bit("Retake"));

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

//本地图片上传
void modifyreaderDialog::on_pushButton_localpict_clicked()
{
    openPicture();
}

//generate qrcode
void modifyreaderDialog::on_pushButton_qrcoode_clicked()
{                                 //example:192.168.1.106
    QString weburl="http%3A%2F%2F"+serverip+"%2FDefault2.aspx%3Fbid%3D0%26rid%3D"+userid;
    QString url ="http://tool.oschina.net/action/qrcode/generate?data="+weburl
            +"&output=image%2Fpng&error=L&type=6&margin=0&size=4";

    url="http://"+serverip+":8886/WebForm1.aspx?id="+userid+"&url=http://"+serverip+"/Default2.aspx?bid=0";
    ui->webView->load(url);
   // QThread::sleep(20);
    //QDesktopServices::openUrl(url);
    url="http://"+serverip+":8886/image/"+userid+".jpg";
   // ui->webView->load(url);
    ui->webView->setDisabled(true);
    //ui->pushButton_qrcoode->setEnabled(false);
    QSqlQuery q;
    q.exec("update reader set qrcode='"+url+"'where rid='"+userid+"'");
    q.next();
    ui->pushButton_showqrc->setEnabled(true);
}
//
void modifyreaderDialog::replayFinished(QNetworkReply *replay)
    {
       //解析html
       QByteArray html=(replay->readAll());
       QPixmap photo;
       photo.loadFromData(html,"PNG");
       ui->label_qrc->setScaledContents(true);
       ui->label_qrc->setPixmap(photo);
       if(replay->isFinished())
        replay->close();
    }
//
void modifyreaderDialog::on_pushButton_showqrc_clicked()
{
     QString url;
      url="http://"+serverip+":8886/image/"+userid+".jpg";
     manager->get(QNetworkRequest(url));
}
