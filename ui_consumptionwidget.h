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
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConsumptionWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QDateEdit *date;
    QLabel *labelDay;
    QPushButton *buttonSearch;
    QLineEdit *editItem;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QDoubleSpinBox *spinPrice;
    QLabel *label_3;
    QDoubleSpinBox *spinAmmount;
    QPushButton *buttonSave;

    void setupUi(QFrame *ConsumptionWidget)
    {
        if (ConsumptionWidget->objectName().isEmpty())
            ConsumptionWidget->setObjectName(QStringLiteral("ConsumptionWidget"));
        ConsumptionWidget->resize(463, 118);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConsumptionWidget->sizePolicy().hasHeightForWidth());
        ConsumptionWidget->setSizePolicy(sizePolicy);
        ConsumptionWidget->setMinimumSize(QSize(0, 0));
        ConsumptionWidget->setMaximumSize(QSize(16777215, 16777215));
        ConsumptionWidget->setFrameShape(QFrame::Box);
        ConsumptionWidget->setFrameShadow(QFrame::Plain);
        verticalLayout = new QVBoxLayout(ConsumptionWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(9, 9, 9, 9);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
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
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        labelDay->setPalette(palette);
        labelDay->setFont(font);

        horizontalLayout_6->addWidget(labelDay);

        buttonSearch = new QPushButton(ConsumptionWidget);
        buttonSearch->setObjectName(QStringLiteral("buttonSearch"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSearch->setIcon(icon);
        buttonSearch->setIconSize(QSize(24, 24));
        buttonSearch->setCheckable(false);
        buttonSearch->setChecked(false);
        buttonSearch->setFlat(true);

        horizontalLayout_6->addWidget(buttonSearch);

        editItem = new QLineEdit(ConsumptionWidget);
        editItem->setObjectName(QStringLiteral("editItem"));
        editItem->setEnabled(false);
        editItem->setFont(font);
        editItem->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(editItem);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(ConsumptionWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(24, 24));
        label_2->setMaximumSize(QSize(24, 24));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/icons/res/price.png")));
        label_2->setScaledContents(true);

        horizontalLayout_3->addWidget(label_2);

        spinPrice = new QDoubleSpinBox(ConsumptionWidget);
        spinPrice->setObjectName(QStringLiteral("spinPrice"));
        QFont font1;
        font1.setPointSize(12);
        spinPrice->setFont(font1);
        spinPrice->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinPrice->setProperty("showGroupSeparator", QVariant(false));
        spinPrice->setMaximum(9999.99);

        horizontalLayout_3->addWidget(spinPrice);

        label_3 = new QLabel(ConsumptionWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(24, 24));
        label_3->setMaximumSize(QSize(24, 24));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/icons/res/ammount.png")));
        label_3->setScaledContents(true);

        horizontalLayout_3->addWidget(label_3);

        spinAmmount = new QDoubleSpinBox(ConsumptionWidget);
        spinAmmount->setObjectName(QStringLiteral("spinAmmount"));
        spinAmmount->setMinimumSize(QSize(0, 0));
        spinAmmount->setFont(font);
        spinAmmount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinAmmount->setProperty("showGroupSeparator", QVariant(false));
        spinAmmount->setDecimals(3);
        spinAmmount->setMaximum(9999.99);

        horizontalLayout_3->addWidget(spinAmmount);

        buttonSave = new QPushButton(ConsumptionWidget);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonSave->sizePolicy().hasHeightForWidth());
        buttonSave->setSizePolicy(sizePolicy1);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSave->setIcon(icon1);
        buttonSave->setIconSize(QSize(24, 24));
        buttonSave->setCheckable(true);
        buttonSave->setFlat(true);

        horizontalLayout_3->addWidget(buttonSave);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(ConsumptionWidget);

        QMetaObject::connectSlotsByName(ConsumptionWidget);
    } // setupUi

    void retranslateUi(QFrame *ConsumptionWidget)
    {
        ConsumptionWidget->setWindowTitle(QApplication::translate("ConsumptionWidget", "Frame", Q_NULLPTR));
        labelDay->setText(QApplication::translate("ConsumptionWidget", "DayOfTheWeek", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonSearch->setToolTip(QApplication::translate("ConsumptionWidget", "Pesquisar produto", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSearch->setText(QString());
        editItem->setPlaceholderText(QApplication::translate("ConsumptionWidget", "Produto", Q_NULLPTR));
        label_2->setText(QString());
        spinPrice->setSpecialValueText(QApplication::translate("ConsumptionWidget", "Pre\303\247o", Q_NULLPTR));
        label_3->setText(QString());
        spinAmmount->setSpecialValueText(QApplication::translate("ConsumptionWidget", "Quantidade", Q_NULLPTR));
        spinAmmount->setSuffix(QString());
#ifndef QT_NO_TOOLTIP
        buttonSave->setToolTip(QApplication::translate("ConsumptionWidget", "Salvar consumo", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSave->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ConsumptionWidget: public Ui_ConsumptionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSUMPTIONWIDGET_H
