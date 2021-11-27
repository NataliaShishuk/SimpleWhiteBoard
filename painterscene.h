#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

enum Phigure
{
    Pen             = 0,
    Rectangle       = 1,
    DashRectangle   = 2,
    Line            = 3,
    DashLine        = 4,
    Circle          = 5,
    DashCircle      = 6,
    Cleaner         = 7,
    Select          = 8
};

enum PenStyle
{
    SolidLine       = 0,
    DashedLine      = 1,
    DotLine         = 2
};

class PainterScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PainterScene(QObject *parent = nullptr);
    ~PainterScene();

    Phigure getPhigure();
    int getPenSize();
    QColor getPenColor();

    void setPhigure(Phigure phigure);
    void setPenSize(int size);
    void setPenColor(QColor color);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    int pen_size;
    QColor pen_color;
    Phigure phigure;

    QPointF previousPoint;

    QGraphicsItem* currentPhigure;
};

#endif
