#include "poly_main.h"
#include "../builtin.h"
#include "../option.h"

namespace Poly
{
    Poly_Main::Poly_Main(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Poly_Main)
    {
    }

    Poly_Main::~Poly_Main() 
    { 
        delete ui; 
    }

    void Poly_Main::setup(Function_Selection* function_selection)
    {
        ui->setupUi(this);
        this->function_selection = function_selection;

        connect(function_selection, SIGNAL(selected_function_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(function_selection, SIGNAL(upper_bound_changed()),
                this, SLOT(changed_function_redraw_graph()));

        connect(function_selection, SIGNAL(lower_bound_changed()),
                this, SLOT(changed_function_redraw_graph()));

        for (size_t i = 0; i < countof(algo_options); i++)
        {
            ui->algorithm_combo->addItem(
                QString::fromUtf8(algo_options[i].chars));
        }

        for (size_t i = 0; i < countof(sample_options); i++)
        {
            ui->sample_algo_combo->addItem(
                QString::fromUtf8(sample_options[i].chars));
        }

        connect(function_selection, SIGNAL(selected_function_changed()),
                this, SLOT(reestimate_polinomial()));

        connect(function_selection, SIGNAL(upper_bound_changed()),
                this, SLOT(reestimate_polinomial()));

        connect(function_selection, SIGNAL(lower_bound_changed()),
                this, SLOT(reestimate_polinomial()));

        connect(ui->degree_spin_box,
                SIGNAL(valueChanged(int)),
                this,
                SLOT(reestimate_polinomial())
        );

        connect(ui->algorithm_combo,
                SIGNAL(currentIndexChanged(int)),
                this,
                SLOT(select_page(int))
        );
        
        connect(ui->algorithm_combo,
                SIGNAL(currentIndexChanged(int)),
                this,
                SLOT(reestimate_polinomial())
        );

        connect(ui->sample_algo_combo,
                SIGNAL(currentIndexChanged(int)),
                this,
                SLOT(reestimate_polinomial())
        );

        connect(ui->lagrange_portions_spinbox,
                SIGNAL(valueChanged(int)),
                this,
                SLOT(reestimate_polinomial())
        );

        ui->poly_table->setModel(&table_model);
    }

    void Poly_Main::reselect()
    {
        function_selection->reset_builtin(funcs, func_count);
        ui->algorithm_combo->setCurrentIndex(1);
    }

    void Poly_Main::changed_function_redraw_graph()
    {
        auto selected_function = function_selection->get_selected_function();

        if (selected_function->expr)
            ui->plot->update_curve(selected_function);
    }

    void Poly_Main::select_page(int index)
    {
        printf("Page switched to %i\n", index);
        ui->algo_pages->setCurrentIndex(index);
    }

    void Poly_Main::reestimate_polinomial()
    {
        auto selected_function = function_selection->get_selected_function();

        if (selected_function->expr == NULL)
            return;

        int sample_algo_index = ui->sample_algo_combo->currentIndex();
        auto sample_algo = sample_algos[sample_algo_index];
        int num_samples = ui->degree_spin_box->value();

        // Lagrange has two version: one of them has an additional variable
        int lagrange_num_portions = ui->lagrange_portions_spinbox->value();
        
        
        enum { LAGRANGE = 0, LAGRANGE_PORTIONS, CUBIC_SPLINE };
        int algo_index = ui->algorithm_combo->currentIndex();
        
        double* samples;
        
        if (algo_index == LAGRANGE_PORTIONS)
        {
            samples = lagrange_sample_portions(
                sample_algo, *selected_function, 
                num_samples, lagrange_num_portions,
                selected_function->lower_bound, selected_function->upper_bound
            );
            ui->plot->update_markers(LAGRANGE_TO_NORMAL_SAMPLES(samples, num_samples, lagrange_num_portions));
            table_model.setSamples(samples, num_samples * lagrange_num_portions);
        }
        else
        {
            samples = samples_alloc(num_samples);
            sample_algo(*selected_function, SAMPLES(samples, num_samples), 
                selected_function->lower_bound, selected_function->upper_bound);
            ui->plot->update_markers(SAMPLES(samples, num_samples));
            table_model.setSamples(samples, num_samples);
        }


        switch (algo_index)
        {
            case LAGRANGE:
            {
                auto result = lagrange_approximate_samples(SAMPLES(samples, num_samples));
                ui->plot->update_poly_curve(*result, 
                    selected_function->lower_bound, selected_function->upper_bound);
                free(result);
                break;
            }
            case LAGRANGE_PORTIONS:
            {
                auto result = lagrange_approximate_samples_portions(
                    LAGRANGE_SAMPLES(samples, num_samples, lagrange_num_portions));
                ui->plot->update_poly_curve(result,
                    selected_function->lower_bound, selected_function->upper_bound);
                lagrange_free_portions(result);
                break;
            }
            case CUBIC_SPLINE: 
            {
                auto result = make_cubic_spline_normal(SAMPLES(samples, num_samples));
                ui->plot->update_poly_curve(*result, 
                    selected_function->lower_bound, selected_function->upper_bound);
                free(result);
                break;
            }
        }

    }
}