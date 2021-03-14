#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);

    ui->function_selection->setup();
    ui->root_finding->setup(ui->function_selection); 
    ui->poly->setup(ui->function_selection);

    ui->poly->reselect(); 

    connect(
        ui->tabWidget, SIGNAL(currentChanged(int)),
        this, SLOT(current_tab_changed(int))
    );
}

MainWindow::~MainWindow() 
{ 
    delete ui;
}

QWidget* MainWindow::get_current_widget()
{
    return ui->tabWidget->widget(current_tab_index);
}

void MainWindow::current_tab_changed(int new_index)
{
    printf("Index changed: %i\n", new_index);
    if (new_index != current_tab_index)
    {
        // could do a common class + virtual method for them, but let's do it the simple way
        switch(new_index)
        {
        case 0:
            ui->root_finding->reselect();
            break;
        case 1:
            ui->poly->reselect();
            break;
        }

        auto current_widget = get_current_widget();
        if (current_widget)
        {
            current_widget->blockSignals(true);
        }

        current_tab_index = new_index;

        auto new_current_widget = get_current_widget();
        if (new_current_widget)
        {
            new_current_widget->blockSignals(false);
        }
    }
}