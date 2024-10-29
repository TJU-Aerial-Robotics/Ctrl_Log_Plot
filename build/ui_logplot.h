/********************************************************************************
** Form generated from reading UI file 'logplot.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGPLOT_H
#define UI_LOGPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_LogPlot
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *est_px;
    QCheckBox *est_py;
    QCheckBox *est_pz;
    QCheckBox *est_vx;
    QCheckBox *est_vy;
    QCheckBox *est_vz;
    QCheckBox *est_ax;
    QCheckBox *est_ay;
    QCheckBox *est_az;
    QCheckBox *est_yaw;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *des_px;
    QCheckBox *des_py;
    QCheckBox *des_pz;
    QCheckBox *des_vx;
    QCheckBox *des_vy;
    QCheckBox *des_vz;
    QCheckBox *des_ax;
    QCheckBox *des_ay;
    QCheckBox *des_az;
    QCheckBox *des_yaw;
    QVBoxLayout *verticalLayout;
    QCheckBox *err_px;
    QCheckBox *err_py;
    QCheckBox *err_pz;
    QCheckBox *err_vx;
    QCheckBox *err_vy;
    QCheckBox *err_vz;
    QCheckBox *err_ax;
    QCheckBox *err_ay;
    QCheckBox *err_az;
    QCheckBox *err_yaw;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *openButton;
    QLineEdit *fileName;
    QCustomPlot *customPlot;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *plotButton;
    QPushButton *clearButton;
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QMainWindow *LogPlot)
    {
        if (LogPlot->objectName().isEmpty())
            LogPlot->setObjectName(QStringLiteral("LogPlot"));
        LogPlot->resize(1336, 547);
        centralwidget = new QWidget(LogPlot);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalSpacer_4 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 4, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 6, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 1, 1, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        est_px = new QCheckBox(centralwidget);
        est_px->setObjectName(QStringLiteral("est_px"));

        verticalLayout_4->addWidget(est_px);

        est_py = new QCheckBox(centralwidget);
        est_py->setObjectName(QStringLiteral("est_py"));

        verticalLayout_4->addWidget(est_py);

        est_pz = new QCheckBox(centralwidget);
        est_pz->setObjectName(QStringLiteral("est_pz"));

        verticalLayout_4->addWidget(est_pz);

        est_vx = new QCheckBox(centralwidget);
        est_vx->setObjectName(QStringLiteral("est_vx"));

        verticalLayout_4->addWidget(est_vx);

        est_vy = new QCheckBox(centralwidget);
        est_vy->setObjectName(QStringLiteral("est_vy"));

        verticalLayout_4->addWidget(est_vy);

        est_vz = new QCheckBox(centralwidget);
        est_vz->setObjectName(QStringLiteral("est_vz"));

        verticalLayout_4->addWidget(est_vz);

        est_ax = new QCheckBox(centralwidget);
        est_ax->setObjectName(QStringLiteral("est_ax"));

        verticalLayout_4->addWidget(est_ax);

        est_ay = new QCheckBox(centralwidget);
        est_ay->setObjectName(QStringLiteral("est_ay"));

        verticalLayout_4->addWidget(est_ay);

        est_az = new QCheckBox(centralwidget);
        est_az->setObjectName(QStringLiteral("est_az"));

        verticalLayout_4->addWidget(est_az);

        est_yaw = new QCheckBox(centralwidget);
        est_yaw->setObjectName(QStringLiteral("est_yaw"));
        est_yaw->setCursor(QCursor(Qt::ArrowCursor));

        verticalLayout_4->addWidget(est_yaw);


        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        des_px = new QCheckBox(centralwidget);
        des_px->setObjectName(QStringLiteral("des_px"));

        verticalLayout_3->addWidget(des_px);

        des_py = new QCheckBox(centralwidget);
        des_py->setObjectName(QStringLiteral("des_py"));

        verticalLayout_3->addWidget(des_py);

        des_pz = new QCheckBox(centralwidget);
        des_pz->setObjectName(QStringLiteral("des_pz"));

        verticalLayout_3->addWidget(des_pz);

        des_vx = new QCheckBox(centralwidget);
        des_vx->setObjectName(QStringLiteral("des_vx"));

        verticalLayout_3->addWidget(des_vx);

        des_vy = new QCheckBox(centralwidget);
        des_vy->setObjectName(QStringLiteral("des_vy"));

        verticalLayout_3->addWidget(des_vy);

        des_vz = new QCheckBox(centralwidget);
        des_vz->setObjectName(QStringLiteral("des_vz"));

        verticalLayout_3->addWidget(des_vz);

        des_ax = new QCheckBox(centralwidget);
        des_ax->setObjectName(QStringLiteral("des_ax"));

        verticalLayout_3->addWidget(des_ax);

        des_ay = new QCheckBox(centralwidget);
        des_ay->setObjectName(QStringLiteral("des_ay"));

        verticalLayout_3->addWidget(des_ay);

        des_az = new QCheckBox(centralwidget);
        des_az->setObjectName(QStringLiteral("des_az"));

        verticalLayout_3->addWidget(des_az);

        des_yaw = new QCheckBox(centralwidget);
        des_yaw->setObjectName(QStringLiteral("des_yaw"));
        des_yaw->setCheckable(true);

        verticalLayout_3->addWidget(des_yaw);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        err_px = new QCheckBox(centralwidget);
        err_px->setObjectName(QStringLiteral("err_px"));

        verticalLayout->addWidget(err_px);

        err_py = new QCheckBox(centralwidget);
        err_py->setObjectName(QStringLiteral("err_py"));

        verticalLayout->addWidget(err_py);

        err_pz = new QCheckBox(centralwidget);
        err_pz->setObjectName(QStringLiteral("err_pz"));

        verticalLayout->addWidget(err_pz);

        err_vx = new QCheckBox(centralwidget);
        err_vx->setObjectName(QStringLiteral("err_vx"));

        verticalLayout->addWidget(err_vx);

        err_vy = new QCheckBox(centralwidget);
        err_vy->setObjectName(QStringLiteral("err_vy"));

        verticalLayout->addWidget(err_vy);

        err_vz = new QCheckBox(centralwidget);
        err_vz->setObjectName(QStringLiteral("err_vz"));

        verticalLayout->addWidget(err_vz);

        err_ax = new QCheckBox(centralwidget);
        err_ax->setObjectName(QStringLiteral("err_ax"));

        verticalLayout->addWidget(err_ax);

        err_ay = new QCheckBox(centralwidget);
        err_ay->setObjectName(QStringLiteral("err_ay"));

        verticalLayout->addWidget(err_ay);

        err_az = new QCheckBox(centralwidget);
        err_az->setObjectName(QStringLiteral("err_az"));

        verticalLayout->addWidget(err_az);

        err_yaw = new QCheckBox(centralwidget);
        err_yaw->setObjectName(QStringLiteral("err_yaw"));
        err_yaw->setCheckable(true);

        verticalLayout->addWidget(err_yaw);


        horizontalLayout_3->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout_3, 3, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

        gridLayout->addLayout(horizontalLayout_2, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openButton = new QPushButton(centralwidget);
        openButton->setObjectName(QStringLiteral("openButton"));

        horizontalLayout->addWidget(openButton);

        fileName = new QLineEdit(centralwidget);
        fileName->setObjectName(QStringLiteral("fileName"));

        horizontalLayout->addWidget(fileName);


        gridLayout->addLayout(horizontalLayout, 7, 0, 1, 1);

        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));

        gridLayout->addWidget(customPlot, 0, 0, 7, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        plotButton = new QPushButton(centralwidget);
        plotButton->setObjectName(QStringLiteral("plotButton"));

        horizontalLayout_4->addWidget(plotButton);

        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName(QStringLiteral("clearButton"));

        horizontalLayout_4->addWidget(clearButton);


        gridLayout->addLayout(horizontalLayout_4, 5, 1, 1, 1);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 7, 1, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(25);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        font.setStrikeOut(false);
        font.setKerning(true);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        LogPlot->setCentralWidget(centralwidget);

        retranslateUi(LogPlot);
        QObject::connect(pushButton, SIGNAL(clicked()), LogPlot, SLOT(close()));

        QMetaObject::connectSlotsByName(LogPlot);
    } // setupUi

    void retranslateUi(QMainWindow *LogPlot)
    {
        LogPlot->setWindowTitle(QApplication::translate("LogPlot", "LogPlot", Q_NULLPTR));
        est_px->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_py->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_pz->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_vx->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_vy->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_vz->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_ax->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_ay->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_az->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        est_yaw->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_px->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_py->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_pz->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_vx->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_vy->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_vz->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_ax->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_ay->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_az->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        des_yaw->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_px->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_py->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_pz->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_vx->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_vy->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_vz->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_ax->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_ay->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_az->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        err_yaw->setText(QApplication::translate("LogPlot", "unused", Q_NULLPTR));
        openButton->setText(QApplication::translate("LogPlot", "Open", Q_NULLPTR));
        plotButton->setText(QApplication::translate("LogPlot", "Resize", Q_NULLPTR));
        clearButton->setText(QApplication::translate("LogPlot", "Clear", Q_NULLPTR));
        pushButton->setText(QApplication::translate("LogPlot", "Close", Q_NULLPTR));
        label->setText(QApplication::translate("LogPlot", "Control Log Plotter", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LogPlot: public Ui_LogPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGPLOT_H
