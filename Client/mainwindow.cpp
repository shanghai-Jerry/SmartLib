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
extern QString usernamewhole;
extern QString userid;
extern QString statuswhole;
extern QString serverip;
QString getbookitems;
QString getreaderitems;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->BookAction,SIGNAL(triggered()),ui->pushButton_booklist,SLOT(click()));
    connect(ui->Readaction,SIGNAL(triggered()),ui->pushButton_readlist,SLOT(click()));
    connect(ui->BorrowAction,SIGNAL(triggered()),ui->pushButton_5,SLOT(click()));

//  getip();

    setFixedSize(this->width(), this->height()); // 禁止改变窗口大小
    //检索项目
    ui->comboBox_search->addItem(QString::fromLocal8Bit("BookName"));
    ui->comboBox_name->addItem(QString::fromLocal8Bit("ReaderName"));
    ui->comboBox_search->addItem(QString::fromLocal8Bit("BookID"));
    ui->comboBox_name->addItem(QString::fromLocal8Bit("ReaderID"));
    //布局初始化
    ui->stackedWidget->setCurrentIndex(0);
   //ui->treeWidget->setHidden(true);
    ui->label_name->setScaledContents(true);
    QString runPath =QCoreApplication::applicationDirPath();
    db=QSqlDatabase::addDatabase("QMYSQL3","maindriver");
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

//    QSqlQuery q;
//    q.exec("select name,status from reader where rid='"+userid+"'");
//    q.next();
//    QString name=q.value(0).toString();
//    QString status=q.value(1).toString();

    ui->label_name->setText(QString::fromLocal8Bit("Welcome：")+usernamewhole+QString::fromLocal8Bit(" Status：")+statuswhole);
    ui->label_name->setOpenExternalLinks(true);

    //QDesktopServices::openUrl(QUrl("file:///"+pic, QUrl::TolerantMode));
    model= new QSqlTableModel(this,db);
    model->setTable("book");
    modelreader = new QSqlTableModel(this,db);
    modelreader->setTable("reader");
    modelrecord = new QSqlTableModel(this,db);
    modelrecord->setTable("borrow");
    //
    setWindowTitle(QString::fromLocal8Bit("SmarLib-Client"));
    setWindowIcon(QIcon(":/img/welcome.png"));
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(10);
    license();
    getip();//建立与服务器的connection
    // 右键菜单
        borrow=new QAction(("Info"),this);
        borrow->setIcon(QIcon(":/img/record.png"));
        ui->tableView_book->addAction(borrow);
        //ui->tableView_reader->addAction(borrow);
        ui->tableView_book->setContextMenuPolicy(Qt::ActionsContextMenu);
       // ui->tableView_reader->setContextMenuPolicy(Qt::ActionsContextMenu);
      connect(borrow,SIGNAL(triggered()),this,SLOT(on_pushButton_modifybook_clicked()));
      connect(ui->tableView_book,SIGNAL(clicked(QModelIndex)),this,SLOT(getbookitemvalue(QModelIndex)));
    //connect(ui->tableView_reader,SIGNAL(clicked(QModelIndex)),this,SLOT(getreaderitemvalue(QModelIndex)));

}
//获取系统ip
void MainWindow::getip()
{
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
        ui->hostLineEdit->setText(serverip);//初设目标服务器ip
        ui->portLineEdit->setText(tr("8889"));//初设目标服务器端口号
    }
    m_blockSize=0;
    //创建一个新的连接（ui->hostLineEdit->text()）
    this->clientSocket =new QTcpSocket;
    this->clientSocket->connectToHost(serverip,ui->portLineEdit->text().toInt());
    connect(this->clientSocket,SIGNAL(connected()),this,SLOT(processConnect()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    //如果连接失败提示，链接错误信息
    connect(this->clientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

}
//权限管理
void MainWindow::license()
{

//    QSqlQuery q;
//    q.exec("select status from reader where rid='"+userid+"'");
//    q.next();
//    QString status=q.value(0).toString();
    if(statuswhole==("Admin"))
        {
          perstatus=1;
          //return;
        }
    else
    {

        ui->pushButton_addbook->setEnabled(false);
        ui->pushButton_delete->setEnabled(false);
        ui->pushButton_radeadd->setEnabled(false);
        ui->pushButton_datacopy->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        ui->pushButton_readdel->setEnabled(false);
        ui->pushButton_6->setEnabled(false);
        ui->tableView_book->setEditTriggers(false);//直接列表修改的权限设置
        ui->tableView_reader->setEditTriggers(false);
        ui->tableView_record->setEditTriggers(false);


        perstatus=0;
    }
}
//
 void MainWindow::mouseDoubleClickEvent(QMouseEvent *ev)
 {
     informationDialog ibdolg;
     if(ibdolg.exec()==QDialog::Accepted)
     {
       ;

     }

 }
//现在的时间
void MainWindow::timerUpDate()
{
  QDateTime time = QDateTime::currentDateTime();
  //获取系统现在的时间
  QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
  //设置系统时间显示格式
  ui->label_time->setText(("SYSTIME:")+str);
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

//图书管理菜单
void MainWindow::on_BookAction_triggered()
{
  ui->stackedWidget->setCurrentIndex(1);
  ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
  //ui->tableView_book->selectRow(0);

}
//读者管理菜单
void MainWindow::on_Readaction_triggered()
{

  ui->stackedWidget->setCurrentIndex(2);
   ui->tableView_reader->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//欢迎菜单
void MainWindow::on_action_triggered()
{
     ui->stackedWidget->setCurrentIndex(0);
     //ui->treeWidget->setVisible(true);

}
//借阅管理菜单

void MainWindow::on_BorrowAction_triggered()
{

   ui->stackedWidget->setCurrentIndex(3);
   ui->tableView_record->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//系统管理菜单
void MainWindow::on_Systemaction_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

//获取选值
void MainWindow::getbookitemvalue(QModelIndex index)
{
   // getbookitems=index.data().toString();


}
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
         model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("BookID"));
         model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("ISBN"));
         model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("BookName"));
         model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("Author"));
         model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("Publish"));
         model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("PubDate"));
         model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("Price"));
         model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("Store"));
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
         QMessageBox::information(NULL,QString::fromLocal8Bit("Update"), QString::fromLocal8Bit("Update Success"));

   }
}
//图书信息
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

