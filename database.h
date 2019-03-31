#ifndef DATABASE_H
#define DATABASE_H

#pragma once
#include <QtSql/QSqlError>
#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QTextEdit>
#include <QFileDialog>

#include "settings.h"

class database
{
public:
    database();
    database(std::string databaseConnectionString);

    QStringList get_sql_queries();

    QSqlDatabase OpenedDatabase;

    static void save_sql_query(QString sqlQueryTextToBeSaved);
    static void load_sql_query(QTextEdit * textEditWidgetToLoadSQLInto);

private:
    static void set_default_filedialog_dir(QFileDialog * dialogToSetDefaultDir);
};



#endif // DATABASE_H
