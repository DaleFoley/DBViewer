#ifndef DESIGNSELECTION_H
#define DESIGNSELECTION_H

#include <QDialog>

namespace Ui {
class DesignSelection;
}

class DesignSelection : public QDialog
{
    Q_OBJECT

public:
    explicit DesignSelection(QWidget *parent = nullptr);
    ~DesignSelection();

private:
    Ui::DesignSelection *ui;
};

#endif // DESIGNSELECTION_H
