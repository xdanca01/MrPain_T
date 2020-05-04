/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionPlay;
    QAction *actionPreferences;
    QAction *actionQuit;
    QAction *actionStop;
    QAction *actionAddStreet;
    QAction *actionAddBus;
    QAction *actionAddStop;
    QAction *actionAbout;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QPushButton *btn_minus;
    QPushButton *btn_plus;
    QLabel *label;
    QSlider *slide_zoom;
    QDoubleSpinBox *btn_speed;
    QLabel *label_2;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_left;
    QPushButton *btn_right;
    QTimeEdit *my_timer;
    QLabel *label_4;
    QGraphicsView *graphicsView;
    QVBoxLayout *verticalLayout_5;
    QListWidget *street_info;
    QTableWidget *traf_info;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOpen_recent;
    QMenu *menuHelp;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(922, 689);
        MainWindow->setMinimumSize(QSize(800, 500));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionPlay = new QAction(MainWindow);
        actionPlay->setObjectName(QStringLiteral("actionPlay"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionAddStreet = new QAction(MainWindow);
        actionAddStreet->setObjectName(QStringLiteral("actionAddStreet"));
        actionAddBus = new QAction(MainWindow);
        actionAddBus->setObjectName(QStringLiteral("actionAddBus"));
        actionAddStop = new QAction(MainWindow);
        actionAddStop->setObjectName(QStringLiteral("actionAddStop"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btn_minus = new QPushButton(centralWidget);
        btn_minus->setObjectName(QStringLiteral("btn_minus"));
        btn_minus->setMinimumSize(QSize(30, 30));
        btn_minus->setMaximumSize(QSize(30, 30));

        gridLayout->addWidget(btn_minus, 13, 0, 1, 1);

        btn_plus = new QPushButton(centralWidget);
        btn_plus->setObjectName(QStringLiteral("btn_plus"));
        btn_plus->setMinimumSize(QSize(30, 30));
        btn_plus->setMaximumSize(QSize(30, 30));

        gridLayout->addWidget(btn_plus, 9, 0, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 6, 0, 1, 1);

        slide_zoom = new QSlider(centralWidget);
        slide_zoom->setObjectName(QStringLiteral("slide_zoom"));
        slide_zoom->setMinimum(10);
        slide_zoom->setMaximum(400);
        slide_zoom->setSingleStep(10);
        slide_zoom->setValue(100);
        slide_zoom->setOrientation(Qt::Vertical);

        gridLayout->addWidget(slide_zoom, 12, 0, 1, 1);

        btn_speed = new QDoubleSpinBox(centralWidget);
        btn_speed->setObjectName(QStringLiteral("btn_speed"));
        btn_speed->setMinimumSize(QSize(80, 30));
        btn_speed->setMaximumSize(QSize(80, 30));
        btn_speed->setDecimals(3);
        btn_speed->setMinimum(0.001);
        btn_speed->setMaximum(100);
        btn_speed->setSingleStep(0.5);
        btn_speed->setValue(1);

        gridLayout->addWidget(btn_speed, 7, 0, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 8, 0, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btn_left = new QPushButton(centralWidget);
        btn_left->setObjectName(QStringLiteral("btn_left"));
        btn_left->setMaximumSize(QSize(40, 40));

        horizontalLayout_2->addWidget(btn_left);

        btn_right = new QPushButton(centralWidget);
        btn_right->setObjectName(QStringLiteral("btn_right"));
        btn_right->setMaximumSize(QSize(40, 40));

        horizontalLayout_2->addWidget(btn_right);


        gridLayout->addLayout(horizontalLayout_2, 5, 0, 1, 1);

        my_timer = new QTimeEdit(centralWidget);
        my_timer->setObjectName(QStringLiteral("my_timer"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(my_timer->sizePolicy().hasHeightForWidth());
        my_timer->setSizePolicy(sizePolicy);
        my_timer->setTime(QTime(12, 0, 0));

        gridLayout->addWidget(my_timer, 2, 0, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        horizontalLayout_4->addLayout(horizontalLayout);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(graphicsView);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        street_info = new QListWidget(centralWidget);
        street_info->setObjectName(QStringLiteral("street_info"));
        sizePolicy.setHeightForWidth(street_info->sizePolicy().hasHeightForWidth());
        street_info->setSizePolicy(sizePolicy);
        street_info->setMinimumSize(QSize(202, 100));
        street_info->setMaximumSize(QSize(220, 100));

        verticalLayout_5->addWidget(street_info);

        traf_info = new QTableWidget(centralWidget);
        if (traf_info->columnCount() < 2)
            traf_info->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        traf_info->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        traf_info->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        traf_info->setObjectName(QStringLiteral("traf_info"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(traf_info->sizePolicy().hasHeightForWidth());
        traf_info->setSizePolicy(sizePolicy2);
        traf_info->setMinimumSize(QSize(202, 0));
        traf_info->setMaximumSize(QSize(220, 9000000));
        traf_info->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        traf_info->horizontalHeader()->setMinimumSectionSize(30);

        verticalLayout_5->addWidget(traf_info);


        horizontalLayout_4->addLayout(verticalLayout_5);


        verticalLayout_3->addLayout(horizontalLayout_4);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 922, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuOpen_recent = new QMenu(menuFile);
        menuOpen_recent->setObjectName(QStringLiteral("menuOpen_recent"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMinimumSize(QSize(0, 40));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(menuOpen_recent->menuAction());
        menuFile->addAction(actionPreferences);
        menuFile->addAction(actionQuit);
        menuOpen_recent->addSeparator();
        menuHelp->addAction(actionAbout);
        toolBar->addAction(actionPlay);
        toolBar->addAction(actionStop);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open...", 0));
        actionPlay->setText(QApplication::translate("MainWindow", "Play", 0));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        actionAddStreet->setText(QApplication::translate("MainWindow", "AddStreet", 0));
        actionAddBus->setText(QApplication::translate("MainWindow", "AddBus", 0));
        actionAddStop->setText(QApplication::translate("MainWindow", "AddStop", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        btn_minus->setText(QApplication::translate("MainWindow", "-", 0));
        btn_plus->setText(QApplication::translate("MainWindow", "+", 0));
        label->setText(QApplication::translate("MainWindow", "Speed:", 0));
        label_2->setText(QApplication::translate("MainWindow", "Zoom:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Rotate:", 0));
        btn_left->setText(QApplication::translate("MainWindow", "Left", 0));
        btn_right->setText(QApplication::translate("MainWindow", "Right", 0));
        my_timer->setDisplayFormat(QApplication::translate("MainWindow", "HH:mm:ss", 0));
        label_4->setText(QApplication::translate("MainWindow", "Time:", 0));
        QTableWidgetItem *___qtablewidgetitem = traf_info->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Stop", 0));
        QTableWidgetItem *___qtablewidgetitem1 = traf_info->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Arrival", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuOpen_recent->setTitle(QApplication::translate("MainWindow", "Open recent", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
