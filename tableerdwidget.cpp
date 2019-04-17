#include "tableerdwidget.h"

//TODO: Credit icon authors.
TableERDWidget::TableERDWidget(QWidget * parent, const QString &tableName, QVector<QString> columns) : QWidget(parent)
{
    if(columns.count() <= 0)
    {
        throw "Require names of columns.";
    }

    if(tableName == "")
    {
        throw "Require a table name.";
    }

    this->TableName = tableName;

    this->setMouseTracking(true);
    this->setMinimumSize(150, 200);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    this->blackPen = QPen(QBrush(Qt::black), qreal(2));
    this->silverPen = QPen(QBrush(QColor("silver")), qreal(5));

    this->whiteBrush = QBrush(QColor("white"));

    QString pathResourcePrimaryKey = ":/icons/icons/pk_key16x16.png";
    QString pathResourceForeignKey = ":/icons/icons/fk_key16x16.png";

    if(!this->primaryKey.load(pathResourcePrimaryKey))
    {
        throw "Failed to locate required resource [" + pathResourcePrimaryKey + "]";
    }

    if(!this->foreignKey.load(pathResourceForeignKey))
    {
        throw "Failed to locate required resource [" + pathResourceForeignKey + "]";
    }

    for(QString column : columns)
    {
        tableColumn * columnToRender = new tableColumn(this);
        columnToRender->setText(column);
        columnToRender->setIndent(30);

        columnToRender->isPrimaryKey = false;
        columnToRender->isForeignKey = false;

        this->columns.push_back(columnToRender);
    }

    this->primaryKeyLabel = new QLabel(this);
    this->foreignKeyLabel = new QLabel(this);
}

void TableERDWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(this->silverPen);
    painter.setBrush(this->whiteBrush);
    painter.drawRoundedRect(0, 0, this->width(), this->height(), qreal(15), qreal(15));

    QString tableName = this->TableName;

    if(tableName != "")
    {
        int widgetX = 0;
        int widgetY = 0;
        int widgetHeight = this->height();
        int widgetWidth = this->width();

        int tableNameRectX = widgetX + 2;
        int tableNameRectY = widgetY + 10;
        int tableNameRectWidth = widgetWidth - 5;
        int tableNameRectHeight = widgetHeight * 10 / 100;

        const QRect rectTableName = QRect(tableNameRectX, tableNameRectY, tableNameRectWidth, tableNameRectHeight);

        painter.setPen(blackPen);

        QFontMetrics tableNameFontMetrics = painter.fontMetrics();
        QString tableNameElided = tableNameFontMetrics.elidedText(this->TableName, Qt::TextElideMode::ElideRight, tableNameRectWidth);

        painter.drawText(rectTableName, Qt::AlignCenter, tableNameElided);

        int lineX = tableNameRectX + 5;
        int lineY = tableNameRectHeight + 25;

        painter.drawLine(lineX, lineY, tableNameRectWidth, lineY);

        int columnYOffset = lineY + 10;

        for(tableColumn * column : this->columns)
        {
            column->setGeometry(tableNameRectX, columnYOffset, tableNameRectWidth - 5, 20);

            if(column->isPrimaryKey)
            {
                this->primaryKeyLabel->setGeometry(tableNameRectX + 5, columnYOffset, 16, 16);

                this->primaryKey = this->primaryKey.scaled(this->primaryKeyLabel->size(), Qt::KeepAspectRatio);
                this->primaryKeyLabel->setPixmap(this->primaryKey);
                this->primaryKeyLabel->show();
            }
            else if(column->isForeignKey)
            {
                this->foreignKeyLabel->setGeometry(tableNameRectX + 5, columnYOffset, 16, 16);

                this->foreignKey = this->foreignKey.scaled(this->foreignKeyLabel->size(), Qt::KeepAspectRatio);
                this->foreignKeyLabel->setPixmap(this->foreignKey);
                this->foreignKeyLabel->show();
            }

            columnYOffset += column->height();
        }
    }
}

void TableERDWidget::resizeEvent(QResizeEvent *)
{

}

void TableERDWidget::mousePressEvent(QMouseEvent *)
{
    if(this->isResizeWidgetOccuring())
    {
        this->setMouseTracking(false);
    }
}

void TableERDWidget::mouseReleaseEvent(QMouseEvent *)
{
    this->setCursor(Qt::CursorShape::ArrowCursor);
    this->setMouseTracking(true);
}

