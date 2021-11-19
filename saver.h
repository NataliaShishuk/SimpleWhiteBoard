#ifndef SAVER_H
#define SAVER_H
#include <qgraphicsscene.h>
#include <qpainter.h>
#include <qfiledialog.h>
#include <qurl.h>
#include <QString>

#include <string>
#include <vector>

#include "customscene.h"

using namespace std;

class Saver
{
public:

    Saver();

    static void SaveInImages(vector<customScene*>& scenes,const QString& filename,const QSize& size);
    static void SaveImage(QGraphicsScene* scene,const QString& filname,const QSize& size);
    static QImage* renderScene(QGraphicsScene* scene,const QSize& size);

};

#endif
