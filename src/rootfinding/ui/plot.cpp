#include "plot.h"
#include <algorithm>

namespace Root_Finding
{
    Plot::Plot(QWidget *parent)
        : BasicPlot(parent)
    {
    }

    std::vector<Zeros_Table_Row> Plot::zeros(
        Expression_Func* func, 
        Root_Finding::Error_Data* error_data, 
        size_t option_index)
    {
        std::vector<Zeros_Table_Row> x_approxs;

        if ((func->upper_bound - func->lower_bound) < error_data->tolerance)
            return x_approxs;

        const double num_steps = 100.0;
        const double step = (func->upper_bound - func->lower_bound) / num_steps;
        const auto zeros_xs = localize(*func, func->lower_bound, func->upper_bound, step);

        reset_number_of_zero_markers(zeros_xs.size());

        // printf("%s method started.\n", option_text[option_index].chars);

        for (size_t i = 0; i < zeros_xs.size(); i++)
        {
            Profiler profiler; 
            profiler_start(&profiler);

            error_data->error_message = STR_NULL; // Reset the previous error
            const double x = zeros_xs[i];

            double approx = option_adaptors[option_index](
                func, { x, x + step }, error_data, &profiler
            );

            if (!str_is_null(error_data->error_message))
            {
                _zeros[i]->hide();
            }
            else
            {
                const double y = func_eval(func, approx);

                _zeros[i]->setValue(QPointF(approx, y));

                profiler_stop(&profiler);

                Zeros_Table_Row row;
                row.x = approx;
                row.y = y;
                row.microsecs = profiler.time_elapsed.count();
                row.num_iters = profiler.num_iters;

                x_approxs.push_back(row);
            }
        }

        replot(); // Render the updated markers

        return x_approxs;
    }

    void Plot::reset_number_of_zero_markers(size_t new_number)
    {
        size_t current_number = _zeros.size();
        // Add more markers, if the current amount does not suffice.
        for (size_t i = current_number; i < new_number; i++)
        {
            const QSize marker_size = QSize(8, 8);

            auto* marker = new QwtPlotMarker();
            _zeros.push_back(marker);
            // This functions DOES take ownership of the symbol, even though it takes in a const*
            marker->setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::red, Qt::NoPen, marker_size));
            marker->attach(this);
        }
        // Show all active markers
        for (size_t i = 0; i < new_number; i++)
        {
            _zeros[i]->show();
        }
        // Hide marker excess.
        for (size_t i = new_number; i < current_number; i++)
        {
            _zeros[i]->hide();
        }
    }
}