#include "profiler.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "equations.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // These do not have a default constructor, which is why they have trash by default
    custom_function_str           = STR_NULL;
    selected_custom_function.text = STR_NULL;

    ui->setupUi(this);
    ui->precision_spin_box->setRange(-10, -5);
    ui->precision_spin_box->setValue(-7);
    ui->precision_spin_box->setSingleStep(0.1);

    // Setup builtin functions
    {
        using namespace Builtin;

        for (int i = 0; i < func_count; i++)
        {
            ui->function_selection_combo->addItem(
                QString::fromUtf8(funcs[i].text.chars));
        }
    }

    // Selecting the builtin function 
    connect(ui->function_selection_combo, 
            SIGNAL(currentIndexChanged(int)), 
            this, 
            SLOT(change_selected_builtin_function(int))
    );

    // Selecting a custom function
    connect(ui->function_custom_edit, 
            SIGNAL(editingFinished()), 
            this, 
            SLOT(change_selected_custom_function())
    );

    // Upper and lower bounds
    // ui->function_upper_spinbox->
}

MainWindow::~MainWindow()
{
    delete ui;
}

const Func* MainWindow::get_selected_function()
{
    if (ui->function_custom_rbutton->isChecked())
        return &selected_custom_function;
    else
        return selected_builtin_function; 
}

void MainWindow::change_selected_builtin_function(int index)
{
    if (index < Builtin::func_count)
    {
        selected_builtin_function = &Builtin::funcs[index];

        puts(selected_builtin_function->text.chars);

        if (ui->function_predefined_rbutton->isChecked())
            selected_function_changed();
    }
}

void MainWindow::change_selected_custom_function()
{
    auto qstr_utf8 = ui->function_custom_edit->text().toUtf8();

    if (qstr_utf8 != "")
    {
        // Since we need this array to be null terminated, and there is no
        // guarantee for that from qstr_utf8, we have to copy the string with malloc.
        str_smart_replace(&custom_function_str, qstr_utf8.data(), qstr_utf8.length());

        selected_custom_function.text = str_view(custom_function_str);
        selected_custom_function.calculator.compile(custom_function_str.chars);

        puts(custom_function_str.chars);

        if (ui->function_custom_rbutton->isChecked())
            selected_function_changed();
    }
}