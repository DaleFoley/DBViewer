#ifndef TABLECOLUMN_H
#define TABLECOLUMN_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class tableColumn : public QLabel
{
public:
    explicit tableColumn(QWidget * parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());

    bool isPrimaryKey;
    bool isForeignKey;

protected:
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;

};

#endif // TABLECOLUMN_H
