#pragma once
#include <QWidget>

#include "./ui_poly_main.h"
#include <func/ui/function_selection.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Poly_Main; }
QT_END_NAMESPACE

namespace Poly
{
    class Poly_Main : public QWidget
    {
        Q_OBJECT

    public:
        Poly_Main(QWidget *parent = NULL);
        ~Poly_Main();
        void setup(Function_Selection*); // Does not take ownership of the object
        void reselect();

    private:
        Ui::Poly_Main *ui;
        Function_Selection* function_selection;

    public slots:
        void changed_function_redraw_graph();
        void reestimate_polinomial();
    };
}