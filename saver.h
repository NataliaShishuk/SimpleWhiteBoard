#ifndef SAVER_H
#define SAVER_H

#include <qpainter.h>
#include <QString>
#include <string>
#include <vector>

#include "painterscene.h"

using namespace std;

class Saver
{
public:
    Saver();

    static void SaveImage(QGraphicsScene* scene, const QString& filname, const QSize& size);

private:
    static QImage* renderScene(QGraphicsScene* scene, const QSize& size);

};

#endif
