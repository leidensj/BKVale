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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConsumptionWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QDateEdit *date;
    QLabel *labelDay;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonSearch;
    QLineEdit *editItem;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *editPrice;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QDoubleSpinBox *spinAmmount;
    QSpacerItem *verticalSpacer;

    void setupUi(QFrame *ConsumptionWidget)
    {
        if (ConsumptionWidget->objectName().isEmpty())
            ConsumptionWidget->setObjectName(QStringLiteral("ConsumptionWidget"));
        ConsumptionWidget->resize(261, 201);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConsumptionWidget->sizePolicy().hasHeightForWidth());
        ConsumptionWidget->setSizePolicy(sizePolicy);
        ConsumptionWidget->setFrameShape(QFrame::StyledPanel);
        ConsumptionWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(ConsumptionWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        date = new QDateEdit(ConsumptionWidget);
        date->setObjectName(QStringLiteral("date"));
        QFont font;
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        date->setFont(font);
        date->setProperty("showGroupSeparator", QVariant(false));
        date->setCalendarPopup(true);
        date->setCurrentSectionIndex(0);

        horizontalLayout_6->addWidget(date);

        labelDay = new QLabel(ConsumptionWidget);
        labelDay->setObjectName(QStringLiteral("labelDay"));
        labelDay->setFont(font);

        horizontalLayout_6->addWidget(labelDay);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonSearch = new QPushButton(ConsumptionWidget);
        buttonSearch->setObjectName(QStringLiteral("buttonSearch"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearch->setIcon(icon);
        buttonSearch->setIconSize(QSize(24, 24));
        buttonSearch->setFlat(true);

        horizontalLayout->addWidget(buttonSearch);

        editItem = new QLineEdit(ConsumptionWidget);
        editItem->setObjectName(QStringLiteral("editItem"));
        editItem->setEnabled(false);
        editItem->setFont(font);
        editItem->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(editItem);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(ConsumptionWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(24, 24));
        label_2->setMaximumSize(QSize(24, 24));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/icons/res/price.png")));
        label_2->setScaledContents(true);

        horizontalLayout_3->addWidget(label_2);

        editPrice = new QLineEdit(ConsumptionWidget);
        editPrice->setObjectName(QStringLiteral("editPrice"));
        editPrice->setEnabled(true);
        editPrice->setFont(font);
        editPrice->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(editPrice);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(ConsumptionWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(24, 24));
        label_3->setMaximumSize(QSize(24, 24));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/icons/res/ammount.png")));
        label_3->setScaledContents(true);

        horizontalLayout_4->addWidget(label_3);

        spinAmmount = new QDoubleSpinBox(ConsumptionWidget);
        spinAmmount->setObjectName(QStringLiteral("spinAmmount"));
        spinAmmount->setFont(font);
        spinAmmount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinAmmount->setProperty("showGroupSeparator", QVariant(false));
        spinAmmount->setDecimals(3);
        spinAmmount->setMaximum(9999.99);

        horizontalLayout_4->addWidget(spinAmmount);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ConsumptionWidget);

        QMetaObject::connectSlotsByName(ConsumptionWidget);
    } // setupUi

    void retranslateUi(QFrame *ConsumptionWidget)
    {
        ConsumptionWidget->setWindowTitle(QApplication::translate("ConsumptionWidget", "Frame", Q_NULLPTR));
        labelDay->setText(QApplication::translate("ConsumptionWidget", "DayOfTheWeek", Q_NULLPTR));
        buttonSearch->setText(QString());
        editItem->setPlaceholderText(QApplication::translate("ConsumptionWidget", "Produto", Q_NULLPTR));
        label_2->setText(QString());
        editPrice->setPlaceholderText(QApplication::translate("ConsumptionWidget", "Pre\303\247o", Q_NULLPTR));
        label_3->setText(QString());
        spinAmmount->setSpecialValueText(QApplication::translate("ConsumptionWidget", "Quantidade", Q_NULLPTR));
        spinAmmount->setSuffix(QString());
    } // retranslateUi

};

namespace Ui {
    class ConsumptionWidget: public Ui_ConsumptionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSUMPTIONWIDGET_H
