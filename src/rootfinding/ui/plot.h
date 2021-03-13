#ifndef _PLOT_H_
#define _PLOT_H_

#include <func/ui/basicplot.h>
#include <func/func.h>
#include "../shared.h"
#include "../option.h"
#include "zeros_table_row.h"

namespace Root_Finding
{
    class Plot: public BasicPlot
    {
        Q_OBJECT

    public:
        explicit Plot(QWidget * parent = NULL);

        std::vector<Zeros_Table_Row> zeros(
            Expression_Func* func, 
            Root_Finding::Error_Data* error_data, 
            size_t option_index
        );

    private:
        std::vector<QwtPlotMarker*> _zeros;
        void reset_number_of_zero_markers(size_t new_size);
    };
}

#endif