void TableERDWidget::leaveEvent(QEvent *)
{
    this->setCursor(Qt::CursorShape::ArrowCursor);
    this->setMouseTracking(true);
}

void TableERDWidget::mouseMoveEvent(QMouseEvent * event)
{
    QPoint currentMousePosition = this->parentWidget()->mapFromGlobal(event->globalPos());
    QPoint parentFormBounds = QPoint(this->parentWidget()->width(), this->parentWidget()->height());

    int mouseX = currentMousePosition.x();
    int mouseY = currentMousePosition.y();

    int widgetX = this->x();
    int widgetY = this->y();

    int widgetWidth = this->width();
    int widgetHeight = this->height();

    if(event->buttons() == Qt::LeftButton && this->isResizeWidgetOccuring() == false)
    {
        this->isWidgetBeingMoved = true;

        int mouseOffsetX = mouseX - widgetWidth / 2;
        int mouseOffsetY = mouseY - widgetHeight / 2;

        if(mouseX < parentFormBounds.x() &&
            mouseY < parentFormBounds.y() &&
            mouseX > 0 &&
            mouseY > 0)
        {
            this->move(mouseOffsetX, mouseOffsetY);
            this->setCursor(Qt::CursorShape::OpenHandCursor);
        }
        else
        {
            this->setCursor(Qt::CursorShape::ArrowCursor);
        }
    }
    else
    {
        this->isWidgetBeingMoved = false;
    }

    if(this->isWidgetBeingMoved == false && event->buttons() != Qt::LeftButton)
    {
//        this->isLeftResize = mouseX >= widgetX - 10 && mouseX < widgetX + 15;
//        this->isTopResize = mouseY >= widgetY - 10 && mouseY < widgetY + 15;
//        this->isTopLeftDiagonalResize = mouseX >= widgetX && mouseX < widgetX + 20 && mouseY >= widgetY && mouseY < widgetY + 20;
//        this->isTopRightDiagonalResize = mouseX <= widgetX + widgetWidth && mouseX > widgetX + widgetWidth - 20 && mouseY >= widgetY && mouseY < widgetY + 20;
//        this->isBottomLeftDiagonalResize = mouseX >= widgetX && mouseX < widgetX + 20 && mouseY <= widgetY + widgetHeight && mouseY > widgetY + widgetHeight - 20;

        this->isBottomRightDiagonalResize = mouseX <= widgetX + widgetWidth && mouseX > widgetX + widgetWidth - 20 && mouseY <= widgetY + widgetHeight && mouseY > widgetY + widgetHeight - 20;
        this->isRightResize = mouseX <= widgetX + widgetWidth + 10 && mouseX > (widgetX + widgetWidth - 15);
        this->isBottomResize = mouseY <= widgetY + widgetHeight + 10 && mouseY > (widgetY + widgetHeight - 15);

        if(isBottomRightDiagonalResize)
        {
            this->setCursor(Qt::CursorShape::SizeFDiagCursor);
        }
        else if(isRightResize)
        {
            this->setCursor(Qt::CursorShape::SizeHorCursor);
        }
        else if(isBottomResize)
        {
            this->setCursor(Qt::CursorShape::SizeVerCursor);
        }
        else
        {
            this->setCursor(Qt::CursorShape::ArrowCursor);
        }
    }

    //TODO: Figure out maths on resizing from the left of the widget.
    if(isBottomRightDiagonalResize && event->buttons() == Qt::LeftButton)
    {
        this->resize(mouseX - widgetX, mouseY - widgetY);
    }
    else if(isRightResize && event->buttons() == Qt::LeftButton)
    {
        this->resize(mouseX - widgetX, widgetHeight);
    }
    else if (isBottomResize && event->buttons() == Qt::LeftButton)
    {
        this->resize(widgetWidth, mouseY - widgetY);
    }
}

bool TableERDWidget::isResizeWidgetOccuring()
{
//    return this->isLeftResize ||
//            this->isRightResize ||
//            this->isTopResize ||
//            this->isBottomResize ||
//            this->isTopLeftDiagonalResize ||
//            this->isTopRightDiagonalResize ||
//            this->isBottomRightDiagonalResize ||
//            this->isBottomLeftDiagonalResize;

    return this->isRightResize ||
           this->isBottomResize ||
           this->isBottomRightDiagonalResize;

}
