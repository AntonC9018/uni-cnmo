#include "zeros_table_model.h"

namespace Root_Finding
{
    Zeros_Table_Model::Zeros_Table_Model(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }

    int Zeros_Table_Model::rowCount(const QModelIndex & /*parent*/) const
    {
        return rows.size();
    }

    int Zeros_Table_Model::columnCount(const QModelIndex & /*parent*/) const
    {
        return countof(zeros_row_header);
    }

    QVariant Zeros_Table_Model::data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            return zeros_row_variant_at(&rows[index.row()], index.column());
        }
        return QVariant();
    }

    QVariant Zeros_Table_Model::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole) 
        {
            if (orientation == Qt::Horizontal)
            {
                return QString(zeros_row_header[section].chars);
            }
        }
        return QVariant();
    }

    void Zeros_Table_Model::swap_data(std::vector<Zeros_Table_Row>& data)
    {
        emit beginResetModel();
        rows.swap(data);
        emit endResetModel();
    }
}