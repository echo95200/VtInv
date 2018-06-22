#ifndef VTINV_H
#define VTINV_H

#include "vtinv_global.h"
#include <QWidget>

class VTINVSHARED_EXPORT VtInv : public QWidget
{
    Q_OBJECT

public:
    explicit VtInv(QWidget *parent = 0);

private slots:
//    void previewClicked();
//    void designClicked();
};

#endif // VTINV_H
