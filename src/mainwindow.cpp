#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->precision_spin_box->setRange(-10, -5);
    ui->precision_spin_box->setValue(-7);
    ui->precision_spin_box->setSingleStep(0.1);

    // ui->function_selection_combo->addItem(
    //     QString::fromUtf8("")
}

MainWindow::~MainWindow()
{
    delete ui;
}