#include "plot.h"

namespace Poly
{
    Plot::Plot(QWidget* parent)
        : BasicPlot(parent)
    {
        _poly_curve.setPen("Blue", 2);
        _poly_curve.setRenderHint(QwtPlotItem::RenderAntialiased, true);
        _poly_curve.attach(this);
    }

    void Plot::update_markers(double* xs, double* ys, size_t count)
    {
        size_t current_number = _markers.size();
        for (size_t i = current_number; i < count; i++)
        {
            const QSize marker_size = QSize(8, 8);
            auto* marker = new QwtPlotMarker();
            _markers.push_back(marker);
            // This functions DOES take ownership of the symbol, even though it takes in a const*
            marker->setSymbol(new QwtSymbol(QwtSymbol::Diamond, Qt::red, Qt::NoPen, marker_size));
            marker->attach(this);
        }
        // Show all active markers
        for (size_t i = 0; i < count; i++)
        {
            _markers[i]->show();
            _markers[i]->setValue(QPointF(xs[i], ys[i]));
        }
        // Hide marker excess.
        for (size_t i = count; i < current_number; i++)
        {
            _markers[i]->hide();
        }
    }
}