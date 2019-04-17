#include "tablecolumn.h"

tableColumn::tableColumn(QWidget * parent, Qt::WindowFlags f) : QLabel::QLabel(parent, f)
{
}

void tableColumn::enterEvent(QEvent *)
{
    this->setStyleSheet("QLabel { background-color : #DFE0E2; }");
}

void tableColumn::leaveEvent(QEvent *)
{
    this->setStyleSheet("");
}
