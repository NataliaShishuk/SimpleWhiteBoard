#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

enum Phigure
{
    Pen             = 0,
    Rectangle       = 1,
    Line            = 3,
    Circle          = 5,
    Cleaner         = 7,
    Select          = 8
};

enum PhigureStyle
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
    PhigureStyle getPenStyle();

    void setPhigure(Phigure phigure);
    void setPenSize(int size);
    void setPenColor(QColor color);
    void setPenStyle(PhigureStyle style);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    int pen_size;
    QColor pen_color;
    Phigure phigure;
    PhigureStyle pen_style;

    QPointF previousPoint;

    QGraphicsItem* currentPhigure;

    Qt::PenStyle getCurrentPenStyle();
};

#endif