//搜索图书
void MainWindow::on_pushButton_clicked()
{

   on_pushButton_booklist_clicked();
   if(!ui->lineEdit_search->text().isEmpty())
   {
      QString searchname=ui->lineEdit_search->text();
     if(ui->comboBox_search->currentIndex()==0)
        model->setFilter(QObject::tr("bookname='%1'").arg(searchname)); //根据姓名进行筛选
     if(ui->comboBox_search->currentIndex()==1)
        model->setFilter(QObject::tr("bookid='%1'").arg(searchname)); //
     int i=model->rowCount();
     if(i>0)
       model->select(); //显示结果
     else
         {
          QMessageBox::information(NULL, QString::fromLocal8Bit("Failed"), QString::fromLocal8Bit("Book NOT Exists"));
          }
  }
   else
       {
         QMessageBox::information(NULL, QString::fromLocal8Bit("Failed"), QString::fromLocal8Bit("Type Text.."));
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
//添加读者
void MainWindow::on_pushButton_radeadd_clicked()
{
   addreaderDialog  dig;
   if( dig.exec()==QDialog::Accepted)
   {
     on_pushButton_readlist_clicked();
   }


}
//删除读者
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
//更新读者
void MainWindow::on_pushButton_4_clicked()
{
  //  on_pushButton_readlist_clicked();
   if( modelreader->submitAll())
     {
       on_pushButton_readlist_clicked();
     QMessageBox::information(NULL, ("Tips"),("Success"));
     }

}
//搜索读者
void MainWindow::on_pushButton_search_clicked()
{
    on_pushButton_readlist_clicked();
   if(!ui->lineEdit_name->text().isEmpty())
   {
      QString searchname=ui->lineEdit_name->text();
      if(ui->comboBox_name->currentIndex()==0)
      modelreader->setFilter(QObject::tr("name='%1'").arg(searchname)); //根据姓名进行筛选
      if(ui->comboBox_name->currentIndex()==1)
      modelreader->setFilter(QObject::tr("rid='%1'").arg(searchname));
      int i=modelreader->rowCount();
      if(i>0)
        modelreader->select(); //显示结果
      else
           QMessageBox::information(NULL, ("Tips"), ("NOT EXISTS"));

  }
   else
       QMessageBox::information(NULL, ("Tips"), ("NOT COMPLETE"));
}

//修改读者信息
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

    db.close();
    //QProcess::startDetached("C:\\Program Files\\PremiumSoft\\Navicat for MySQL\\navicat.exe",QStringList());
    QString g_strCurrentDir;
    QString strImage = QFileDialog::getOpenFileName(
    this,
   "Please Select tools",
    "/home");
    if (strImage.isNull())
    {
    return;
    }


}


//record记录
void MainWindow::on_pushButton_5_clicked()
{

         QTableView *view=ui->tableView_record;
         QString runPath =QCoreApplication::applicationDirPath();
         modelrecord = new QSqlTableModel(this, db);
         modelrecord->setTable("borrow");
         modelrecord->setEditStrategy(QSqlTableModel::OnManualSubmit);
         modelrecord->setSort(0,Qt::AscendingOrder);
         if(perstatus==0)
         {
         modelrecord->setFilter(QObject::tr("rid='%1'").arg(userid)); //根据rid进行筛选
         modelrecord->select();
         }
         else
         {
         modelrecord->select();
         }
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
//添加记录
void MainWindow::on_pushButton_6_clicked()
{
    newrecordDialog nedlg;
   if(nedlg.exec()==QDialog::Accepted)
   {

      on_pushButton_5_clicked();//更新书籍记录
      on_pushButton_booklist_clicked();//更新图书列表

   }
}
//删除记录(归还)
void MainWindow::on_pushButton_deleterecord_clicked()
{
    if(!ui->hostLineEdit->text().isEmpty() && !ui->portLineEdit->text().isEmpty())
    {

     ui->label_connect->setText(QString::fromLocal8Bit("Connecting...."));
      //根据端口设置的ip和端口号链接到目标服务器

     QTableView *view=ui->tableView_record;
     int n=view->currentIndex().row();
     QString bookid=modelrecord->data(modelrecord->index(n,1)).toString();
     QString bookname=modelrecord->data(modelrecord->index(n,2)).toString();
     QString rid=modelrecord->data(modelrecord->index(n,0)).toString();

     if(statuswhole=="Admin")
     {
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
         q.exec("update borrow set status= 'done' where bookid ='"+bookid+"'and rid='"+rid+"'");
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
     else
     {
     QSqlQuery q;
     q.exec("update borrow set dealwith = '"+QString::fromLocal8Bit("Request Return")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
     q.next();
     q.exec("update borrow set  status= '"+QString::fromLocal8Bit("Waiting for deal with")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
     q.next();
     sendtext=bookid;
      ui->label_connect->setText(QString::fromLocal8Bit("Connect Success"));
      if(!bookid.isEmpty())
      sendMessage(rid+" "+ QString::fromLocal8Bit("Request Return ,bookID：")+sendtext+QString::fromLocal8Bit("BookName：")+bookname);
      on_pushButton_5_clicked();//更新书籍记录
      on_pushButton_booklist_clicked();//更新图书列表
     }

    }
    else
    {
     QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("Type IP and Port"));
    }

}
//续借（更改应还日期）
void MainWindow::on_pushButton_continue_clicked()
{
    if(!ui->hostLineEdit->text().isEmpty() && !ui->portLineEdit->text().isEmpty())
    {
    ui->label_connect->setText(QString::fromLocal8Bit("Connecting...."));
    QTableView *view=ui->tableView_record;
    int n=view->currentIndex().row();
    QString bookid=modelrecord->data(modelrecord->index(n,1)).toString();
    QString bookname=modelrecord->data(modelrecord->index(n,2)).toString();
    QString rid=modelrecord->data(modelrecord->index(n,0)).toString();
    QString returntime=modelrecord->data(modelrecord->index(n,5)).toString();
    QDate newreturntime=QDate::fromString(returntime,"yyyy/MM/dd dddd");
    QString time=newreturntime.addDays(5).toString("yyyy/MM/dd dddd");
    if(statuswhole=="Admin")
    {
        if(!bookid.isEmpty()&&!rid.isEmpty())
        {
            QSqlQuery q;
            q.exec("update borrow set returntime='"+time+"'where rid='"+rid+"'and bookid='"+bookid+"'");
            q.next();
            q.exec("select times from borrow where bookid ='"+bookid+"'and rid='"+rid+"'");
            q.next();
            int times=q.value(0).toInt();
            times++;
            q.exec("select conlimit from readertype where rtype=(select rtype from reader where rid='"+rid+"')");
            q.next();
            int conlimit=q.value(0).toInt();
            if(times<=conlimit)
            {
            q.exec("update borrow set times = '"+QString::number(times)+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
            q.next();
            q.exec("update borrow set status= 'done' where bookid ='"+bookid+"'and rid='"+rid+"'");
            q.next();
            q.exec("update borrow set dealwith = '"+QString::fromLocal8Bit("")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
            q.next();
            QDateTime datetime = QDateTime::currentDateTime();
            QString curTime = datetime.toString("yyyy-MM-dd hh:mm:ss dddd");
            sendMessage(rid+" Renew the book"+bookname+"\n"+curTime+"\n");
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
              QMessageBox::information(this,("Tips"),QString::fromLocal8Bit("Not complete..."));
       }
    }
    else
    {
        QSqlQuery q;
        q.exec("select conlimit from readertype where rtype=(select rtype from reader where rid='"+rid+"')");
        q.next();
        int conlimit=q.value(0).toInt();
        q.exec("select times from borrow where bookid ='"+bookid+"'and rid='"+rid+"'");
        q.next();
        int times=q.value(0).toInt();
        times++;
        if(times<=conlimit)
        {
    //    q.exec("update borrow set times = '"+QString::number(times)+"'where bookid ='"+bookid+"'and rid='"+userid+"'");
    //    q.next();
          q.exec("update borrow set dealwith= '"+QString::fromLocal8Bit("Request Renew")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
          q.next();
          q.exec("update borrow set status= '"+QString::fromLocal8Bit("Waiting")+"'where bookid ='"+bookid+"'and rid='"+rid+"'");
          q.next();
          sendtext=bookid;
          ui->label_connect->setText(QString::fromLocal8Bit("Connect Success"));
          if(!bookid.isEmpty())
          sendMessage(rid+" "+QString::fromLocal8Bit(" Request renew ,bookID：")+sendtext+QString::fromLocal8Bit("BookName:")+bookname);
          on_pushButton_5_clicked();//更新书籍记录
          on_pushButton_booklist_clicked();//更新图书列表
        }
        else
        {
          QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("OverTimes"));
        }
    }
  }
}
//用户退出
void MainWindow::on_pushButton_changepwd_clicked()
{

  close();


}
void MainWindow::sleep(int msec)//自定义Qt延时函数,单位毫秒
{
    QDateTime last = QDateTime::currentDateTime();
    QDateTime now;
    while (1)
    {
        now = QDateTime::currentDateTime();
        if (last.msecsTo(now) >= msec)
        {
            break;
        }
    }
}
//个人信息
void MainWindow::on_pushButton__perinfo_clicked()
{
   informationDialog ibdolg;
   if(ibdolg.exec()==QDialog::Accepted)
   {

   }
}
//服务器

void MainWindow::on_pushButton_listen_clicked()
{
    getip();
}

void MainWindow::on_pushButton_close_clicked()
{

}
void MainWindow::processConnect()
{

 ui->label_connect->setText(QString::fromLocal8Bit("Server connected"));

}
//向服务器发送消息
/*
* 发送数据槽函数，通过用户对应的Socket将数据发送出去
* 输入参数: QString message 要发送的数据
*			QString name    发送用户名
*/
void MainWindow::sendMessage(QString message)
{
    if (message.isEmpty())
    {
        return;
    }
    QByteArray sendBuf;								//用于暂存要发送的数据
    QDataStream out(&sendBuf,QIODevice::WriteOnly); //使用数据流写入数据
    //out.setVersion(QDataStream::Qt_4_6);			//设定版本
    out << (quint16)0;								//2字节
    out <<message;						    //将str写入sendBuf
    out.device()->seek(0);							//返回到sendBuf的起始位置
    out << (quint16)(sendBuf.size() - sizeof(quint16));//写入数据长度
    this->clientSocket->write(sendBuf);

}

void MainWindow::displayError(QAbstractSocket::SocketError) //显示链接错误
{
    QMessageBox::information(this,QString::fromLocal8Bit("Error！！！"),QString::fromLocal8Bit("Sorry,Service closed！\nPlease try again..."));
     ui->label_connect->setText(QString::fromLocal8Bit("Server closed...."));

}
void MainWindow::receiveData()
{
    QDataStream in(clientSocket);
    //in.setVersion(QDataStream::Qt_4_6);//设置数据流版本
    QString connAddr(clientSocket->peerAddress().toString());
    connAddr.append(" - " + QString::number(clientSocket->peerPort()));//获取连接对象的地址
    //有数据，读取
    QString message;
    if (clientSocket->bytesAvailable() > 0)
    {
        in >> m_blockSize;
        in >> message;			//将接收到的数据存放到变量中
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(connAddr+" - response:\n"+message);
    ui->listWidget_response->addItem(item);
    ui->listWidget_response->setCurrentItem(item);
}

