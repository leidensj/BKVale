/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BaitaAssistant
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionPrint;
    QAction *actionSettings;
    QAction *actionInfo;
    QAction *actionItems;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabNotes;
    QVBoxLayout *verticalLayout;
    QWidget *tabReminder;
    QVBoxLayout *verticalLayout_2;
    QWidget *tabShop;
    QWidget *tabConsumption;
    QVBoxLayout *verticalLayout_3;
    QToolBar *toolMain;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BaitaAssistant)
    {
        if (BaitaAssistant->objectName().isEmpty())
            BaitaAssistant->setObjectName(QStringLiteral("BaitaAssistant"));
        BaitaAssistant->resize(1000, 600);
        BaitaAssistant->setMinimumSize(QSize(535, 349));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/baita.png"), QSize(), QIcon::Normal, QIcon::Off);
        BaitaAssistant->setWindowIcon(icon);
        actionConnect = new QAction(BaitaAssistant);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/connected.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon1);
        actionDisconnect = new QAction(BaitaAssistant);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/disconnected.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon2);
        actionPrint = new QAction(BaitaAssistant);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/res/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon3);
        actionSettings = new QAction(BaitaAssistant);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/res/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon4);
        actionInfo = new QAction(BaitaAssistant);
        actionInfo->setObjectName(QStringLiteral("actionInfo"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/res/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInfo->setIcon(icon5);
        actionItems = new QAction(BaitaAssistant);
        actionItems->setObjectName(QStringLiteral("actionItems"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/res/item.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionItems->setIcon(icon6);
        centralWidget = new QWidget(BaitaAssistant);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QFont font;
        font.setPointSize(12);
        tabWidget->setFont(font);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(24, 24));
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabNotes = new QWidget();
        tabNotes->setObjectName(QStringLiteral("tabNotes"));
        verticalLayout = new QVBoxLayout(tabNotes);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/res/note.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabNotes, icon7, QString());
        tabReminder = new QWidget();
        tabReminder->setObjectName(QStringLiteral("tabReminder"));
        verticalLayout_2 = new QVBoxLayout(tabReminder);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/res/postit.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabReminder, icon8, QString());
        tabShop = new QWidget();
        tabShop->setObjectName(QStringLiteral("tabShop"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/res/shop.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabShop, icon9, QString());
        tabConsumption = new QWidget();
        tabConsumption->setObjectName(QStringLiteral("tabConsumption"));
        verticalLayout_3 = new QVBoxLayout(tabConsumption);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/res/stock.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabConsumption, icon10, QString());

        horizontalLayout->addWidget(tabWidget);

        BaitaAssistant->setCentralWidget(centralWidget);
        toolMain = new QToolBar(BaitaAssistant);
        toolMain->setObjectName(QStringLiteral("toolMain"));
        toolMain->setMovable(false);
        toolMain->setIconSize(QSize(24, 24));
        toolMain->setFloatable(false);
        BaitaAssistant->addToolBar(Qt::TopToolBarArea, toolMain);
        statusBar = new QStatusBar(BaitaAssistant);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BaitaAssistant->setStatusBar(statusBar);

        toolMain->addAction(actionItems);
        toolMain->addSeparator();
        toolMain->addAction(actionConnect);
        toolMain->addAction(actionDisconnect);
        toolMain->addAction(actionPrint);
        toolMain->addSeparator();
        toolMain->addAction(actionSettings);
        toolMain->addAction(actionInfo);

        retranslateUi(BaitaAssistant);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BaitaAssistant);
    } // setupUi

    void retranslateUi(QMainWindow *BaitaAssistant)
    {
        BaitaAssistant->setWindowTitle(QApplication::translate("BaitaAssistant", "Baita Assistente", Q_NULLPTR));
        actionConnect->setText(QApplication::translate("BaitaAssistant", "Conectar \303\240 impressora", Q_NULLPTR));
        actionDisconnect->setText(QApplication::translate("BaitaAssistant", "Desconectar da impressora", Q_NULLPTR));
        actionPrint->setText(QApplication::translate("BaitaAssistant", "Imprimir", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionPrint->setShortcut(QApplication::translate("BaitaAssistant", "Ctrl+P", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionSettings->setText(QApplication::translate("BaitaAssistant", "Configura\303\247\303\265es", Q_NULLPTR));
        actionInfo->setText(QApplication::translate("BaitaAssistant", "Informa\303\247\303\265es", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionInfo->setToolTip(QApplication::translate("BaitaAssistant", "Informa\303\247\303\265es", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionItems->setText(QApplication::translate("BaitaAssistant", "Produtos", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionItems->setToolTip(QApplication::translate("BaitaAssistant", "Gerenciar Produtos", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tabNotes), QApplication::translate("BaitaAssistant", "Vales", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabReminder), QApplication::translate("BaitaAssistant", "Lembretes", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabShop), QApplication::translate("BaitaAssistant", "Compras", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabConsumption), QApplication::translate("BaitaAssistant", "Consumo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BaitaAssistant: public Ui_BaitaAssistant {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
