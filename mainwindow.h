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


    QCursor custom_cursor;

    QImage standart_cursor;
    QImage cleaner_cursor;
    QImage circle_cursor;
    QImage line_cursor;
    QImage rectangle_cursor;

    void onDraw(Phigure phigure);

private slots:
     void slotTimer();
     void setColor();
     void resizePen(int value);
     void clearScene();
     void nextScene();
     void prevScene();
     void saveInImage();
     void Cleaner();

     void onDrawPen();
     void onDrawRectangle();
     void onDrawDashRectangle();
     void onDrawLine();
     void onDrawDashLine();
     void onDrawCircle();
     void onDrawDashCircle();
};

#endif // MAINWINDOW_H
