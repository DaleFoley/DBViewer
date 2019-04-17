#include "designselection.h"
#include "ui_designselection.h"

DesignSelection::DesignSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DesignSelection)
{
    ui->setupUi(this);
}

DesignSelection::~DesignSelection()
{
    delete ui;
}
