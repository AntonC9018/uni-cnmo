#include "poly_table_model.h"

namespace Poly
{
    Poly_Table_Model::Poly_Table_Model(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    int Poly_Table_Model::rowCount(const QModelIndex & /*parent*/) const
    {
        return num_samples;
    }

    int Poly_Table_Model::columnCount(const QModelIndex & /*parent*/) const
    {
        return 2;
    }

    enum { X = 0, Y };

    QVariant Poly_Table_Model::data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            int col = index.column();
            switch (col)
            {
                case X:
                    return QString::number(samples[index.row()], 'f', 4);
                case Y:
                    return QString::number(samples[num_samples + index.row()], 'f', 4);
            }
        }
        return QVariant();
    }

    QVariant Poly_Table_Model::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole) 
        {
            if (orientation == Qt::Horizontal)
            {
                switch (section)
                {
                    case X: return "x";
                    case Y: return "f(x)";
                }
            }
        }
        return QVariant();
    }

    void Poly_Table_Model::setSamples(double* samples, size_t num_samples)
    {
        emit beginResetModel();

        if (this->samples) free(this->samples);
        this->samples = samples;
        this->num_samples = num_samples;

        emit endResetModel();
    }
}