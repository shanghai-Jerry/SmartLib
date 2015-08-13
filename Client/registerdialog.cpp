#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>

registerdialog::registerdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerdialog)
{
     ui->setupUi(this);
     setWindowIcon(QIcon(":/img/welcome.png"));
     setWindowTitle(QString::fromLocal8Bit("小型图书借阅管理系统"));
     ui->checkBoxs->setHidden(true);
     ui->label_message->setText(QString::fromLocal8Bit("数据库检查结果"));
     ui->lineEdit_2->setEchoMode(QLineEdit::Password);
     ui->pushButton_ok->setEnabled(false);
     ui->lineEdit_5->setReadOnly(true);
     ui->lineEdit_5->setText(QString::fromLocal8Bit("请选择出生日期"));
     //dateset
     ui->calendarWidget->hide();
     connect(ui->lineEdit_1,SIGNAL(textChanged(QString)),ui->checkBoxs,SLOT(click()));
     connect(ui->pushButton_dataset,SIGNAL(clicked()),ui->calendarWidget,SLOT(show()));
     connect(ui->pushButton_dataset,SIGNAL(clicked()),ui->pushButton_dataset,SLOT(hide()));
     connect(ui->calendarWidget,SIGNAL(clicked(const QDate &)),this,SLOT(setdate()));
     //picture
     camera=new QCamera(this);
     viewfinder=new QCameraViewfinder(this);
     imageCapture=new QCameraImageCapture(camera);
     ui->horizontalLayout_capture->addWidget(viewfinder);
     ui->label_headpict->setScaledContents(true);
     ui->pushButton_Save->setEnabled(false);
     connect(ui->pushButton_Capture, SIGNAL(clicked()), this, SLOT(captureImage()));
     connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));

     //connect(ui->pushButton_Save, SIGNAL(clicked()), this, SLOT(saveImage()));

     //databaseconnect
     QString runPath =QCoreApplication::applicationDirPath();
     db=QSqlDatabase::addDatabase("QSQLITE");
     db.setDatabaseName(runPath+"/mydb.db");
     db.open();
}
//获取图片
void registerdialog::captureImage()
{

    ui->label_picstatus->setText(tr("此处获取图片..."));
    imageCapture->capture();
    ui->label_picstatus->setHidden(true);
    ui->pushButton_Save->setEnabled(true);
}
//显示图片
void registerdialog::displayImage(int /*id*/, QImage image)
{
    ui->label_headpict->setPixmap(QPixmap::fromImage(image));

}
//日历设置时间
void registerdialog::setdate()
{
  QDate date=ui->calendarWidget->selectedDate();
  QString dtstr=date.toString("yyyy-MM-dd");

  ui->lineEdit_5->setText(dtstr);
  ui->calendarWidget->hide();
  ui->pushButton_dataset->setVisible(true);

}
void registerdialog::addreaderchange()
{

      QSqlQuery q;
      q.exec("select count(*) from reader where rid='"+ui->lineEdit_1->text()+"'");
      q.next();
      int rowcnt = q.value(0).toInt();
      if(rowcnt>0)
      {
        QList<QString> qresult;
        ui->label_message->setText(QString::fromLocal8Bit("读者已存在!!"));
        q.exec("select rid,pwd,name,sex,birth,major,status from reader where rid='"+ui->lineEdit_1->text()+"'");
        q.next();
        for(int index= 1; index<7; index++) {
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
      }
      else
      {
        ui->label_message->setText(QString::fromLocal8Bit("编号可以使用"));

      }
}
registerdialog::~registerdialog()
{
    delete ui;
    db.close();
}

void registerdialog::on_pushButton_2_clicked()
{
    accept();
    close();
}

void registerdialog::on_pushButton_ok_clicked()
{
    QString rid=ui->lineEdit_1->text();
    QString pwd = ui->lineEdit_2->text();
    QString name = ui->lineEdit_3->text();
    QString sex = ui->lineEdit_4->text();
    QString birth = ui->lineEdit_5->text();
    QString major = ui->lineEdit_6->text();
    QString status = ui->lineEdit_7->text();
    if(!rid.isEmpty()&&!pwd.isEmpty()&&!name.isEmpty()&&!sex.isEmpty()&&!birth.isEmpty()&&!major.isEmpty()&&!status.isEmpty())
    {
     QSqlQuery q;
     q.exec("insert into reader(rid,pwd,name,sex,birth,major,status,pic)values('"+rid+"','"+pwd+"','"+name+"','"+sex+"','"+birth+"','"+major+"','"+status+"','"+takepicName+"')");
    //db.close();
     accept();
     QMessageBox::information(NULL, QString::fromLocal8Bit("读者添加"), tr("Add Success"));

     close();
    }
    else
    {
       QMessageBox::information(NULL, QString::fromLocal8Bit("读者注册"), QString::fromLocal8Bit("请填写完整"));
    }
//   accept();
}

void registerdialog::on_checkBoxs_clicked()
{
    if(!ui->lineEdit_1->text().isEmpty())
    {
      if(ui->checkBoxs->isChecked())
      {
         addreaderchange();
      }
    else
    {
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->label_message->setText(QString::fromLocal8Bit("编号可以使用"));
     }

    }
}
//take pict
void registerdialog::on_pushButton_takepicture_clicked()
{
    camera->setViewfinder(viewfinder);
    camera->start();
}
//save
void registerdialog::on_pushButton_Save_clicked()
{

    QString rid=ui->lineEdit_1->text();
    QString pwd = ui->lineEdit_2->text();
    QString name = ui->lineEdit_3->text();
    QString sex = ui->lineEdit_4->text();
    QString birth = ui->lineEdit_5->text();
    QString major = ui->lineEdit_6->text();
    QString status = ui->lineEdit_7->text();
    takepicName =QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存到文件"),QDir::currentPath(),tr("png(*.png)"));

    if(takepicName.isEmpty())
    {
       ui->label_picstatus->setText(tr("Failure"));
        return;
    }
    const QPixmap *pixmap=ui->label_headpict->pixmap();
    if(pixmap)
    {
        pixmap->save(takepicName);
        QMessageBox::information(NULL, tr("Tips"), tr("Success!"));
       if(!rid.isEmpty()&&!pwd.isEmpty()&&!name.isEmpty()&&!sex.isEmpty()&&!birth.isEmpty()&&!major.isEmpty()&&!status.isEmpty())
       {
           ui->pushButton_ok->setEnabled(true);
       }
       else
       {
          QMessageBox::information(NULL, QString::fromLocal8Bit("读者注册"), QString::fromLocal8Bit("请填写完整"));
       }
       //ui->lineEdit_7->setText(takepicName);

    }

}

