#include "newrecorddialog.h"
#include "ui_newrecorddialog.h"
#include<QDebug>
extern QString getbookitems;
extern QString  getreaderitems;
extern QString userid;
newrecordDialog::newrecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newrecordDialog)
{
    ui->setupUi(this);

    ui->tableView_reader->hide();
    ui->tableView_book->hide();
    QDateTime datetime = QDateTime::currentDateTime();
    QString curTime = datetime.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->dateEdit->setDateTime(datetime);
    ui->comboBox_lastdays->addItem(QString::fromLocal8Bit("3天"));
    ui->comboBox_lastdays->addItem(QString::fromLocal8Bit("7天"));
    ui->comboBox_lastdays->addItem(QString::fromLocal8Bit("20天"));
    setWindowIcon(QIcon(":/img/welcome.png"));
    setWindowTitle(QString::fromLocal8Bit("小型图书借阅管理系统"));
    QString runPath =QCoreApplication::applicationDirPath();
    db=QSqlDatabase::addDatabase("QMYSQL3","newrecodriver");
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
    ui->lineEdit_1->setText(userid);
    QSqlQuery q;
    q.exec("select count(*) from borrow where rid='"+userid+"'");
    q.next();
    int borrn=q.value(0).toInt();
    q.exec("select conlimit from readertype where rtype=(select rtype from admin where rid='"+userid+"')");
    q.next();
    int conlimit=q.value(0).toInt();
    q.exec("select name,sex from admin where rid='"+userid+"'");
    q.next();
    QString name=q.value(0).toString();
    QString sex=q.value(1).toString();
    int allowborr=conlimit-borrn;
    ui->label_allowborrow->setText(QString::number(allowborr));
    ui->label_alreadborrow->setText(QString::number(borrn));
    ui->label_name->setText(name);
    ui->label_sex->setText(sex);
    ui->calendarWidget->hide();

    connect(ui->pushButton_click,SIGNAL(clicked()),ui->calendarWidget,SLOT(show()));
    ui->calendarWidget->setSelectionMode(QCalendarWidget::SingleSelection);
    connect(ui->calendarWidget,SIGNAL(clicked(const QDate &)),this,SLOT(setdate()));
    connect(ui->tableView_reader,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(getdoubleclickitem(QModelIndex)));
    connect(ui->tableView_book,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(getdoubleclickitembook(QModelIndex)));

}
//bookget
void newrecordDialog::getdoubleclickitembook(QModelIndex index)
{
    //
    QTableView *viewbook=ui->tableView_book;
    int n=viewbook->currentIndex().row();
    QString bookid=model->data(model->index(n,0)).toString();
    QString bookname=model->data(model->index(n,2)).toString();
    ui->lineEdit_2->setText(bookid);
    QSqlQuery q;
    q.exec("select storenum from book where bookid='"+bookid+"'");
    q.next();
    ui->label_storenum->setText(q.value(0).toString());
    ui->label_bookname->setText(bookname);
    ui->tableView_book->hide();
}
//readerget
 void newrecordDialog::getdoubleclickitem(QModelIndex index)
 {
     QTableView *view=ui->tableView_reader;
     int n=view->currentIndex().row();
     QString rid=modelreader->data(modelreader->index(n,0)).toString();
     QString name=modelreader->data(modelreader->index(n,2)).toString();
     QString sex=modelreader->data(modelreader->index(n,3)).toString();
     ui->lineEdit_1->setText(rid);
     ui->tableView_reader->hide();
     QSqlQuery q;
     q.exec("select count(*) from borrow where rid='"+rid+"'");
     q.next();
     int borrn=q.value(0).toInt();
     q.exec("select conlimit from readertype where rtype=(select rtype from reader where rid='"+rid+"')");
     q.next();
     int conlimit=q.value(0).toInt();
     int allowborr=conlimit-borrn;
     ui->label_allowborrow->setText(QString::number(allowborr));
     ui->label_alreadborrow->setText(QString::number(borrn));
     ui->label_name->setText(name);
     ui->label_sex->setText(sex);



 }
//
void newrecordDialog::setdate()
{
  QDate date=ui->calendarWidget->selectedDate();
  QString dtstr=date.toString("yyyy-MM-dd");
  ui->dateEdit->setDate(date);
  ui->calendarWidget->hide();

}

