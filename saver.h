#ifndef SAVER_H
#define SAVER_H

#include <qpainter.h>
#include <qpdfwriter.h>

#include "painterscene.h"

enum SaveType
{
    Image   = 0,
    PDF     = 1
};

class Saver
{
public:
    Saver() {};
    ~Saver() {};

    void saveScene(QGraphicsScene* scene, const QString& filePath, SaveType type = SaveType::Image);

private:
    void saveAsImage(const QImage* image, const QString& filePath);
    void saveAsPDF(const QImage* image, const QString& filePath);

};

#endif
