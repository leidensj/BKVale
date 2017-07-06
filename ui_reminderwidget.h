/********************************************************************************
** Form generated from reading UI file 'reminderwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMINDERWIDGET_H
#define UI_REMINDERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ReminderWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *editTitle;
    QHBoxLayout *horizontalLayout;
    QRadioButton *buttonFontBig;
    QRadioButton *buttonFontSmall;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *editMessage;

    void setupUi(QFrame *ReminderWidget)
    {
        if (ReminderWidget->objectName().isEmpty())
            ReminderWidget->setObjectName(QStringLiteral("ReminderWidget"));
        ReminderWidget->resize(400, 300);
        ReminderWidget->setFrameShape(QFrame::StyledPanel);
        ReminderWidget->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(ReminderWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        editTitle = new QLineEdit(ReminderWidget);
        editTitle->setObjectName(QStringLiteral("editTitle"));
        editTitle->setClearButtonEnabled(false);

        verticalLayout->addWidget(editTitle);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonFontBig = new QRadioButton(ReminderWidget);
        buttonFontBig->setObjectName(QStringLiteral("buttonFontBig"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/text.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonFontBig->setIcon(icon);
        buttonFontBig->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonFontBig);

        buttonFontSmall = new QRadioButton(ReminderWidget);
        buttonFontSmall->setObjectName(QStringLiteral("buttonFontSmall"));
        buttonFontSmall->setIcon(icon);

        horizontalLayout->addWidget(buttonFontSmall);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        editMessage = new QPlainTextEdit(ReminderWidget);
        editMessage->setObjectName(QStringLiteral("editMessage"));
        editMessage->setBackgroundVisible(false);
        editMessage->setCenterOnScroll(false);

        verticalLayout->addWidget(editMessage);


        retranslateUi(ReminderWidget);

        QMetaObject::connectSlotsByName(ReminderWidget);
    } // setupUi

    void retranslateUi(QFrame *ReminderWidget)
    {
        ReminderWidget->setWindowTitle(QApplication::translate("ReminderWidget", "Frame", Q_NULLPTR));
        editTitle->setPlaceholderText(QApplication::translate("ReminderWidget", "T\303\255tulo", Q_NULLPTR));
        buttonFontBig->setText(QString());
        buttonFontSmall->setText(QString());
        editMessage->setDocumentTitle(QString());
        editMessage->setPlainText(QString());
        editMessage->setPlaceholderText(QApplication::translate("ReminderWidget", "Mensagem", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ReminderWidget: public Ui_ReminderWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMINDERWIDGET_H
