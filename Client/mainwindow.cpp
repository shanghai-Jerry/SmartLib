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

    setFixedSize(this->width(), this->height()); // ��ֹ�ı䴰�ڴ�С
    //������Ŀ
    ui->comboBox_search->addItem(QString::fromLocal8Bit("BookName"));
    ui->comboBox_name->addItem(QString::fromLocal8Bit("ReaderName"));
    ui->comboBox_search->addItem(QString::fromLocal8Bit("BookID"));
    ui->comboBox_name->addItem(QString::fromLocal8Bit("ReaderID"));
    //���ֳ�ʼ��
    ui->stackedWidget->setCurrentIndex(0);
   //ui->treeWidget->setHidden(true);
    ui->label_name->setScaledContents(true);
    QString runPath =QCoreApplication::applicationDirPath();
    db=QSqlDatabase::addDatabase("QMYSQL3","maindriver");
    db.setHostName(serverip);
    db.setDatabaseName("mydb"); // ����֮ǰ���������ݿ�
    db.setUserName("root"); // ���Ǵ������û���
    db.setPassword("!aq8813797769101!&)"); //�û�������

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

    ui->label_name->setText(QString::fromLocal8Bit("Welcome��")+usernamewhole+QString::fromLocal8Bit(" Status��")+statuswhole);
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
    getip();//�������������connection
    // �Ҽ��˵�
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
//��ȡϵͳip
void MainWindow::getip()
{
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())
    {
        ui->hostLineEdit->setText(serverip);//����Ŀ�������ip
        ui->portLineEdit->setText(tr("8889"));//����Ŀ��������˿ں�
    }
    m_blockSize=0;
    //����һ���µ����ӣ�ui->hostLineEdit->text()��
    this->clientSocket =new QTcpSocket;
    this->clientSocket->connectToHost(serverip,ui->portLineEdit->text().toInt());
    connect(this->clientSocket,SIGNAL(connected()),this,SLOT(processConnect()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    //�������ʧ����ʾ�����Ӵ�����Ϣ
    connect(this->clientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

}
//Ȩ�޹���
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
        ui->tableView_book->setEditTriggers(false);//ֱ���б��޸ĵ�Ȩ������
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
//���ڵ�ʱ��
void MainWindow::timerUpDate()
{
  QDateTime time = QDateTime::currentDateTime();
  //��ȡϵͳ���ڵ�ʱ��
  QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
  //����ϵͳʱ����ʾ��ʽ
  ui->label_time->setText(("SYSTIME:")+str);
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

//ͼ�����˵�
void MainWindow::on_BookAction_triggered()
{
  ui->stackedWidget->setCurrentIndex(1);
  ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
  //ui->tableView_book->selectRow(0);

}
//���߹���˵�
void MainWindow::on_Readaction_triggered()
{

  ui->stackedWidget->setCurrentIndex(2);
   ui->tableView_reader->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//��ӭ�˵�
void MainWindow::on_action_triggered()
{
     ui->stackedWidget->setCurrentIndex(0);
     //ui->treeWidget->setVisible(true);

}
//���Ĺ���˵�

void MainWindow::on_BorrowAction_triggered()
{

   ui->stackedWidget->setCurrentIndex(3);
   ui->tableView_record->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//ϵͳ����˵�
void MainWindow::on_Systemaction_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

//��ȡѡֵ
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

//ͼ���б�
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
//���ͼ��
void MainWindow::on_pushButton_addbook_clicked()
{
   addbookDialog adddig;

    if(adddig.exec()==QDialog::Accepted)
    {
       on_pushButton_booklist_clicked();

    }

}

//����ͼ��
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
//ͼ����Ϣ
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

//����ͼ��
void MainWindow::on_pushButton_clicked()
{

   on_pushButton_booklist_clicked();
   if(!ui->lineEdit_search->text().isEmpty())
   {
      QString searchname=ui->lineEdit_search->text();
     if(ui->comboBox_search->currentIndex()==0)
        model->setFilter(QObject::tr("bookname='%1'").arg(searchname)); //������������ɸѡ
     if(ui->comboBox_search->currentIndex()==1)
        model->setFilter(QObject::tr("bookid='%1'").arg(searchname)); //
     int i=model->rowCount();
     if(i>0)
       model->select(); //��ʾ���
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

//�����б�
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
//��Ӷ���
void MainWindow::on_pushButton_radeadd_clicked()
{
   addreaderDialog  dig;
   if( dig.exec()==QDialog::Accepted)
   {
     on_pushButton_readlist_clicked();
   }


}
//ɾ������
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
//���¶���
void MainWindow::on_pushButton_4_clicked()
{
  //  on_pushButton_readlist_clicked();
   if( modelreader->submitAll())
     {
       on_pushButton_readlist_clicked();
     QMessageBox::information(NULL, ("Tips"),("Success"));
     }

}
//��������
void MainWindow::on_pushButton_search_clicked()
{
    on_pushButton_readlist_clicked();
   if(!ui->lineEdit_name->text().isEmpty())
   {
      QString searchname=ui->lineEdit_name->text();
      if(ui->comboBox_name->currentIndex()==0)
      modelreader->setFilter(QObject::tr("name='%1'").arg(searchname)); //������������ɸѡ
      if(ui->comboBox_name->currentIndex()==1)
      modelreader->setFilter(QObject::tr("rid='%1'").arg(searchname));
      int i=modelreader->rowCount();
      if(i>0)
        modelreader->select(); //��ʾ���
      else
           QMessageBox::information(NULL, ("Tips"), ("NOT EXISTS"));

  }
   else
       QMessageBox::information(NULL, ("Tips"), ("NOT COMPLETE"));
}

//�޸Ķ�����Ϣ
void MainWindow::on_pushButton_modifyreader_clicked()
{
     modifyreaderDialog mddialog;
     if( mddialog.exec()==QDialog::Accepted)
     {
        on_pushButton_readlist_clicked();
     }

}
//���ݱ���
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


//record��¼
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
         modelrecord->setFilter(QObject::tr("rid='%1'").arg(userid)); //����rid����ɸѡ
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
//��Ӽ�¼
void MainWindow::on_pushButton_6_clicked()
{
    newrecordDialog nedlg;
   if(nedlg.exec()==QDialog::Accepted)
   {

      on_pushButton_5_clicked();//�����鼮��¼
      on_pushButton_booklist_clicked();//����ͼ���б�

   }
}
//ɾ����¼(�黹)
void MainWindow::on_pushButton_deleterecord_clicked()
{
    if(!ui->hostLineEdit->text().isEmpty() && !ui->portLineEdit->text().isEmpty())
    {

     ui->label_connect->setText(QString::fromLocal8Bit("Connecting...."));
      //���ݶ˿����õ�ip�Ͷ˿ں����ӵ�Ŀ�������

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
         sendMessage(rid+" �黹��ͼ�飺"+bookname+"\n"+curTime+"\n");
         //sendMessage(QString::fromLocal8Bit("�黹�ɹ�")+"\n"+curTime+"\n");
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
      sendMessage(rid+" "+ QString::fromLocal8Bit("Request Return ,bookID��")+sendtext+QString::fromLocal8Bit("BookName��")+bookname);
      on_pushButton_5_clicked();//�����鼮��¼
      on_pushButton_booklist_clicked();//����ͼ���б�
     }

    }
    else
    {
     QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("Type IP and Port"));
    }

}
//���裨����Ӧ�����ڣ�
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
          sendMessage(rid+" "+QString::fromLocal8Bit(" Request renew ,bookID��")+sendtext+QString::fromLocal8Bit("BookName:")+bookname);
          on_pushButton_5_clicked();//�����鼮��¼
          on_pushButton_booklist_clicked();//����ͼ���б�
        }
        else
        {
          QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("OverTimes"));
        }
    }
  }
}
//�û��˳�
void MainWindow::on_pushButton_changepwd_clicked()
{

  close();


}
void MainWindow::sleep(int msec)//�Զ���Qt��ʱ����,��λ����
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
//������Ϣ
void MainWindow::on_pushButton__perinfo_clicked()
{
   informationDialog ibdolg;
   if(ibdolg.exec()==QDialog::Accepted)
   {

   }
}
//������

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
//�������������Ϣ
/*
* �������ݲۺ�����ͨ���û���Ӧ��Socket�����ݷ��ͳ�ȥ
* �������: QString message Ҫ���͵�����
*			QString name    �����û���
*/
void MainWindow::sendMessage(QString message)
{
    if (message.isEmpty())
    {
        return;
    }
    QByteArray sendBuf;								//�����ݴ�Ҫ���͵�����
    QDataStream out(&sendBuf,QIODevice::WriteOnly); //ʹ��������д������
    //out.setVersion(QDataStream::Qt_4_6);			//�趨�汾
    out << (quint16)0;								//2�ֽ�
    out <<message;						    //��strд��sendBuf
    out.device()->seek(0);							//���ص�sendBuf����ʼλ��
    out << (quint16)(sendBuf.size() - sizeof(quint16));//д�����ݳ���
    this->clientSocket->write(sendBuf);

}

void MainWindow::displayError(QAbstractSocket::SocketError) //��ʾ���Ӵ���
{
    QMessageBox::information(this,QString::fromLocal8Bit("Error������"),QString::fromLocal8Bit("Sorry,Service closed��\nPlease try again..."));
     ui->label_connect->setText(QString::fromLocal8Bit("Server closed...."));

}
void MainWindow::receiveData()
{
    QDataStream in(clientSocket);
    //in.setVersion(QDataStream::Qt_4_6);//�����������汾
    QString connAddr(clientSocket->peerAddress().toString());
    connAddr.append(" - " + QString::number(clientSocket->peerPort()));//��ȡ���Ӷ���ĵ�ַ
    //�����ݣ���ȡ
    QString message;
    if (clientSocket->bytesAvailable() > 0)
    {
        in >> m_blockSize;
        in >> message;			//�����յ������ݴ�ŵ�������
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(connAddr+" - response:\n"+message);
    ui->listWidget_response->addItem(item);
    ui->listWidget_response->setCurrentItem(item);
}

