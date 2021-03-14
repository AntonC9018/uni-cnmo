#include "poly_main.h"
#include "../builtin.h"

namespace Poly
{
    Poly_Main::Poly_Main(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::Poly_Main)
    {
    }
    
    Poly_Main::~Poly_Main() { delete ui; }

    void Poly_Main::setup(Function_Selection* function_selection)
    {
        ui->setupUi(this);
        this->function_selection = function_selection;
    }

    void Poly_Main::reselect()
    {
        function_selection->reset_builtin(funcs, func_count);
    }
}