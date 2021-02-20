#include "profiler.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "builtin.h"

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

    ui->function_builtin_rbutton->setChecked(true);

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
    ui->function_upper_spinbox->setRange(-1024, 1024);
    connect(ui->function_upper_spinbox,
            SIGNAL(valueChanged(double)),
            this,
            SLOT(change_upper_bound(double))
    );
    
    ui->function_lower_spinbox->setRange(-1024, 1024);
    connect(ui->function_lower_spinbox,
            SIGNAL(valueChanged(double)),
            this,
            SLOT(change_lower_bound(double))
    );

    // Algorithms
    ui->algorithm_combo->addItem("Bisection");
    ui->algorithm_combo->addItem("Newton");
    ui->algorithm_combo->addItem("Secant");
    ui->algorithm_combo->addItem("Chord");

    // Draw some function
    ui->plot->updateCurve(&Builtin::funcs[0]);

    connect(this,
            SIGNAL(selected_function_changed()),
            this,
            SLOT(changed_function_redraw_graph())
    );

    connect(this,
            SIGNAL(upper_bound_changed()),
            this,
            SLOT(changed_function_redraw_graph())
    );
    
    connect(this,
            SIGNAL(lower_bound_changed()),
            this,
            SLOT(changed_function_redraw_graph())
    );

    connect(ui->function_custom_rbutton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(changed_function_redraw_graph())
    );
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

        if (ui->function_builtin_rbutton->isChecked())
        {
            emit selected_function_changed();
        }
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
            emit selected_function_changed();
    }
}

void MainWindow::change_upper_bound(double value)
{
    double new_value = std::max(value, selected_custom_function.lower_bound);
    if (selected_custom_function.upper_bound != new_value)
    {
        selected_custom_function.upper_bound = new_value;
        emit upper_bound_changed();
    }
    // change the text
    if (new_value != value)
    {
        ui->function_upper_spinbox->setValue(new_value);
    }
}

void MainWindow::change_lower_bound(double value)
{
    double new_value = std::min(value, selected_custom_function.upper_bound);
    if (selected_custom_function.lower_bound != new_value)
    {
        selected_custom_function.lower_bound = new_value;
        emit lower_bound_changed();
    }
    // change the text
    if (new_value != value)
    {
        ui->function_lower_spinbox->setValue(new_value);
    }
}


void MainWindow::changed_function_redraw_graph()
{
    ui->plot->updateCurve(get_selected_function());
}