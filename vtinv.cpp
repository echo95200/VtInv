#include "vtinv.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QSqlQuery>
#include "LimeReport"
#include <QSqlDatabase>

VtInv::VtInv(QWidget *parent)
    :QWidget(parent)
{
    this->showNormal();
}

/*
void setDatabase(LimeReport::ReportEngine *report, QSqlDatabase m_db, QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port, QString invoiceNumber)
{

    // Initialization the database
        QFile dbFile(dbFilePath);
        if(dbFile.exists())
        {
            m_db = QSqlDatabase::addDatabase(dbDriver);
            m_db.setDatabaseName(dbFile.fileName());
            m_db.setUserName(dbUserName);
            m_db.setPassword(dbPassword);
            m_db.setHostName(dbHostName);
            m_db.setPort(Port);
            if(!m_db.open()){
                QMessageBox::about(NULL,"Info","Database file cannot be opened!");
            }
        }
        else {
            QMessageBox::about(NULL,"Info","Database file does not exist!");
        }

    // Set the database
    QString sqlStrTicket = "SELECT T_TICKET.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrInvoice = "SELECT T_INVOICE.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicDet = "SELECT T_TICKET_DETAIL.* "
                           "FROM T_INVOICE "
                           "INNER JOIN T_TICKET_DETAIL ON T_INVOICE.TICKET_ID = T_TICKET_DETAIL.TICKET_ID "
                           "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicPay = "SELECT T_TICKET_PAYMENT.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_PAYMENT ON T_INVOICE.TICKET_ID = T_TICKET_PAYMENT.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicTax = "SELECT T_TICKET_TAX.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrTicTil = "SELECT T_TITLE.* FROM T_TICKET "
                          "INNER JOIN T_INVOICE ON T_TICKET.ID = T_INVOICE.TICKET_ID "
                          "INNER JOIN T_TITLE ON T_TITLE.ID = T_TICKET.TITLE_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ";

    QString sqlQueryTax = "SELECT T_TICKET_TAX.TAX_RATE,T_TICKET_TAX.TOTAL_HT,"
                          "T_TICKET_TAX.TOTAL_TVA,TOTAL_TTC "
                          "FROM T_INVOICE "
                          "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlQuery = "SELECT T_TICKET_PAYMENT.PAY_TYPE,T_TICKET_PAYMENT.TOTAL "
                       "FROM T_TICKET_PAYMENT "
                       "INNER JOIN T_INVOICE ON T_TICKET_PAYMENT.TICKET_ID = T_INVOICE.TICKET_ID "
                       "WHERE T_INVOICE.INVOICE_NUM = ";

    //Get the information of the Ticket and Invoice
    QSqlQueryModel* T_TICKET_MODEL = new QSqlQueryModel();
    sqlStrTicket.append(invoiceNumber);
    T_TICKET_MODEL->setQuery(sqlStrTicket,m_db);
    report->dataManager()->addModel("T_TICKET",T_TICKET_MODEL,true);

    QSqlQueryModel* T_INVOICE_MODEL = new QSqlQueryModel();
    sqlStrInvoice.append(invoiceNumber);
    T_INVOICE_MODEL->setQuery(sqlStrInvoice,m_db);
    report->dataManager()->addModel("T_INVOICE",T_INVOICE_MODEL,true);

    // Add if
    QSqlQueryModel* T_TICKET_DETAIL_MODEL = new QSqlQueryModel();
    sqlStrIicDet.append(invoiceNumber);
    T_TICKET_DETAIL_MODEL->setQuery(sqlStrIicDet,m_db);
    report->dataManager()->addModel("T_TICKET_DETAIL",T_TICKET_DETAIL_MODEL,true);

    QSqlQueryModel* T_TICKET_PAYMENT_MODEL = new QSqlQueryModel();
    sqlStrIicPay.append(invoiceNumber);
    T_TICKET_PAYMENT_MODEL->setQuery(sqlStrIicPay,m_db);
    report->dataManager()->addModel("T_TICKET_PAYMENT",T_TICKET_PAYMENT_MODEL,true);

    QSqlQueryModel* T_TICKET_TAX_MODEL = new QSqlQueryModel();
    sqlStrIicTax.append(invoiceNumber);
    T_TICKET_TAX_MODEL->setQuery(sqlStrIicTax,m_db);
    report->dataManager()->addModel("T_TICKET_TAX",T_TICKET_TAX_MODEL,true);

    QSqlQueryModel* T_TICKET_TITLE = new QSqlQueryModel();
    sqlStrTicTil.append(invoiceNumber);
    T_TICKET_TITLE->setQuery(sqlStrTicTil,m_db);
    report->dataManager()->addModel("T_TITLE",T_TICKET_TITLE,true);

    // Get the information of the table T_TICKET_TAX
    sqlQueryTax.append(invoiceNumber);
    QSqlQuery queryTax(sqlQueryTax);
    QStringList TaxList,HTList,TVAList,TTCList;
    int sizeQueryTax = 0;
    while (queryTax.next()) {
        sizeQueryTax++;
        TaxList.append(QString::number(queryTax.value(0).toDouble(),10,2));
        HTList.append(QString::number(queryTax.value(1).toDouble(),10,2));
        TVAList.append(QString::number(queryTax.value(2).toDouble(),10,2));
        TTCList.append(QString::number(queryTax.value(3).toDouble(),10,2));
    }
    while (sizeQueryTax < 4) {
        sizeQueryTax++;
        TaxList.append("");
        HTList.append("");
        TVAList.append("");
        TTCList.append("");
    }

    // insert the data tax into report include rate HT TVA TTC
    for(int i=0;i<4;i++)
    {
        QStandardItemModel *modelTax = new QStandardItemModel();
        modelTax->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("TAX_RATE")));
        modelTax->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("TOTAL_HT")));
        modelTax->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("TOTAL_TVA")));
        modelTax->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("TOTAL_TTC")));
        modelTax->setItem(0,0,new QStandardItem(TaxList.value(i)));
        modelTax->setItem(0,1,new QStandardItem(HTList.value(i)));
        modelTax->setItem(0,2,new QStandardItem(TVAList.value(i)));
        modelTax->setItem(0,3,new QStandardItem(TTCList.value(i)));
        report->dataManager()->addModel("TAX"+QString::number(i),modelTax,true);
    }

    // Select the data from database
    sqlQuery.append(invoiceNumber);
    QSqlQuery query(sqlQuery);
    QStringList payTypeList,payTotalList;
    int sizeQuery = 0;
    while (query.next()) {
        sizeQuery ++;
        payTypeList.append(query.value(0).toString());
        payTotalList.append(QString::number(query.value(1).toDouble(),10,2));
    }
    while (sizeQuery < 5) {
        sizeQuery++;
        payTypeList.append("");
        payTotalList.append("");
    }

    // insert the data payment into report include payment type and total
    for(int i=0;i<5;i++)
    {
        QStandardItemModel *modelPayment = new QStandardItemModel();
        modelPayment->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("PAYMENT_TYPE")));
        modelPayment->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("PAYMENT_TOTA")));
        modelPayment->setItem(0,0,new QStandardItem(payTypeList.value(i)));
        modelPayment->setItem(0,1,new QStandardItem(payTotalList.value(i)));
        report->dataManager()->addModel("PAYMENT"+QString::number(i),modelPayment,true);
    }
}
*/

