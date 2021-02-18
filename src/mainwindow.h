#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <cparse/shunting-yard.h>

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
    calculator selected_function;

signals:
public slots:
    void index_changed_function_combo(int index);
    void editing_finished_custom_function();
};
