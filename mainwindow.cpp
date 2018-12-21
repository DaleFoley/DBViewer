#include "mainwindow.h"
#include "ui_mainwindow.h"

//TODO: Error handling.
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog fileDialog;
    fileDialog.setViewMode(QFileDialog::ViewMode::Detail);
    fileDialog.setFileMode(QFileDialog::FileMode::ExistingFile);

    //Open our database
    if(fileDialog.exec())
    {
        QString pathToOpenedFile = fileDialog.selectedFiles().at(0);
        std::string connectionString = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};DSN='';DBQ=" + pathToOpenedFile.toStdString();
        this->currentDatabase = QSharedPointer<database>(new database(connectionString));
    }

    //Populate list with all tables in database
    QStringList listOfTables = this->currentDatabase.data()->OpenedDatabase.tables(QSql::TableType::AllTables);
    this->ui->listDBTables->addItems(listOfTables);
}

void MainWindow::on_listDBTables_currentRowChanged(int currentRow)
{
    //TODO: Save results to pointer, don't bother processing tables that have already been processed during runtime.
    //QTableWidget copy constructor is private..how can we store already rendered tables??
    this->ui->tableDBTableOutput->clear();

    QString tableName = this->ui->listDBTables->item(currentRow)->text();

    QSqlQuery selectTableData = this->currentDatabase.data()->OpenedDatabase.exec("select * from " + tableName);
    QSqlRecord selectedRecord = selectTableData.record();

    //Get column count and draw the columns on our table
    QStringList columnNames;
    int columnCount = selectedRecord.count();
    for(int i = 0; i != columnCount; i++)
    {
        columnNames.append(selectedRecord.fieldName(i));
        this->ui->tableDBTableOutput->insertColumn(i);
    }
    this->ui->tableDBTableOutput->setHorizontalHeaderLabels(columnNames);
    this->ui->tableDBTableOutput->setColumnCount(columnCount);

    //Populate rows.
    int rowCount = 0;
    while(selectTableData.next())
    {
        //Is there a better way to do this?
        this->ui->tableDBTableOutput->setRowCount(1 + rowCount);

        for(int columnIndex = 0; columnIndex != columnCount; columnIndex++)
        {
            QTableWidgetItem * tableItem = new QTableWidgetItem(selectTableData.value(columnIndex).toString());
            this->ui->tableDBTableOutput->setItem(rowCount, columnIndex, tableItem);
        }
        rowCount++;
    }
}
