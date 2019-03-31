#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QSharedPointer>
#include <QListWidgetItem>
#include <QMap>
#include <QTableWidget>
#include <stdexcept>

#include "database.h"

namespace Ui
{
    class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_actionOpenDatabase_triggered();
        void on_listDBTables_currentRowChanged(int currentRow);

        void on_pushButtonExecuteQuery_clicked();

        void on_listWidgetDBQueries_currentRowChanged(int currentRow);

    private:
        Ui::MainWindow *ui;

        const QString SQLQueryStatus = "SQL Query Status: ";
        QSharedPointer<database> currentDatabase;

        void render_table(QSqlRecord recordToBeProcessed, QSqlQuery underlyingQuery);
        void process_row_change(QListWidget * listCollection, int currentRowIndex);
        void display_message(std::string msg);
};

#endif // MAINWINDOW_H