void preview(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber)
{
    LimeReport::ReportEngine *report = new LimeReport::ReportEngine();
    QSqlDatabase m_db;
    //setDatabase(report,m_db,dbDriver,dbFilePath,dbUserName,dbPassword,dbHostName,Port,invoiceNumber);

    // Initialization the database
        QFile dbFile(dbFilePath);
        if(dbFile.exists())
        {
            m_db = QSqlDatabase::addDatabase(dbDriver);
            m_db.setDatabaseName(dbFile.fileName());
            m_db.setUserName(dbUserName);
            m_db.setPassword(dbPassword);
            m_db.setHostName(dbHostName);
            m_db.setPort(Port);
            if(!m_db.open()){
                QMessageBox::about(NULL,"Info","Database file cannot be opened!");
            }
        }
        else {
            QMessageBox::about(NULL,"Info","Database file does not exist!");
        }

    // Set the database
    QString sqlStrTicket = "SELECT T_TICKET.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrInvoice = "SELECT T_INVOICE.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicDet = "SELECT T_TICKET_DETAIL.* "
                           "FROM T_INVOICE "
                           "INNER JOIN T_TICKET_DETAIL ON T_INVOICE.TICKET_ID = T_TICKET_DETAIL.TICKET_ID "
                           "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicPay = "SELECT T_TICKET_PAYMENT.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_PAYMENT ON T_INVOICE.TICKET_ID = T_TICKET_PAYMENT.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicTax = "SELECT T_TICKET_TAX.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrTicTil = "SELECT T_TITLE.* FROM T_TICKET "
                          "INNER JOIN T_INVOICE ON T_TICKET.ID = T_INVOICE.TICKET_ID "
                          "INNER JOIN T_TITLE ON T_TITLE.ID = T_TICKET.TITLE_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ";

    QString sqlQueryTax = "SELECT T_TICKET_TAX.TAX_RATE,T_TICKET_TAX.TOTAL_HT,"
                          "T_TICKET_TAX.TOTAL_TVA,TOTAL_TTC "
                          "FROM T_INVOICE "
                          "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlQuery = "SELECT T_TICKET_PAYMENT.PAY_TYPE,T_TICKET_PAYMENT.TOTAL "
                       "FROM T_TICKET_PAYMENT "
                       "INNER JOIN T_INVOICE ON T_TICKET_PAYMENT.TICKET_ID = T_INVOICE.TICKET_ID "
                       "WHERE T_INVOICE.INVOICE_NUM = ";

    //Get the information of the Ticket and Invoice
    QSqlQueryModel* T_TICKET_MODEL = new QSqlQueryModel();
    sqlStrTicket.append(invoiceNumber);
    T_TICKET_MODEL->setQuery(sqlStrTicket,m_db);
    report->dataManager()->addModel("T_TICKET",T_TICKET_MODEL,true);

    QSqlQueryModel* T_INVOICE_MODEL = new QSqlQueryModel();
    sqlStrInvoice.append(invoiceNumber);
    T_INVOICE_MODEL->setQuery(sqlStrInvoice,m_db);
    report->dataManager()->addModel("T_INVOICE",T_INVOICE_MODEL,true);

    // Add if
    QSqlQueryModel* T_TICKET_DETAIL_MODEL = new QSqlQueryModel();
    sqlStrIicDet.append(invoiceNumber);
    T_TICKET_DETAIL_MODEL->setQuery(sqlStrIicDet,m_db);
    report->dataManager()->addModel("T_TICKET_DETAIL",T_TICKET_DETAIL_MODEL,true);

    QSqlQueryModel* T_TICKET_PAYMENT_MODEL = new QSqlQueryModel();
    sqlStrIicPay.append(invoiceNumber);
    T_TICKET_PAYMENT_MODEL->setQuery(sqlStrIicPay,m_db);
    report->dataManager()->addModel("T_TICKET_PAYMENT",T_TICKET_PAYMENT_MODEL,true);

    QSqlQueryModel* T_TICKET_TAX_MODEL = new QSqlQueryModel();
    sqlStrIicTax.append(invoiceNumber);
    T_TICKET_TAX_MODEL->setQuery(sqlStrIicTax,m_db);
    report->dataManager()->addModel("T_TICKET_TAX",T_TICKET_TAX_MODEL,true);

    QSqlQueryModel* T_TICKET_TITLE = new QSqlQueryModel();
    sqlStrTicTil.append(invoiceNumber);
    T_TICKET_TITLE->setQuery(sqlStrTicTil,m_db);
    report->dataManager()->addModel("T_TITLE",T_TICKET_TITLE,true);

    // Get the information of the table T_TICKET_TAX
    sqlQueryTax.append(invoiceNumber);
    QSqlQuery queryTax(sqlQueryTax);
    QStringList TaxList,HTList,TVAList,TTCList;
    int sizeQueryTax = 0;
    while (queryTax.next()) {
        sizeQueryTax++;
        TaxList.append(QString::number(queryTax.value(0).toDouble(),10,2));
        HTList.append(QString::number(queryTax.value(1).toDouble(),10,2));
        TVAList.append(QString::number(queryTax.value(2).toDouble(),10,2));
        TTCList.append(QString::number(queryTax.value(3).toDouble(),10,2));
    }
    while (sizeQueryTax < 4) {
        sizeQueryTax++;
        TaxList.append("");
        HTList.append("");
        TVAList.append("");
        TTCList.append("");
    }

    // insert the data tax into report include rate HT TVA TTC
    for(int i=0;i<4;i++)
    {
        QStandardItemModel *modelTax = new QStandardItemModel();
        modelTax->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("TAX_RATE")));
        modelTax->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("TOTAL_HT")));
        modelTax->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("TOTAL_TVA")));
        modelTax->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("TOTAL_TTC")));
        modelTax->setItem(0,0,new QStandardItem(TaxList.value(i)));
        modelTax->setItem(0,1,new QStandardItem(HTList.value(i)));
        modelTax->setItem(0,2,new QStandardItem(TVAList.value(i)));
        modelTax->setItem(0,3,new QStandardItem(TTCList.value(i)));
        report->dataManager()->addModel("TAX"+QString::number(i),modelTax,true);
    }

    // Select the data from database
    sqlQuery.append(invoiceNumber);
    QSqlQuery query(sqlQuery);
    QStringList payTypeList,payTotalList;
    int sizeQuery = 0;
    while (query.next()) {
        sizeQuery ++;
        payTypeList.append(query.value(0).toString());
        payTotalList.append(QString::number(query.value(1).toDouble(),10,2));
    }
    while (sizeQuery < 5) {
        sizeQuery++;
        payTypeList.append("");
        payTotalList.append("");
    }

    // insert the data payment into report include payment type and total
    for(int i=0;i<5;i++)
    {
        QStandardItemModel *modelPayment = new QStandardItemModel();
        modelPayment->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("PAYMENT_TYPE")));
        modelPayment->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("PAYMENT_TOTA")));
        modelPayment->setItem(0,0,new QStandardItem(payTypeList.value(i)));
        modelPayment->setItem(0,1,new QStandardItem(payTotalList.value(i)));
        report->dataManager()->addModel("PAYMENT"+QString::number(i),modelPayment,true);
    }

    QString fileName = QFileDialog::getOpenFileName(NULL,"Select report file","/home/echo/","*.lrxml");
    if (!fileName.isEmpty()) {
        report->loadFromFile(fileName);
        report->previewReport();
    }
}

