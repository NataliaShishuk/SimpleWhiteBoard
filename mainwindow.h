#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QColorDialog>
#include <QImage>
#include <QCursor>
#include <QUndoStack>

#include "painterscene.h"
#include "saver.h"
#include "commands.h"

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
    void resizeEvent(QResizeEvent * event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QTimer* timer;
    Saver* saver;
    vector<PainterScene*> scenes;
    bool isGridVisible = false;

    size_t sceneId = 0;

    Ui::MainWindow *ui;

    QAction *undoAction;
    QAction *redoAction;

    QList<QGraphicsItem*> selectedItemsToCopy;

    PainterScene* getCurrentScene();

    void reloadCustomCursor();

    void setDrawMenu();
    void setColorMenu();
    void setSizeMenu();
    void setSaveMenu();

    void onDraw(Phigure phigure, PhigureLine style = PhigureLine::SolidLine, PhigureFill fill = PhigureFill::Border);
    void setPenColor(QColor color);
    void setPenSize(qreal size);
    void saveCurrentScene(SaveType type);
    void createUndoStackAndActions();

    QPixmap drawPattern(int step, const QColor &color);

private slots:
     void slotTimer();

     void nextScene();
     void prevScene();


     // select
     void onSelect();

     // eraser
     void clearScene();

     // draw
     void onClean();
     void onDrawPen();
     void onDrawRectangle();
     void onDrawDashRectangle();
     void onDrawFillRectangle();
     void onDrawLine();
     void onDrawDashLine();
     void onDrawDotLine();
     void onDrawCircle();
     void onDrawDashCircle();
     void onDrawFillCircle();
     // triangle
     void onDrawTriangle();
     void onDrawDashedTriangle();
     void onDrawFilledTriangle();
     // rhombus
     void onDrawRhombus();
     void onDrawDashedRhombus();
     void onDrawFilledRhombus();
     // parallelogram
     void onDrawParallelogram();
     void onDrawDashedParallelogram();
     void onDrawFilledParallelogram();

     // colors
     void onSetWhiteColor();
     void onSetRedColor();
     void onSetYellowColor();
     void onSetLightGreenColor();
     void onSetBlueColor();

     void onSetPurpleColor();
     void onSetPinkColor();
     void onSetOrangeColor();
     void onSetGreenColor();
     void onSetSkyBlueColor();

     void onSetBlackColor();
     void onSetDarkRedColor();
     void onSetDarkOrangeColor();
     void onSetDarkGreenColor();
     void onSetDarkBlueColor();

     // size
     void onSmallSize();
     void onNormalSize();
     void onMediumSize();
     void onLargeSize();

     // save
     void saveAsImage();
     void saveAsPdf();
     void saveAllAsImage();
     void saveAllAsPdf();
     void saveProject();

     // grid
     void onSetGrid();
};

#endif // MAINWINDOW_H
