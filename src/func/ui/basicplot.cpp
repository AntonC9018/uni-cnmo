#include "basicplot.h"

BasicPlot::BasicPlot(QWidget *parent)
    : QwtPlot(parent)
    , _canvas(this)
    , _tracker(&_canvas)
    , _zero_scale(QwtScaleDraw::BottomScale, 0.0)
{
    {
        plotLayout()->setAlignCanvasToScales(true);
        for (int axis = 0; axis < QwtPlot::axisCnt; axis++)
            axisWidget(axis)->setMargin(0);

        QPalette palette;
        palette.setColor(QPalette::Foreground, QColor("Blue"));
        palette.setColor(QPalette::Text,       QColor("Blue"));
        _zero_scale.setPalette(palette);
        _zero_scale.attach(this);

        _canvas.setAutoFillBackground(false);
        _canvas.setFrameStyle(QFrame::NoFrame);
        setCanvas(&_canvas);

        set_label(str_lit("Graph"));

        _label.attach(this);

        _grid.setMajorPen( Qt::gray, 0, Qt::SolidLine );
        _grid.setMinorPen( Qt::gray, 0, Qt::SolidLine );
        _grid.attach(this);
    }
    {
        _curve.setPen("DarkOrange", 2);
        _curve.setRenderHint(QwtPlotItem::RenderAntialiased, true);
        _curve.attach(this);
    }
    {
        // for the demo we want the tracker to be active without
        _tracker.setStateMachine(new QwtPickerTrackerMachine());
        _tracker.setRubberBandPen(QPen("MediumOrchid"));
    }
}

void BasicPlot::update_curve(Expression_Func* func)
{
    update_curve(*func, func->lower_bound, func->upper_bound);
    set_label(func->text);
}

void BasicPlot::set_label(str_view_t name)
{
    QwtText title(QString::fromUtf8(name.chars, name.length));
    QFont font;
    font.setBold(true);
    title.setFont(font);
    title.setColor(Qt::black);
    title.setRenderFlags( Qt::AlignHCenter | Qt::AlignTop );
    _label.setText(title);
}