void design(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber)
{
    LimeReport::ReportEngine *report = new LimeReport::ReportEngine();
    QSqlDatabase m_db;
//    setDatabase(report,m_db,dbDriver,dbFilePath,dbUserName,dbPassword,dbHostName,Port,invoiceNumber);
//    report->designReport();
    // Initialization the database
        QFile dbFile(dbFilePath);
        if(dbFile.exists())
        {
            m_db = QSqlDatabase::addDatabase(dbDriver);
            m_db.setDatabaseName(dbFile.fileName());
            m_db.setUserName(dbUserName);
            m_db.setPassword(dbPassword);
            m_db.setHostName(dbHostName);
            m_db.setPort(Port);
            if(!m_db.open()){
                QMessageBox::about(NULL,"Info","Database file cannot be opened!");
            }
        }
        else {
            QMessageBox::about(NULL,"Info","Database file does not exist!");
        }

    // Set the database
    QString sqlStrTicket = "SELECT T_TICKET.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrInvoice = "SELECT T_INVOICE.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicDet = "SELECT T_TICKET_DETAIL.* "
                           "FROM T_INVOICE "
                           "INNER JOIN T_TICKET_DETAIL ON T_INVOICE.TICKET_ID = T_TICKET_DETAIL.TICKET_ID "
                           "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicPay = "SELECT T_TICKET_PAYMENT.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_PAYMENT ON T_INVOICE.TICKET_ID = T_TICKET_PAYMENT.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrIicTax = "SELECT T_TICKET_TAX.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlStrTicTil = "SELECT T_TITLE.* FROM T_TICKET "
                          "INNER JOIN T_INVOICE ON T_TICKET.ID = T_INVOICE.TICKET_ID "
                          "INNER JOIN T_TITLE ON T_TITLE.ID = T_TICKET.TITLE_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ";

    QString sqlQueryTax = "SELECT T_TICKET_TAX.TAX_RATE,T_TICKET_TAX.TOTAL_HT,"
                          "T_TICKET_TAX.TOTAL_TVA,TOTAL_TTC "
                          "FROM T_INVOICE "
                          "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ";
    QString sqlQuery = "SELECT T_TICKET_PAYMENT.PAY_TYPE,T_TICKET_PAYMENT.TOTAL "
                       "FROM T_TICKET_PAYMENT "
                       "INNER JOIN T_INVOICE ON T_TICKET_PAYMENT.TICKET_ID = T_INVOICE.TICKET_ID "
                       "WHERE T_INVOICE.INVOICE_NUM = ";

    //Get the information of the Ticket and Invoice
    QSqlQueryModel* T_TICKET_MODEL = new QSqlQueryModel();
    sqlStrTicket.append(invoiceNumber);
    T_TICKET_MODEL->setQuery(sqlStrTicket,m_db);
    report->dataManager()->addModel("T_TICKET",T_TICKET_MODEL,true);

    QSqlQueryModel* T_INVOICE_MODEL = new QSqlQueryModel();
    sqlStrInvoice.append(invoiceNumber);
    T_INVOICE_MODEL->setQuery(sqlStrInvoice,m_db);
    report->dataManager()->addModel("T_INVOICE",T_INVOICE_MODEL,true);

    // Add if
    QSqlQueryModel* T_TICKET_DETAIL_MODEL = new QSqlQueryModel();
    sqlStrIicDet.append(invoiceNumber);
    T_TICKET_DETAIL_MODEL->setQuery(sqlStrIicDet,m_db);
    report->dataManager()->addModel("T_TICKET_DETAIL",T_TICKET_DETAIL_MODEL,true);

    QSqlQueryModel* T_TICKET_PAYMENT_MODEL = new QSqlQueryModel();
    sqlStrIicPay.append(invoiceNumber);
    T_TICKET_PAYMENT_MODEL->setQuery(sqlStrIicPay,m_db);
    report->dataManager()->addModel("T_TICKET_PAYMENT",T_TICKET_PAYMENT_MODEL,true);

    QSqlQueryModel* T_TICKET_TAX_MODEL = new QSqlQueryModel();
    sqlStrIicTax.append(invoiceNumber);
    T_TICKET_TAX_MODEL->setQuery(sqlStrIicTax,m_db);
    report->dataManager()->addModel("T_TICKET_TAX",T_TICKET_TAX_MODEL,true);

    QSqlQueryModel* T_TICKET_TITLE = new QSqlQueryModel();
    sqlStrTicTil.append(invoiceNumber);
    T_TICKET_TITLE->setQuery(sqlStrTicTil,m_db);
    report->dataManager()->addModel("T_TITLE",T_TICKET_TITLE,true);

    // Get the information of the table T_TICKET_TAX
    sqlQueryTax.append(invoiceNumber);
    QSqlQuery queryTax(sqlQueryTax);
    QStringList TaxList,HTList,TVAList,TTCList;
    int sizeQueryTax = 0;
    while (queryTax.next()) {
        sizeQueryTax++;
        TaxList.append(QString::number(queryTax.value(0).toDouble(),10,2));
        HTList.append(QString::number(queryTax.value(1).toDouble(),10,2));
        TVAList.append(QString::number(queryTax.value(2).toDouble(),10,2));
        TTCList.append(QString::number(queryTax.value(3).toDouble(),10,2));
    }
    while (sizeQueryTax < 4) {
        sizeQueryTax++;
        TaxList.append("");
        HTList.append("");
        TVAList.append("");
        TTCList.append("");
    }

    // insert the data tax into report include rate HT TVA TTC
    for(int i=0;i<4;i++)
    {
        QStandardItemModel *modelTax = new QStandardItemModel();
        modelTax->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("TAX_RATE")));
        modelTax->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("TOTAL_HT")));
        modelTax->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("TOTAL_TVA")));
        modelTax->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("TOTAL_TTC")));
        modelTax->setItem(0,0,new QStandardItem(TaxList.value(i)));
        modelTax->setItem(0,1,new QStandardItem(HTList.value(i)));
        modelTax->setItem(0,2,new QStandardItem(TVAList.value(i)));
        modelTax->setItem(0,3,new QStandardItem(TTCList.value(i)));
        report->dataManager()->addModel("TAX"+QString::number(i),modelTax,true);
    }

    // Select the data from database
    sqlQuery.append(invoiceNumber);
    QSqlQuery query(sqlQuery);
    QStringList payTypeList,payTotalList;
    int sizeQuery = 0;
    while (query.next()) {
        sizeQuery ++;
        payTypeList.append(query.value(0).toString());
        payTotalList.append(QString::number(query.value(1).toDouble(),10,2));
    }
    while (sizeQuery < 5) {
        sizeQuery++;
        payTypeList.append("");
        payTotalList.append("");
    }

    // insert the data payment into report include payment type and total
    for(int i=0;i<5;i++)
    {
        QStandardItemModel *modelPayment = new QStandardItemModel();
        modelPayment->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("PAYMENT_TYPE")));
        modelPayment->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("PAYMENT_TOTA")));
        modelPayment->setItem(0,0,new QStandardItem(payTypeList.value(i)));
        modelPayment->setItem(0,1,new QStandardItem(payTotalList.value(i)));
        report->dataManager()->addModel("PAYMENT"+QString::number(i),modelPayment,true);
    }

    report->designReport();
}


