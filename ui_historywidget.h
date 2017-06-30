/********************************************************************************
** Form generated from reading UI file 'historywidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYWIDGET_H
#define UI_HISTORYWIDGET_H

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

class Ui_HistoryWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonOpen;
    QPushButton *buttonRefresh;
    QPushButton *buttonDelete;
    QSpacerItem *horizontalSpacer;
    QTableView *table;

    void setupUi(QFrame *HistoryWidget)
    {
        if (HistoryWidget->objectName().isEmpty())
            HistoryWidget->setObjectName(QStringLiteral("HistoryWidget"));
        HistoryWidget->resize(273, 300);
        HistoryWidget->setFrameShape(QFrame::StyledPanel);
        HistoryWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(HistoryWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonOpen = new QPushButton(HistoryWidget);
        buttonOpen->setObjectName(QStringLiteral("buttonOpen"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOpen->setIcon(icon);
        buttonOpen->setCheckable(false);
        buttonOpen->setFlat(false);

        horizontalLayout->addWidget(buttonOpen);

        buttonRefresh = new QPushButton(HistoryWidget);
        buttonRefresh->setObjectName(QStringLiteral("buttonRefresh"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon1);

        horizontalLayout->addWidget(buttonRefresh);

        buttonDelete = new QPushButton(HistoryWidget);
        buttonDelete->setObjectName(QStringLiteral("buttonDelete"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDelete->setIcon(icon2);

        horizontalLayout->addWidget(buttonDelete);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        table = new QTableView(HistoryWidget);
        table->setObjectName(QStringLiteral("table"));
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setSortingEnabled(true);
        table->horizontalHeader()->setHighlightSections(false);

        verticalLayout->addWidget(table);


        retranslateUi(HistoryWidget);

        buttonOpen->setDefault(false);


        QMetaObject::connectSlotsByName(HistoryWidget);
    } // setupUi

    void retranslateUi(QFrame *HistoryWidget)
    {
        HistoryWidget->setWindowTitle(QApplication::translate("HistoryWidget", "Frame", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonOpen->setToolTip(QApplication::translate("HistoryWidget", "Abrir vale", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonOpen->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonRefresh->setToolTip(QApplication::translate("HistoryWidget", "Atualizar vales", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonRefresh->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonDelete->setToolTip(QApplication::translate("HistoryWidget", "Apagar vale", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonDelete->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class HistoryWidget: public Ui_HistoryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYWIDGET_H
