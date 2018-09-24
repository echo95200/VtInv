#include "vtinv.h"

VtInv::VtInv(QWidget *parent)
    :QWidget(parent)
{
    //this->showNormal();
}

LimeReport::ReportEngine* VtInv::initDatabase(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber)
{
    //Initialization Database
    QFile dbFile(dbFilePath);
    if (QSqlDatabase::contains("LIMEREPORT")) {
        m_db = QSqlDatabase::database("LIMEREPORT");
    } else {
        m_db = QSqlDatabase::addDatabase(dbDriver,"LIMEREPORT");
    }
    m_db.setDatabaseName(dbFile.fileName());
    m_db.setUserName(dbUserName);
    m_db.setPassword(dbPassword);
    m_db.setHostName(dbHostName);
    m_db.setPort(Port);
    if (!m_db.open()) {
        QMessageBox::about(NULL,"Info","Database file cannot be opened!");
        return NULL;
    } else {
        // Set the database
        QString sqlStrTicket = "SELECT T_TICKET.* FROM T_INVOICE "
                                "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                                "WHERE T_INVOICE.INVOICE_NUM = ?";
        QString sqlStrInvoice = "SELECT T_INVOICE.* FROM T_INVOICE "
                                "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                                "WHERE T_INVOICE.INVOICE_NUM = ?";
        QString sqlStrIicDet = "SELECT T_TICKET_DETAIL.* "
                               "FROM T_INVOICE "
                               "INNER JOIN T_TICKET_DETAIL ON T_INVOICE.TICKET_ID = T_TICKET_DETAIL.TICKET_ID "
                               "WHERE T_INVOICE.INVOICE_NUM = ?";
        QString sqlStrIicPay = "SELECT T_TICKET_PAYMENT.* FROM T_INVOICE "
                                "INNER JOIN T_TICKET_PAYMENT ON T_INVOICE.TICKET_ID = T_TICKET_PAYMENT.TICKET_ID "
                                "WHERE T_INVOICE.INVOICE_NUM = ?";
        QString sqlStrIicTax = "SELECT T_TICKET_TAX.* FROM T_INVOICE "
                                "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                                "WHERE T_INVOICE.INVOICE_NUM = ?";
        QString sqlStrTicTil = "SELECT T_TITLE.* FROM T_TICKET "
                              "INNER JOIN T_INVOICE ON T_TICKET.ID = T_INVOICE.TICKET_ID "
                              "INNER JOIN T_TITLE ON T_TITLE.ID = T_TICKET.TITLE_ID "
                              "WHERE T_INVOICE.INVOICE_NUM = ?";

        QString sqlQueryTax = "SELECT T_TICKET_TAX.TAX_RATE,T_TICKET_TAX.TOTAL_HT,"
                              "T_TICKET_TAX.TOTAL_TVA,TOTAL_TTC "
                              "FROM T_INVOICE "
                              "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                              "WHERE T_INVOICE.INVOICE_NUM = ?";
        QString sqlQuery = "SELECT T_TICKET_PAYMENT.PAY_TYPE,T_TICKET_PAYMENT.TOTAL "
                           "FROM T_TICKET_PAYMENT "
                           "INNER JOIN T_INVOICE ON T_TICKET_PAYMENT.TICKET_ID = T_INVOICE.TICKET_ID "
                           "WHERE T_INVOICE.INVOICE_NUM = ?";

        m_pReportEngine = new LimeReport::ReportEngine();

        //Get the information of the Ticket and Invoice
        QSqlQueryModel* T_TICKET_MODEL = new QSqlQueryModel();
        QSqlQuery queryTicketModel(m_db);
        queryTicketModel.prepare(sqlStrTicket);
        queryTicketModel.bindValue(0,invoiceNumber);
        queryTicketModel.exec();
        T_TICKET_MODEL->setQuery(queryTicketModel);
        m_pReportEngine->dataManager()->addModel("T_TICKET",T_TICKET_MODEL,true);

        QSqlQueryModel* T_INVOICE_MODEL = new QSqlQueryModel();
        QSqlQuery queryInvoiceModel(m_db);
        queryInvoiceModel.prepare(sqlStrInvoice);
        queryInvoiceModel.bindValue(0,invoiceNumber);
        queryInvoiceModel.exec();
        T_INVOICE_MODEL->setQuery(queryInvoiceModel);
        m_pReportEngine->dataManager()->addModel("T_INVOICE",T_INVOICE_MODEL,true);

        // Add if
        QSqlQueryModel* T_TICKET_DETAIL_MODEL = new QSqlQueryModel();
        QSqlQuery queryTicketDetailModel(m_db);
        queryTicketDetailModel.prepare(sqlStrIicDet);
        queryTicketDetailModel.bindValue(0,invoiceNumber);
        queryTicketDetailModel.exec();
        T_TICKET_DETAIL_MODEL->setQuery(queryTicketDetailModel);
        m_pReportEngine->dataManager()->addModel("T_TICKET_DETAIL",T_TICKET_DETAIL_MODEL,true);

        QSqlQueryModel* T_TICKET_PAYMENT_MODEL = new QSqlQueryModel();
        QSqlQuery queryTicketPaymentModel(m_db);
        queryTicketPaymentModel.prepare(sqlStrIicPay);
        queryTicketPaymentModel.bindValue(0,invoiceNumber);
        queryTicketPaymentModel.exec();
        T_TICKET_PAYMENT_MODEL->setQuery(queryTicketPaymentModel);
        m_pReportEngine->dataManager()->addModel("T_TICKET_PAYMENT",T_TICKET_PAYMENT_MODEL,true);

        QSqlQueryModel* T_TICKET_TAX_MODEL = new QSqlQueryModel();
        QSqlQuery queryTicketTaxModel(m_db);
        queryTicketTaxModel.prepare(sqlStrIicTax);
        queryTicketTaxModel.bindValue(0,invoiceNumber);
        queryTicketTaxModel.exec();
        T_TICKET_TAX_MODEL->setQuery(queryTicketTaxModel);
        m_pReportEngine->dataManager()->addModel("T_TICKET_TAX",T_TICKET_TAX_MODEL,true);

        QSqlQueryModel* T_TICKET_TITLE = new QSqlQueryModel();
        QSqlQuery queryTicketTitle(m_db);
        queryTicketTitle.prepare(sqlStrTicTil);
        queryTicketTitle.bindValue(0,invoiceNumber);
        queryTicketTitle.exec();
        T_TICKET_TITLE->setQuery(queryTicketTitle);
        m_pReportEngine->dataManager()->addModel("T_TITLE",T_TICKET_TITLE,true);

        // Get the information of the table T_TICKET_TAX
        QSqlQuery queryTax(m_db);
        queryTax.prepare(sqlQuery);
        queryTax.bindValue(0,invoiceNumber);
        queryTax.exec();
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
        for(int i=0;i<4;i++) {
            QStandardItemModel *modelTax = new QStandardItemModel();
            modelTax->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("TAX_RATE")));
            modelTax->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("TOTAL_HT")));
            modelTax->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("TOTAL_TVA")));
            modelTax->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("TOTAL_TTC")));
            modelTax->setItem(0,0,new QStandardItem(TaxList.value(i)));
            modelTax->setItem(0,1,new QStandardItem(HTList.value(i)));
            modelTax->setItem(0,2,new QStandardItem(TVAList.value(i)));
            modelTax->setItem(0,3,new QStandardItem(TTCList.value(i)));
            m_pReportEngine->dataManager()->addModel("TAX"+QString::number(i),modelTax,true);
        }

        // Select the data from database
        QSqlQuery query(m_db);
        query.prepare(sqlQuery);
        query.bindValue(0,invoiceNumber);
        query.exec();
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
        for(int i=0;i<5;i++) {
            QStandardItemModel *modelPayment = new QStandardItemModel();
            modelPayment->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("PAYMENT_TYPE")));
            modelPayment->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("PAYMENT_TOTA")));
            modelPayment->setItem(0,0,new QStandardItem(payTypeList.value(i)));
            modelPayment->setItem(0,1,new QStandardItem(payTotalList.value(i)));
            m_pReportEngine->dataManager()->addModel("PAYMENT"+QString::number(i),modelPayment,true);
        }
    return m_pReportEngine;
    }

}

