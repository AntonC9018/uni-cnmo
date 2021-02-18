#include <QApplication>

#include "mainwindow.h"
#include "equations.h"

int main(int argc, char *argv[])
{
    Builtin::compile();
    auto tm = Builtin::make_token_map();
    tm["x"] = 5;

    for (int i = 0; i < countof(Builtin::expression_strings); i++)
    {
        printf("Evaluating %s at 5: ", Builtin::expression_strings[i]);
        printf("%f\n", Builtin::calculators[i].eval(tm).asDouble());
    }

    QApplication app(argc, argv);
    MainWindow window;

    window.show();

    return app.exec();
}
