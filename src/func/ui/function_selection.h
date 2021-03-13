#pragma once
#include <QWidget>
#include "../func.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Function_Selection; }
QT_END_NAMESPACE

class Function_Selection : public QWidget
{
    Q_OBJECT

public:
    Function_Selection(QWidget *parent);
    void setup(Expression_Func* builtin_functions, size_t func_count);
    ~Function_Selection();
    Expression_Func* get_selected_function();

private:
    Ui::Function_Selection *ui;

    str_t custom_function_str;
    Expression_Func custom_function;
    Expression_Func* builtin_functions;

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
    void trigger_selected_function_changed();
};