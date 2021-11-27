#ifndef SAVER_H
#define SAVER_H

#include <qpainter.h>
#include <qpdfwriter.h>

#include "painterscene.h"

enum SaveType {
    Image   = 0,
    PDF     = 1
};

class Saver
{
public:
    Saver();
    ~Saver();

    void saveScene(QGraphicsScene* scene, const QString& filePath, const QSize& size, SaveType type = SaveType::Image);

private:
    QImage* renderScene(QGraphicsScene* scene, const QSize& size);

    void saveAsImage(const QImage* image, const QString& filePath);
    void saveAsPDF(const QImage* image, const QString& filePath);

};

#endif
