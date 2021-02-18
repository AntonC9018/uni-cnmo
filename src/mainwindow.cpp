#include <strlib.h>
#include "profiler.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "equations.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->precision_spin_box->setRange(-10, -5);
    ui->precision_spin_box->setValue(-7);
    ui->precision_spin_box->setSingleStep(0.1);

    {
        using namespace Builtin;

        compile();

        for (int i = 0; i < expression_count; i++)
        {
            ui->function_selection_combo->addItem(
                QString::fromUtf8(expression_strings[i]));
        }

        connect(ui->function_selection_combo, 
                SIGNAL(currentIndexChanged(int)), 
                this, 
                SLOT(index_changed_function_combo(int))
        );
    }

    connect(ui->function_custom_edit, 
            SIGNAL(editingFinished()), 
            this, 
            SLOT(editing_finished_custom_function())
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::index_changed_function_combo(int index)
{
    if (ui->function_predefined_rbutton->isChecked())
    {
        // TODO: set this function somewhere
        if (index < Builtin::expression_count)
        {
            // For now, evaluate it to prove the point
            auto calc = Builtin::calculators[index];
            auto tm   = Builtin::make_token_map();   tm["x"] = 5;

            printf("Evaluating %s at %f: %f\n", 
                Builtin::expression_strings[index], 5.0f, calc.eval(tm).asDouble()
            );
        }
    }
}

void MainWindow::editing_finished_custom_function()
{
    if (ui->function_custom_rbutton->isChecked())
    {
        auto qstr_utf8 = ui->function_custom_edit->text().toUtf8();
        // Since we need this array to be null terminated, and there is no
        // guarantee for that from qstr_utf8, we have to copy the string with malloc.
        str_t str = str_copy(qstr_utf8.data(), qstr_utf8.length());

        calculator calc(str.chars);
        auto tm = Builtin::make_token_map();
        tm["x"] = 5;

        printf("Evaluating %s at %f: %f\n", 
            str.chars, 5.0f, calc.eval(tm).asDouble()
        );

        str_free(str);
    }
}