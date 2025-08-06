// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

/*
  window.cpp

  A minimal subclass of QTableView with slots to allow the selection model
  to be monitored.
*/

#include <QAbstractItemModel>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QStatusBar>

#include "window.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(int rows, int columns, QObject *parent = nullptr)
        : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return 0;
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return 0;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        return QVariant();
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Selected items in a table model");

    model = new TableModel(8, 4, this);

    table = new QTableView(this);
    table->setModel(model);

    selectionModel = table->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateSelection);
    connect(selectionModel, &QItemSelectionModel::currentChanged,
            this, &MainWindow::changeCurrent);

    statusBar();
    setCentralWidget(table);
}

//! [0]
void MainWindow::updateSelection(const QItemSelection &selected,
    const QItemSelection &deselected)
{
    QModelIndexList items = selected.indexes();

    for (const QModelIndex &index : std::as_const(items)) {
        QString text = QString("(%1,%2)").arg(index.row()).arg(index.column());
        model->setData(index, text);
//! [0] //! [1]
    }
//! [1]

//! [2]
    items = deselected.indexes();

    for (const QModelIndex &index : std::as_const(items)) {
        model->setData(index, QString());
    }
}
//! [2]

//! [3]
void MainWindow::changeCurrent(const QModelIndex &current,
    const QModelIndex &previous)
{
    statusBar()->showMessage(
        tr("Moved from (%1,%2) to (%3,%4)")
            .arg(previous.row()).arg(previous.column())
            .arg(current.row()).arg(current.column()));
}
//! [3]
