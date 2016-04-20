/********************************************************************************
** Form generated from reading UI file 'qtmodeleditor.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMODELEDITOR_H
#define UI_QTMODELEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTModelEditorClass
{
public:
    QWidget *centralWidget;
    QDockWidget *SceneWidget;
    QWidget *SceneWidgetContents;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *LogWidget;
    QWidget *LogWidgetContents;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QMainWindow *QTModelEditorClass)
    {
        if (QTModelEditorClass->objectName().isEmpty())
            QTModelEditorClass->setObjectName(QStringLiteral("QTModelEditorClass"));
        QTModelEditorClass->resize(962, 873);
        QTModelEditorClass->setMouseTracking(true);
        QTModelEditorClass->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(QTModelEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMouseTracking(true);
        centralWidget->setStyleSheet(QStringLiteral(" background-color:rgba(191, 191, 191, 191)"));
        SceneWidget = new QDockWidget(centralWidget);
        SceneWidget->setObjectName(QStringLiteral("SceneWidget"));
        SceneWidget->setEnabled(true);
        SceneWidget->setGeometry(QRect(0, 0, 960, 662));
        SceneWidget->setMouseTracking(true);
        SceneWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        SceneWidgetContents = new QWidget();
        SceneWidgetContents->setObjectName(QStringLiteral("SceneWidgetContents"));
        SceneWidgetContents->setMouseTracking(true);
        SceneWidget->setWidget(SceneWidgetContents);
        QTModelEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QTModelEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 962, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        QTModelEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QTModelEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QTModelEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QTModelEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QTModelEditorClass->setStatusBar(statusBar);
        LogWidget = new QDockWidget(QTModelEditorClass);
        LogWidget->setObjectName(QStringLiteral("LogWidget"));
        LogWidget->setMinimumSize(QSize(960, 140));
        LogWidget->setStyleSheet(QStringLiteral(" background-color:rgba(191, 191, 191, 191)"));
        LogWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        LogWidgetContents = new QWidget();
        LogWidgetContents->setObjectName(QStringLiteral("LogWidgetContents"));
        plainTextEdit = new QPlainTextEdit(LogWidgetContents);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(0, 0, 960, 120));
        LogWidget->setWidget(LogWidgetContents);
        QTModelEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), LogWidget);

        menuBar->addAction(menuFile->menuAction());

        retranslateUi(QTModelEditorClass);

        QMetaObject::connectSlotsByName(QTModelEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *QTModelEditorClass)
    {
        QTModelEditorClass->setWindowTitle(QApplication::translate("QTModelEditorClass", "QTModelEditor", 0));
        SceneWidget->setWindowTitle(QApplication::translate("QTModelEditorClass", "\347\274\226\350\276\221\345\231\250\350\247\206\345\233\276", 0));
        menuFile->setTitle(QApplication::translate("QTModelEditorClass", "\346\226\207\344\273\266", 0));
        LogWidget->setWindowTitle(QApplication::translate("QTModelEditorClass", "\350\276\223\345\207\272\350\247\206\345\233\276", 0));
    } // retranslateUi

};

namespace Ui {
    class QTModelEditorClass: public Ui_QTModelEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMODELEDITOR_H
