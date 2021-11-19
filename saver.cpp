#include "saver.h"

Saver::Saver(){}

void Saver::SaveInImages(vector<customScene*>& scene, const QString &filename, const QSize &size){

    if(filename.size() > 0){

        for(size_t i = 0; i < scene.size();i++){

            QImage* img = renderScene(scene.at(i),size);
            img->save((filename.toStdString() + "_" + to_string(i) + ".png").c_str());

            delete img;
        }
    }

}

void Saver::SaveImage(QGraphicsScene* scene,const QString& filename,const QSize& size){

    if(filename.size() > 0){

        QImage* img = renderScene(scene,size);
        img->save((filename.toStdString() + ".png").c_str());

        delete img;
    }
}

QImage* Saver::renderScene(QGraphicsScene* scene,const QSize& size){


    QImage* img = new QImage(size * 2,QImage::Format_ARGB32);
    QPainter* painter = new QPainter(img);
    painter->setRenderHint(QPainter::Antialiasing);

    scene->render(painter);


    delete painter;

    return img;

}
