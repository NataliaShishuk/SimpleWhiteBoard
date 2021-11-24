#include "painterscene.h"

PainterScene::PainterScene(QObject *parent) : QGraphicsScene(parent)
{
   initialize();
}

Phigure PainterScene::getPhigure()
{
    return this->phigure;
}

int PainterScene::getSize()
{
    return this->size;
}

QColor PainterScene::getColor()
{
    return this->color;
}

void PainterScene::setPhigure(Phigure phigure)
{
    this->phigure = phigure;
}

void PainterScene::setSize(int size)
{
    this->size = size;
}

void PainterScene::setColor(QColor color)
{
    this->color = color;
}

void PainterScene::initialize()
{
    dashpolyItem = addPolygon(QPolygon(),color);
    polyItem = addPolygon(QPolygon(),color);
    ellipse = addEllipse(previousPoint.x(),previousPoint.y(),0,0,color);
    dashellipse = addEllipse(previousPoint.x(),previousPoint.y(),0,0,color);
    lineItem = addLine(0,0,0,0,color);
    dashlineItem = addLine(0,0,0,0,color);
}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    previousPoint = event->scenePos();

    if(phigure == Phigure::Rectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        polyItem = addPolygon(polygon,color);
    }
    else if(phigure == Phigure::DashRectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        dashpolyItem = addPolygon(polygon, color);
    }
    else if(phigure == Phigure::Circle)
    {
        ellipse = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, color);
    }
    else if(phigure == Phigure::DashCircle)
    {
        dashellipse = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, color);
    }
    else if(phigure == Phigure::Line)
    {
        lineItem = addLine(0, 0, 0, 0, color);
    }
    else if(phigure == Phigure::DashLine)
    {
        dashlineItem = addLine(0, 0, 0, 0, color);
    }
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
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
                           QPen(color,size, Qt::SolidLine, Qt::RoundCap));

        break;
    }

    case Phigure::DashLine:
    {
        removeItem(dashlineItem);

        dashlineItem = addLine(previousPoint.x(),
                               previousPoint.y(),
                               currentPosition.x(),
                               currentPosition.y(),
                               QPen(color, size, Qt::DashLine, Qt::RoundCap));

        break;
    }

    case Phigure::Circle:
    {
        removeItem(ellipse);

        ellipse = addEllipse(previousPoint.x(),
                             previousPoint.y(),
                             point.x(),
                             isShiftPressed ? point.x() : point.y(),
                             QPen(color, size, Qt::SolidLine, Qt::RoundCap));

        break;
    }

    case Phigure::DashCircle:
    {
        removeItem(dashellipse);

        dashellipse = addEllipse(previousPoint.x(),
                                 previousPoint.y(),
                                 point.x(),
                                 isShiftPressed ? point.x() : point.y(),
                                 QPen(color, size, Qt::DashLine, Qt::RoundCap));

        break;
    }

    case Phigure::Pen:
    {
        addLine(previousPoint.x(),
                previousPoint.y(),
                currentPosition.x(),
                currentPosition.y(),
                QPen(color, size, Qt::SolidLine, Qt::RoundCap));

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
                              QPen(color, size, Qt::SolidLine));

        break;
    }

    case Phigure::DashRectangle:
    {
        removeItem(dashpolyItem);

        dashpolyItem = addPolygon(QPolygonF(QRectF(previousPoint.x(),
                                                   previousPoint.y(),
                                                   point.x(),
                                                   isShiftPressed ? point.x() : point.y())),
                                  QPen(color, size, Qt::DashLine));

        break;
    }

    case Phigure::Cleaner:
    {
        cleanerLine = addLine(previousPoint.x(),
                              previousPoint.y(),
                              currentPosition.x(),
                              currentPosition.y(),
                              QPen(QColor(255, 255, 255, 255), size * 2, Qt::SolidLine, Qt::RoundCap));

        QList<QGraphicsItem*> colliding = cleanerLine->collidingItems();

        for(auto i = 0; i < colliding.size(); i++)
        {
            removeItem(colliding.at(i));
        }

        previousPoint = currentPosition;

        break;
    }

    }
}

