/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <mygraphicsview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    MyGraphicsView *graphicsView;
    QPushButton *pushButton;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QSlider *ViscositySlider;
    QPlainTextEdit *iterationTime;
    QSlider *DiffusionSlider;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPlainTextEdit *frameTime;
    QComboBox *comboBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLabel *label_6;
    QLabel *label_5;
    QPlainTextEdit *density;
    QPlainTextEdit *horizontalSpeed;
    QPlainTextEdit *verticalSpeed;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(987, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        graphicsView = new MyGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(20, 120, 200, 200));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(810, 20, 80, 25));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 10, 741, 64));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout->setHorizontalSpacing(3);
        gridLayout->setVerticalSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        ViscositySlider = new QSlider(layoutWidget);
        ViscositySlider->setObjectName(QString::fromUtf8("ViscositySlider"));
        ViscositySlider->setMinimum(1);
        ViscositySlider->setMaximum(999);
        ViscositySlider->setSingleStep(25);
        ViscositySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(ViscositySlider, 1, 0, 1, 1);

        iterationTime = new QPlainTextEdit(layoutWidget);
        iterationTime->setObjectName(QString::fromUtf8("iterationTime"));
        iterationTime->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(iterationTime->sizePolicy().hasHeightForWidth());
        iterationTime->setSizePolicy(sizePolicy1);
        iterationTime->setMaximumSize(QSize(16777215, 40));
        QFont font;
        font.setPointSize(9);
        iterationTime->setFont(font);
#if QT_CONFIG(accessibility)
        iterationTime->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
#if QT_CONFIG(accessibility)
        iterationTime->setAccessibleDescription(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        iterationTime->setInputMethodHints(Qt::ImhDigitsOnly);
        iterationTime->setLineWrapMode(QPlainTextEdit::NoWrap);
        iterationTime->setPlainText(QString::fromUtf8(""));
        iterationTime->setTabStopDistance(50.000000000000000);
        iterationTime->setPlaceholderText(QString::fromUtf8("ms"));

        gridLayout->addWidget(iterationTime, 1, 2, 1, 1);

        DiffusionSlider = new QSlider(layoutWidget);
        DiffusionSlider->setObjectName(QString::fromUtf8("DiffusionSlider"));
        DiffusionSlider->setMinimum(1);
        DiffusionSlider->setMaximum(1000);
        DiffusionSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(DiffusionSlider, 1, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        frameTime = new QPlainTextEdit(layoutWidget);
        frameTime->setObjectName(QString::fromUtf8("frameTime"));
        frameTime->setEnabled(true);
        sizePolicy1.setHeightForWidth(frameTime->sizePolicy().hasHeightForWidth());
        frameTime->setSizePolicy(sizePolicy1);
        frameTime->setMaximumSize(QSize(16777215, 40));
        frameTime->setFont(font);
        frameTime->setInputMethodHints(Qt::ImhDigitsOnly);
        frameTime->setPlainText(QString::fromUtf8(""));
        frameTime->setPlaceholderText(QString::fromUtf8("ms"));

        gridLayout->addWidget(frameTime, 1, 3, 1, 1);

        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(20, 80, 261, 22));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(350, 130, 312, 114));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 1, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

        density = new QPlainTextEdit(gridLayoutWidget);
        density->setObjectName(QString::fromUtf8("density"));

        gridLayout_2->addWidget(density, 1, 0, 1, 1);

        horizontalSpeed = new QPlainTextEdit(gridLayoutWidget);
        horizontalSpeed->setObjectName(QString::fromUtf8("horizontalSpeed"));

        gridLayout_2->addWidget(horizontalSpeed, 1, 1, 1, 1);

        verticalSpeed = new QPlainTextEdit(gridLayoutWidget);
        verticalSpeed->setObjectName(QString::fromUtf8("verticalSpeed"));

        gridLayout_2->addWidget(verticalSpeed, 1, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 987, 25));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, &QPushButton::clicked, graphicsView, qOverload<>(&MyGraphicsView::start));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Diffusion", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Viscosity", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Iteration time", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Frame time", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "orginal", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "edited", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "openMP", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "cuda", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("MainWindow", "MA", nullptr));

        label_7->setText(QCoreApplication::translate("MainWindow", "Vertical Speed", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Horizontal Speed", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Density", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
