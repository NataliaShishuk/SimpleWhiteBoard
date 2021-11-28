#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>

enum Phigure
{
    Pen             = 0,
    Rectangle       = 1,
    Triangle        = 2,
    Line            = 3,
    Circle          = 5,
    Rhombus         = 6,
    Cleaner         = 7,
    Select          = 8
};

enum PhigureLine
{
    SolidLine       = 0,
    DashedLine      = 1,
    DotLine         = 2
};

enum PhigureFill
{
    Border          = 0,
    Full            = 1
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
    PhigureLine getPhigureLine();
    PhigureFill getPhigureFill();

    void setPhigure(Phigure phigure);
    void setPenSize(int size);
    void setPenColor(QColor color);
    void setPhigureLine(PhigureLine style);
    void setPhigureFill(PhigureFill fill);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    int pen_size;
    QColor pen_color;
    Phigure phigure;
    PhigureLine phigure_line;
    PhigureFill phigure_fill;

    QPointF previousPoint;

    QGraphicsItem* currentPhigure;

    Qt::PenStyle getCurrentPenStyle();
};

#endif
