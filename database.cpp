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
