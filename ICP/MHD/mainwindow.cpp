#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_plus, SIGNAL(clicked()), this, SLOT(plus()));
    connect(ui->btn_minus, SIGNAL(clicked()), this, SLOT(minus()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plus()
{
    ui->slide_zoom->setValue(ui->slide_zoom->value() + 1);
}

void MainWindow::minus()
{
    ui->slide_zoom->setValue(ui->slide_zoom->value() - 1);
}

