#pragma once
#include <QWidget>

#include "./ui_root_finding_main.h"
#include <func/ui/function_selection.h>
#include "plot.h"
#include "zeros_table_model.h"

#include "../option.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Root_Finding_Main; }
QT_END_NAMESPACE

namespace Root_Finding
{
    class Root_Finding_Main : public QWidget
    {
        Q_OBJECT

    public:
        Root_Finding_Main(QWidget *parent = NULL);
        ~Root_Finding_Main();
        void setup(Function_Selection*); // Does not take ownership of the object
        void reselect();

    private:
        Ui::Root_Finding_Main *ui;
        Function_Selection* function_selection;
        Zeros_Table_Model zeros_model;

    public slots:
        void changed_function_redraw_graph();
        void reestimate_zeros();
    };
}