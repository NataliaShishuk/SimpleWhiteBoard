#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QPainter>
#include <QFileDialog>
#include <QActionGroup>
#include <QWidgetAction>
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
    auto menu = new QMenu();

    menu->setStyleSheet("QPushButton { "
                        "border:none;"
                        "qproperty-iconSize: 24px;"
                        "width:35px;"
                        "height:35px;"
                        "}"
                        "QPushButton:hover {"
                        "cursor: pointer;"
                        "border-radius: 5px;"
                        "background-color: #b3cccc;"
                        "}");

    auto menuLayout = new QGridLayout();

    auto penButton = new QPushButton();
    auto rectangleButton = new QPushButton();
    auto dashedRectangleButton = new QPushButton();
    auto lineButton = new QPushButton();
    auto dashedLineButton = new QPushButton();
    auto circleButton = new QPushButton();
    auto dashedCircleButton = new QPushButton();

    penButton->setIcon(QIcon(":/icons/draw/pen.png"));
    rectangleButton->setIcon(QIcon(":/icons/draw/rectangle.png"));
    dashedRectangleButton->setIcon(QIcon(":/icons/draw/rectangle_dashed.png"));
    lineButton->setIcon(QIcon(":/icons/draw/line.png"));
    dashedLineButton->setIcon(QIcon(":/icons/draw/line_dashed.png"));
    circleButton->setIcon(QIcon(":/icons/draw/circle.png"));
    dashedCircleButton->setIcon(QIcon(":/icons/draw/circle_dashed.png"));

    connect(penButton, SIGNAL(clicked()), this, SLOT(onDrawPen()));
    connect(rectangleButton, SIGNAL(clicked()), this, SLOT(onDrawRectangle()));
    connect(dashedRectangleButton, SIGNAL(clicked()), this, SLOT(onDrawDashRectangle()));
    connect(lineButton, SIGNAL(clicked()), this, SLOT(onDrawLine()));
    connect(dashedLineButton, SIGNAL(clicked()), this, SLOT(onDrawDashLine()));
    connect(circleButton, SIGNAL(clicked()), this, SLOT(onDrawCircle()));
    connect(dashedCircleButton, SIGNAL(clicked()), this, SLOT(onDrawDashCircle()));

    connect(penButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(rectangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedRectangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(lineButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedLineButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(circleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedCircleButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(penButton, 0, 0);
    menuLayout->addWidget(rectangleButton, 0, 1);
    menuLayout->addWidget(dashedRectangleButton, 1, 1);
    menuLayout->addWidget(lineButton, 0, 2);
    menuLayout->addWidget(dashedLineButton, 1, 2);
    menuLayout->addWidget(circleButton, 0, 3);
    menuLayout->addWidget(dashedCircleButton, 1, 3);

    menu->setLayout(menuLayout);

    ui->drawButton->setMenu(menu);
}

void MainWindow::setSizeMenu()
{
    auto menu = new QMenu();

    menu->setStyleSheet("QPushButton { "
                        "border:none;"
                        "qproperty-iconSize: 24px;"
                        "width:100px;"
                        "height:35px;"
                        "}"
                        "QPushButton:hover {"
                        "cursor: pointer;"
                        "border-radius: 5px;"
                        "background-color: #b3cccc;"
                        "}");

    auto menuLayout = new QGridLayout();

    auto smallButton = new QPushButton("Small");
    auto normalButton = new QPushButton("Normal");
    auto mediumButton = new QPushButton("Medium");
    auto largeButton = new QPushButton("Large");

    connect(smallButton, SIGNAL(clicked()), this, SLOT(onSmallSize()));
    connect(normalButton, SIGNAL(clicked()), this, SLOT(onNormalSize()));
    connect(mediumButton, SIGNAL(clicked()), this, SLOT(onMediumSize()));
    connect(largeButton, SIGNAL(clicked()), this, SLOT(onLargeSize()));

    connect(smallButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(normalButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(mediumButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(largeButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(smallButton, 0, 0);
    menuLayout->addWidget(normalButton, 1, 0);
    menuLayout->addWidget(mediumButton, 2, 0);
    menuLayout->addWidget(largeButton, 3, 0);

    menu->setLayout(menuLayout);

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
