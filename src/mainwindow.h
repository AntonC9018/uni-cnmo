#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <strlib.h>

#include "func.h"
#include "qwt/plot.h"
#include "rootfinding/option.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Expression_Func* selected_builtin_function;
    
    str_t custom_function_str;
    Expression_Func selected_custom_function;

    // Returns the custom selected function or the builtin function
    // depending on the current selection.
    Expression_Func* get_selected_function();

    Root_Finding::Option root_finding_option;

signals:
    void selected_function_changed();
    void selected_method_changed();
    void upper_bound_changed();
    void lower_bound_changed();

public slots:
    void change_selected_custom_function();
    void change_selected_builtin_function(int index);
    void change_upper_bound(double value);
    void change_lower_bound(double value);
    void changed_function_redraw_graph();
    void reestimate_zeros();
};
