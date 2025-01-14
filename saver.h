#ifndef SAVER_H
#define SAVER_H

#include <qpainter.h>
#include <qpdfwriter.h>
#include <QVector>

#include "painterscene.h"

using namespace std;

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

    void saveScene(PainterScene* scene, const QString& filePath, SaveType type = SaveType::Image);
    void saveScenes(QVector<PainterScene*>& scenes, const QString& filePath, SaveType type = SaveType::Image);
    void saveProject(QVector<PainterScene*>& scenes, const QString& filePath);

private:
    QImage* renderScene(PainterScene* scene);
    void saveAsImage(const QImage* image, const QString& filePath);
    void saveAsPDF(const QImage* image, const QString& filePath);

};

#endif
