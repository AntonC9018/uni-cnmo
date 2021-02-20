#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <cparse/shunting-yard.h>
#include <strlib.h>
#include "qwt/plot.h"
#include "func.h"

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
    Plot *plot;

    const Func* selected_builtin_function;
    
    str_t custom_function_str;
    Func selected_custom_function;

    // Returns the custom selected function or the builtin function
    // depending on the current selection.
    const Func* get_selected_function();

signals:
    void selected_function_changed();
    void upper_bound_changed();
    void lower_bound_changed();

public slots:
    void change_selected_custom_function();
    void change_selected_builtin_function(int index);
    void change_upper_bound(double value);
    void change_lower_bound(double value);
    void changed_function_redraw_graph();
};
