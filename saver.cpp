#include "saver.h"

void Saver::SaveImage(QGraphicsScene* scene,const QString& filename,const QSize& size)
{
    QImage* img = renderScene(scene,size);

    img->save((filename.toStdString()).c_str());

    delete img;
}

QImage* Saver::renderScene(QGraphicsScene* scene,const QSize& size)
{
    QImage* img = new QImage(size * 2, QImage::Format_ARGB32);
    QPainter* painter = new QPainter(img);

    painter->setRenderHint(QPainter::Antialiasing);
    scene->render(painter);

    delete painter;

    return img;
}
