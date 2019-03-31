#include "mainwindow.h"
#include "ui_mainwindow.h"

//TODO: Error handling.
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    settings::PathApplication = QCoreApplication::applicationDirPath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display_message(std::string msg)
{
    QMessageBox message;
    message.setText(QString::fromStdString(msg));
    message.exec();
}

void MainWindow::process_row_change(QListWidget * listCollection, int currentRowIndex)
{
    this->ui->tableDBTableOutput->reset();

    QString tableName = listCollection->item(currentRowIndex)->text();

    if(this->currentDatabase != nullptr)
    {
        QSqlQuery selectTableData = this->currentDatabase.data()->OpenedDatabase.exec("select * from " + tableName);
        this->render_table(selectTableData.record(), selectTableData);

        if(selectTableData.isActive())
        {
            int rowsReturned = selectTableData.size();
            this->ui->labelQueryStatus->setText("Number of rows returned [" + QString::number(rowsReturned) + "]");
        }
    }
}

void MainWindow::render_table(QSqlRecord recordToBeProcessed, QSqlQuery underlyingQuery)
{
    QStringList columnNames;
    
    int columnCount = recordToBeProcessed.count();
    for(int i = 0; i != columnCount; i++)
    {
        columnNames.append(recordToBeProcessed.fieldName(i));
        this->ui->tableDBTableOutput->insertColumn(i);
    }
    this->ui->tableDBTableOutput->setHorizontalHeaderLabels(columnNames);
    this->ui->tableDBTableOutput->setColumnCount(columnCount);

    //Populate rows.
    int rowCount = 0;
    while(underlyingQuery.next())
    {
        //Is there a better way to do this?
        this->ui->tableDBTableOutput->setRowCount(1 + rowCount);

        for(int columnIndex = 0; columnIndex != columnCount; columnIndex++)
        {
            QTableWidgetItem * tableItem = new QTableWidgetItem(underlyingQuery.value(columnIndex).toString());
            this->ui->tableDBTableOutput->setItem(rowCount, columnIndex, tableItem);
        }
        rowCount++;
    }
}

void MainWindow::on_actionOpenDatabase_triggered()
{
    QFileDialog fileDialog;

    try
    {
        fileDialog.setViewMode(QFileDialog::ViewMode::Detail);
        fileDialog.setFileMode(QFileDialog::FileMode::ExistingFile);

        //Open our database
        if(fileDialog.exec())
        {
            QString pathToOpenedFile = fileDialog.selectedFiles().at(0);
            //std::string connectionString = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + pathToOpenedFile.toStdString();

            QString pathWorkbookFile = settings::PathApplication + QDir::separator() + "System.mdw";
            std::string connectionString = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};Uid=Admin;Pwd=;ExtendedAnsiSQL=1;DBQ=" + pathToOpenedFile.toStdString() + ";" +
                    "SystemDB=" + pathWorkbookFile.toStdString();;

            this->currentDatabase = QSharedPointer<database>(new database(connectionString));

            //Populate list with all tables in database and get queries as well.
            QStringList listOfTables = this->currentDatabase.data()->OpenedDatabase.tables(QSql::TableType::AllTables);
            QStringList listOfQueries = this->currentDatabase->get_sql_queries();

            this->ui->listDBTables->reset();
            this->ui->listWidgetDBQueries->reset();

            //Remove 'query' types so we can add them to a different list.
            for(const QString &queryToBeRemoved: listOfQueries)
            {
                listOfTables.removeAll(queryToBeRemoved);
            }

            this->ui->listDBTables->addItems(listOfTables);
            this->ui->listWidgetDBQueries->addItems(listOfQueries);
        }
    }
    catch(const std::exception &ex)
    {
        std::string errorMessage("Encountered error [" + std::string(ex.what()) + "]");
        this->display_message(errorMessage);
    }
}

void MainWindow::on_listDBTables_currentRowChanged(int currentRow)
{
    this->process_row_change(this->ui->listDBTables, currentRow);
}

void MainWindow::on_listWidgetDBQueries_currentRowChanged(int currentRow)
{
    this->process_row_change(this->ui->listWidgetDBQueries, currentRow);
}

void MainWindow::on_pushButtonExecuteQuery_clicked()
{
    if(this->currentDatabase != nullptr)
    {
        QSqlQuery customQuery = this->currentDatabase.data()->OpenedDatabase.exec(this->ui->textEditQuery->toPlainText());
        QSqlError queryError = customQuery.lastError();

        if(queryError.isValid())
        {
            QMessageBox errorMessage;
            errorMessage.setText("Encountered SQL Error [" + queryError.text() + "]");
            errorMessage.setIcon(QMessageBox::Icon::Critical);
            errorMessage.exec();

            return;
        }

        if(customQuery.isActive())
        {
            int numberOfRowsAffected = 0;

            if(customQuery.isSelect())
            {
                numberOfRowsAffected = customQuery.size();
                this->ui->labelQueryStatus->setText("Number of rows returned [" + QString::number(numberOfRowsAffected) + "]");
            }
            else
            {
                numberOfRowsAffected = customQuery.numRowsAffected();
                this->ui->labelQueryStatus->setText("Number of rows affected [" + QString::number(numberOfRowsAffected) + "]");
            }

            this->render_table(customQuery.record(), customQuery);
        }
    }
}

void MainWindow::on_pushButtonSaveQuery_clicked()
{
    try
    {
        QString sqlQueryTextToSave = this->ui->textEditQuery->toPlainText();
        if(sqlQueryTextToSave == "")
        {
            QMessageBox emptySQLText;
            emptySQLText.setText("Require SQL text to save.");
            emptySQLText.setIcon(QMessageBox::Icon::Information);
            emptySQLText.setModal(true);
            emptySQLText.exec();
        }
        else
        {
            database::save_sql_query(this->ui->textEditQuery->toPlainText());
        }
    }
    catch(const std::exception &ex)
    {
        std::string errorMessage("Encountered error [" + std::string(ex.what()) + "]");
        this->display_message(errorMessage);
    }
}

void MainWindow::on_pushButtonLoadQuery_clicked()
{
    try
    {
        database::load_sql_query(this->ui->textEditQuery);
    }
    catch (const std::exception &ex)
    {
        std::string errorMessage("Encountered error [" + std::string(ex.what()) + "]");
        this->display_message(errorMessage);
    }
}
