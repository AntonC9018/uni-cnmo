#pragma once
#include <QVariant>
#include <mydefines.h>
#include <strlib.h>

struct Zeros_Table_Row
{
    double x;
    double y;
    u32 num_iters;
    s64 microsecs;
};

const str_view_t zeros_row_header[4] { 
    str_lit("x"),
    str_lit("y"),
    str_lit("iterations"),
    str_lit("μs")
};


inline QVariant zeros_row_variant_at(const Zeros_Table_Row* row, const int column_index)
{
    enum Zero_Row_Index
    {
        X = 0, Y, ITERATIONS, TIME
    };

    switch (column_index)
    {
    case ITERATIONS:
        return row->num_iters;
    case TIME:
        return row->microsecs;
    case Y: 
        // display in scientific notation
        return QString::number(row->y, 'e', 2);
    case X:
        // display normally, with, say, precision of 6
        return QString::number(row->x, 'f', 6);
    }
    return 0;
}