newrecordDialog::~newrecordDialog()
{
    delete ui;
    db.close();
}
//添加
void newrecordDialog::on_pushButton_clicked()
{

    QString rid = ui->lineEdit_1->text();
    QString bookid = ui->lineEdit_2->text();
    ui->dateEdit->setDate(QDate::currentDate());
    QString borrowtime = ui->dateEdit->text();
    QString returntime;

    if(ui->comboBox_lastdays->currentIndex()==0)
    {
     returntime=ui->dateEdit->date().addDays(3).toString("yyyy/MM/dd dddd");
    }
    else if(ui->comboBox_lastdays->currentIndex()==1)
    {
      returntime=ui->dateEdit->date().addDays(7).toString("yyyy/MM/dd dddd");
    }
    else
    {
         returntime=ui->dateEdit->date().addDays(20).toString("yyyy/MM/dd dddd");
    }
    if(!rid.isEmpty()&&!bookid.isEmpty())
    {

        QSqlQuery q;
        q.exec("select count(*) from borrow where rid='"+rid+"'and bookid='"+bookid+"'");
        q.next();
        int rowcnt3 = q.value(0).toInt();
        if(rowcnt3>0)
        {
          QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("读者记录存在")) ;
        }
        else
        {
            q.exec("select count(*) from reader where rid='"+rid+"'");
            q.next();
            int rowcnt1 = q.value(0).toInt();
            q.exec("select count(*),bookname,publish from book where bookid='"+bookid+"'");
            q.next();
            QString bookname = q.value(1).toString();
            QString publish = q.value(2).toString();
            int rowcnt2 = q.value(0).toInt();
            if(rowcnt1>0 && rowcnt2>0)
            {
             QString action= QString::fromLocal8Bit("无");
             q.exec("insert into borrow(rid,bookid,bookname,publish,borrowtime,returntime)values('"+rid+"','"+bookid+"','"+bookname+"','"+publish+"','"+borrowtime+"','"+returntime+"')");
             q.next();
             QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("读者记录成功添加"));
             q.exec("select storenum from book where bookid='"+bookid+"'");
             q.next();
             int store=q.value(0).toInt();
             store--;
             QString storenum = QString::number(store);
             q.exec("update book set storenum = '"+storenum+"'where bookid ='"+bookid+"'");
             q.next();
             //QMessageBox::information(NULL, tr("Tips"), QString::fromLocal8Bit("库存量更新成功!"));
             accept();
             close();
            }
            else if(rowcnt1>0)
            {
                 QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("图书编号不存在，拒绝操作"));
            }
           else if(rowcnt2>0)
            {
                q.exec("select count(*) from admin where rid='"+rid+"'");
                q.next();
                int rowcntadmin = q.value(0).toInt();
                if(rowcntadmin>0)
                {
                    q.exec("insert into borrow(rid,bookid,bookname,publish,borrowtime,returntime)values('"+rid+"','"+bookid+"','"+bookname+"','"+publish+"','"+borrowtime+"','"+returntime+"')");
                    q.next();
                    QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("读者记录成功添加"));
                    accept();
                    close();
                }
                else
                {
                    QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("读者编号不存在，拒绝操作"));
                }

            }
            else
            {
              QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("读者编号和图书编号都不存在，拒绝操作"));
            }
        }
    }
    else
    {
       QMessageBox::information(NULL, QString::fromLocal8Bit("添加读者记录"), QString::fromLocal8Bit("请填写完整"));
    }
  getbookitems.clear();
  getreaderitems.clear();

}
//取消
void newrecordDialog::on_pushButton_2_clicked()
{
    getbookitems.clear();
    getreaderitems.clear();
    accept();
    close();

}

//reader
void newrecordDialog::on_toolButton_clicked()
{
    ui->tableView_reader->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_reader->selectRow(0);
    ui->tableView_reader->setEditTriggers(false);
    modelreader = new QSqlTableModel(this,db);
    modelreader->setTable("reader");
    modelreader->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelreader->setSort(0,Qt::AscendingOrder);
    modelreader->select();
    modelreader->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("读者编号"));
    modelreader->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("读者密码"));
    modelreader->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("姓名"));
    modelreader->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("性别"));
    modelreader->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("出生年月"));
    modelreader->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("专业"));
    modelreader->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("身份"));
    ui->tableView_reader->setModel(modelreader);
    ui->tableView_reader->hideColumn(1);
    ui->tableView_reader->hideColumn(7);
    ui->tableView_reader->show();

}
//book
void newrecordDialog::on_toolButton_2_clicked()
{

    ui->tableView_book->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_book->selectRow(0);
    ui->tableView_book->setEditTriggers(false);
    model= new QSqlTableModel(this,db);
    model->setTable("book");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setSort(0,Qt::AscendingOrder);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("图书编号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("ISBN"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("书名"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("作者"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("出版社"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("出版日期"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("价格"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("库存量"));
    ui->tableView_book->setModel(model);
    ui->tableView_book->show();

}
