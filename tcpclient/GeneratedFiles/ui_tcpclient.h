/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_tcpclientClass
{
public:

    void setupUi(QDialog *tcpclientClass)
    {
        if (tcpclientClass->objectName().isEmpty())
            tcpclientClass->setObjectName(QStringLiteral("tcpclientClass"));
        tcpclientClass->resize(600, 400);

        retranslateUi(tcpclientClass);

        QMetaObject::connectSlotsByName(tcpclientClass);
    } // setupUi

    void retranslateUi(QDialog *tcpclientClass)
    {
        tcpclientClass->setWindowTitle(QApplication::translate("tcpclientClass", "tcpclient", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class tcpclientClass: public Ui_tcpclientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
