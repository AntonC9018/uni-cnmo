#pragma once
#include <QWidget>

#include "./ui_root_finding_main.h"
#include "../option.h"
#include "plot.h"
#include "zeros_table_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Root_Finding_Main; }
QT_END_NAMESPACE

class Root_Finding_Main : public QWidget
{
    Q_OBJECT

public:
    Root_Finding_Main(QWidget *parent = NULL);
    ~Root_Finding_Main();

private:
    Ui::Root_Finding_Main *ui;
    Zeros_Table_Model zeros_model;

public slots:
    void changed_function_redraw_graph();
    void reestimate_zeros();
};
