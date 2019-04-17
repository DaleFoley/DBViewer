#include "designview.h"
#include "ui_designview.h"

DesignView::DesignView(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::DesignView)
{
    ui->setupUi(this);
}

DesignView::~DesignView()
{
    delete ui;
}

void DesignView::showEvent(QShowEvent * event)
{
    if(this->currentDatabase != nullptr)
    {
        for(const QString &tableName : this->tables)
        {
            QSqlRecord sqlTableRecord = this->currentDatabase->OpenedDatabase.record(tableName);
            QVector<QString> columnNames;

            int numberOfColumns = sqlTableRecord.count();
            for(int columnIndex = 0; columnIndex != numberOfColumns; columnIndex++)
            {
                columnNames.append(sqlTableRecord.fieldName(columnIndex));
            }

            TableERDWidget * designView = new TableERDWidget(this, tableName, columnNames);
            designView->show();

            break;
        }

    }
}

void DesignView::on_pushButtonClose_clicked()
{
    this->close();
}
