/********************************************************************************
** Form generated from reading UI file 'notewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTEWIDGET_H
#define UI_NOTEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NoteWidget
{
public:
    QVBoxLayout *verticalLayout_5;
    QSplitter *splitter;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QFrame *frameNote;
    QVBoxLayout *verticalLayout;
    QFrame *frameHeader;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *number;
    QLabel *label;
    QDateEdit *date;
    QSpacerItem *horizontalSpacer;
    QFrame *frameSupplier;
    QHBoxLayout *horizontalLayout_3;
    QFrame *frameTable;
    QVBoxLayout *verticalLayout_3;
    QFrame *frameTotal;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *total;

    void setupUi(QFrame *NoteWidget)
    {
        if (NoteWidget->objectName().isEmpty())
            NoteWidget->setObjectName(QStringLiteral("NoteWidget"));
        NoteWidget->resize(849, 378);
        NoteWidget->setFrameShape(QFrame::NoFrame);
        NoteWidget->setFrameShadow(QFrame::Sunken);
        verticalLayout_5 = new QVBoxLayout(NoteWidget);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(NoteWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        dockWidget = new QDockWidget(splitter);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy);
        dockWidget->setMinimumSize(QSize(120, 223));
        dockWidget->setMaximumSize(QSize(524287, 524287));
        dockWidget->setFeatures(QDockWidget::DockWidgetClosable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        dockWidget->setWidget(dockWidgetContents);
        splitter->addWidget(dockWidget);
        frameNote = new QFrame(splitter);
        frameNote->setObjectName(QStringLiteral("frameNote"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frameNote->sizePolicy().hasHeightForWidth());
        frameNote->setSizePolicy(sizePolicy1);
        frameNote->setFrameShape(QFrame::StyledPanel);
        frameNote->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frameNote);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frameHeader = new QFrame(frameNote);
        frameHeader->setObjectName(QStringLiteral("frameHeader"));
        frameHeader->setFrameShape(QFrame::StyledPanel);
        frameHeader->setFrameShadow(QFrame::Plain);
        verticalLayout_2 = new QVBoxLayout(frameHeader);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        frame_3 = new QFrame(frameHeader);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_3);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(frame_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        number = new QSpinBox(frame_3);
        number->setObjectName(QStringLiteral("number"));
        number->setMinimumSize(QSize(90, 0));
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        number->setPalette(palette);
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        number->setFont(font1);
        number->setWrapping(false);
        number->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        number->setReadOnly(true);
        number->setButtonSymbols(QAbstractSpinBox::NoButtons);
        number->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
        number->setProperty("showGroupSeparator", QVariant(false));
        number->setMinimum(0);
        number->setMaximum(999999999);
        number->setDisplayIntegerBase(10);

        horizontalLayout_2->addWidget(number);

        label = new QLabel(frame_3);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        date = new QDateEdit(frame_3);
        date->setObjectName(QStringLiteral("date"));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        date->setFont(font2);
        date->setWrapping(false);
        date->setFrame(true);
        date->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        date->setCalendarPopup(true);

        horizontalLayout_2->addWidget(date);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(frame_3);

        frameSupplier = new QFrame(frameHeader);
        frameSupplier->setObjectName(QStringLiteral("frameSupplier"));
        frameSupplier->setFrameShape(QFrame::StyledPanel);
        frameSupplier->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frameSupplier);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout_2->addWidget(frameSupplier);


        verticalLayout->addWidget(frameHeader);

        frameTable = new QFrame(frameNote);
        frameTable->setObjectName(QStringLiteral("frameTable"));
        frameTable->setFrameShape(QFrame::StyledPanel);
        frameTable->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frameTable);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(frameTable);

        frameTotal = new QFrame(frameNote);
        frameTotal->setObjectName(QStringLiteral("frameTotal"));
        frameTotal->setFrameShape(QFrame::StyledPanel);
        frameTotal->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameTotal);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        total = new QLineEdit(frameTotal);
        total->setObjectName(QStringLiteral("total"));
        total->setEnabled(true);
        total->setMinimumSize(QSize(120, 0));
        total->setMaximumSize(QSize(120, 16777215));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        total->setPalette(palette1);
        total->setFont(font1);
        total->setFrame(true);
        total->setEchoMode(QLineEdit::Normal);
        total->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        total->setReadOnly(true);
        total->setClearButtonEnabled(false);

        horizontalLayout->addWidget(total);


        verticalLayout->addWidget(frameTotal);

        splitter->addWidget(frameNote);

        verticalLayout_5->addWidget(splitter);


        retranslateUi(NoteWidget);

        QMetaObject::connectSlotsByName(NoteWidget);
    } // setupUi

    void retranslateUi(QFrame *NoteWidget)
    {
        NoteWidget->setWindowTitle(QApplication::translate("NoteWidget", "BKFrame", Q_NULLPTR));
        dockWidget->setWindowTitle(QApplication::translate("NoteWidget", "Pesquisar", Q_NULLPTR));
        label_2->setText(QApplication::translate("NoteWidget", "N\303\272mero:", Q_NULLPTR));
        number->setPrefix(QString());
        label->setText(QApplication::translate("NoteWidget", "Data:", Q_NULLPTR));
        total->setPlaceholderText(QApplication::translate("NoteWidget", "TOTAL", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NoteWidget: public Ui_NoteWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTEWIDGET_H
