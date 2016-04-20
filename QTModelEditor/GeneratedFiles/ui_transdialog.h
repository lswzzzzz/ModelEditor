/********************************************************************************
** Form generated from reading UI file 'transdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSDIALOG_H
#define UI_TRANSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_TransDialog
{
public:

    void setupUi(QDialog *TransDialog)
    {
        if (TransDialog->objectName().isEmpty())
            TransDialog->setObjectName(QStringLiteral("TransDialog"));
        TransDialog->resize(961, 640);
        TransDialog->setMouseTracking(true);
        TransDialog->setAcceptDrops(true);
        TransDialog->setLayoutDirection(Qt::LeftToRight);

        retranslateUi(TransDialog);

        QMetaObject::connectSlotsByName(TransDialog);
    } // setupUi

    void retranslateUi(QDialog *TransDialog)
    {
        TransDialog->setWindowTitle(QApplication::translate("TransDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class TransDialog: public Ui_TransDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSDIALOG_H
