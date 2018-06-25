#ifndef VTINV_H
#define VTINV_H

#include "vtinv_global.h"
#include <QWidget>

class VTINVSHARED_EXPORT VtInv : public QWidget
{
    Q_OBJECT

public:
    explicit VtInv(QWidget *parent = 0);

//private slots:
//    void slotPrintClicked();
//    void designClicked();

//private:
//    LimeReport::ReportEngine m_report;
//    LimeReport::PreviewReportWidget* m_preview;
};

//extern "C"{
//    VTINVSHARED_EXPORT VtInv* getVtInv();
//}


#endif // VTINV_H