void preview(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber)
{
    VtInv* lib = new VtInv();
    LimeReport::ReportEngine* report = lib->initDatabase(dbDriver,dbFilePath,dbUserName,dbPassword,dbHostName,Port,invoiceNumber);
    LimeReport::PreviewReportWidget* m_preview = report->createPreviewWidget();
    QWidget *widget = new QWidget();
    QToolBar *pToolBar = new QToolBar();
    QToolButton* pToolButton = new QToolButton();

    QAction *pAction = new QAction();
    pAction->setIcon(QIcon(":/png/Images/Print.png"));
    pAction->setToolTip("Print");
    pToolButton->setDefaultAction(pAction);
    QObject::connect(pAction,SIGNAL(triggered()),m_preview,SLOT(print()));

    QToolButton* pToolButtonPDF = new QToolButton();
    QAction *pActionPDF = new QAction();
    pActionPDF->setIcon(QIcon(":/png/Images/PDF.png"));
    pActionPDF->setToolTip("PDF");
    pToolButtonPDF->setDefaultAction(pActionPDF);
    QObject::connect(pActionPDF,SIGNAL(triggered()),m_preview,SLOT(printToPDF()));

    QToolButton* pToolButtonPre = new QToolButton();
    QAction *pActionPre = new QAction();
    pActionPre->setIcon(QIcon(":/png/Images/Prev.png"));
    pActionPre->setToolTip("Prev");
    pToolButtonPre->setDefaultAction(pActionPre);
    QObject::connect(pActionPre,SIGNAL(triggered()),m_preview,SLOT(priorPage()));

    QToolButton* pToolButtonNext = new QToolButton();
    QAction *pActionNext = new QAction();
    pActionNext->setIcon(QIcon(":/png/Images/Next.png"));
    pActionNext->setToolTip("Next");
    pToolButtonNext->setDefaultAction(pActionNext);
    QObject::connect(pActionNext,SIGNAL(triggered()),m_preview,SLOT(nextPage()));

    QToolButton* pToolButtonEnd = new QToolButton();
    QAction *pActionEnd = new QAction();
    pActionEnd->setIcon(QIcon(":/png/Images/End.png"));
    pActionEnd->setToolTip("End");
    pToolButtonEnd->setDefaultAction(pActionEnd);
    QObject::connect(pActionEnd,SIGNAL(triggered()),widget,SLOT(close()));

    pToolBar->addWidget(pToolButton);
    pToolBar->addWidget(pToolButtonPDF);
    pToolBar->addWidget(pToolButtonPre);
    pToolBar->addWidget(pToolButtonNext);
    pToolBar->addWidget(pToolButtonEnd);
    pToolBar->setIconSize(QSize(96,48));

    QVBoxLayout *pVLayout = new QVBoxLayout();
    pVLayout->addWidget(pToolBar);
    pVLayout->addWidget(m_preview);

    widget->setLayout(pVLayout);

    if (QFile::exists(QApplication::applicationDirPath()+ QDir::separator() + "invoice.lrxml")){
        report->loadFromFile(QApplication::applicationDirPath()+ QDir::separator() + "invoice.lrxml");
        m_preview->refreshPages();
        widget->setWindowFlags(Qt::WindowStaysOnTopHint);
        widget->show();
    }else {
        QMessageBox::information(NULL,"Tips","The file xml doesn't exist!");
    }

}

