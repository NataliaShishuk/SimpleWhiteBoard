#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QColor>
#include <QGraphicsPolygonItem>

#include <vector>

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
    ~PainterScene();
    void initialize();

    bool CleanerVariant = false;

    Phigure getPhigure();
    int getSize();
    QColor getColor();

    void setPhigure(Phigure phigure);
    void setSize(int size);
    void setColor(QColor color);

private:
    int size;
    QColor color;
    Phigure phigure;

    QPointF     previousPoint;
    QGraphicsPolygonItem* polyItem;
    QGraphicsEllipseItem* ellipse;
    QGraphicsLineItem* lineItem;

    QGraphicsPolygonItem* dashpolyItem;
    QGraphicsEllipseItem* dashellipse;
    QGraphicsLineItem* dashlineItem;
    QGraphicsLineItem* cleanerLine;

    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif