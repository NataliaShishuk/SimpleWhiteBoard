#include <QDir>

#include "saver.h"

void Saver::saveScene(PainterScene *scene, const QString &filePath, SaveType type)
{
    QImage* image = renderScene(scene);

    if(type == SaveType::Image)
    {
        saveAsImage(image, filePath);
    }
    else if(type == SaveType::PDF)
    {
        saveAsPDF(image, filePath);
    }
}

void Saver::saveScenes(vector<PainterScene*> &scenes, const QString &filePath, SaveType type)
{
    int lenght = scenes.size();

    if(type == SaveType::Image)
    {
        for (int i = 0 ; i < lenght; i++)
        {
            PainterScene* scene = scenes.at(i);
            QImage* image = renderScene(scene);
            QString path = QDir(filePath).filePath("scene-" + QString::number(i + 1) + ".png");

            saveAsImage(image, path);
        }
    }
    else if(type == SaveType::PDF)
    {
        QPdfWriter writer(filePath);
        QImage* image = renderScene(scenes.at(0));

        writer.setPageSize(QPageSize(image->size()));
        writer.setPageMargins(QMargins(0, 0, 0, 0));

        QPainter painter(&writer);

        painter.drawImage(QRect(0, 0, painter.device()->width(), painter.device()->height()), *image);

        if(lenght != 1)
        {
            writer.newPage();
        }

        delete image;

        for (int i = 1; i < lenght; i++)
        {
            PainterScene* scene = scenes.at(i);
            QImage* image = renderScene(scene);

            writer.setPageSize(QPageSize(image->size()));
            writer.setPageMargins(QMargins(0, 0, 0, 0));

            painter.drawImage(QRect(0, 0, painter.device()->width(), painter.device()->height()), *image);

            if(i != lenght - 1)
            {
                writer.newPage();
            }

            delete image;
        }

        painter.end();
    }
}

void Saver::saveProject(vector<PainterScene *> &scenes, const QString &filePath)
{

}

QImage* Saver::renderScene(PainterScene* scene)
{
    QImage* image = new QImage(scene->width(), scene->height(), QImage::Format_ARGB32);
    QPainter* painter = new QPainter(image);

    painter->setRenderHint(QPainter::Antialiasing);
    scene->render(painter);

    delete painter;

    return image;
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

    painter.drawImage(QRect(0, 0, painter.device()->width(), painter.device()->height()), *image);

    painter.end();
}
