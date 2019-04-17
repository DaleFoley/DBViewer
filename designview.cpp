#include "designview.h"
#include "ui_designview.h"

DesignView::DesignView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DesignView)
{
    ui->setupUi(this);
}

DesignView::~DesignView()
{
    delete ui;
}
