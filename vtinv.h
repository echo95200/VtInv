#ifndef VTINV_H
#define VTINV_H

#include "vtinv_global.h"
#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <LimeReport>

#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QSize>
#include <QToolButton>
#include <QDebug>

class VTINVSHARED_EXPORT VtInv : public QWidget
{
    Q_OBJECT

public:
    explicit VtInv(QWidget *parent = 0);

    LimeReport::ReportEngine* initDatabase(QString dbDriver, QString dbFilePath, QString dbUserName, QString dbPassword, QString dbHostName, int Port,QString invoiceNumber);

private:
    QSqlDatabase m_db;
    LimeReport::ReportEngine* m_pReportEngine;
};



#endif // VTINV_H
