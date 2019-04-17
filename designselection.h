#ifndef DESIGNSELECTION_H
#define DESIGNSELECTION_H

#include <QDialog>
#include "database.h"
#include "designview.h"

namespace Ui {
class DesignSelection;
}

class DesignSelection : public QDialog
{
    Q_OBJECT

public:
    explicit DesignSelection(QWidget * parent = nullptr);
    ~DesignSelection();

    QSharedPointer<database> currentDatabase;

protected slots:
    void showEvent(QShowEvent * event);

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonAdd_clicked();

private:
    Ui::DesignSelection *ui;
};

#endif // DESIGNSELECTION_H
