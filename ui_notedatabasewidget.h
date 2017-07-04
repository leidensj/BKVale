/********************************************************************************
** Form generated from reading UI file 'notedatabasewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTEDATABASEWIDGET_H
#define UI_NOTEDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NoteDatabaseWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonOpen;
    QPushButton *buttonRefresh;
    QPushButton *buttonRemove;
    QSpacerItem *horizontalSpacer;
    QTableView *table;

    void setupUi(QFrame *NoteDatabaseWidget)
    {
        if (NoteDatabaseWidget->objectName().isEmpty())
            NoteDatabaseWidget->setObjectName(QStringLiteral("NoteDatabaseWidget"));
        NoteDatabaseWidget->resize(273, 300);
        NoteDatabaseWidget->setFrameShape(QFrame::StyledPanel);
        NoteDatabaseWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(NoteDatabaseWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonOpen = new QPushButton(NoteDatabaseWidget);
        buttonOpen->setObjectName(QStringLiteral("buttonOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOpen->setIcon(icon);
        buttonOpen->setIconSize(QSize(24, 24));
        buttonOpen->setCheckable(false);
        buttonOpen->setFlat(true);

        horizontalLayout->addWidget(buttonOpen);

        buttonRefresh = new QPushButton(NoteDatabaseWidget);
        buttonRefresh->setObjectName(QStringLiteral("buttonRefresh"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon1);
        buttonRefresh->setIconSize(QSize(24, 24));
        buttonRefresh->setFlat(true);

        horizontalLayout->addWidget(buttonRefresh);

        buttonRemove = new QPushButton(NoteDatabaseWidget);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRemove->setIcon(icon2);
        buttonRemove->setIconSize(QSize(24, 24));
        buttonRemove->setFlat(true);

        horizontalLayout->addWidget(buttonRemove);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        table = new QTableView(NoteDatabaseWidget);
        table->setObjectName(QStringLiteral("table"));
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setSortingEnabled(true);
        table->horizontalHeader()->setHighlightSections(false);

        verticalLayout->addWidget(table);


        retranslateUi(NoteDatabaseWidget);

        buttonOpen->setDefault(false);


        QMetaObject::connectSlotsByName(NoteDatabaseWidget);
    } // setupUi

    void retranslateUi(QFrame *NoteDatabaseWidget)
    {
        NoteDatabaseWidget->setWindowTitle(QApplication::translate("NoteDatabaseWidget", "Frame", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonOpen->setToolTip(QApplication::translate("NoteDatabaseWidget", "Abrir vale", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonOpen->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonRefresh->setToolTip(QApplication::translate("NoteDatabaseWidget", "Atualizar vales", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonRefresh->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonRemove->setToolTip(QApplication::translate("NoteDatabaseWidget", "Apagar vale", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonRemove->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class NoteDatabaseWidget: public Ui_NoteDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTEDATABASEWIDGET_H
