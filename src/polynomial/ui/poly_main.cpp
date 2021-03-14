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
}