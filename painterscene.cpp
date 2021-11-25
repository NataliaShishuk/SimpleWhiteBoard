#include "painterscene.h"

PainterScene::PainterScene(QObject *parent)
    : QGraphicsScene(parent),
      phigure(Pen),
      pen_size(2),
      pen_color(Qt::black)
{
   initialize();

   setBackgroundBrush(Qt::white);
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

void PainterScene::initialize()
{
    dashpolyItem = addPolygon(QPolygon(),pen_color);
    polyItem = addPolygon(QPolygon(),pen_color);
    ellipse = addEllipse(previousPoint.x(),previousPoint.y(),0,0,pen_color);
    dashellipse = addEllipse(previousPoint.x(),previousPoint.y(),0,0,pen_color);
    lineItem = addLine(0,0,0,0,pen_color);
    dashlineItem = addLine(0,0,0,0,pen_color);
}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    previousPoint = event->scenePos();

    if(phigure == Phigure::Rectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        polyItem = addPolygon(polygon,pen_color);
    }
    else if(phigure == Phigure::DashRectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        dashpolyItem = addPolygon(polygon, pen_color);
    }
    else if(phigure == Phigure::Circle)
    {
        ellipse = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, pen_color);
    }
    else if(phigure == Phigure::DashCircle)
    {
        dashellipse = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, pen_color);
    }
    else if(phigure == Phigure::Line)
    {
        lineItem = addLine(0, 0, 0, 0, pen_color);
    }
    else if(phigure == Phigure::DashLine)
    {
        dashlineItem = addLine(0, 0, 0, 0, pen_color);
    }
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        return;
    }

    auto currentPosition = event->scenePos();
    auto point = currentPosition - previousPoint;

    auto isShiftPressed = event->modifiers() == Qt::ShiftModifier;

    switch (phigure)
    {

    case Phigure::Line:
    {
        removeItem(lineItem);

        lineItem = addLine(previousPoint.x(),
                           previousPoint.y(),
                           currentPosition.x(),
                           currentPosition.y(),
                           QPen(pen_color,pen_size, Qt::SolidLine, Qt::RoundCap));

        break;
    }

    case Phigure::DashLine:
    {
        removeItem(dashlineItem);

        dashlineItem = addLine(previousPoint.x(),
                               previousPoint.y(),
                               currentPosition.x(),
                               currentPosition.y(),
                               QPen(pen_color, pen_size, Qt::DashLine, Qt::RoundCap));

        break;
    }

    case Phigure::Circle:
    {
        removeItem(ellipse);

        ellipse = addEllipse(previousPoint.x(),
                             previousPoint.y(),
                             point.x(),
                             isShiftPressed ? point.x() : point.y(),
                             QPen(pen_color, pen_size, Qt::SolidLine, Qt::RoundCap));

        break;
    }

    case Phigure::DashCircle:
    {
        removeItem(dashellipse);

        dashellipse = addEllipse(previousPoint.x(),
                                 previousPoint.y(),
                                 point.x(),
                                 isShiftPressed ? point.x() : point.y(),
                                 QPen(pen_color, pen_size, Qt::DashLine, Qt::RoundCap));

        break;
    }

    case Phigure::Pen:
    {
        addLine(previousPoint.x(),
                previousPoint.y(),
                currentPosition.x(),
                currentPosition.y(),
                QPen(pen_color, pen_size, Qt::SolidLine, Qt::RoundCap));

        previousPoint = currentPosition;

        break;
    }

    case Phigure::Rectangle:
    {
        removeItem(polyItem);

        polyItem = addPolygon(QPolygonF(QRectF(previousPoint.x(),
                                               previousPoint.y(),
                                               point.x(),
                                               isShiftPressed ? point.x() : point.y())),
                              QPen(pen_color, pen_size, Qt::SolidLine));

        break;
    }

    case Phigure::DashRectangle:
    {
        removeItem(dashpolyItem);

        dashpolyItem = addPolygon(QPolygonF(QRectF(previousPoint.x(),
                                                   previousPoint.y(),
                                                   point.x(),
                                                   isShiftPressed ? point.x() : point.y())),
                                  QPen(pen_color, pen_size, Qt::DashLine));

        break;
    }

    case Phigure::Cleaner:
    {
        cleanerLine = addPolygon(QPolygonF(QRectF(currentPosition.x() - pen_size,
                                                   currentPosition.y() - pen_size,
                                                   pen_size * 2,
                                                   pen_size * 2)),
                                  QPen(Qt::white, pen_size, Qt::SolidLine, Qt::RoundCap));

        QList<QGraphicsItem*> colliding = cleanerLine->collidingItems(Qt::IntersectsItemBoundingRect);

        for(auto i = 0; i < colliding.size(); i++)
        {
            removeItem(colliding.at(i));
        }

        previousPoint = currentPosition;

        break;
    }

    }
}

