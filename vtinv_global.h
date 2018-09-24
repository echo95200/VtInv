#ifndef VTINV_GLOBAL_H
#define VTINV_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VTINV_LIBRARY)
#  define VTINVSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VTINVSHARED_EXPORT Q_DECL_IMPORT
#endif

VTINVSHARED_EXPORT void preview(QString dbDriver,
                                QString dbFilePath,
                                QString dbUserName,
                                QString dbPassword,
                                QString dbHostName,
                                int Port,
                                QString invoiceNumber);

VTINVSHARED_EXPORT void design(QString dbDriver,
                               QString dbFilePath,
                               QString dbUserName,
                               QString dbPassword,
                               QString dbHostName,
                               int Port,
                               QString invoiceNumber);


#endif // VTINV_GLOBAL_H
