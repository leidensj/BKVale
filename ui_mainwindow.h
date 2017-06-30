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
    QAction *actionNoteAdd;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionPrint;
    QAction *actionNoteNew;
    QAction *actionSettings;
    QAction *actionNoteRemove;
    QAction *actionNoteSearch;
    QAction *actionInfo;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tabNotes;
    QVBoxLayout *verticalLayout;
    QWidget *tabPostits;
    QWidget *tabShop;
    QToolBar *toolMain;
    QStatusBar *statusBar;
    QToolBar *toolNotes;
    QToolBar *toolPostits;

    void setupUi(QMainWindow *BaitaAssistant)
    {
        if (BaitaAssistant->objectName().isEmpty())
            BaitaAssistant->setObjectName(QStringLiteral("BaitaAssistant"));
        BaitaAssistant->resize(822, 390);
        BaitaAssistant->setMinimumSize(QSize(535, 349));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/baita.png"), QSize(), QIcon::Normal, QIcon::Off);
        BaitaAssistant->setWindowIcon(icon);
        actionNoteAdd = new QAction(BaitaAssistant);
        actionNoteAdd->setObjectName(QStringLiteral("actionNoteAdd"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNoteAdd->setIcon(icon1);
        actionConnect = new QAction(BaitaAssistant);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/connected.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon2);
        actionDisconnect = new QAction(BaitaAssistant);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/res/disconnected.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon3);
        actionPrint = new QAction(BaitaAssistant);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/res/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon4);
        actionNoteNew = new QAction(BaitaAssistant);
        actionNoteNew->setObjectName(QStringLiteral("actionNoteNew"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icons/res/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNoteNew->setIcon(icon5);
        actionSettings = new QAction(BaitaAssistant);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icons/res/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon6);
        actionNoteRemove = new QAction(BaitaAssistant);
        actionNoteRemove->setObjectName(QStringLiteral("actionNoteRemove"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icons/res/remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNoteRemove->setIcon(icon7);
        actionNoteSearch = new QAction(BaitaAssistant);
        actionNoteSearch->setObjectName(QStringLiteral("actionNoteSearch"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icons/res/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNoteSearch->setIcon(icon8);
        actionInfo = new QAction(BaitaAssistant);
        actionInfo->setObjectName(QStringLiteral("actionInfo"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icons/res/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInfo->setIcon(icon9);
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
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icons/res/note.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabNotes, icon10, QString());
        tabPostits = new QWidget();
        tabPostits->setObjectName(QStringLiteral("tabPostits"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icons/res/postit.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabPostits, icon11, QString());
        tabShop = new QWidget();
        tabShop->setObjectName(QStringLiteral("tabShop"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icons/res/shop.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tabShop, icon12, QString());

        horizontalLayout->addWidget(tabWidget);

        BaitaAssistant->setCentralWidget(centralWidget);
        toolMain = new QToolBar(BaitaAssistant);
        toolMain->setObjectName(QStringLiteral("toolMain"));
        toolMain->setMovable(false);
        toolMain->setIconSize(QSize(24, 24));
        toolMain->setFloatable(false);
        BaitaAssistant->addToolBar(Qt::LeftToolBarArea, toolMain);
        statusBar = new QStatusBar(BaitaAssistant);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BaitaAssistant->setStatusBar(statusBar);
        toolNotes = new QToolBar(BaitaAssistant);
        toolNotes->setObjectName(QStringLiteral("toolNotes"));
        toolNotes->setMovable(false);
        BaitaAssistant->addToolBar(Qt::TopToolBarArea, toolNotes);
        BaitaAssistant->insertToolBarBreak(toolNotes);
        toolPostits = new QToolBar(BaitaAssistant);
        toolPostits->setObjectName(QStringLiteral("toolPostits"));
        toolPostits->setMovable(false);
        BaitaAssistant->addToolBar(Qt::TopToolBarArea, toolPostits);
        BaitaAssistant->insertToolBarBreak(toolPostits);

        toolMain->addAction(actionConnect);
        toolMain->addAction(actionDisconnect);
        toolMain->addAction(actionPrint);
        toolMain->addSeparator();
        toolMain->addAction(actionSettings);
        toolMain->addAction(actionInfo);
        toolNotes->addAction(actionNoteNew);
        toolNotes->addAction(actionNoteSearch);
        toolNotes->addAction(actionNoteAdd);
        toolNotes->addAction(actionNoteRemove);

        retranslateUi(BaitaAssistant);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BaitaAssistant);
    } // setupUi

    void retranslateUi(QMainWindow *BaitaAssistant)
    {
        BaitaAssistant->setWindowTitle(QApplication::translate("BaitaAssistant", "Baita Assistente", Q_NULLPTR));
        actionNoteAdd->setText(QApplication::translate("BaitaAssistant", "Adicionar item", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNoteAdd->setToolTip(QApplication::translate("BaitaAssistant", "Adicionar item", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNoteAdd->setShortcut(QApplication::translate("BaitaAssistant", "Alt++", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionConnect->setText(QApplication::translate("BaitaAssistant", "Conectar \303\240 impressora", Q_NULLPTR));
        actionDisconnect->setText(QApplication::translate("BaitaAssistant", "Desconectar da impressora", Q_NULLPTR));
        actionPrint->setText(QApplication::translate("BaitaAssistant", "Imprimir", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionPrint->setShortcut(QApplication::translate("BaitaAssistant", "Ctrl+P", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionNoteNew->setText(QApplication::translate("BaitaAssistant", "Novo vale", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNoteNew->setToolTip(QApplication::translate("BaitaAssistant", "Novo vale", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionSettings->setText(QApplication::translate("BaitaAssistant", "Configura\303\247\303\265es", Q_NULLPTR));
        actionNoteRemove->setText(QApplication::translate("BaitaAssistant", "Remover item", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNoteRemove->setToolTip(QApplication::translate("BaitaAssistant", "Remover item", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNoteRemove->setShortcut(QApplication::translate("BaitaAssistant", "Alt+-", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionNoteSearch->setText(QApplication::translate("BaitaAssistant", "Pesquisar vales", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionNoteSearch->setToolTip(QApplication::translate("BaitaAssistant", "Pesquisar vales", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNoteSearch->setShortcut(QApplication::translate("BaitaAssistant", "Ctrl+F", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionInfo->setText(QApplication::translate("BaitaAssistant", "Informa\303\247\303\265es", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionInfo->setToolTip(QApplication::translate("BaitaAssistant", "Informa\303\247\303\265es", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tabNotes), QApplication::translate("BaitaAssistant", "Vales", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabPostits), QApplication::translate("BaitaAssistant", "Lembretes", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tabShop), QApplication::translate("BaitaAssistant", "Compras", Q_NULLPTR));
        toolNotes->setWindowTitle(QApplication::translate("BaitaAssistant", "toolBar", Q_NULLPTR));
        toolPostits->setWindowTitle(QApplication::translate("BaitaAssistant", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BaitaAssistant: public Ui_BaitaAssistant {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
