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
    int getSize();
    QColor getColor();

    void setPhigure(Phigure phigure);
    void setSize(int size);
    void setColor(QColor color);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int size;
    QColor color;
    Phigure phigure;

    QPointF previousPoint;
    QGraphicsPolygonItem* polyItem;
    QGraphicsEllipseItem* ellipse;
    QGraphicsLineItem* lineItem;

    QGraphicsPolygonItem* dashpolyItem;
    QGraphicsEllipseItem* dashellipse;
    QGraphicsLineItem* dashlineItem;
    QGraphicsLineItem* cleanerLine;
};

#endif