/*

void preview(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber)
{   
    LimeReport::ReportEngine *report = new LimeReport::ReportEngine();
    QSqlDatabase m_db;

    // Initialization the database
    QFile dbFile(dbFilePath);
    m_db = QSqlDatabase::addDatabase(dbDriver);
    m_db.setDatabaseName(dbFile.fileName());
    m_db.setUserName(dbUserName);
    m_db.setPassword(dbPassword);
    m_db.setHostName(dbHostName);
    m_db.setPort(Port);
    if(!m_db.open()){
        QMessageBox::about(NULL,"Info","Database file cannot be opened!");
    }

    // Set the database
    QString sqlStrTicket = "SELECT T_TICKET.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ?";
    QString sqlStrInvoice = "SELECT T_INVOICE.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET ON T_INVOICE.TICKET_ID = T_TICKET.ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ?";
    QString sqlStrIicDet = "SELECT T_TICKET_DETAIL.* "
                           "FROM T_INVOICE "
                           "INNER JOIN T_TICKET_DETAIL ON T_INVOICE.TICKET_ID = T_TICKET_DETAIL.TICKET_ID "
                           "WHERE T_INVOICE.INVOICE_NUM = ?";
    QString sqlStrIicPay = "SELECT T_TICKET_PAYMENT.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_PAYMENT ON T_INVOICE.TICKET_ID = T_TICKET_PAYMENT.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ?";
    QString sqlStrIicTax = "SELECT T_TICKET_TAX.* FROM T_INVOICE "
                            "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                            "WHERE T_INVOICE.INVOICE_NUM = ?";
    QString sqlStrTicTil = "SELECT T_TITLE.* FROM T_TICKET "
                          "INNER JOIN T_INVOICE ON T_TICKET.ID = T_INVOICE.TICKET_ID "
                          "INNER JOIN T_TITLE ON T_TITLE.ID = T_TICKET.TITLE_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ?";

    QString sqlQueryTax = "SELECT T_TICKET_TAX.TAX_RATE,T_TICKET_TAX.TOTAL_HT,"
                          "T_TICKET_TAX.TOTAL_TVA,TOTAL_TTC "
                          "FROM T_INVOICE "
                          "INNER JOIN T_TICKET_TAX ON T_INVOICE.TICKET_ID = T_TICKET_TAX.TICKET_ID "
                          "WHERE T_INVOICE.INVOICE_NUM = ?";
    QString sqlQuery = "SELECT T_TICKET_PAYMENT.PAY_TYPE,T_TICKET_PAYMENT.TOTAL "
                       "FROM T_TICKET_PAYMENT "
                       "INNER JOIN T_INVOICE ON T_TICKET_PAYMENT.TICKET_ID = T_INVOICE.TICKET_ID "
                       "WHERE T_INVOICE.INVOICE_NUM = ?";

    //Get the information of the Ticket and Invoice
    QSqlQueryModel* T_TICKET_MODEL = new QSqlQueryModel();
    QSqlQuery queryTicketModel(m_db);
    queryTicketModel.prepare(sqlStrTicket);
    queryTicketModel.bindValue(0,invoiceNumber);
    queryTicketModel.exec();
    T_TICKET_MODEL->setQuery(queryTicketModel);
//    sqlStrTicket.append(invoiceNumber);
//    T_TICKET_MODEL->setQuery(sqlStrTicket,m_db);
    report->dataManager()->addModel("T_TICKET",T_TICKET_MODEL,true);

    QSqlQueryModel* T_INVOICE_MODEL = new QSqlQueryModel();
    QSqlQuery queryInvoiceModel(m_db);
    queryInvoiceModel.prepare(sqlStrInvoice);
    queryInvoiceModel.bindValue(0,invoiceNumber);
    queryInvoiceModel.exec();
    T_INVOICE_MODEL->setQuery(queryInvoiceModel);
//    sqlStrInvoice.append(invoiceNumber);
//    T_INVOICE_MODEL->setQuery(sqlStrInvoice,m_db);
    report->dataManager()->addModel("T_INVOICE",T_INVOICE_MODEL,true);

    // Add if
    QSqlQueryModel* T_TICKET_DETAIL_MODEL = new QSqlQueryModel();
    QSqlQuery queryTicketDetailModel(m_db);
    queryTicketDetailModel.prepare(sqlStrIicDet);
    queryTicketDetailModel.bindValue(0,invoiceNumber);
    queryTicketDetailModel.exec();
    T_TICKET_DETAIL_MODEL->setQuery(queryTicketDetailModel);
//    sqlStrIicDet.append(invoiceNumber);
//    T_TICKET_DETAIL_MODEL->setQuery(sqlStrIicDet,m_db);
    report->dataManager()->addModel("T_TICKET_DETAIL",T_TICKET_DETAIL_MODEL,true);

    QSqlQueryModel* T_TICKET_PAYMENT_MODEL = new QSqlQueryModel();
//    sqlStrIicPay.append(invoiceNumber);
//    T_TICKET_PAYMENT_MODEL->setQuery(sqlStrIicPay,m_db);
    QSqlQuery queryTicketPaymentModel(m_db);
    queryTicketPaymentModel.prepare(sqlStrIicPay);
    queryTicketPaymentModel.bindValue(0,invoiceNumber);
    queryTicketPaymentModel.exec();
    T_TICKET_PAYMENT_MODEL->setQuery(queryTicketPaymentModel);
    report->dataManager()->addModel("T_TICKET_PAYMENT",T_TICKET_PAYMENT_MODEL,true);

    QSqlQueryModel* T_TICKET_TAX_MODEL = new QSqlQueryModel();
//    sqlStrIicTax.append(invoiceNumber);
//    T_TICKET_TAX_MODEL->setQuery(sqlStrIicTax,m_db);
    QSqlQuery queryTicketTaxModel(m_db);
    queryTicketTaxModel.prepare(sqlStrIicTax);
    queryTicketTaxModel.bindValue(0,invoiceNumber);
    queryTicketTaxModel.exec();
    T_TICKET_TAX_MODEL->setQuery(queryTicketTaxModel);
    report->dataManager()->addModel("T_TICKET_TAX",T_TICKET_TAX_MODEL,true);

    QSqlQueryModel* T_TICKET_TITLE = new QSqlQueryModel();
//    sqlStrTicTil.append(invoiceNumber);
//    T_TICKET_TITLE->setQuery(sqlStrTicTil,m_db);
    QSqlQuery queryTicketTitle(m_db);
    queryTicketTitle.prepare(sqlStrTicTil);
    queryTicketTitle.bindValue(0,invoiceNumber);
    queryTicketTitle.exec();
    T_TICKET_TITLE->setQuery(queryTicketTitle);
    report->dataManager()->addModel("T_TITLE",T_TICKET_TITLE,true);

    // Get the information of the table T_TICKET_TAX
    //sqlQueryTax.append(invoiceNumber);
    QSqlQuery queryTax(m_db);
    queryTax.prepare(sqlQuery);
    queryTax.bindValue(0,invoiceNumber);
    queryTax.exec();
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
//    sqlQuery.append(invoiceNumber);
    QSqlQuery query(m_db);
    query.prepare(sqlQuery);
    query.bindValue(0,invoiceNumber);
    query.exec();
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


//    QString fileName = QFileDialog::getOpenFileName(NULL,"Select report file","/home/echo/","*.lrxml");
//    if (!fileName.isEmpty()) {
//        report->loadFromFile(fileName);
//        report->previewReport();
//    }

    LimeReport::PreviewReportWidget* m_preview = report->createPreviewWidget();
    QWidget *widget = new QWidget();
    QToolBar *pToolBar = new QToolBar();
    QToolButton* pToolButton = new QToolButton();

    QAction *pAction = new QAction();
    pAction->setIcon(QIcon(":/png/Images/Print.png"));
    pAction->setToolTip("Print");
    pToolButton->setDefaultAction(pAction);
    QObject::connect(pAction,SIGNAL(triggered()),m_preview,SLOT(print()));

    QToolButton* pToolButtonPDF = new QToolButton();
    QAction *pActionPDF = new QAction();
    pActionPDF->setIcon(QIcon(":/png/Images/PDF.png"));
    pActionPDF->setToolTip("PDF");
    pToolButtonPDF->setDefaultAction(pActionPDF);
    QObject::connect(pActionPDF,SIGNAL(triggered()),m_preview,SLOT(printToPDF()));
//    QObject::connect(pActionPDF,SIGNAL(triggered()),widget,SLOT())

    QToolButton* pToolButtonPre = new QToolButton();
    QAction *pActionPre = new QAction();
    pActionPre->setIcon(QIcon(":/png/Images/Prev.png"));
    pActionPre->setToolTip("Prev");
    pToolButtonPre->setDefaultAction(pActionPre);
    QObject::connect(pActionPre,SIGNAL(triggered()),m_preview,SLOT(priorPage()));

    QToolButton* pToolButtonNext = new QToolButton();
    QAction *pActionNext = new QAction();
    pActionNext->setIcon(QIcon(":/png/Images/Next.png"));
    pActionNext->setToolTip("Next");
    pToolButtonNext->setDefaultAction(pActionNext);
    QObject::connect(pActionNext,SIGNAL(triggered()),m_preview,SLOT(nextPage()));

    QToolButton* pToolButtonEnd = new QToolButton();
    QAction *pActionEnd = new QAction();
    pActionEnd->setIcon(QIcon(":/png/Images/End.png"));
    pActionEnd->setToolTip("End");
    pToolButtonEnd->setDefaultAction(pActionEnd);
    QObject::connect(pActionEnd,SIGNAL(triggered()),widget,SLOT(close()));

    pToolBar->addWidget(pToolButton);
    pToolBar->addWidget(pToolButtonPDF);
    pToolBar->addWidget(pToolButtonPre);
    pToolBar->addWidget(pToolButtonNext);
    pToolBar->addWidget(pToolButtonEnd);
    pToolBar->setIconSize(QSize(96,48));

    QVBoxLayout *pVLayout = new QVBoxLayout();
    pVLayout->addWidget(pToolBar);
    pVLayout->addWidget(m_preview);

    widget->setLayout(pVLayout);

    //Change the position of the application
    QDesktopWidget* desktop = QApplication::desktop();

    int screenWidth = desktop->screenGeometry().width();
    int screenHeight = desktop->screenGeometry().height();

//    int x = screenWidth*0.1;
//    int y = screenHeight*0.1;

//    widget->resize(screenWidth*0.8,screenHeight*0.8);
//    widget->move(x,y);
    if (QFile::exists(QApplication::applicationDirPath()+ QDir::separator() + "invoice.lrxml")){
        report->loadFromFile(QApplication::applicationDirPath()+ QDir::separator() + "invoice.lrxml");
        m_preview->refreshPages();
        widget->setWindowFlags(Qt::WindowStaysOnTopHint);
        widget->show();
    }else {
        QMessageBox::information(NULL,"Tips","The file xml doesn't exist!");
    }


}

*/

void design(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber)
{
    LimeReport::ReportEngine *report = new LimeReport::ReportEngine();
    QSqlDatabase m_db;
//    setDatabase(report,m_db,dbDriver,dbFilePath,dbUserName,dbPassword,dbHostName,Port,invoiceNumber);
//    report->designReport();
    // Initialization the database
    QFile dbFile(dbFilePath);
    m_db = QSqlDatabase::addDatabase(dbDriver);
    m_db.setDatabaseName(dbFile.fileName());
    m_db.setUserName(dbUserName);
    m_db.setPassword(dbPassword);
    m_db.setHostName(dbHostName);
    m_db.setPort(Port);
    if(!m_db.open()){
        QMessageBox::about(NULL,"Info","Database file cannot be opened!");
        return;
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

    report->loadFromFile(QApplication::applicationDirPath()+ QDir::separator() + "invoice.lrxml");
    report->designReport();
}
