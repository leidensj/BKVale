/********************************************************************************
** Form generated from reading UI file 'productwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCTWIDGET_H
#define UI_PRODUCTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ProductWidget
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *cmdFrame;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonCreate;
    QPushButton *buttonSave;
    QPushButton *buttonDiscard;
    QPushButton *buttonRefresh;
    QPushButton *buttonRemove;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *filterLayout;
    QCheckBox *buttonContains;
    QTableView *table;

    void setupUi(QFrame *ProductWidget)
    {
        if (ProductWidget->objectName().isEmpty())
            ProductWidget->setObjectName(QStringLiteral("ProductWidget"));
        ProductWidget->resize(635, 394);
        ProductWidget->setFrameShape(QFrame::StyledPanel);
        ProductWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(ProductWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cmdFrame = new QFrame(ProductWidget);
        cmdFrame->setObjectName(QStringLiteral("cmdFrame"));
        cmdFrame->setFrameShape(QFrame::NoFrame);
        cmdFrame->setFrameShadow(QFrame::Plain);
        cmdFrame->setLineWidth(0);
        horizontalLayout = new QHBoxLayout(cmdFrame);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        buttonCreate = new QPushButton(cmdFrame);
        buttonCreate->setObjectName(QStringLiteral("buttonCreate"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/newitem.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonCreate->setIcon(icon);
        buttonCreate->setIconSize(QSize(24, 24));
        buttonCreate->setFlat(true);

        horizontalLayout->addWidget(buttonCreate);

        buttonSave = new QPushButton(cmdFrame);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSave->setIcon(icon1);
        buttonSave->setIconSize(QSize(24, 24));
        buttonSave->setFlat(true);

        horizontalLayout->addWidget(buttonSave);

        buttonDiscard = new QPushButton(cmdFrame);
        buttonDiscard->setObjectName(QStringLiteral("buttonDiscard"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/revert.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDiscard->setIcon(icon2);
        buttonDiscard->setIconSize(QSize(24, 24));
        buttonDiscard->setFlat(true);

        horizontalLayout->addWidget(buttonDiscard);

        buttonRefresh = new QPushButton(cmdFrame);
        buttonRefresh->setObjectName(QStringLiteral("buttonRefresh"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/res/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon3);
        buttonRefresh->setIconSize(QSize(24, 24));
        buttonRefresh->setFlat(true);

        horizontalLayout->addWidget(buttonRefresh);

        buttonRemove = new QPushButton(cmdFrame);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/res/trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRemove->setIcon(icon4);
        buttonRemove->setIconSize(QSize(24, 24));
        buttonRemove->setFlat(true);

        horizontalLayout->addWidget(buttonRemove);

        horizontalSpacer = new QSpacerItem(402, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(cmdFrame);

        filterLayout = new QHBoxLayout();
        filterLayout->setObjectName(QStringLiteral("filterLayout"));
        buttonContains = new QCheckBox(ProductWidget);
        buttonContains->setObjectName(QStringLiteral("buttonContains"));

        filterLayout->addWidget(buttonContains);


        verticalLayout->addLayout(filterLayout);

        table = new QTableView(ProductWidget);
        table->setObjectName(QStringLiteral("table"));
        QFont font;
        font.setPointSize(12);
        table->setFont(font);
        table->setAlternatingRowColors(false);
        table->setIconSize(QSize(24, 24));
        table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        table->setSortingEnabled(true);

        verticalLayout->addWidget(table);


        retranslateUi(ProductWidget);

        QMetaObject::connectSlotsByName(ProductWidget);
    } // setupUi

    void retranslateUi(QFrame *ProductWidget)
    {
        ProductWidget->setWindowTitle(QApplication::translate("ProductWidget", "Frame", Q_NULLPTR));
        buttonCreate->setText(QString());
        buttonSave->setText(QString());
        buttonDiscard->setText(QString());
        buttonRefresh->setText(QString());
        buttonRemove->setText(QString());
        buttonContains->setText(QApplication::translate("ProductWidget", "Contendo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ProductWidget: public Ui_ProductWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCTWIDGET_H
