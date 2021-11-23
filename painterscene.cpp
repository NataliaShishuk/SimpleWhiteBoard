#include "painterscene.h"

PainterScene::PainterScene(QObject *parent) : QGraphicsScene(parent)
{
   initialize();
}

PainterScene::~PainterScene()
{

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

    if(phigure == Phigure::DashRectangle)
    {
        QPolygonF polygon;

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));
        polygon.append(QPointF(previousPoint.x(), previousPoint.y()));

        dashpolyItem = addPolygon(polygon, color);
    }

    if(phigure == Phigure::Circle)
    {
        ellipse = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, color);
    }

    if(phigure == Phigure::DashCircle)
    {
        dashellipse = addEllipse(previousPoint.x(), previousPoint.y(), 0, 0, color);
    }

    if(phigure == Phigure::Line)
    {
        lineItem = addLine(0, 0, 0, 0, color);
    }

    if(phigure == Phigure::DashLine)
    {
        dashlineItem = addLine(0, 0, 0, 0, color);
    }
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF point = event->scenePos() - previousPoint;
    float magnitude;
    QPolygonF polygon;

    switch (phigure) {

    case Phigure::Line:

        removeItem(lineItem);

        lineItem = addLine(previousPoint.x(),previousPoint.y(),event->scenePos().x(),event->scenePos().y(),QPen(color,size,Qt::SolidLine,Qt::RoundCap));


        break;

    case Phigure::DashLine:

        removeItem(dashlineItem);

        dashlineItem = addLine(previousPoint.x(),previousPoint.y(),event->scenePos().x(),event->scenePos().y(),QPen(color,size,Qt::DashLine,Qt::RoundCap));


        break;

    case Phigure::Circle:

        removeItem(ellipse);

        magnitude = sqrt(point.x() * point.x() + point.y() * point.y());

        ellipse = addEllipse(previousPoint.x() - magnitude/2,previousPoint.y() - magnitude/2,magnitude,magnitude,QPen(color,size,Qt::SolidLine,Qt::RoundCap));

        break;

    case Phigure::DashCircle:

        removeItem(dashellipse);

        magnitude = sqrt(point.x() * point.x() + point.y() * point.y());

        dashellipse = addEllipse(previousPoint.x() - magnitude/2,previousPoint.y() - magnitude/2,magnitude,magnitude,QPen(color,size,Qt::DashLine,Qt::RoundCap));

        break;

    case Phigure::Pen:

        addLine(previousPoint.x(),
                previousPoint.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(color,size,Qt::SolidLine,Qt::RoundCap));
        previousPoint = event->scenePos();

        break;
    case Phigure::Rectangle:

        removeItem(polyItem);

        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x() + point.x(),previousPoint.y()));
        polygon.append(QPointF(previousPoint.x() + point.x(),previousPoint.y() + point.y()));
        polygon.append(QPointF(previousPoint.x(),previousPoint.y() + point.y()));



        polyItem = addPolygon(polygon,QPen(color,size,Qt::SolidLine));

        break;

    case Phigure::DashRectangle:

        removeItem(dashpolyItem);



        polygon.append(QPointF(previousPoint));
        polygon.append(QPointF(previousPoint.x() + point.x(),previousPoint.y()));
        polygon.append(QPointF(previousPoint.x() + point.x(),previousPoint.y() + point.y()));
        polygon.append(QPointF(previousPoint.x(),previousPoint.y() + point.y()));



        dashpolyItem = addPolygon(polygon,QPen(color,size,Qt::DashLine));

        break;
    case Phigure::Cleaner:


//        if(CleanerVariant){

//            addLine(previousPoint.x(),
//                    previousPoint.y(),
//                    event->scenePos().x(),
//                    event->scenePos().y(),
//                    QPen(this->backgroundBrush().color(),size * 3,Qt::SolidLine,Qt::RoundCap));
//            previousPoint = event->scenePos();

//        }
//        else
//        {

//        cleanerLine = addLine(previousPoint.x(),
//                 previousPoint.y(),
//                 event->scenePos().x(),
//                 event->scenePos().y(),
//                 QPen(QColor(255,255,255,255),size * 2,Qt::SolidLine,Qt::RoundCap));

//       QList<QGraphicsItem*> colliding = cleanerLine->collidingItems();

//       for(qsizetype i = 0; i < colliding.size();i++){

//           removeItem(colliding.at(i));

//       }
//      previousPoint = event->scenePos();
//
//      break;
//
//       }

        cleanerLine = addLine(previousPoint.x(),
                 previousPoint.y(),
                 event->scenePos().x(),
                 event->scenePos().y(),
                 QPen(QColor(255,255,255,255),size * 2,Qt::SolidLine,Qt::RoundCap));

       QList<QGraphicsItem*> colliding = cleanerLine->collidingItems();

       for(qsizetype i = 0; i < colliding.size();i++){

           removeItem(colliding.at(i));

       }
      previousPoint = event->scenePos();

      break;



    }
}

