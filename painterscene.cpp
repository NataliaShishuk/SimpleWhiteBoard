#include "painterscene.h"

PainterScene::PainterScene(QObject *parent)
    : QGraphicsScene(parent),
      phigure(Pen),
      pen_size(2),
      pen_color(Qt::black),
      phigure_line(SolidLine),
      phigure_fill(Border)
{
   setBackgroundBrush(Qt::white);
   setItemIndexMethod(QGraphicsScene::NoIndex);
}

PainterScene::~PainterScene()
{

}

Phigure PainterScene::getPhigure()
{
    return this->phigure;
}

int PainterScene::getPenSize()
{
    return this->pen_size;
}

QColor PainterScene::getPenColor()
{
    return this->pen_color;
}

PhigureLine PainterScene::getPhigureLine()
{
    return this->phigure_line;
}

PhigureFill PainterScene::getPhigureFill()
{
    return this->phigure_fill;
}

void PainterScene::setPhigure(Phigure phigure)
{
    this->phigure = phigure;
}

void PainterScene::setPenSize(int size)
{
    this->pen_size = size;
}

void PainterScene::setPenColor(QColor color)
{
    this->pen_color = color;
}

void PainterScene::setPhigureLine(PhigureLine style)
{
    this->phigure_line = style;
}

void PainterScene::setPhigureFill(PhigureFill fill)
{
    this->phigure_fill = fill;
}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return QGraphicsScene::mousePressEvent(event);
    }

    previousPoint = event->scenePos();

    if(phigure == Phigure::Rectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        currentPhigure = addPolygon(polygon, pen_color);
    }
    else if(phigure == Phigure::Circle)
    {
        currentPhigure = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, pen_color);
    }
    else if(phigure == Phigure::Line)
    {
        currentPhigure = addLine(0, 0, 0, 0, pen_color);
    }

    return QGraphicsScene::mousePressEvent(event);
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        return QGraphicsScene::mouseMoveEvent(event);
    }

    auto currentPosition = event->scenePos();
    auto point = currentPosition - previousPoint;

    auto isShiftPressed = event->modifiers() == Qt::ShiftModifier;

    if(phigure != Phigure::Pen &&
       phigure != Phigure::Select)
    {
        if(currentPhigure != nullptr)
        {
            removeItem(currentPhigure);
        }
    }

    Qt::PenStyle penStyle = getCurrentPenStyle();

    QPen pen(pen_color, pen_size, penStyle, Qt::SquareCap);
    QBrush brush(pen_color, phigure_fill == Border ? Qt::NoBrush : Qt::SolidPattern);

    switch (phigure)
    {

    case Phigure::Line:
    {
        currentPhigure = addLine(previousPoint.x(),
                           previousPoint.y(),
                           currentPosition.x(),
                           currentPosition.y(),
                           pen);

        break;
    }

    case Phigure::Circle:
    {
        currentPhigure = addEllipse(previousPoint.x(),
                             previousPoint.y(),
                             point.x(),
                             isShiftPressed ? point.x() : point.y(),
                             pen,
                             brush);

        break;
    }

    case Phigure::Pen:
    {
        currentPhigure = addLine(previousPoint.x(),
                previousPoint.y(),
                currentPosition.x(),
                currentPosition.y(),
                pen);

        previousPoint = currentPosition;

        break;
    }

    case Phigure::Rectangle:
    {
        currentPhigure = addPolygon(QPolygonF(QRectF(previousPoint.x(),
                                    previousPoint.y(),
                                    point.x(),
                                    isShiftPressed ? point.x() : point.y())),
                                    pen,
                                    brush);

        break;
    }

    case Phigure::Cleaner:
    {
        pen.setColor(Qt::white);

        currentPhigure = addPolygon(QPolygonF(QRectF(currentPosition.x() - pen_size,
                                    currentPosition.y() - pen_size,
                                    pen_size * 2,
                                    pen_size * 2)),
                                    pen);

        QList<QGraphicsItem*> colliding = currentPhigure->collidingItems(Qt::IntersectsItemBoundingRect);

        for(auto i = 0; i < colliding.size(); i++)
        {
            removeItem(colliding.at(i));
        }

        previousPoint = currentPosition;

        break;
    }

    case Phigure::Select:
    {
        return QGraphicsScene::mouseMoveEvent(event);
    }

    }

    if(phigure != Phigure::Pen)
    {
        if(currentPhigure != nullptr)
        {
            currentPhigure->setFlag(QGraphicsItem::ItemIsMovable);
        }
    }
}

Qt::PenStyle PainterScene::getCurrentPenStyle()
{
    switch (phigure_line)
    {
        case SolidLine:
            return Qt::SolidLine;
        case DashedLine:
            return Qt::DashLine;
        case DotLine:
            return Qt::DotLine;
    }
}

