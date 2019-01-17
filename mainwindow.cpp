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

void MainWindow::display_message(std::string msg)
{
    QMessageBox message;
    message.setText(QString::fromStdString(msg));
    message.exec();
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
            std::string connectionString = "Driver={Microsoft Access Driver (*.mdb, *.accdb)};Uid=Admin;Pwd=;ExtendedAnsiSQL=1;DBQ=" + pathToOpenedFile.toStdString();
            this->currentDatabase = QSharedPointer<database>(new database(connectionString));

            //Populate list with all tables in database
            QStringList listOfTables = this->currentDatabase.data()->OpenedDatabase.tables(QSql::TableType::AllTables);
            this->ui->listDBTables->addItems(listOfTables);
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
    //TODO: Save results to pointer, don't bother processing tables that have already been processed during runtime.
    //QTableWidget copy constructor is private..how can we store already rendered tables??
    this->ui->tableDBTableOutput->clear();

    QString tableName = this->ui->listDBTables->item(currentRow)->text();

    if(this->currentDatabase != nullptr)
    {
        QSqlQuery selectTableData = this->currentDatabase.data()->OpenedDatabase.exec("select * from " + tableName);
        this->render_table(selectTableData.record(), selectTableData);
    }
}

void MainWindow::on_pushButtonExecuteQuery_clicked()
{
    if(this->currentDatabase != nullptr)
    {
        QSqlQuery customQuery = this->currentDatabase.data()->OpenedDatabase.exec(this->ui->textEditQuery->toPlainText());
        this->render_table(customQuery.record(), customQuery);
    }
}
