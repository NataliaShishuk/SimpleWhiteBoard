#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QPainter>
#include <QFileDialog>
#include <QActionGroup>
#include <QWidgetAction>
#include <QFontDatabase>
#include <qmessagebox.h>

#include "saver.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto scene = new PainterScene(ui->graphicsView);

    scenes.push_back(scene);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    reloadCustomCursor();

    QFontDatabase::addApplicationFont(":/fonts/lato_regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/lato_bold.ttf");

    connect(ui->eraserButton, SIGNAL(clicked()), this, SLOT(onClean()));
    connect(ui->previousSceneButton, SIGNAL(clicked()), this, SLOT(prevScene()));
    connect(ui->nextSceneButton, SIGNAL(clicked()), this, SLOT(nextScene()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveInImage()));
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(onSelect()));

    setDrawMenu();
    setColorMenu();
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

void MainWindow::setColorMenu()
{
    auto menu = new QMenu();

    menu->setStyleSheet("QPushButton { "
                        "border:none;"
                        "qproperty-iconSize: 24px;"
                        "width:30px;"
                        "height:30px;"
                        "border-radius: 5px;"
                        "}"
                        "QPushButton:hover {"
                        "cursor: pointer;"
                        "border-radius: 5px;"
                        "background-color: #b3cccc;"
                        "}");

    auto menuLayout = new QGridLayout();

    auto whiteButton = new QPushButton();
    auto redButton = new QPushButton();
    auto yellowButton = new QPushButton();
    auto lightGreenButton = new QPushButton();
    auto blueButton = new QPushButton();

    whiteButton->setStyleSheet("background-color: rgba(255,255,255,255);");
    redButton->setStyleSheet("background-color: rgba(255,25,25,255);");
    yellowButton->setStyleSheet("background-color: rgba(255,222,50,255);");
    lightGreenButton->setStyleSheet("background-color: rgba(130,199,134,255);");
    blueButton->setStyleSheet("background-color: rgba(46,140,255,255);");

    connect(whiteButton, SIGNAL(clicked()), this, SLOT(onSetWhiteColor()));
    connect(redButton, SIGNAL(clicked()), this, SLOT(onSetRedColor()));
    connect(yellowButton, SIGNAL(clicked()), this, SLOT(onSetYellowColor()));
    connect(lightGreenButton, SIGNAL(clicked()), this, SLOT(onSetLightGreenColor()));
    connect(blueButton, SIGNAL(clicked()), this, SLOT(onSetBlueColor()));

    connect(whiteButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(redButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(yellowButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(lightGreenButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(blueButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(whiteButton, 0, 0);
    menuLayout->addWidget(redButton, 0, 1);
    menuLayout->addWidget(yellowButton, 0, 2);
    menuLayout->addWidget(lightGreenButton, 0, 3);
    menuLayout->addWidget(blueButton, 0, 4);

    auto purpleButton = new QPushButton();
    auto pinkButton = new QPushButton();
    auto orangeButton = new QPushButton();
    auto greenButton = new QPushButton();
    auto skyBlueButton = new QPushButton();

    purpleButton->setStyleSheet("background-color: rgba(180,121,255,255);");
    pinkButton->setStyleSheet("background-color: rgba(255,56,199,255);");
    orangeButton->setStyleSheet("background-color: rgba(255,138,0,255);");
    greenButton->setStyleSheet("background-color: rgba(73,214,30,255);");
    skyBlueButton->setStyleSheet("background-color: rgba(81,216,235,255);");

    connect(purpleButton, SIGNAL(clicked()), this, SLOT(onSetPurpleColor()));
    connect(pinkButton, SIGNAL(clicked()), this, SLOT(onSetPinkColor()));
    connect(orangeButton, SIGNAL(clicked()), this, SLOT(onSetOrangeColor()));
    connect(greenButton, SIGNAL(clicked()), this, SLOT(onSetGreenColor()));
    connect(skyBlueButton, SIGNAL(clicked()), this, SLOT(onSetSkyBlueColor()));

    connect(purpleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(pinkButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(orangeButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(greenButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(skyBlueButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(purpleButton, 1, 0);
    menuLayout->addWidget(pinkButton, 1, 1);
    menuLayout->addWidget(orangeButton, 1, 2);
    menuLayout->addWidget(greenButton, 1, 3);
    menuLayout->addWidget(skyBlueButton, 1, 4);

    auto blackButton = new QPushButton();
    auto darkRedButton = new QPushButton();
    auto darkOrangeButton = new QPushButton();
    auto darkGreenButton = new QPushButton();
    auto darkBlueButton = new QPushButton();

    blackButton->setStyleSheet("background-color: rgba(0,0,0,255);");
    darkRedButton->setStyleSheet("background-color: rgba(127,0,0,255);");
    darkOrangeButton->setStyleSheet("background-color: rgba(119,68,8,255);");
    darkGreenButton->setStyleSheet("background-color: rgba(11,114,40,255);");
    darkBlueButton->setStyleSheet("background-color: rgba(20,79,195,255);");

    connect(blackButton, SIGNAL(clicked()), this, SLOT(onSetBlackColor()));
    connect(darkRedButton, SIGNAL(clicked()), this, SLOT(onSetDarkRedColor()));
    connect(darkOrangeButton, SIGNAL(clicked()), this, SLOT(onSetDarkOrangeColor()));
    connect(darkGreenButton, SIGNAL(clicked()), this, SLOT(onSetDarkGreenColor()));
    connect(darkBlueButton, SIGNAL(clicked()), this, SLOT(onSetDarkBlueColor()));

    connect(blackButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(darkRedButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(darkOrangeButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(darkGreenButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(darkBlueButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(blackButton, 2, 0);
    menuLayout->addWidget(darkRedButton, 2, 1);
    menuLayout->addWidget(darkOrangeButton, 2, 2);
    menuLayout->addWidget(darkGreenButton, 2, 3);
    menuLayout->addWidget(darkBlueButton, 2, 4);

    auto otherButton = new QPushButton("Other");

    otherButton->setStyleSheet("font-weight: bold;");

    connect(otherButton, SIGNAL(clicked()), this, SLOT(setColor()));

    menuLayout->addWidget(otherButton, 3, 0, 1, 5);

    menu->setLayout(menuLayout);

    ui->colorButton->setMenu(menu);
}

void MainWindow::setSizeMenu()
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

    auto smallButton = new QPushButton("S");
    auto normalButton = new QPushButton("N");
    auto mediumButton = new QPushButton("M");
    auto largeButton = new QPushButton("L");

    connect(smallButton, SIGNAL(clicked()), this, SLOT(onSmallSize()));
    connect(normalButton, SIGNAL(clicked()), this, SLOT(onNormalSize()));
    connect(mediumButton, SIGNAL(clicked()), this, SLOT(onMediumSize()));
    connect(largeButton, SIGNAL(clicked()), this, SLOT(onLargeSize()));

    connect(smallButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(normalButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(mediumButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(largeButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(smallButton, 0, 0);
    menuLayout->addWidget(normalButton, 0, 1);
    menuLayout->addWidget(mediumButton, 0, 2);
    menuLayout->addWidget(largeButton, 0, 3);

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

void MainWindow::onSetWhiteColor()
{
    getCurrentScene()->setPenColor(Qt::white);
}

void MainWindow::onSetRedColor()
{
    getCurrentScene()->setPenColor(QColor(255, 25, 25, 255));
}

void MainWindow::onSetYellowColor()
{
    getCurrentScene()->setPenColor(QColor(255, 222, 50, 255));
}

void MainWindow::onSetLightGreenColor()
{
    getCurrentScene()->setPenColor(QColor(130, 199, 134, 255));
}

void MainWindow::onSetBlueColor()
{
    getCurrentScene()->setPenColor(QColor(46, 140, 255, 255));
}

void MainWindow::onSetPurpleColor()
{
    getCurrentScene()->setPenColor(QColor(180, 121, 255, 255));
}

void MainWindow::onSetPinkColor()
{
    getCurrentScene()->setPenColor(QColor(255, 56, 199, 255));
}

void MainWindow::onSetOrangeColor()
{
    getCurrentScene()->setPenColor(QColor(255, 138, 0, 255));
}

void MainWindow::onSetGreenColor()
{
    getCurrentScene()->setPenColor(QColor(73, 214, 30, 255));
}

void MainWindow::onSetSkyBlueColor()
{
    getCurrentScene()->setPenColor(QColor(81, 216, 235, 255));
}

void MainWindow::onSetBlackColor()
{
    getCurrentScene()->setPenColor(QColor(0, 0, 0, 255));
}

void MainWindow::onSetDarkRedColor()
{
    getCurrentScene()->setPenColor(QColor(127, 0, 0, 255));
}

void MainWindow::onSetDarkOrangeColor()
{
    getCurrentScene()->setPenColor(QColor(119, 68, 8, 255));
}

void MainWindow::onSetDarkGreenColor()
{
    getCurrentScene()->setPenColor(QColor(11, 114, 40, 255));
}

void MainWindow::onSetDarkBlueColor()
{
    getCurrentScene()->setPenColor(QColor(20, 79, 195, 255));
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

void MainWindow::onSelect()
{
    onDraw(Phigure::Select);
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

        case Phigure::Select:
        {
            cursor = Qt::ArrowCursor;
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
