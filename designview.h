#ifndef DESIGNVIEW_H
#define DESIGNVIEW_H

#include <QDialog>

#include "database.h"
#include "tableerdwidget.h"

namespace Ui {
class DesignView;
}

class DesignView : public QDialog
{
    Q_OBJECT

public:
    explicit DesignView(QWidget *parent = nullptr);
    ~DesignView();

    QSharedPointer<database> currentDatabase;

    QStringList tables;
    QStringList queries;

    QVector<TableERDWidget *> renderedERDWidgets;
protected slots:
    void showEvent(QShowEvent * event);

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::DesignView * ui;
};

#endif // DESIGNVIEW_H
