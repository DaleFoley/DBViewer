#include "designview.h"
#include "ui_designview.h"

DesignView::DesignView(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::DesignView)
{
    ui->setupUi(this);
    this->stackMultiplier = 1;
}

DesignView::~DesignView()
{
    delete ui;
}

void DesignView::showEvent(QShowEvent * event)
{
    //this->showMaximized();

    if(this->currentDatabase != nullptr)
    {
        int offsetX = 0;
        int offsetY = 0;

        for(const QString &tableName : this->tables)
        {
            QSqlRecord sqlTableRecord = this->currentDatabase->OpenedDatabase.record(tableName);
            QVector<QString> columnNames;

            int numberOfColumns = sqlTableRecord.count();
            if(numberOfColumns == 0) {continue;}

            for(int columnIndex = 0; columnIndex != numberOfColumns; columnIndex++)
            {
                columnNames.append(sqlTableRecord.fieldName(columnIndex));
            }

            this->create_erd_widget(tableName, columnNames, offsetX, offsetY);
        }

        for(const QString &queryName : this->queries)
        {
            QSqlRecord sqlTableRecord = this->currentDatabase->OpenedDatabase.record(queryName);
            QVector<QString> columnNames;

            int numberOfColumns = sqlTableRecord.count();
            //TODO: Zero columns, what do we tell the user?
            if(numberOfColumns == 0) {continue;}

            for(int columnIndex = 0; columnIndex != numberOfColumns; columnIndex++)
            {
                columnNames.append(sqlTableRecord.fieldName(columnIndex));
            }

            this->create_erd_widget(queryName, columnNames, offsetX, offsetY);
        }
    }
}

void DesignView::on_pushButtonClose_clicked()
{
    this->close();
}

void DesignView::create_erd_widget(const QString &tableName, QVector<QString> columns, int &offsetX, int &offsetY)
{
    int windowWidth = this->size().width();
    int windowHeight = this->size().height();

    TableERDWidget * erdWidget = new TableERDWidget(this, tableName, columns);
    int erdWidth = erdWidget->size().width();
    int erdHeight = erdWidget->size().height();

    int previewOffsetY = erdHeight + offsetY + 35;
    int previewOffsetX = (erdWidth * 2) + offsetX + 35;

    if(previewOffsetX > windowWidth)
    {
        //TODO: layer multiplier for X coordinate?
        offsetX = 35 * this->stackMultiplier;
        offsetY = 0;

        erdWidget->setGeometry(offsetX, offsetY, erdWidth, erdHeight);
        offsetY = erdHeight + offsetY + 35;

        this->stackMultiplier += 1;
    }
    else if(previewOffsetY > windowHeight)
    {
        offsetX = erdWidth + offsetX + 35;
        offsetY = 0;

        erdWidget->setGeometry(offsetX, offsetY, erdWidth, erdHeight);
        offsetY = erdHeight + offsetY + 35;
    }
    else
    {
        erdWidget->setGeometry(offsetX, offsetY, erdWidth, erdHeight);
        offsetY = previewOffsetY;
    }

    erdWidget->show();
}
