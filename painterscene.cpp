#include "painterscene.h"

PainterScene::PainterScene(QObject *parent)
    : QGraphicsScene(parent),
      phigure(Pen),
      pen_size(2),
      pen_color(Qt::black)
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

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return QGraphicsScene::mousePressEvent(event);
    }

    previousPoint = event->scenePos();

    if(phigure == Phigure::Rectangle ||
       phigure == Phigure::DashRectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        currentPhigure = addPolygon(polygon, pen_color);
    }
    else if(phigure == Phigure::Circle ||
            phigure == Phigure::DashCircle)
    {
        currentPhigure = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, pen_color);
    }
    else if(phigure == Phigure::Line ||
            phigure == Phigure::DashLine)
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

    switch (phigure)
    {

    case Phigure::Line:
    {
        currentPhigure = addLine(previousPoint.x(),
                           previousPoint.y(),
                           currentPosition.x(),
                           currentPosition.y(),
                           QPen(pen_color,pen_size, Qt::SolidLine, Qt::RoundCap));

        break;
    }

    case Phigure::DashLine:
    {
        currentPhigure = addLine(previousPoint.x(),
                               previousPoint.y(),
                               currentPosition.x(),
                               currentPosition.y(),
                               QPen(pen_color, pen_size, Qt::DashLine, Qt::RoundCap));

        break;
    }

    case Phigure::Circle:
    {
        currentPhigure = addEllipse(previousPoint.x(),
                             previousPoint.y(),
                             point.x(),
                             isShiftPressed ? point.x() : point.y(),
                             QPen(pen_color, pen_size, Qt::SolidLine, Qt::RoundCap));

        break;
    }

    case Phigure::DashCircle:
    {
        currentPhigure = addEllipse(previousPoint.x(),
                                 previousPoint.y(),
                                 point.x(),
                                 isShiftPressed ? point.x() : point.y(),
                                 QPen(pen_color, pen_size, Qt::DashLine, Qt::RoundCap));

        break;
    }

    case Phigure::Pen:
    {
        currentPhigure = addLine(previousPoint.x(),
                previousPoint.y(),
                currentPosition.x(),
                currentPosition.y(),
                QPen(pen_color, pen_size, Qt::SolidLine, Qt::RoundCap));

        previousPoint = currentPosition;

        break;
    }

    case Phigure::Rectangle:
    {
        currentPhigure = addPolygon(QPolygonF(QRectF(previousPoint.x(),
                                               previousPoint.y(),
                                               point.x(),
                                               isShiftPressed ? point.x() : point.y())),
                              QPen(pen_color, pen_size, Qt::SolidLine));

        break;
    }

    case Phigure::DashRectangle:
    {
        currentPhigure = addPolygon(QPolygonF(QRectF(previousPoint.x(),
                                                   previousPoint.y(),
                                                   point.x(),
                                                   isShiftPressed ? point.x() : point.y())),
                                  QPen(pen_color, pen_size, Qt::DashLine));

        break;
    }

    case Phigure::Cleaner:
    {
        currentPhigure = addPolygon(QPolygonF(QRectF(currentPosition.x() - pen_size,
                                                   currentPosition.y() - pen_size,
                                                   pen_size * 2,
                                                   pen_size * 2)),
                                  QPen(Qt::white, pen_size, Qt::SolidLine, Qt::RoundCap));

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
            currentPhigure->setFlag(QGraphicsItem::ItemIsFocusScope);
        }
    }
}

