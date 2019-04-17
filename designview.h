#ifndef DESIGNVIEW_H
#define DESIGNVIEW_H

#include <QWidget>

namespace Ui {
class DesignView;
}

class DesignView : public QWidget
{
    Q_OBJECT

public:
    explicit DesignView(QWidget *parent = nullptr);
    ~DesignView();

private:
    Ui::DesignView *ui;
};

#endif // DESIGNVIEW_H
