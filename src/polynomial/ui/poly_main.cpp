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
                SLOT(reestimate_polinomial())
        );

        connect(ui->sample_algo_combo,
                SIGNAL(currentIndexChanged(int)),
                this,
                SLOT(reestimate_polinomial())
        );
    }

    void Poly_Main::reselect()
    {
        function_selection->reset_builtin(funcs, func_count);
    }

    void Poly_Main::changed_function_redraw_graph()
    {
        auto selected_function = function_selection->get_selected_function();

        if (selected_function->expr)
            ui->plot->update_curve(selected_function);
    }

    void Poly_Main::reestimate_polinomial()
    {
        auto selected_function = function_selection->get_selected_function();

        if (selected_function->expr == NULL)
            return;

        int sample_algo_index = ui->sample_algo_combo->currentIndex();
        auto sample_algo = sample_algos[sample_algo_index];
        int num_samples = ui->degree_spin_box->value();
        
        auto samples = sample_algo(*selected_function, num_samples,
            selected_function->lower_bound, selected_function->upper_bound);

        int algo_index = ui->algorithm_combo->currentIndex();
        enum { LAGRANGE = 0, LAGRANGE_PORTIONS, CUBIC_SPLINE };

        ui->plot->update_markers(SAMPLES(samples, num_samples));

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
            case CUBIC_SPLINE: 
            {
                auto result = make_cubic_spline_normal(SAMPLES(samples, num_samples));
                ui->plot->update_poly_curve(*result, 
                    selected_function->lower_bound, selected_function->upper_bound);
                free(result);
                break;
            }
        }

        free(samples);
    }
}