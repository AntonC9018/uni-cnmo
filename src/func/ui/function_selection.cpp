#include "function_selection.h"
#include "./ui_function_selection.h"

Function_Selection::Function_Selection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Function_Selection) 
{
}

void Function_Selection::setup(Expression_Func* builtin_functions, size_t func_count)
{
    ui->setupUi(this);

    // These do not have a default constructor, which is why they have trash by default
    custom_function_str = STR_NULL;
    func_clear(&custom_function);

    // Setup builtin functions
    {        
        for (int i = 0; i < func_count; i++)
        {
            ui->function_selection_combo->addItem(
                QString::fromUtf8(builtin_functions[i].text.chars));
        }
        this->builtin_functions = builtin_functions;
    }

    ui->function_builtin_rbutton->setChecked(true);

    // Selecting the builtin function 
    connect(ui->function_selection_combo, 
            SIGNAL(currentIndexChanged(int)), 
            this, 
            SLOT(change_selected_builtin_function(int))
    );

    // Selecting a custom function
    connect(ui->function_custom_edit, 
            SIGNAL(editingFinished()), 
            this, 
            SLOT(change_selected_custom_function())
    );

    // Upper and lower bounds
    ui->function_upper_spinbox->setRange(-1024, 1024);
    connect(ui->function_upper_spinbox,
            SIGNAL(valueChanged(double)),
            this,
            SLOT(change_upper_bound(double))
    );
    
    ui->function_lower_spinbox->setRange(-1024, 1024);
    connect(ui->function_lower_spinbox,
            SIGNAL(valueChanged(double)),
            this,
            SLOT(change_lower_bound(double))
    );

    connect(ui->function_custom_rbutton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(trigger_selected_function_changed())
    );

    ui->function_selection_combo->setCurrentIndex(1);
}

Expression_Func* Function_Selection::get_selected_function()
{
    return ui->function_custom_rbutton->isChecked() 
        ? &custom_function
        : &builtin_functions[ui->function_selection_combo->currentIndex()];
}

void Function_Selection::change_selected_builtin_function(int index)
{
    if (ui->function_builtin_rbutton->isChecked())
    {
        emit selected_function_changed();
    }
}

void Function_Selection::change_selected_custom_function()
{
    auto qstr_utf8 = ui->function_custom_edit->text().toUtf8();

    int error = 0;

    func_free(&custom_function);

    if (qstr_utf8 != "")
    {
        // Since we need this array to be null terminated, and there is no
        // guarantee for that from qstr_utf8, we have to copy the string with malloc.
        str_smart_replace(&custom_function_str, qstr_utf8.data(), qstr_utf8.length());

        custom_function.text = str_view(custom_function_str);
        func_compile(&custom_function, &error);
    }

    if (ui->function_custom_rbutton->isChecked() && !error)
    {
        emit selected_function_changed();
    }
}

void Function_Selection::change_upper_bound(double value)
{
    double new_value = std::max(value, custom_function.lower_bound);
    if (custom_function.upper_bound != new_value)
    {
        custom_function.upper_bound = new_value;
        emit upper_bound_changed();
    }
    // change the text
    if (new_value != value)
    {
        ui->function_upper_spinbox->setValue(new_value);
    }
}

void Function_Selection::change_lower_bound(double value)
{
    double new_value = std::min(value, custom_function.upper_bound);
    if (custom_function.lower_bound != new_value)
    {
        custom_function.lower_bound = new_value;
        emit lower_bound_changed();
    }
    // change the text
    if (new_value != value)
    {
        ui->function_lower_spinbox->setValue(new_value);
    }
}

void Function_Selection::trigger_selected_function_changed()
{
    emit selected_function_changed();
}

Function_Selection::~Function_Selection()
{
    func_free(&custom_function);
}