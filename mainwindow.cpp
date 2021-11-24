#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QPainter>
#include <QFileDialog>
#include <QActionGroup>
#include <qmessagebox.h>

#include "saver.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto scene = new PainterScene(this);

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    scenes.push_back(scene);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    reloadCustomCursor();

    connect(ui->eraserButton, SIGNAL(clicked()), this, SLOT(onClean()));
    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->previousSceneButton, SIGNAL(clicked()), this, SLOT(prevScene()));
    connect(ui->nextSceneButton, SIGNAL(clicked()), this, SLOT(nextScene()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveInImage()));

    getCurrentScene()->setBackgroundBrush(QBrush(Qt::white));

    setDrawMenu();
    setSizeMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDrawMenu()
{
    QMenu *menu = new QMenu();
    QActionGroup *actionGroup = new QActionGroup(menu);

    QAction* action = menu->addAction(QIcon(":/icons/draw/pen.png"),
                    "Pen",
                    this,
                    SLOT(onDrawPen()));

    action->setChecked(true);
    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/draw/rectangle.png"),
                    "Rectangle",
                    this,
                    SLOT(onDrawRectangle()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/draw/rectangle_dashed.png"),
                    "Dashed rectangle",
                    this,
                    SLOT(onDrawDashRectangle()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/draw/line.png"),
                    "Line",
                    this,
                    SLOT(onDrawLine()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/draw/line_dashed.png"),
                    "Dashed Line",
                    this,
                    SLOT(onDrawDashLine()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/draw/circle.png"),
                    "Circle",
                    this,
                    SLOT(onDrawCircle()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/draw/circle_dashed.png"),
                    "Dashed Circle",
                    this,
                    SLOT(onDrawDashCircle()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    ui->drawButton->setMenu(menu);
}

void MainWindow::setSizeMenu()
{
    QMenu *menu = new QMenu();
    QActionGroup *actionGroup = new QActionGroup(menu);

    QAction* action = menu->addAction(QIcon(":/icons/size/small.png"),
                    "Small",
                    this,
                    SLOT(onSmallSize()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/size/normal.png"),
                    "Normal",
                    this,
                    SLOT(onNormalSize()));

    action->setChecked(true);
    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/size/medium.png"),
                    "Medium",
                    this,
                    SLOT(onMediumSize()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    action = menu->addAction(QIcon(":/icons/size/large.png"),
                    "Large",
                    this,
                    SLOT(onLargeSize()));

    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    ui->sizeButton->setMenu(menu);
}

void MainWindow::onDraw(Phigure phigure)
{
    getCurrentScene()->setPhigure(phigure);

    reloadCustomCursor();
}

void MainWindow::onClean()
{
    onDraw(Phigure::Cleaner);
}

void MainWindow::onDrawPen()
{
    onDraw(Phigure::Pen);
}

void MainWindow::onDrawRectangle()
{
    onDraw(Phigure::Rectangle);
}

void MainWindow::onDrawDashRectangle()
{
    onDraw(Phigure::DashRectangle);
}

void MainWindow::onDrawLine()
{
    onDraw(Phigure::Line);
}

void MainWindow::onDrawDashLine()
{
    onDraw(Phigure::DashLine);
}

void MainWindow::onDrawCircle()
{
    onDraw(Phigure::Circle);
}

void MainWindow::onDrawDashCircle()
{
    onDraw(Phigure::DashCircle);
}

void MainWindow::onSmallSize()
{
    getCurrentScene()->setPenSize(1);
}

void MainWindow::onNormalSize()
{
    getCurrentScene()->setPenSize(2);
}

void MainWindow::onMediumSize()
{
    getCurrentScene()->setPenSize(5);
}

void MainWindow::onLargeSize()
{
    getCurrentScene()->setPenSize(10);
}

PainterScene* MainWindow::getCurrentScene()
{
    return scenes.at(sceneId);
}

void MainWindow::setColor()
{
    QColor color = QColorDialog::getColor(getCurrentScene()->getPenColor(),
                                          this,
                                          "Color",
                                          QColorDialog::ColorDialogOption::ShowAlphaChannel);

    getCurrentScene()->setPenColor(color);
}

void MainWindow::clearScene()
{
    auto result = QMessageBox::question(
                this,
                "Clear",
                "Do you really want to clear the current scene?",
                QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes)
    {
        getCurrentScene()->clear();
        getCurrentScene()->initialize();
    }
}

void MainWindow::nextScene()
{
    QColor color = getCurrentScene()->getPenColor();
    int real = getCurrentScene()->getPenSize();
    QRectF rect = getCurrentScene()->sceneRect();
    Phigure phigure = getCurrentScene()->getPhigure();

    sceneId++;

    if(sceneId >= scenes.size())
    {
        scenes.push_back(new PainterScene());

        getCurrentScene()->setPenSize(real);
        getCurrentScene()->setPenColor(color);

        getCurrentScene()->setSceneRect(rect);
        getCurrentScene()->setBackgroundBrush(scenes.at(sceneId - 1)->backgroundBrush());
    }
    else
    {
        getCurrentScene()->setPenSize(real);
        getCurrentScene()->setPenColor(color);
    }

    getCurrentScene()->setPhigure(phigure);
    ui->graphicsView->setScene(getCurrentScene());

    ui->sceneLabel->setText(tr((std::to_string(sceneId + 1)).c_str()));
}

void MainWindow::prevScene()
{
    if(sceneId > 0)
    {
        QColor color = getCurrentScene()->getPenColor();
        int real = getCurrentScene()->getPenSize();
        Phigure phigure = getCurrentScene()->getPhigure();

        sceneId--;

        getCurrentScene()->setPenSize(real);
        getCurrentScene()->setPenColor(color);
        getCurrentScene()->setPhigure(phigure);
        getCurrentScene()->setSceneRect(getCurrentScene()->sceneRect());

        ui->graphicsView->setScene(getCurrentScene());

        ui->sceneLabel->setText(tr((std::to_string(sceneId+1)).c_str()));
    }
}

void MainWindow::saveInImage()
{
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 "Save Image",
                                                 "untitled.png",
                                                 "Images (*.png)");

    Saver::SaveImage(getCurrentScene(), fileName, ui->graphicsView->sizeHint() * 2);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    auto result = QMessageBox::question(
                this,
                "Exit",
                "Do you really want to exit?",
                QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    auto key = event->key();

    if(key == Qt::Key_Left)
    {
        prevScene();
    }
    else if(key == Qt::Key_Right)
    {
        nextScene();
    }
}

void MainWindow::reloadCustomCursor()
{
    auto phigure = getCurrentScene()->getPhigure();

    QCursor cursor;

    switch (phigure)
    {
        case Phigure::Pen:
        {
            cursor = QCursor(QPixmap::fromImage(QImage(":/cursors/pen.png")), 1, 16);
            break;
        }

        case Phigure::Cleaner:
        {
            cursor = QCursor(QPixmap::fromImage(QImage(":/cursors/eraser.png")), 8, 8);
            break;
        }

        default:
        {
            cursor = QCursor(QPixmap::fromImage(QImage(":/cursors/target.png")), 8, 8);
            break;
        }
    }

    ui->graphicsView->viewport()->setCursor(cursor);
}
