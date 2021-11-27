#include "saver.h"

Saver::Saver()
{

}

Saver::~Saver()
{

}

void Saver::saveScene(QGraphicsScene *scene, const QString &filePath, const QSize &size, SaveType type)
{
    QImage* image = renderScene(scene,size);

    if(type == SaveType::Image)
    {
        saveAsImage(image, filePath);
    }
    else if(type == SaveType::PDF)
    {
        saveAsPDF(image, filePath);
    }

    delete image;
}

QImage *Saver::renderScene(QGraphicsScene *scene, const QSize &size)
{
    QImage* img = new QImage(size, QImage::Format_ARGB32);
    QPainter* painter = new QPainter(img);

    painter->setRenderHint(QPainter::Antialiasing);
    scene->render(painter);

    delete painter;

    return img;
}

void Saver::saveAsImage(const QImage *image, const QString &filePath)
{
    image->save(filePath);
}

void Saver::saveAsPDF(const QImage *image, const QString &filePath)
{
    QPdfWriter writer(filePath);

    writer.setPageSize(QPageSize(image->size()));
    writer.setPageMargins(QMargins(0, 0, 0, 0));

    QPainter painter(&writer);

    painter.drawImage(QPoint(0, 0), *image);

    painter.end();
}
