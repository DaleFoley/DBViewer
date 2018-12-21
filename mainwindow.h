#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QSharedPointer>
#include <QListWidgetItem>
#include <QMap>
#include <QTableWidget>

#include "database.h"

namespace Ui
{
    class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_actionOpen_triggered();
        void on_listDBTables_currentRowChanged(int currentRow);

    private:
        Ui::MainWindow *ui;

        QSharedPointer<database> currentDatabase;
};

#endif // MAINWINDOW_H
