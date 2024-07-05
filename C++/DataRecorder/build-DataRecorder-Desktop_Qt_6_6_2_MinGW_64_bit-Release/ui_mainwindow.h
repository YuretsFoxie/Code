/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *controlPanel;
    QVBoxLayout *verticalLayout_2;
    QPushButton *connectButton;
    QPushButton *testButton;
    QPushButton *clearButton;
    QPushButton *quitButton;
    QSpacerItem *verticalSpacer;
    QLabel *connectLabel;
    QVBoxLayout *verticalLayout;
    QFrame *graphs;
    QVBoxLayout *verticalLayout_3;
    QCustomPlot *scope;
    QCustomPlot *spectrum;
    QTextBrowser *textBrowser;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(378, 338);
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("accessories-calculator")));
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"	border-image: url(:/Resources/images/Stars-galaxy.png);\n"
"}\n"
"\n"
"QFrame {\n"
"	background-color: rgba(0, 0, 0, 0.7);\n"
"	border: 0px solid green;\n"
"}\n"
"\n"
"QTextBrowser {\n"
"	color: rgba(0, 255, 0, 1.0);\n"
"	background-color: rgba(0, 0, 0, 0.7);\n"
"	font-size: 18px;\n"
"	border: 0px solid rgba(0, 255, 0, 1.0);\n"
"	selection-color: rgba(0, 0, 0, 1.0);\n"
"	selection-background-color: rgba(0, 255, 0, 1.0);\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"	background:  clear;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"	background: rgba(0, 255, 0, 1.0);\n"
"	min-height: 50px;\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { 		\n"
"	background: clear;\n"
"}\n"
"\n"
"QPushButton {\n"
"	color: rgba(0, 255, 0, 1.0);\n"
"	background-color: rgba(0, 0, 0, 0.0);\n"
"	font-size: 20px;\n"
"	border: 0px solid green;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	color: black;\n"
"	background-color: rgba(0, 255, 0, 0.5);\n"
"	font-size: 20px;\n"
"	border: 0px solid green;"
                        "\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	color: black;\n"
"	background-color: rgba(0, 255, 0, 0.7);\n"
"	font-size: 20px;\n"
"	border: 0px solid green;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"	color: rgba(100, 100, 100, 1.0);\n"
"	background-color: rgba(0, 0, 0, 0.0);\n"
"	font-size: 20px;\n"
"	border: 0px solid green;\n"
"}\n"
"\n"
"QLabel {\n"
"	background-color: transparent;\n"
"	color: rgba(0, 255, 0, 1.0);\n"
"	padding: 10px;\n"
"	font-size: 20px;\n"
"}\n"
""));
        MainWindow->setDockNestingEnabled(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setAutoFillBackground(false);
        centralwidget->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        controlPanel = new QFrame(centralwidget);
        controlPanel->setObjectName("controlPanel");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(controlPanel->sizePolicy().hasHeightForWidth());
        controlPanel->setSizePolicy(sizePolicy);
        controlPanel->setMaximumSize(QSize(120, 16777215));
        controlPanel->setFrameShape(QFrame::StyledPanel);
        controlPanel->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(controlPanel);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        connectButton = new QPushButton(controlPanel);
        connectButton->setObjectName("connectButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(connectButton->sizePolicy().hasHeightForWidth());
        connectButton->setSizePolicy(sizePolicy1);
        connectButton->setMinimumSize(QSize(120, 50));
        connectButton->setMaximumSize(QSize(120, 50));

        verticalLayout_2->addWidget(connectButton);

        testButton = new QPushButton(controlPanel);
        testButton->setObjectName("testButton");
        sizePolicy1.setHeightForWidth(testButton->sizePolicy().hasHeightForWidth());
        testButton->setSizePolicy(sizePolicy1);
        testButton->setMinimumSize(QSize(120, 50));
        testButton->setMaximumSize(QSize(120, 50));
        testButton->setLayoutDirection(Qt::LeftToRight);

        verticalLayout_2->addWidget(testButton);

        clearButton = new QPushButton(controlPanel);
        clearButton->setObjectName("clearButton");
        sizePolicy1.setHeightForWidth(clearButton->sizePolicy().hasHeightForWidth());
        clearButton->setSizePolicy(sizePolicy1);
        clearButton->setMinimumSize(QSize(120, 50));
        clearButton->setMaximumSize(QSize(120, 50));

        verticalLayout_2->addWidget(clearButton);

        quitButton = new QPushButton(controlPanel);
        quitButton->setObjectName("quitButton");
        sizePolicy1.setHeightForWidth(quitButton->sizePolicy().hasHeightForWidth());
        quitButton->setSizePolicy(sizePolicy1);
        quitButton->setMinimumSize(QSize(120, 50));
        quitButton->setMaximumSize(QSize(120, 50));

        verticalLayout_2->addWidget(quitButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        connectLabel = new QLabel(controlPanel);
        connectLabel->setObjectName("connectLabel");
        sizePolicy1.setHeightForWidth(connectLabel->sizePolicy().hasHeightForWidth());
        connectLabel->setSizePolicy(sizePolicy1);
        connectLabel->setMinimumSize(QSize(120, 50));
        connectLabel->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(connectLabel);


        horizontalLayout->addWidget(controlPanel);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        graphs = new QFrame(centralwidget);
        graphs->setObjectName("graphs");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(graphs->sizePolicy().hasHeightForWidth());
        graphs->setSizePolicy(sizePolicy2);
        graphs->setFrameShape(QFrame::StyledPanel);
        graphs->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(graphs);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        scope = new QCustomPlot(graphs);
        scope->setObjectName("scope");

        verticalLayout_3->addWidget(scope);

        spectrum = new QCustomPlot(graphs);
        spectrum->setObjectName("spectrum");

        verticalLayout_3->addWidget(spectrum);


        verticalLayout->addWidget(graphs);

        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy3);
        textBrowser->setMaximumSize(QSize(16777215, 200));

        verticalLayout->addWidget(textBrowser);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Data Recorder", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        testButton->setText(QCoreApplication::translate("MainWindow", "Generator", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        quitButton->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        connectLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
