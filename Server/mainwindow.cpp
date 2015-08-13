#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTableWidget>
#include<QStringList>
#include<QString>
#include "addbookdialog.h"
#include<QFileDialog>
#include<QDataStream>
#include "logindialog.h"
#include<QMenu>
#include<QDebug>
#include<QAction>
//extern QString name;
extern QString userid;
extern QString statuswhole;
QString getbookitems;
QString getreaderitems;
QString connAddr;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->BookAction,SIGNAL(triggered()),ui->pushButton_booklist,SLOT(click()));
    connect(ui->Readaction,SIGNAL(triggered()),ui->pushButton_readlist,SLOT(click()));
    connect(ui->BorrowAction,SIGNAL(triggered()),ui->pushButton_5,SLOT(click()));
    getip();
    //initServerSocket();
    on_pushButton_listen_clicked();//

    setFixedSize(this->width(), this->height()); //
    //
    ui->comboBox_search->addItem(("BookName"));
    ui->comboBox_search->addItem(("BookID"));
    ui->comboBox_name->addItem(("ReaderName"));
    ui->comboBox_name->addItem(("ReaderID"));

    //
    ui->stackedWidget->setCurrentIndex(0);
//ui->treeWidget->setHidden(true);
    ui->label_name->setScaledContents(true);
    QString runPath =QCoreApplication::applicationDirPath();
    db=QSqlDatabase::addDatabase("QMYSQL3","main");
    db.setHostName("localhost");
    db.setDatabaseName("mydb"); //
    db.setUserName("root"); //
    db.setPassword("!aq8813797769101!&)"); //
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
    q.exec("select name,status from admin where rid='"+userid+"'");
    q.next();
    QString name=q.value(0).toString();
    QString status=q.value(1).toString();

    ui->label_name->setText(("Welcome:")+name+("  Status:")+status);
    ui->label_name->setOpenExternalLinks(true);

    //QDesktopServices::openUrl(QUrl("file:///"+pic, QUrl::TolerantMode));
    model= new QSqlTableModel(this,db);
    model->setTable("book");
    modelreader = new QSqlTableModel(this,db);
    modelreader->setTable("reader");
    modelrecord = new QSqlTableModel(this,db);
    modelrecord->setTable("borrow");
    //
    setWindowTitle(QString::fromLocal8Bit(""));
    setWindowIcon(QIcon(":/img/welcome.png"));
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(10);
       info=new QAction(("BookInfo"),this);
       info->setIcon(QIcon(":/img/record.png"));
       ui->tableView_book->addAction(info);
       ui->tableView_book->setContextMenuPolicy(Qt::ActionsContextMenu);

       connect(ui->tableView_book,SIGNAL(clicked(QModelIndex)),this,SLOT(getbookitemvalue(QModelIndex)));
       connect(info,SIGNAL(triggered()),this,SLOT(on_pushButton_modifybook_clicked()));
}
//获取ip地址
void MainWindow::getip()
{
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);

    foreach(QHostAddress address,info.addresses())
    {
       if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
           connAddr=address.toString();
           ui->hostLineEdit->setText(connAddr);
           ui->portLineEdit->setText(tr("8889"));
        }
    }

}
//权限事件
void MainWindow::license()
{


}
//双击事件
 void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
 {
     informationDialog ibdolg;
     if(ibdolg.exec()==QDialog::Accepted)
     {
       ;

     }

 }
