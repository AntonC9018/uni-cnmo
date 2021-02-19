#include <strlib.h>
#include "profiler.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "equations.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->precision_spin_box->setRange(-10, -5);
    ui->precision_spin_box->setValue(-7);
    ui->precision_spin_box->setSingleStep(0.1);

    {
        using namespace Builtin;

        compile();

        for (int i = 0; i < expression_count; i++)
        {
            ui->function_selection_combo->addItem(
                QString::fromUtf8(expression_strings[i]));
        }

        connect(ui->function_selection_combo, 
                SIGNAL(currentIndexChanged(int)), 
                this, 
                SLOT(index_changed_function_combo(int))
        );
    }

    connect(ui->function_custom_edit, 
            SIGNAL(editingFinished()), 
            this, 
            SLOT(editing_finished_custom_function())
    );

    connect(ui->function_custom_rbutton, 
            SIGNAL(toggled(bool)),
            this,
            SLOT(toggled_custom_selection(bool))
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_selected_function(calculator func)
{
    puts(func.str().c_str());
    selected_function = func;
    selected_function_changed(func);
}

void MainWindow::index_changed_function_combo(int index)
{
    if (ui->function_predefined_rbutton->isChecked())
        change_selected_function_to_builtin(index);
}

void MainWindow::change_selected_function_to_builtin(int index)
{
    if (index < Builtin::expression_count)
        set_selected_function(Builtin::calculators[index]);
}

void MainWindow::editing_finished_custom_function()
{
    if (ui->function_custom_rbutton->isChecked())
        change_selected_function_to_custom();
}

void MainWindow::change_selected_function_to_custom()
{
    auto qstr_utf8 = ui->function_custom_edit->text().toUtf8();

    if (qstr_utf8 != "")
    {
        // Since we need this array to be null terminated, and there is no
        // guarantee for that from qstr_utf8, we have to copy the string with malloc.
        str_t str = str_copy(qstr_utf8.data(), qstr_utf8.length());

        calculator calc(str.chars);
        set_selected_function(calc);

        str_free(str);
    }
}

void MainWindow::toggled_custom_selection(bool checked)
{
    if (checked)
        change_selected_function_to_custom();
    else
        change_selected_function_to_builtin(ui->function_selection_combo->currentIndex());
}