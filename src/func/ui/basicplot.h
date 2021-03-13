#pragma once

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

class BasicPlot: public QwtPlot
{
    Q_OBJECT

public:
    explicit BasicPlot(QWidget * parent = NULL);
    void update_curve(Expression_Func* func);

private:
    QwtPlotScaleItem _zero_scale;
    QwtPlotCanvas _canvas;
    QwtPlotTextLabel _label;
    QwtPlotGrid _grid;
    QwtPlotCurve _curve;
    CurveTracker _tracker;
};