#include "painterscene.h"
#include "commands.h"

PainterScene::PainterScene(QObject *parent)
    : QGraphicsScene(parent),
      phigure(Pen),
      pen_size(2),
      pen_color(Qt::black),
      phigure_line(SolidLine),
      phigure_fill(Border)
{
    this->currentPhigure = nullptr;
    this->undoStack = new QUndoStack();

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

void PainterScene::clearScene()
{
    QList<QGraphicsItem*> allItems = items();

    if(allItems.size() == 0)
    {
        return;
    }

    undoStack->push(new ClearSceneCommand(this));

    clear();
}

QGraphicsItem *PainterScene::createCopy(QGraphicsItem *item)
{
    if(item == nullptr)
    {
        return nullptr;
    }

    // QGraphicsRectItem
    if(item->type() == QGraphicsRectItem::Type)
    {
        QGraphicsRectItem* rectItem = (QGraphicsRectItem*)item;
        QGraphicsRectItem* copy = new QGraphicsRectItem(rectItem->parentItem());

        copy->setRect(rectItem->rect());
        copy->setPen(rectItem->pen());
        copy->setBrush(rectItem->brush());

        return copy;
    }

    // QGraphicsPolygonItem
    if(item->type() == QGraphicsPolygonItem::Type)
    {
        QGraphicsPolygonItem* polygonItem = (QGraphicsPolygonItem*)item;
        QGraphicsPolygonItem* copy = new QGraphicsPolygonItem(polygonItem->parentItem());

        copy->setPolygon(polygonItem->polygon());
        copy->setPen(polygonItem->pen());
        copy->setBrush(polygonItem->brush());

        return copy;
    }

    // QGraphicsLineItem
    if(item->type() == QGraphicsLineItem::Type)
    {
        QGraphicsLineItem* lineItem = (QGraphicsLineItem*)item;
        QGraphicsLineItem* copy = new QGraphicsLineItem(lineItem->parentItem());

        copy->setLine(lineItem->line());
        copy->setPen(lineItem->pen());

        return copy;
    }

    // QGraphicsEllipseItem
    if(item->type() == QGraphicsEllipseItem::Type)
    {
        QGraphicsEllipseItem* ellipseItem = (QGraphicsEllipseItem*)item;
        QGraphicsEllipseItem* copy = new QGraphicsEllipseItem(ellipseItem->parentItem());

        copy->setRect(ellipseItem->rect());
        copy->setStartAngle(ellipseItem->startAngle());
        copy->setSpanAngle(ellipseItem->spanAngle());
        copy->setPen(ellipseItem->pen());
        copy->setBrush(ellipseItem->brush());

        return copy;
    }

    // QGraphicsPathItem
    if(item->type() == QGraphicsPathItem::Type)
    {
        QGraphicsPathItem* pathItem = (QGraphicsPathItem*)item;
        QGraphicsPathItem* copy = new QGraphicsPathItem(pathItem->parentItem());

        copy->setPath(pathItem->path());
        copy->setPen(pathItem->pen());
        copy->setBrush(pathItem->brush());

        return copy;
    }

    return nullptr;
}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    previousPosition = event->scenePos();

    if(phigure == Phigure::Pen)
    {
        currentPhigure = addLine(previousPosition.x(),
                                 previousPosition.y(),
                                 previousPosition.x(),
                                 previousPosition.y(),
                                 QPen(pen_color, pen_size, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
    }
    else if(phigure == Phigure::Cleaner)
    {
        qreal width = pen_size * 7.2;

        auto rect = QRectF(previousPosition.x() - width / 2,
                           previousPosition.y() - width / 2,
                           width,
                           width);

        QList<QGraphicsItem*> colliding = items(rect);

        for (QGraphicsItem* item : colliding)
        {
            undoStack->push(new RemoveSceneItemCommand(this, item));

            removeItem(item);
        }
    }
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if (!(event->buttons() & Qt::LeftButton))
    {
        return;
    }

    auto currentPosition = event->scenePos();
    auto point = currentPosition - previousPosition;

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

    QPen pen(pen_color, pen_size, penStyle, Qt::SquareCap, Qt::MiterJoin);
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
        currentPhigure = addLine(previousPosition.x(),
                                 previousPosition.y(),
                                 currentPosition.x(),
                                 currentPosition.y(),
                                 pen);

        break;
    }

    case Phigure::Circle:
    {
        currentPhigure = addEllipse(previousPosition.x(),
                                    previousPosition.y(),
                                    point.x(),
                                    isShiftPressed ? point.x() : point.y(),
                                    pen,
                                    brush);

        break;
    }

    case Phigure::Triangle:
    {
        auto width = currentPosition.x() - previousPosition.x();

        QPolygonF polygon;

        polygon.append(QPointF(QPoint(currentPosition.x() - width / 2, previousPosition.y())));
        polygon.append(QPointF(previousPosition.x(), isShiftPressed ? previousPosition.y() + width : currentPosition.y()));
        polygon.append(QPointF(isShiftPressed ? previousPosition.x() + width : currentPosition.x(), isShiftPressed ? previousPosition.y() + width : currentPosition.y()));

        currentPhigure = addPolygon(polygon, pen, brush);

        break;
    }

    case Phigure::Rhombus:
    {
        auto width = currentPosition.x() - previousPosition.x();
        auto height = currentPosition.y() - previousPosition.y();

        if(isShiftPressed)
        {
            width = qMax(width, height);
        }

        QPolygonF polygon;

        polygon.append(QPointF(QPoint(previousPosition.x() + width / 2, previousPosition.y())));
        polygon.append(QPointF(QPoint(previousPosition.x(), isShiftPressed ? previousPosition.y() + width / 2 : currentPosition.y() - height / 2)));
        polygon.append(QPointF(QPoint(previousPosition.x() + width / 2, isShiftPressed ? previousPosition.y() + width : currentPosition.y())));
        polygon.append(QPointF(QPoint(isShiftPressed ? previousPosition.x() + width : currentPosition.x(), isShiftPressed ? previousPosition.y() + width / 2 : currentPosition.y() - height / 2)));

        currentPhigure = addPolygon(polygon, pen, brush);

        break;
    }

    case Phigure::Parallelogram:
    {
        auto width = currentPosition.x() - previousPosition.x();
        auto height = currentPosition.y() - previousPosition.y();

        auto shift = qAbs(qMax(width, height)) * 0.35;

        QPolygonF polygon;

        polygon.append(QPointF(previousPosition));
        polygon.append(QPointF(QPoint(previousPosition.x() - shift, currentPosition.y())));
        polygon.append(QPointF(currentPosition));
        polygon.append(QPointF(QPoint(currentPosition.x() + shift, previousPosition.y())));

        currentPhigure = addPolygon(polygon, pen, brush);

        break;
    }

    case Phigure::Pen:
    {
        path.moveTo(previousPosition);
        path.lineTo(currentPosition.x(), currentPosition.y());

        removeItem(currentPhigure);

        currentPhigure = addPath(path, pen);

        previousPosition = currentPosition;

        break;
    }

    case Phigure::Rectangle:
    {
        currentPhigure = addPolygon(QPolygonF(QRectF(previousPosition.x(),
                                    previousPosition.y(),
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
            undoStack->push(new RemoveSceneItemCommand(this, item));

            removeItem(item);
        }

        previousPosition = currentPosition;

        break;
    }

    case Phigure::Select:
    {
        QList<QGraphicsItem*> allItems = items();

        for (auto item : allItems)
        {
            if(item->isSelected())
            {
                return;
            }
        }

        removeItem(currentPhigure);

        currentPhigure = addPolygon(QPolygonF(QRectF(previousPosition.x(),
                                    previousPosition.y(),
                                    point.x(),
                                    point.y())),
                                    QPen(QColor(8, 105, 181, 255), 0.3),
                                    QBrush(QColor(183, 213, 236, 40)));

        break;
    }

    }

    if(currentPhigure != nullptr)
    {
        currentPhigure->setAcceptHoverEvents(true);
    }
}

void PainterScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    if(event->button() != Qt::LeftButton)
    {
        return;
    }

    if(phigure == Phigure::Select)
    {
        if(currentPhigure != nullptr)
        {
            QList<QGraphicsItem*> colliding = items(currentPhigure->boundingRect());

            for (QGraphicsItem* item : colliding)
            {
                item->setSelected(true);
            }

            removeItem(currentPhigure);
        }
    }
    else if(phigure != Phigure::Cleaner)
    {
        if(currentPhigure != nullptr)
        {
            undoStack->push(new AddSceneItemCommand(this, currentPhigure));
        }
    }

    currentPhigure = nullptr;

    // del old path
    path = QPainterPath();
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

