/********************************************************************************
** Form generated from reading UI file 'consumptionwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSUMPTIONWIDGET_H
#define UI_CONSUMPTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConsumptionWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateEdit *dateInitial;
    QLabel *label_2;
    QDateEdit *dateFinal;
    QLineEdit *editTotal;
    QPushButton *buttonChart;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonCreate;
    QPushButton *buttonRefresh;
    QPushButton *buttonRemove;
    QSpacerItem *horizontalSpacer_2;
    QTableView *table;

    void setupUi(QFrame *ConsumptionWidget)
    {
        if (ConsumptionWidget->objectName().isEmpty())
            ConsumptionWidget->setObjectName(QStringLiteral("ConsumptionWidget"));
        ConsumptionWidget->resize(618, 348);
        ConsumptionWidget->setFrameShape(QFrame::StyledPanel);
        ConsumptionWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(ConsumptionWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        frame = new QFrame(ConsumptionWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, 9, 9, 9);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        dateInitial = new QDateEdit(frame);
        dateInitial->setObjectName(QStringLiteral("dateInitial"));
        dateInitial->setCalendarPopup(true);

        horizontalLayout->addWidget(dateInitial);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        dateFinal = new QDateEdit(frame);
        dateFinal->setObjectName(QStringLiteral("dateFinal"));
        dateFinal->setWrapping(false);
        dateFinal->setProperty("showGroupSeparator", QVariant(false));
        dateFinal->setCalendarPopup(true);

        horizontalLayout->addWidget(dateFinal);

        editTotal = new QLineEdit(frame);
        editTotal->setObjectName(QStringLiteral("editTotal"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        editTotal->setPalette(palette);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        editTotal->setFont(font);
        editTotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editTotal->setReadOnly(true);

        horizontalLayout->addWidget(editTotal);

        buttonChart = new QPushButton(frame);
        buttonChart->setObjectName(QStringLiteral("buttonChart"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/chart.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonChart->setIcon(icon);
        buttonChart->setIconSize(QSize(24, 24));
        buttonChart->setFlat(true);

        horizontalLayout->addWidget(buttonChart);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(frame);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        buttonCreate = new QPushButton(ConsumptionWidget);
        buttonCreate->setObjectName(QStringLiteral("buttonCreate"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/file.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonCreate->setIcon(icon1);
        buttonCreate->setIconSize(QSize(24, 24));
        buttonCreate->setFlat(true);

        horizontalLayout_3->addWidget(buttonCreate);

        buttonRefresh = new QPushButton(ConsumptionWidget);
        buttonRefresh->setObjectName(QStringLiteral("buttonRefresh"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon2);
        buttonRefresh->setIconSize(QSize(24, 24));
        buttonRefresh->setFlat(true);

        horizontalLayout_3->addWidget(buttonRefresh);

        buttonRemove = new QPushButton(ConsumptionWidget);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/res/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRemove->setIcon(icon3);
        buttonRemove->setIconSize(QSize(24, 24));
        buttonRemove->setFlat(true);

        horizontalLayout_3->addWidget(buttonRemove);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        table = new QTableView(ConsumptionWidget);
        table->setObjectName(QStringLiteral("table"));
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setAlternatingRowColors(true);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setSortingEnabled(true);

        verticalLayout->addWidget(table);


        retranslateUi(ConsumptionWidget);

        QMetaObject::connectSlotsByName(ConsumptionWidget);
    } // setupUi

    void retranslateUi(QFrame *ConsumptionWidget)
    {
        ConsumptionWidget->setWindowTitle(QApplication::translate("ConsumptionWidget", "Frame", Q_NULLPTR));
        label->setText(QApplication::translate("ConsumptionWidget", "Data inicial:", Q_NULLPTR));
        dateInitial->setDisplayFormat(QApplication::translate("ConsumptionWidget", "dd/MM/yyyy", Q_NULLPTR));
        label_2->setText(QApplication::translate("ConsumptionWidget", "Data final:", Q_NULLPTR));
        dateFinal->setDisplayFormat(QApplication::translate("ConsumptionWidget", "dd/MM/yyyy", Q_NULLPTR));
        editTotal->setPlaceholderText(QApplication::translate("ConsumptionWidget", "Consumo Total", Q_NULLPTR));
        buttonChart->setText(QString());
        buttonCreate->setText(QString());
        buttonRefresh->setText(QString());
        buttonRemove->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ConsumptionWidget: public Ui_ConsumptionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSUMPTIONWIDGET_H
