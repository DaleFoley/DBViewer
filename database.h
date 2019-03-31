#ifndef DATABASE_H
#define DATABASE_H

#pragma once
#include <QtSql/QSqlError>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

class database
{
public:
    database();
    database(std::string databaseConnectionString);

    QStringList get_sql_queries();

    QSqlDatabase OpenedDatabase;
};



#endif // DATABASE_H
