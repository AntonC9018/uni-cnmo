#include <QApplication>
#include "mainwindow.h"
#include "qwt/plot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    window.show();
    
    Plot plot;
    plot.resize( 600, 400 );
    plot.show();

    return app.exec();
}
