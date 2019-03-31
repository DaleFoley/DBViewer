#include "database.h"

database::database(){}
database::database(std::string databaseConnectionString)
{   
    this->OpenedDatabase = QSqlDatabase::addDatabase("QODBC");
    OpenedDatabase.setDatabaseName(QString::fromStdString(databaseConnectionString));

    if(!OpenedDatabase.open())
    {
       QMessageBox message;
       message.setText("Failed to open database [" + QString::fromStdString(databaseConnectionString) + "] with error [" + OpenedDatabase.lastError().text() + "]");
       message.exec();
    }
}

QStringList database::get_sql_queries()
{
    QStringList rtnSqlQueries;

    QString sqlQueryToReturnQueryTypes = "select msysobj.Name from MSysObjects as msysobj left outer join MSysQueries as msysq on msysq.ObjectId = msysobj.Id where msysq.Attribute = 0;";

    QSqlQuery sqlQuery = this->OpenedDatabase.exec(sqlQueryToReturnQueryTypes);
    QSqlError queryError = sqlQuery.lastError();

    if(queryError.isValid())
    {
        QMessageBox errorMessage;
        errorMessage.setText("Encountered SQL Error [" + queryError.text() + "]");
        errorMessage.setIcon(QMessageBox::Icon::Critical);
        errorMessage.exec();

        return rtnSqlQueries;
    }

    while(sqlQuery.next())
    {
        rtnSqlQueries.append(sqlQuery.value(0).toString());
    }

    return rtnSqlQueries;
}

void database::set_default_filedialog_dir(QFileDialog * dialogToSetDefaultDir)
{
    if(settings::PathApplication != "")
    {
        QString pathSavedSQL = settings::PathApplication + QDir::separator() + "saved_sql";

        QDir dirSavedSQL;
        if(!dirSavedSQL.exists(pathSavedSQL))
        {
            dirSavedSQL.mkdir(pathSavedSQL);
        }

        dialogToSetDefaultDir->setDirectory(pathSavedSQL);
    }
}

void database::save_sql_query(QString sqlQueryTextToBeSaved)
{
    QFileDialog saveSQLQueryDialog;
    saveSQLQueryDialog.setModal(true);
    saveSQLQueryDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveSQLQueryDialog.setViewMode(QFileDialog::ViewMode::Detail);
    saveSQLQueryDialog.setDefaultSuffix("sql");

    set_default_filedialog_dir(&saveSQLQueryDialog);

    if(saveSQLQueryDialog.exec())
    {
        QString savedFileName = saveSQLQueryDialog.selectedFiles().at(0);

        QFile saveFile(savedFileName);
        saveFile.open(QFile::OpenModeFlag::WriteOnly);

        QByteArray bufferOfDataToWrite = sqlQueryTextToBeSaved.toLocal8Bit();
        const char * dataToWrite = bufferOfDataToWrite.data();

        saveFile.write(dataToWrite);
        saveFile.close();
    }
}

void database::load_sql_query(QTextEdit * textEditWidgetToLoadSQLInto)
{
    QFileDialog loadSQLQueryDialog;
    loadSQLQueryDialog.setAcceptMode(QFileDialog::AcceptOpen);
    loadSQLQueryDialog.setFilter(QDir::Filter::Files);
    loadSQLQueryDialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    loadSQLQueryDialog.setNameFilter("*.sql");
    loadSQLQueryDialog.setViewMode(QFileDialog::ViewMode::Detail);

    set_default_filedialog_dir(&loadSQLQueryDialog);

    if(loadSQLQueryDialog.exec())
    {
        QString loadedFileName = loadSQLQueryDialog.selectedFiles().at(0);

        QFile loadFile(loadedFileName);
        loadFile.open(QFile::OpenModeFlag::ReadOnly);

        QByteArray bufferOfDataToRead = loadFile.readAll();
        QString dataRead = QString::fromLocal8Bit(bufferOfDataToRead);

        textEditWidgetToLoadSQLInto->setPlainText(dataRead);
    }
}
