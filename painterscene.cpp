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
    QGraphicsScene::mousePressEvent(event);

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

        currentPhigure = addPolygon(polygon, pen_color);
    }
    else if(phigure == Phigure::Triangle)
    {
        currentPhigure = addPolygon(QPolygonF(), pen_color);
    }
    else if(phigure == Phigure::Rhombus)
    {
        currentPhigure = addPolygon(QPolygonF(), pen_color);
    }
    else if(phigure == Phigure::Circle)
    {
        currentPhigure = addEllipse(previousPoint.x(),
                                    previousPoint.y(),
                                    0,
                                    0,
                                    pen_color);
    }
    else if(phigure == Phigure::Line)
    {
        currentPhigure = addLine(0, 0, 0, 0, pen_color);
    }
    else if(phigure == Phigure::Pen)
    {
        currentPhigure = addLine(previousPoint.x(),
                                 previousPoint.y(),
                                 previousPoint.x(),
                                 previousPoint.y(),
                                 QPen(pen_color, pen_size, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    }
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
       phigure != Phigure::Select &&
       phigure != Phigure::Cleaner)
    {
        if(currentPhigure != nullptr)
        {
            removeItem(currentPhigure);
        }
    }

    Qt::PenStyle penStyle = getCurrentPenStyle();

    QPen pen(pen_color, pen_size, penStyle, Qt::SquareCap, Qt::RoundJoin);
    QBrush brush(pen_color, phigure_fill == Border ? Qt::NoBrush : Qt::SolidPattern);

    if(pen.style() == Qt::CustomDashLine)
    {
        QVector<qreal> dashes ;
              dashes.append(5);
              dashes.append(9);

        pen.setDashPattern(dashes);
        pen.setDashOffset(0);
    }
    else if(pen.style() == Qt::DotLine)
    {
        QVector<qreal> dashes ;
              dashes.append(0.5);
              dashes.append(9);

        pen.setDashPattern(dashes);
        pen.setDashOffset(0);
    }

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

    case Phigure::Triangle:
    {
        auto width = qMax(currentPosition.x() - previousPoint.x(),
                          currentPosition.y() - previousPoint.y());

        QPolygonF polygon;

        polygon.append(QPointF(QPoint(previousPoint.x() + width / 2, previousPoint.y())));
        polygon.append(QPointF(previousPoint.x(), isShiftPressed ? previousPoint.y() + width : currentPosition.y()));
        polygon.append(QPointF(isShiftPressed ? previousPoint.x() + width : currentPosition.x(), isShiftPressed ? previousPoint.y() + width : currentPosition.y()));

        currentPhigure = addPolygon(polygon, pen, brush);

        break;
    }

    case Phigure::Rhombus:
    {
        auto width = currentPosition.x() - previousPoint.x();
        auto height = currentPosition.y() - previousPoint.y();

        if(isShiftPressed)
        {
            width = qMax(width, height);
        }

        QPolygonF polygon;

        polygon.append(QPointF(QPoint(previousPoint.x() + width / 2, previousPoint.y())));
        polygon.append(QPointF(QPoint(previousPoint.x(), isShiftPressed ? previousPoint.y() + width / 2 : currentPosition.y() - height / 2)));
        polygon.append(QPointF(QPoint(previousPoint.x() + width / 2, isShiftPressed ? previousPoint.y() + width : currentPosition.y())));
        polygon.append(QPointF(QPoint(isShiftPressed ? previousPoint.x() + width : currentPosition.x(), isShiftPressed ? previousPoint.y() + width / 2 : currentPosition.y() - height / 2)));

        currentPhigure = addPolygon(polygon, pen, brush);

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
        qreal width = pen_size * 7.2;

        auto rect = QRectF(currentPosition.x() - width / 2,
                           currentPosition.y() - width / 2,
                           width,
                           width);

        QList<QGraphicsItem*> colliding = items(rect);

        for (QGraphicsItem* item : colliding)
        {
            removeItem(item);
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
            return Qt::CustomDashLine;
        case DotLine:
            return Qt::DotLine;
    }
}

