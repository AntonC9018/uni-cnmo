#pragma once
#include <QAbstractTableModel>
#include "zeros_table_row.h"

class Zeros_Table_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    Zeros_Table_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void swap_data(std::vector<Zeros_Table_Row>& data);

    std::vector<Zeros_Table_Row> rows;
};