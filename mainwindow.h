#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QColorDialog>
#include <QImage>
#include <QCursor>

#include <vector>

#include "painterscene.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer* timer;

    vector<PainterScene*> scene;

    size_t sceneId = 0;

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent * event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    PainterScene* Scene(){
        return  scene.at(sceneId);
    }
    QImage renderImage(size_t index);

    void reloadCustomCursor();

    void setDrawMenu();
    void setSizeMenu();
    void onDraw(Phigure phigure);

private slots:
     void slotTimer();
     void setColor();
     void clearScene();
     void nextScene();
     void prevScene();
     void saveInImage();
     void Cleaner();

     // draw
     void onDrawPen();
     void onDrawRectangle();
     void onDrawDashRectangle();
     void onDrawLine();
     void onDrawDashLine();
     void onDrawCircle();
     void onDrawDashCircle();

     // size
     void onSmallSize();
     void onNormalSize();
     void onMediumSize();
     void onLargeSize();
};

#endif // MAINWINDOW_H
