#include "profiler.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <func/builtin.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // These do not have a default constructor, which is why they have trash by default
    custom_function_str = STR_NULL;
    func_clear(&selected_custom_function);

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
    using namespace Root_Finding;

    for (size_t i = 0; i < countof(option_text); i++)
    {
        ui->algorithm_combo->addItem(option_text[i].chars);
    }

    connect(this, SIGNAL(selected_function_changed()),
            this, SLOT(changed_function_redraw_graph()));

    connect(this, SIGNAL(selected_function_changed()),
            this, SLOT(reestimate_zeros()));

    connect(this, SIGNAL(upper_bound_changed()),
            this, SLOT(changed_function_redraw_graph()));

    connect(this, SIGNAL(upper_bound_changed()), 
            this, SLOT(reestimate_zeros()));
    
    connect(this, SIGNAL(lower_bound_changed()),
            this, SLOT(changed_function_redraw_graph()));

    connect(this, SIGNAL(lower_bound_changed()), 
            this, SLOT(reestimate_zeros()));

    connect(ui->function_custom_rbutton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(changed_function_redraw_graph())
    );

    connect(ui->precision_spin_box,
            SIGNAL(valueChanged(double)),
            this,
            SLOT(reestimate_zeros())
    );
    
    connect(ui->algorithm_combo,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(reestimate_zeros())
    );

    ui->function_selection_combo->setCurrentIndex(1);

    ui->zeros_table->setModel(&zeros_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Expression_Func* MainWindow::get_selected_function()
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

        if (ui->function_builtin_rbutton->isChecked())
            emit selected_function_changed();
    }
}

void MainWindow::change_selected_custom_function()
{
    auto qstr_utf8 = ui->function_custom_edit->text().toUtf8();

    int error = 0;

    func_free(&selected_custom_function);

    if (qstr_utf8 != "")
    {
        // Since we need this array to be null terminated, and there is no
        // guarantee for that from qstr_utf8, we have to copy the string with malloc.
        str_smart_replace(&custom_function_str, qstr_utf8.data(), qstr_utf8.length());

        selected_custom_function.text = str_view(custom_function_str);
        func_compile(&selected_custom_function, &error);

        if (!error && ui->function_custom_rbutton->isChecked())
            emit selected_function_changed();
    }

    if (error)
    {
        puts("The function is invalid");

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
    auto selected_function = get_selected_function();

    if (selected_function->expr)
        ui->plot->update_curve(selected_function);
}

void MainWindow::reestimate_zeros()
{
    using namespace Root_Finding;

    auto selected_function = get_selected_function();

    if (selected_function->expr)
    {
        Error_Data error_data;
        error_data.error_message = STR_NULL;
        error_data.max_iters = 1000;
        error_data.tolerance = pow(10, ui->precision_spin_box->value());

        auto new_zeros = ui->plot->zeros(
            selected_function, &error_data, ui->algorithm_combo->currentIndex()
        );

        zeros_model.swap_data(new_zeros);
    }
}
