#include "addreaderdialog.h"
#include "ui_addreaderdialog.h"
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QDebug>
#include <QSqlError>
QString statuswhole;
addreaderDialog::addreaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addreaderDialog)
{
     ui->setupUi(this);
     rid=ui->lineEdit_1->text();
     pwd = ui->lineEdit_2->text();
     name = ui->lineEdit_3->text();
     sex = ui->lineEdit_4->currentText();
     birth = ui->lineEdit_5->text();
     major = ui->lineEdit_6->text();
     rtype=ui->comboBox_rt->currentText();
     status = ui->lineEdit_7->currentText();
     setWindowIcon(QIcon(":/img/welcome.png"));
     setWindowTitle(QString::fromLocal8Bit("SmartLibrary"));
     ui->label_message->setText(QString::fromLocal8Bit("Checking..."));
     ui->lineEdit_2->setEchoMode(QLineEdit::Password);
     ui->pushButton_ok->setEnabled(false);
      ui->pushButton_takepicture->setText(QString::fromLocal8Bit("Take Picture"));
     ui->pushButton_Capture->setEnabled(false);
     ui->lineEdit_4->addItem(QString::fromLocal8Bit("Male"));
     ui->lineEdit_4->addItem(QString::fromLocal8Bit("Femal"));
//     ui->lineEdit_7->addItem(QString::fromLocal8Bit("Student"));
//     ui->lineEdit_7->addItem(QString::fromLocal8Bit("Teacher"));
     ui->lineEdit_7->addItem(QString::fromLocal8Bit("Admin"));

     ui->lineEdit_5->setText(QString::fromLocal8Bit("Set Birthday"));
     ui->lineEdit_5->setReadOnly(true);
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
     db.setDatabaseName("mydb"); // ����֮ǰ���������ݿ�
     db.setUserName("root"); // ���Ǵ������û���
     db.setPassword("!aq8813797769101!&)"); //�û�������
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
     int  rtypenum=0;
//     ui->comboBox_rt->addItem(("��ͨ"));
//     ui->comboBox_rt->addItem(("����"));
//     ui->comboBox_rt->addItem(("����"));
     q.exec("select rtype from readertype");
     while(q.next())
     {
         ui->comboBox_rt->addItem(q.value(0).toString());
     }

}

//��ȡͼƬ
void addreaderDialog::captureImage()
{

    //ui->label_picstatus->setText(tr("�˴���ȡͼƬ..."));
    imageCapture->capture();
    //ui->label_picstatus->setHidden(true);


}
//��ʾͼƬ
void addreaderDialog::displayImage(int /*id*/, QImage image)
{
    ui->label_headpict->setPixmap(QPixmap::fromImage(image));
    rid=ui->lineEdit_1->text();
    pwd = ui->lineEdit_2->text();
    name = ui->lineEdit_3->text();
    sex = ui->lineEdit_4->currentText();
    birth = ui->lineEdit_5->text();
    major = ui->lineEdit_6->text();
    rtype=ui->comboBox_rt->currentText();
    status = ui->lineEdit_7->currentText();
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
    //        QFile* file=new QFile(takepicName); //fileNameΪ�����������ļ���
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
       QMessageBox::information(NULL, QString::fromLocal8Bit("Register"), QString::fromLocal8Bit("Please Complete"));
       ui->pushButton_ok->setEnabled(false);
    }

}
//��������ʱ��
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
      ui->label_message->setText(QString::fromLocal8Bit("Already Exists!!"));

    }
    else
    {
      q.exec("select count(*) from reader where rid='"+ui->lineEdit_1->text()+"'");
      q.next();
        int rowcnt2 = q.value(0).toInt();
        if(rowcnt2>0)
            ui->label_message->setText(QString::fromLocal8Bit("Already Exists!!"));
      else
      ui->label_message->setText(QString::fromLocal8Bit("OK"));
      ui->lineEdit_5->setText(QString::fromLocal8Bit("Set Birthday"));

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
                 file=new QFile(":/img/logininback.png"); //fileNameΪ�����������ļ���
               else
               {
               file=new QFile(takepicName); //ʹ�����չ����ô����
               }
               file->open(QIODevice::ReadOnly);
               data = file->readAll();
               file->close();
               QString sql;
               QVariant var(data);
               if(ui->lineEdit_7->currentText()==QString::fromLocal8Bit("Admin"))
                 sql= "insert into admin(rid,pwd,name,sex,birth,major,rtype,status,images)values(?,?,?,?,?,?,?,?,?)";
                else
                 sql="insert into reader(rid,pwd,name,sex,birth,major,rtype,status,images)values(?,?,?,?,?,?,?,?,?)";
              // QString sql = "insert into reader(rid,pwd,name,sex,birth,major,status,images)values(?,?,?,?,?,?,?,?)";
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
    QMessageBox::information(NULL, QString::fromLocal8Bit("Add"), tr("Add Success"));

    close();


}

void addreaderDialog::on_checkBoxs_clicked()
{
    if(!ui->lineEdit_1->text().isEmpty())
    {
         addreaderchange();
     }
}
//take pict
void addreaderDialog::on_pushButton_takepicture_clicked()
{
    camera->setViewfinder(viewfinder);
    camera->start();
    ui->pushButton_Capture->setEnabled(true);
    ui->pushButton_takepicture->setText(QString::fromLocal8Bit("Retake"));


}
//�ϴ�ͼƬ
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
    status = ui->lineEdit_7->currentText();
    rtype=ui->comboBox_rt->currentText();
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
    ui->label_headpict->setPixmap(QPixmap(strImage).scaled(ui->label_headpict->size()));

   //����ͼƬ�����ݿ��У�
    QString path = strImage;
    takepicName =path;
    ui->lineEdit_photopath->setText(path);
    if(!rid.isEmpty()&&!pwd.isEmpty()&&!name.isEmpty()&&!birth.isEmpty()&&!major.isEmpty())
    {
       ui->pushButton_ok->setEnabled(true);
    }
    else
    {
     QMessageBox::information(NULL, QString::fromLocal8Bit("Register"), QString::fromLocal8Bit("Please Complete"));

    }
}
