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
