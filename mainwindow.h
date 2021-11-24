#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QColorDialog>
#include <QImage>
#include <QCursor>

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

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    vector<PainterScene*> scenes;

    size_t sceneId = 0;

    Ui::MainWindow *ui;

    PainterScene* getCurrentScene();

    void reloadCustomCursor();

    void setDrawMenu();
    void setSizeMenu();
    void onDraw(Phigure phigure);

private slots:
     void setColor();
     void nextScene();
     void prevScene();
     void saveInImage();

     // eraser
     void clearScene();

     // draw
     void onClean();
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
