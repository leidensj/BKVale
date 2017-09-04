/********************************************************************************
** Form generated from reading UI file 'reminderview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMINDERVIEW_H
#define UI_REMINDERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ReminderView
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *editTitle;
    QHBoxLayout *horizontalLayout;
    QCheckBox *buttonUppercase;
    QFrame *line;
    QRadioButton *buttonFontBig;
    QRadioButton *buttonFontSmall;
    QSpacerItem *horizontalSpacer;
    QPlainTextEdit *editMessage;

    void setupUi(QFrame *ReminderView)
    {
        if (ReminderView->objectName().isEmpty())
            ReminderView->setObjectName(QStringLiteral("ReminderView"));
        ReminderView->resize(400, 300);
        ReminderView->setFrameShape(QFrame::StyledPanel);
        ReminderView->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(ReminderView);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        editTitle = new QLineEdit(ReminderView);
        editTitle->setObjectName(QStringLiteral("editTitle"));
        editTitle->setClearButtonEnabled(false);

        verticalLayout->addWidget(editTitle);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonUppercase = new QCheckBox(ReminderView);
        buttonUppercase->setObjectName(QStringLiteral("buttonUppercase"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/uppercase.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUppercase->setIcon(icon);
        buttonUppercase->setIconSize(QSize(24, 24));
        buttonUppercase->setChecked(false);
        buttonUppercase->setTristate(true);

        horizontalLayout->addWidget(buttonUppercase);

        line = new QFrame(ReminderView);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        buttonFontBig = new QRadioButton(ReminderView);
        buttonFontBig->setObjectName(QStringLiteral("buttonFontBig"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/text.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonFontBig->setIcon(icon1);
        buttonFontBig->setIconSize(QSize(24, 24));

        horizontalLayout->addWidget(buttonFontBig);

        buttonFontSmall = new QRadioButton(ReminderView);
        buttonFontSmall->setObjectName(QStringLiteral("buttonFontSmall"));
        buttonFontSmall->setIcon(icon1);
        buttonFontSmall->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(buttonFontSmall);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        editMessage = new QPlainTextEdit(ReminderView);
        editMessage->setObjectName(QStringLiteral("editMessage"));
        editMessage->setBackgroundVisible(false);
        editMessage->setCenterOnScroll(false);

        verticalLayout->addWidget(editMessage);


        retranslateUi(ReminderView);

        QMetaObject::connectSlotsByName(ReminderView);
    } // setupUi

    void retranslateUi(QFrame *ReminderView)
    {
        ReminderView->setWindowTitle(QApplication::translate("ReminderView", "Frame", Q_NULLPTR));
        editTitle->setPlaceholderText(QApplication::translate("ReminderView", "T\303\255tulo", Q_NULLPTR));
        buttonUppercase->setText(QString());
        buttonFontBig->setText(QString());
        buttonFontSmall->setText(QString());
        editMessage->setDocumentTitle(QString());
        editMessage->setPlainText(QString());
        editMessage->setPlaceholderText(QApplication::translate("ReminderView", "Mensagem", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ReminderView: public Ui_ReminderView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMINDERVIEW_H
