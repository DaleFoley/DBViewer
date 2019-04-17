#ifndef TABLEERDWIDGET_H
#define TABLEERDWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>

#include <QEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPen>
#include <QLabel>
#include <QPixmap>

#include <tablecolumn.h>

class TableERDWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableERDWidget(QWidget * parent, const QString &tableName, QVector<QString> columns);

    QString TableName;

    QVector<tableColumn *> columns;
    QVector<QString> columnNames;

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;
    void mouseMoveEvent(QMouseEvent * event) override;

private:
    bool isLeftResize;
    bool isRightResize;
    bool isTopResize;
    bool isBottomResize;
    bool isTopLeftDiagonalResize;
    bool isTopRightDiagonalResize;
    bool isBottomRightDiagonalResize;
    bool isBottomLeftDiagonalResize;

    bool isWidgetBeingMoved;
    bool isWidgetBeingResized;

    QPen blackPen;
    QPen silverPen;

    QBrush whiteBrush;

    QLabel * primaryKeyLabel;
    QLabel * foreignKeyLabel;

    QPixmap primaryKey;
    QPixmap foreignKey;

    bool isResizeWidgetOccuring();

    void drawColumns();
};

#endif // TABLEERDWIDGET_H
