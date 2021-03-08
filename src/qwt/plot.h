#ifndef _PLOT_H_
#define _PLOT_H_

#include <qwt_plot.h>
#include <qwt_point_data.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_zoneitem.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_scale_widget.h>
#include <qwt_symbol.h>
#include <qwt_plot_scaleitem.h>

#include "curvetracker.h"
#include "../func.h"

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    explicit Plot(QWidget * parent = NULL);

    void update_curve(Func* func);

    void zeros(Func* func);

private:
    QwtPlotScaleItem _zero_scale;
    QwtPlotCanvas _canvas;
    QwtPlotTextLabel _label;
    QwtPlotGrid _grid;
    QwtPlotCurve _curve;
    CurveTracker _tracker;

    std::vector<QwtPlotMarker*> _zeros;
    void reset_number_of_zero_markers(size_t new_size);

};

#endif

