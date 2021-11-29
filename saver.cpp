#include "saver.h"

void Saver::saveScene(QGraphicsScene *scene, const QString &filePath, SaveType type)
{
    QImage* image = new QImage(scene->width(), scene->height(), QImage::Format_ARGB32);
    QPainter* painter = new QPainter(image);

    painter->setRenderHint(QPainter::Antialiasing);
    scene->render(painter);

    if(type == SaveType::Image)
    {
        saveAsImage(image, filePath);
    }
    else if(type == SaveType::PDF)
    {
        saveAsPDF(image, filePath);
    }
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