//时间
void MainWindow::timerUpDate()
{
  QDateTime time = QDateTime::currentDateTime();
  //
  QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
  //
  ui->label_time->setText(QString::fromLocal8Bit("SYSTIME:")+str);
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

//图书界面
void MainWindow::on_BookAction_triggered()
{
  ui->stackedWidget->setCurrentIndex(1);
  ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
  //ui->tableView_book->selectRow(0);

}
//读者界面
void MainWindow::on_Readaction_triggered()
{

  ui->stackedWidget->setCurrentIndex(2);
   ui->tableView_reader->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//首页
void MainWindow::on_action_triggered()
{
     ui->stackedWidget->setCurrentIndex(0);
     //ui->treeWidget->setVisible(true);

}

//借书界面
void MainWindow::on_BorrowAction_triggered()
{

   ui->stackedWidget->setCurrentIndex(3);
   ui->tableView_record->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//设置界面
void MainWindow::on_Systemaction_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}
//
void MainWindow::getbookitemvalue(QModelIndex index)
{
    //getbookitems=index.data().toString();




}
//
void MainWindow::getreaderitemvalue(QModelIndex index)
{
    getreaderitems=index.data().toString();
    if(!getreaderitems.isEmpty()&&!getbookitems.isEmpty())
    {
      //connect(borrow,SIGNAL(triggered()),this,SLOT(on_pushButton_6_clicked()));
        on_pushButton_6_clicked();

    }

}
//图书列表
void MainWindow::on_pushButton_booklist_clicked()
{

         //QTableView *view=ui->tableView_book;

         model= new QSqlTableModel(this,db);
         model->setTable("book");
         model->setEditStrategy(QSqlTableModel::OnManualSubmit);
         model->setSort(0,Qt::AscendingOrder);
         model->select();
         model->setHeaderData(0, Qt::Horizontal,("BookID"));
         model->setHeaderData(1, Qt::Horizontal,("ISBN"));
         model->setHeaderData(2, Qt::Horizontal,("BookName"));
         model->setHeaderData(3, Qt::Horizontal,("Author"));
         model->setHeaderData(4, Qt::Horizontal,("Publish"));
         model->setHeaderData(5, Qt::Horizontal,("Date"));
         model->setHeaderData(6, Qt::Horizontal,("Price"));
         model->setHeaderData(7, Qt::Horizontal,("Store"));
         ui->tableView_book->setModel(model);
         ui->tableView_book->hideColumn(9);
         ui->tableView_book->hideColumn(10);
         ui->tableView_book->hideColumn(11);
         ui->tableView_book->show();


}
//添加图书
void MainWindow::on_pushButton_addbook_clicked()
{
   addbookDialog adddig;

    if(adddig.exec()==QDialog::Accepted)
    {
       on_pushButton_booklist_clicked();

    }

}

//更新图书
void MainWindow::on_pushButton_delete_clicked()
{

   if(model->submitAll())
   {
         QSqlQuery q;
         q.exec("delete from book where storenum<='0'");
         q.next();
         on_pushButton_booklist_clicked();
         QMessageBox::information(NULL,("Update"),("Update Success"));

   }
}

//检索图书
void MainWindow::on_pushButton_clicked()
{

   on_pushButton_booklist_clicked();
   if(!ui->lineEdit_search->text().isEmpty())
   {
      QString searchname=ui->lineEdit_search->text();
     if(ui->comboBox_search->currentIndex()==0)
        model->setFilter(QObject::tr("bookname='%1'").arg(searchname)); //
     if(ui->comboBox_search->currentIndex()==1)
        model->setFilter(QObject::tr("bookid='%1'").arg(searchname)); //
     int i=model->rowCount();
     if(i>0)
       model->select(); //
     else
         {
          QMessageBox::information(NULL, ("Tips"), ("NOT EXISTS"));
          }
  }
   else
       {
         QMessageBox::information(NULL, ("Tips"), ("NOT COMPLETE"));
         }

}
//图书的基本信息
void MainWindow::on_pushButton_modifybook_clicked()
{
    QTableView *view=ui->tableView_book;
    int n=view->currentIndex().row();
    getbookitems=model->data(model->index(n,0)).toString();
    modifybookDialog modybdolg;
    if(modybdolg.exec()==QDialog::Accepted)
    {
        ;

    }
}



//读者列表
void MainWindow::on_pushButton_readlist_clicked()
{

         QString runPath =QCoreApplication::applicationDirPath();
//         db=QSqlDatabase::addDatabase("QSQLITE");
//         db.setDatabaseName(runPath+"/mydb.db");
//         //db.setDatabaseName("F:/Desktop/Gradu Design/SmartLibrary/SmaLib/mydb.db");
//         db.open();
         //QTableView *view=ui->tableView_reader;
         modelreader = new QSqlTableModel(this,db);
         modelreader->setTable("reader");
         modelreader->setEditStrategy(QSqlTableModel::OnFieldChange);
         modelreader->setSort(0,Qt::AscendingOrder);
         modelreader->select();
         modelreader->setHeaderData(0, Qt::Horizontal,("Reader ID"));
         modelreader->setHeaderData(1, Qt::Horizontal,("Password"));
         modelreader->setHeaderData(2, Qt::Horizontal,("ReaderName"));
         modelreader->setHeaderData(3, Qt::Horizontal,("Sex"));
         modelreader->setHeaderData(4, Qt::Horizontal,("Birthday"));
         modelreader->setHeaderData(5, Qt::Horizontal,("Profession"));
         modelreader->setHeaderData(6, Qt::Horizontal,("ReaderType"));
         modelreader->setHeaderData(7, Qt::Horizontal,("ReaderStature"));
         ui->tableView_reader->setModel(modelreader);
         ui->tableView_reader->hideColumn(1);
         ui->tableView_reader->hideColumn(8);
         ui->tableView_reader->hideColumn(9);
         ui->tableView_reader->hideColumn(10);
         ui->tableView_reader->show();
}
//
void MainWindow::on_pushButton_radeadd_clicked()
{
   addreaderDialog  dig;
   if( dig.exec()==QDialog::Accepted)
   {
     on_pushButton_readlist_clicked();
   }


}
//
void MainWindow::on_pushButton_readdel_clicked()
{
    QTableView *view=ui->tableView_reader;
    int n=view->currentIndex().row();
    QString rid=modelreader->data(modelreader->index(n,0)).toString();
    QSqlQuery q;
    q.exec("delete from reader where rid='"+rid+"'");
     QMessageBox::information(NULL, ("Tips"), ("Success"));
     on_pushButton_readlist_clicked();



}
//更新
void MainWindow::on_pushButton_4_clicked()
{
  //  on_pushButton_readlist_clicked();
   if( modelreader->submitAll())
     {
       on_pushButton_readlist_clicked();
     QMessageBox::information(NULL, ("Tips"),("Success"));
     }

}
//检索读者
void MainWindow::on_pushButton_search_clicked()
{
    on_pushButton_readlist_clicked();
   if(!ui->lineEdit_name->text().isEmpty())
   {
      QString searchname=ui->lineEdit_name->text();
      if(ui->comboBox_name->currentIndex()==0)
      modelreader->setFilter(QObject::tr("name='%1'").arg(searchname)); //
      if(ui->comboBox_name->currentIndex()==1)
      modelreader->setFilter(QObject::tr("rid='%1'").arg(searchname));
      int i=modelreader->rowCount();
      if(i>0)
        modelreader->select(); //
      else
            QMessageBox::information(NULL, ("Tips"), ("NOT EXISTS"));

  }
   else
         QMessageBox::information(NULL, ("Tips"), ("NOT COMPLETE"));
}

//修改读者
void MainWindow::on_pushButton_modifyreader_clicked()
{
     modifyreaderDialog mddialog;
     if( mddialog.exec()==QDialog::Accepted)
     {
        on_pushButton_readlist_clicked();
     }

}
//数据备份
void MainWindow::on_pushButton_datacopy_clicked()
{

//    db.close();
//    //QProcess::startDetached("C:\\Program Files\\PremiumSoft\\Navicat for MySQL\\navicat.exe",QStringList());

//    QString strImage = QFileDialog::getOpenFileName(
//    this,
//   "Please Select tools",
//    "/home");
//    if (strImage.isNull())
//    {
//    return;
//    }
    QString Cmd = QString("mysqldump.exe --add-drop-table -u%1 -p%2 mydbback").arg("root","!aq8813797769101!&)");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "backdab",
                                tr("Text files (*.txt)"));
    QString Path = QString("%1").arg(fileName);
    QProcess *poc=new QProcess;
    poc->setStandardOutputFile(Path);
    poc->start(Cmd);
    QMessageBox::information(this,("Tips"),QString::fromLocal8Bit("backup complete..."));
}
//数据库还原
//void MainWindow::on_pushButton_restore_clicked()
//{
//    QString Cmd = QString("mysql.exe -u%1 -p%2 mydbback").arg("root","!aq8813797769101!&)");
//    QString filename = QFileDialog::getOpenFileName(
//       this,
//      "Please Select file(.sql)",
//       QDir::currentPath());
//    QString Path = QString("%1").arg(filename);
//    QProcess *poc=new QProcess;
//    poc->setStandardInputFile(Path);
//    poc->start(Cmd);
//}

//借阅记录
void MainWindow::on_pushButton_5_clicked()
{

         QTableView *view=ui->tableView_record;
         QString runPath =QCoreApplication::applicationDirPath();
         modelrecord = new QSqlTableModel(this, db);
         modelrecord->setTable("borrow");
         modelrecord->setEditStrategy(QSqlTableModel::OnManualSubmit);
         modelrecord->setSort(0,Qt::AscendingOrder);
         modelrecord->select();
         modelrecord->setHeaderData(0, Qt::Horizontal,("ReaderID"));
         modelrecord->setHeaderData(1, Qt::Horizontal,("BookID"));
         modelrecord->setHeaderData(2, Qt::Horizontal,("BookName"));
         modelrecord->setHeaderData(3, Qt::Horizontal,("Publish"));
         modelrecord->setHeaderData(4, Qt::Horizontal,("BorrowDate"));
         modelrecord->setHeaderData(5, Qt::Horizontal,("ReturnDate"));
         modelrecord->setHeaderData(6, Qt::Horizontal,("RenewTimes"));
         modelrecord->setHeaderData(7, Qt::Horizontal,("RequestType"));
         modelrecord->setHeaderData(8, Qt::Horizontal,("RequestStatus"));
         view->setModel(modelrecord);
         //view->setEditTriggers(false);
         view->show();

}
//添加借阅记录
void MainWindow::on_pushButton_6_clicked()
{
    newrecordDialog nedlg;
   if(nedlg.exec()==QDialog::Accepted)
   {

      on_pushButton_5_clicked();//
      on_pushButton_booklist_clicked();//

   }
}
//归还图书
void MainWindow::on_pushButton_deleterecord_clicked()
{
     QTableView *view=ui->tableView_record;
     int n=view->currentIndex().row();
     QString bookid=modelrecord->data(modelrecord->index(n,1)).toString();
     QString bookname=modelrecord->data(modelrecord->index(n,2)).toString();
     QString rid=modelrecord->data(modelrecord->index(n,0)).toString();
     //ui->label_teat->setText(bookid);
     if(!bookid.isEmpty()&&!rid.isEmpty())
     {
     QSqlQuery q;
     q.exec("select storenum from book where bookid='"+bookid+"'");
     q.next();
     int store=q.value(0).toInt();
     store++;
     QString storenum = QString::number(store);
     q.exec("update book set storenum = '"+storenum+"'where bookid ='"+bookid+"'");
     q.next();
     q.exec("delete from borrow where rid='"+rid+"'and bookid='"+bookid+"'");
     q.next();
     QDateTime time = QDateTime::currentDateTime();
     QString curTime = time.toString("yyyy-MM-dd hh:mm:ss dddd");
     q.exec("update borrow set status= '已处理' where bookid ='"+bookid+"'and rid='"+rid+"'");
     q.next();
     q.exec("update borrow set dealwith = '"+QString::fromLocal8Bit("")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
     q.next();
     sendMessage(rid+" 归还了图书："+bookname+"\n"+curTime+"\n");
     //sendMessage(QString::fromLocal8Bit("归还成功")+"\n"+curTime+"\n");
     qDebug()<<QString::fromLocal8Bit("Return Success");
     on_pushButton_5_clicked();//
     on_pushButton_booklist_clicked();//
     }
     else
     {
       QMessageBox::information(this,("Tips"),QString::fromLocal8Bit("Not complete..."));
     }


}
//续借图书
void MainWindow::on_pushButton_continue_clicked()
{
   QTableView *view=ui->tableView_record;
   int n=view->currentIndex().row();
   QString bookid=modelrecord->data(modelrecord->index(n,1)).toString();
   QString rid=modelrecord->data(modelrecord->index(n,0)).toString();
   //ui->label_teat->setText(bookid);
   if(!bookid.isEmpty()&&!rid.isEmpty())
   {
   QString bookid=modelrecord->data(modelrecord->index(n,1)).toString();
   QString rid=modelrecord->data(modelrecord->index(n,0)).toString();
   QString bookname=modelrecord->data(modelrecord->index(n,2)).toString();
   QString returntime=modelrecord->data(modelrecord->index(n,5)).toString();
   QDate newreturntime=QDate::fromString(returntime,"yyyy/MM/dd dddd");
   QString time=newreturntime.addDays(5).toString("yyyy/MM/dd dddd");
   QString action=QString::fromLocal8Bit("Renew Success");
   QSqlQuery q;
   q.exec("update borrow set returntime='"+time+"'where rid='"+rid+"'and bookid='"+bookid+"'");
   q.next();
   q.exec("select times from borrow where bookid ='"+bookid+"'and rid='"+rid+"'");
   q.next();
   int times=q.value(0).toInt();
   times++;
   q.exec("select conlimit from readertype where rtype=(select rtype from admin where rid='"+userid+"')");
   q.next();
   int conlimit=q.value(0).toInt();
   if(times<=conlimit)
   {
   q.exec("update borrow set times = '"+QString::number(times)+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
   q.next();
   q.exec("update borrow set status= '已处理' where bookid ='"+bookid+"'and rid='"+rid+"'");
   q.next();
   q.exec("update borrow set dealwith = '"+QString::fromLocal8Bit("")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
   q.next();
   QDateTime datetime = QDateTime::currentDateTime();
   QString curTime = datetime.toString("yyyy-MM-dd hh:mm:ss dddd");
   sendMessage(rid+" Renew the book："+bookname+"\n"+curTime+"\n");
   qDebug()<<QString::fromLocal8Bit("");
   on_pushButton_5_clicked();
   }
   else
   {
      QMessageBox::information(this,QString::fromLocal8Bit("Tips"),QString::fromLocal8Bit("Reach Limit..."));
   }
   }
   else
   {
     QMessageBox::information(this,QString::fromLocal8Bit("Tips"),QString::fromLocal8Bit("NOT complete..."));
   }

}
//退出登陆
void MainWindow::on_pushButton_changepwd_clicked()
{
   close();


}
//读者类型
void MainWindow::on_pushButton__perinfo_clicked()
{
   informationDialog ibdolg;
   if(ibdolg.exec()==QDialog::Accepted)
   {

   }
}
//监听端口
void MainWindow::on_pushButton_listen_clicked()
{

    m_nameMap = new QMap<QString, QString>;
    m_server = new server;
    //
    m_server->listen(QHostAddress::Any,8889);
    qDebug()<<QObject::tr("start listening....");

    connect(m_server, SIGNAL(bytesArrived(QString, QString)), this, SLOT(processDataSlot(QString, QString)));//
    connect(m_server, SIGNAL(newConnect(QString)), this, SLOT(processConnectSlot(QString)));		//
    connect(m_server, SIGNAL(finishConn(QString)), this, SLOT(finishConn(QString)));	//
    ui->label_result->setText(("Service started"));	   //
}

void MainWindow::on_pushButton_close_clicked()
{
     m_server->close();
    ui->label_result->setText(("service Closed"));
}
void MainWindow::processConnectSlot(QString name)
{
    qDebug()<<QObject::tr("processConnectSlot");

    QString str(name + " - request: \n");
    m_nameMap->insert(name,str);	//
    qDebug()<<QObject::tr("processConnectSlot-o");

    //
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(name);
    ui->label_cstatus->setText((name) + " - connected!");
    ui->listWidget_user->addItem(item);
    ui->listWidget_user->setCurrentItem(item);

}
/*
*
* QString message
*QString name
*/
void MainWindow::processDataSlot(QString message, QString name)
{

    qDebug()<<QObject::tr("processDataSlot");
    //
    QDateTime time = QDateTime::currentDateTime();
    QString curTime = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    QMap<QString, QString>::Iterator pstr = m_nameMap->find(name);
    pstr->append(message+"\n"+curTime+"\n");
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(*pstr);
    ui->listWidget_request->addItem(item);
    ui->listWidget_request->setCurrentItem(item);

    for (int row = 0; row < ui->listWidget_user->count(); ++row)
    {
        if (ui->listWidget_user->item(row)->text() == name)
        {
            ui->listWidget_user->setCurrentItem(ui->listWidget_user->item(row));
            break;
        }
    }

}
//
void MainWindow::sendMessage(QString message)
{
     QString sendName;
    for (int row = 0; row < ui->listWidget_user->count(); ++row)
    {
       sendName=ui->listWidget_user->item(row)->text();
        m_server->sendMessage(message,sendName);//
    }
   // QString sendName(ui->listWidget_user->currentItem()->text());

}
/*
*
* QListWidgetItem* item
*/
void MainWindow::itemClicked(QListWidgetItem* item)
{
    QString curName(item->text());
    //
    QMap<QString, QString>::Iterator pstr = m_nameMap->find(curName);
}
void MainWindow::finishConn(QString nameAddr)
{

    m_nameMap->remove(nameAddr);	//
    ui->label_cstatus->setText(nameAddr + " - disconnected!");
    //
    for (int row = 0; row < ui->listWidget_user->count(); ++row)
    {
        if (ui->listWidget_user->item(row)->text() == nameAddr)
        {
            delete ui->listWidget_user->takeItem(row);
            break;
        }
    }
}
void MainWindow::displayError(QAbstractSocket::SocketError) //
{
    QMessageBox::information(this,QString::fromLocal8Bit("Tips"),QString::fromLocal8Bit("sorry!\n connect error..."));
    ui->label_result->setText(QString::fromLocal8Bit("disconnect...."));
}
void MainWindow::setQRData(QString data)
{
    this->data=data;
    update();
}

void MainWindow::paintEvent(QPaintEvent *pe)
{


}

//系统管理员用户列表
void MainWindow::on_pushButton_listuser_clicked()
{
    modeluser = new QSqlTableModel(this,db);
    modeluser->setTable("admin");
    modeluser->setEditStrategy(QSqlTableModel::OnFieldChange);
    modeluser->setSort(0,Qt::AscendingOrder);
    modeluser->select();
    modeluser->setHeaderData(0, Qt::Horizontal,("ID"));
    modeluser->setHeaderData(1, Qt::Horizontal,("Pwd"));
    modeluser->setHeaderData(2, Qt::Horizontal,("Name"));
    modeluser->setHeaderData(3, Qt::Horizontal,("Sex"));
    modeluser->setHeaderData(4, Qt::Horizontal,("Birthday"));
    modeluser->setHeaderData(5, Qt::Horizontal,("Profession"));
    modeluser->setHeaderData(6, Qt::Horizontal,("Type"));
    modeluser->setHeaderData(7, Qt::Horizontal,("Status"));
    ui->tableView_lisruser->setModel(modeluser);
    ui->tableView_lisruser->hideColumn(1);
    ui->tableView_lisruser->hideColumn(8);
    ui->tableView_lisruser->hideColumn(9);
    ui->tableView_lisruser->show();

}

