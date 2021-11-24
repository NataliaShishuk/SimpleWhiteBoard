#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

enum Phigure {
    Pen             = 0,
    Rectangle       = 1,
    DashRectangle   = 2,
    Line            = 3,
    DashLine        = 4,
    Circle          = 5,
    DashCircle      = 6,
    Cleaner         = 7
};

class PainterScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PainterScene(QObject *parent = nullptr);

    void initialize();

    Phigure getPhigure();
    int getPenSize();
    QColor getPenColor();

    void setPhigure(Phigure phigure);
    void setPenSize(int size);
    void setPenColor(QColor color);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int pen_size;
    QColor pen_color;
    Phigure phigure;

    QPointF previousPoint;
    QGraphicsPolygonItem* polyItem;
    QGraphicsEllipseItem* ellipse;
    QGraphicsLineItem* lineItem;

    QGraphicsPolygonItem* dashpolyItem;
    QGraphicsEllipseItem* dashellipse;
    QGraphicsLineItem* dashlineItem;
    QGraphicsPolygonItem* cleanerLine;
};

#endif
