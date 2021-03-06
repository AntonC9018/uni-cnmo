#pragma once
#include <QMainWindow>

#include "./ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int current_tab_index;
    QWidget* get_current_widget();

public slots:
    void current_tab_changed(int new_tab_index);
};