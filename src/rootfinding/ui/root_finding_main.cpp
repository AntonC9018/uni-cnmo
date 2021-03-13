#include "root_finding_main.h"
#include "../builtin.h"

namespace Root_Finding
{
    Root_Finding_Main::Root_Finding_Main(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Root_Finding_Main)
    {
        ui->setupUi(this);
        ui->precision_spin_box->setRange(-10, -5);
        ui->precision_spin_box->setValue(-7);
        ui->precision_spin_box->setSingleStep(0.1);

        // Algorithms
        using namespace Root_Finding;

        for (size_t i = 0; i < countof(option_text); i++)
        {
            ui->algorithm_combo->addItem(option_text[i].chars);
        }

        connect(ui->function_selection, SIGNAL(selected_function_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(ui->function_selection, SIGNAL(selected_function_changed()),
                this, SLOT(reestimate_zeros()));

        connect(ui->function_selection, SIGNAL(upper_bound_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(ui->function_selection, SIGNAL(upper_bound_changed()), 
                this, SLOT(reestimate_zeros()));
        
        connect(ui->function_selection, SIGNAL(lower_bound_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(ui->function_selection, SIGNAL(lower_bound_changed()), 
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

        ui->function_selection->setup(funcs, func_count);
        ui->zeros_table->setModel(&zeros_model);
    }

    Root_Finding_Main::~Root_Finding_Main()
    {
        delete ui;
    }

    void Root_Finding_Main::changed_function_redraw_graph()
    {
        auto selected_function = ui->function_selection->get_selected_function();

        if (selected_function->expr)
            ui->plot->update_curve(selected_function);
    }

    void Root_Finding_Main::reestimate_zeros()
    {
        using namespace Root_Finding;

        auto selected_function = ui->function_selection->get_selected_function();

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