#include "designselection.h"
#include "ui_designselection.h"

DesignSelection::DesignSelection(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::DesignSelection)
{
    ui->setupUi(this);
}

DesignSelection::~DesignSelection()
{
    delete ui;
}

void DesignSelection::showEvent(QShowEvent * event)
{
    QWidget::showEvent(event);

    if(this->currentDatabase != nullptr)
    {
        this->ui->listWidgetTables->addItems(this->currentDatabase->tables);
        this->ui->listWidgetQueries->addItems(this->currentDatabase->queries);
    }

}

void DesignSelection::on_pushButtonCancel_clicked()
{
    this->close();
}

void DesignSelection::on_pushButtonAdd_clicked()
{
    DesignView designView;
    designView.currentDatabase = this->currentDatabase;

    QStringList selectedTableItems;
    for(QListWidgetItem * selectedTableItem : this->ui->listWidgetTables->selectedItems())
    {
        selectedTableItems.append(selectedTableItem->text());
    }

    designView.tables = selectedTableItems;

    designView.setModal(true);
    designView.exec();

}
