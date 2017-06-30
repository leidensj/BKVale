/********************************************************************************
** Form generated from reading UI file 'settingsdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDLG_H
#define UI_SETTINGSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDlg
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *general;
    QVBoxLayout *verticalLayout_3;
    QFormLayout *formLayout_2;
    QWidget *serial;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QComboBox *cbFlow;
    QLabel *label_4;
    QComboBox *cbBaudrate;
    QLabel *label_3;
    QComboBox *cbStopBits;
    QComboBox *cbDataBits;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_2;
    QComboBox *cbParity;
    QComboBox *cbPort;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingsDlg)
    {
        if (SettingsDlg->objectName().isEmpty())
            SettingsDlg->setObjectName(QStringLiteral("SettingsDlg"));
        SettingsDlg->resize(407, 247);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/res/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        SettingsDlg->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SettingsDlg);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(SettingsDlg);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        general = new QWidget();
        general->setObjectName(QStringLiteral("general"));
        verticalLayout_3 = new QVBoxLayout(general);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));

        verticalLayout_3->addLayout(formLayout_2);

        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/res/general.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(general, icon1, QString());
        serial = new QWidget();
        serial->setObjectName(QStringLiteral("serial"));
        verticalLayout_2 = new QVBoxLayout(serial);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        cbFlow = new QComboBox(serial);
        cbFlow->setObjectName(QStringLiteral("cbFlow"));

        formLayout->setWidget(2, QFormLayout::FieldRole, cbFlow);

        label_4 = new QLabel(serial);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        cbBaudrate = new QComboBox(serial);
        cbBaudrate->setObjectName(QStringLiteral("cbBaudrate"));

        formLayout->setWidget(1, QFormLayout::FieldRole, cbBaudrate);

        label_3 = new QLabel(serial);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        cbStopBits = new QComboBox(serial);
        cbStopBits->setObjectName(QStringLiteral("cbStopBits"));

        formLayout->setWidget(4, QFormLayout::FieldRole, cbStopBits);

        cbDataBits = new QComboBox(serial);
        cbDataBits->setObjectName(QStringLiteral("cbDataBits"));

        formLayout->setWidget(3, QFormLayout::FieldRole, cbDataBits);

        label_5 = new QLabel(serial);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(serial);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        label_2 = new QLabel(serial);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        cbParity = new QComboBox(serial);
        cbParity->setObjectName(QStringLiteral("cbParity"));

        formLayout->setWidget(5, QFormLayout::FieldRole, cbParity);

        cbPort = new QComboBox(serial);
        cbPort->setObjectName(QStringLiteral("cbPort"));

        formLayout->setWidget(0, QFormLayout::FieldRole, cbPort);

        label = new QLabel(serial);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);


        verticalLayout_2->addLayout(formLayout);

        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/res/serial.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(serial, icon2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(SettingsDlg);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SettingsDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsDlg, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsDlg);
    } // setupUi

    void retranslateUi(QDialog *SettingsDlg)
    {
        SettingsDlg->setWindowTitle(QApplication::translate("SettingsDlg", "Configura\303\247\303\265es", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(general), QApplication::translate("SettingsDlg", "Geral", Q_NULLPTR));
        label_4->setText(QApplication::translate("SettingsDlg", "Bits de dados", Q_NULLPTR));
        label_3->setText(QApplication::translate("SettingsDlg", "Controle de fluxo", Q_NULLPTR));
        label_5->setText(QApplication::translate("SettingsDlg", "Bits de parada", Q_NULLPTR));
        label_6->setText(QApplication::translate("SettingsDlg", "Paridade", Q_NULLPTR));
        label_2->setText(QApplication::translate("SettingsDlg", "Taxa de transmiss\303\243o", Q_NULLPTR));
        label->setText(QApplication::translate("SettingsDlg", "Porta", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(serial), QApplication::translate("SettingsDlg", "Serial", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingsDlg: public Ui_SettingsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDLG_H
