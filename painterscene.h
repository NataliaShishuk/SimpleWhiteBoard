#ifndef PAINTERSCENE_H
#define PAINTERSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>
#include <QUndoStack>

enum Phigure
{
    Pen             = 0,
    Rectangle       = 1,
    Triangle        = 2,
    Line            = 3,
    Circle          = 4,
    Rhombus         = 5,
    Parallelogram   = 6,
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

    void clearScene();

    bool hasChanged() {return is_changed && !items().empty();}

    // undo-redo
    QUndoStack* undoStack;

    QGraphicsItem *createCopy(QGraphicsItem* item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    int pen_size;
    QColor pen_color;
    Phigure phigure;
    PhigureLine phigure_line;
    PhigureFill phigure_fill;

    QPointF previousPosition;

    QGraphicsItem* currentPhigure;
    QPainterPath path;

    Qt::PenStyle getCurrentPenStyle();

    bool is_changed;
};

#endif
