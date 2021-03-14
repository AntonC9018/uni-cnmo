#include "root_finding_main.h"
#include "../builtin.h"

namespace Root_Finding
{
    Root_Finding_Main::Root_Finding_Main(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Root_Finding_Main)
    {
    }

    void Root_Finding_Main::setup(Function_Selection* function_selection)
    {
        
        ui->setupUi(this);
        ui->precision_spin_box->setRange(-10, -5);
        ui->precision_spin_box->setValue(-7);
        ui->precision_spin_box->setSingleStep(0.1);
        
        // Algorithms
        for (size_t i = 0; i < countof(option_text); i++)
        {
            ui->algorithm_combo->addItem(option_text[i].chars);
        }

        // There must be a way to reuse this piece of code, besides inheritance
        this->function_selection = function_selection;
        connect(function_selection, SIGNAL(selected_function_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(function_selection, SIGNAL(upper_bound_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(function_selection, SIGNAL(lower_bound_changed()),
                this, SLOT(changed_function_redraw_graph()));


        connect(function_selection, SIGNAL(selected_function_changed()),
                this, SLOT(reestimate_zeros()));

        connect(function_selection, SIGNAL(upper_bound_changed()), 
                this, SLOT(reestimate_zeros()));

        connect(function_selection, SIGNAL(lower_bound_changed()), 
                this, SLOT(reestimate_zeros()));

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

        ui->zeros_table->setModel(&zeros_model);
    }

    void Root_Finding_Main::reselect()
    {
        function_selection->reset_builtin(funcs, func_count);
    }

    Root_Finding_Main::~Root_Finding_Main()
    {
        delete ui;
    }

    void Root_Finding_Main::changed_function_redraw_graph()
    {
        auto selected_function = function_selection->get_selected_function();

        if (selected_function->expr)
            ui->plot->update_curve(selected_function);
    }

    void Root_Finding_Main::reestimate_zeros()
    {
        using namespace Root_Finding;

        auto selected_function = function_selection->get_selected_function();

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
}