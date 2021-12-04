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

    saver = new Saver();

    auto scene = new PainterScene();

    scenes.push_back(scene);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    reloadCustomCursor();

    QFontDatabase::addApplicationFont(":/fonts/lato_regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/lato_bold.ttf");

    timer = new QTimer();

    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);

    timer->start(100);

    connect(ui->eraserButton, SIGNAL(clicked()), this, SLOT(onClean()));
    connect(ui->previousSceneButton, SIGNAL(clicked()), this, SLOT(prevScene()));
    connect(ui->nextSceneButton, SIGNAL(clicked()), this, SLOT(nextScene()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveInImage()));
    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(onSelect()));
    connect(ui->showGridButton, SIGNAL(clicked()), this, SLOT(onSetGrid()));

    setDrawMenu();
    setColorMenu();
    setSizeMenu();
    setSaveMenu();
    createUndoStackAndActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);

    QWidget::resizeEvent(event);
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
    auto fillRectangleButton = new QPushButton();
    auto lineButton = new QPushButton();
    auto dashedLineButton = new QPushButton();
    auto dotedLineButton = new QPushButton();
    auto circleButton = new QPushButton();
    auto dashedCircleButton = new QPushButton();
    auto fillCircleButton = new QPushButton();
    auto triangleButton = new QPushButton();
    auto dashedTriangleButton = new QPushButton();
    auto filledTriangleButton = new QPushButton();
    auto rhombusButton = new QPushButton();
    auto dashedRhombusButton = new QPushButton();
    auto filledRhombusButton = new QPushButton();

    penButton->setIcon(QIcon(":/icons/draw/pen.png"));
    rectangleButton->setIcon(QIcon(":/icons/draw/rectangle.png"));
    dashedRectangleButton->setIcon(QIcon(":/icons/draw/rectangle_dashed.png"));
    fillRectangleButton->setIcon(QIcon(":/icons/draw/rectangle_filled.png"));
    lineButton->setIcon(QIcon(":/icons/draw/line.png"));
    dashedLineButton->setIcon(QIcon(":/icons/draw/line_dashed.png"));
    dotedLineButton->setIcon(QIcon(":/icons/draw/line_doted.png"));
    circleButton->setIcon(QIcon(":/icons/draw/circle.png"));
    dashedCircleButton->setIcon(QIcon(":/icons/draw/circle_dashed.png"));
    fillCircleButton->setIcon(QIcon(":/icons/draw/circle_filled.png"));
    triangleButton->setIcon(QIcon(":/icons/draw/triangle.png"));
    dashedTriangleButton->setIcon(QIcon(":/icons/draw/triangle_dashed.png"));
    filledTriangleButton->setIcon(QIcon(":/icons/draw/triangle_filled.png"));
    rhombusButton->setIcon(QIcon(":/icons/draw/rhombus.png"));
    dashedRhombusButton->setIcon(QIcon(":/icons/draw/rhombus_dashed.png"));
    filledRhombusButton->setIcon(QIcon(":/icons/draw/rhombus_filled.png"));

    penButton->setCursor(Qt::PointingHandCursor);
    rectangleButton->setCursor(Qt::PointingHandCursor);
    dashedRectangleButton->setCursor(Qt::PointingHandCursor);
    fillRectangleButton->setCursor(Qt::PointingHandCursor);
    lineButton->setCursor(Qt::PointingHandCursor);
    dashedLineButton->setCursor(Qt::PointingHandCursor);
    dotedLineButton->setCursor(Qt::PointingHandCursor);
    circleButton->setCursor(Qt::PointingHandCursor);
    dashedCircleButton->setCursor(Qt::PointingHandCursor);
    fillCircleButton->setCursor(Qt::PointingHandCursor);
    triangleButton->setCursor(Qt::PointingHandCursor);
    dashedTriangleButton->setCursor(Qt::PointingHandCursor);
    filledTriangleButton->setCursor(Qt::PointingHandCursor);
    rhombusButton->setCursor(Qt::PointingHandCursor);
    dashedRhombusButton->setCursor(Qt::PointingHandCursor);
    filledRhombusButton->setCursor(Qt::PointingHandCursor);

    connect(penButton, SIGNAL(clicked()), this, SLOT(onDrawPen()));
    connect(rectangleButton, SIGNAL(clicked()), this, SLOT(onDrawRectangle()));
    connect(dashedRectangleButton, SIGNAL(clicked()), this, SLOT(onDrawDashRectangle()));
    connect(fillRectangleButton, SIGNAL(clicked()), this, SLOT(onDrawFillRectangle()));
    connect(lineButton, SIGNAL(clicked()), this, SLOT(onDrawLine()));
    connect(dashedLineButton, SIGNAL(clicked()), this, SLOT(onDrawDashLine()));
    connect(dotedLineButton, SIGNAL(clicked()), this, SLOT(onDrawDotLine()));
    connect(circleButton, SIGNAL(clicked()), this, SLOT(onDrawCircle()));
    connect(dashedCircleButton, SIGNAL(clicked()), this, SLOT(onDrawDashCircle()));
    connect(fillCircleButton, SIGNAL(clicked()), this, SLOT(onDrawFillCircle()));
    connect(triangleButton, SIGNAL(clicked()), this, SLOT(onDrawTriangle()));
    connect(dashedTriangleButton, SIGNAL(clicked()), this, SLOT(onDrawDashedTriangle()));
    connect(filledTriangleButton, SIGNAL(clicked()), this, SLOT(onDrawFilledTriangle()));
    connect(rhombusButton, SIGNAL(clicked()), this, SLOT(onDrawRhombus()));
    connect(dashedRhombusButton, SIGNAL(clicked()), this, SLOT(onDrawDashedRhombus()));
    connect(filledRhombusButton, SIGNAL(clicked()), this, SLOT(onDrawFilledRhombus()));

    connect(penButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(rectangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedRectangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(fillRectangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(lineButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedLineButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dotedLineButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(circleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedCircleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(fillCircleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(triangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedTriangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(filledTriangleButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(rhombusButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedRhombusButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(filledRhombusButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(penButton, 0, 0);
    menuLayout->addWidget(lineButton, 0, 1);
    menuLayout->addWidget(dashedLineButton, 1, 1);
    menuLayout->addWidget(dotedLineButton, 2, 1);
    menuLayout->addWidget(rectangleButton, 0, 2);
    menuLayout->addWidget(dashedRectangleButton, 1, 2);
    menuLayout->addWidget(fillRectangleButton, 2, 2);
    menuLayout->addWidget(circleButton, 0, 3);
    menuLayout->addWidget(dashedCircleButton, 1, 3);
    menuLayout->addWidget(fillCircleButton, 2, 3);
    menuLayout->addWidget(triangleButton, 0, 4);
    menuLayout->addWidget(dashedTriangleButton, 1, 4);
    menuLayout->addWidget(filledTriangleButton, 2, 4);
    menuLayout->addWidget(rhombusButton, 0, 5);
    menuLayout->addWidget(dashedRhombusButton, 1, 5);
    menuLayout->addWidget(filledRhombusButton, 2, 5);

    menu->setLayout(menuLayout);

    ui->drawButton->setMenu(menu);
}

void MainWindow::setColorMenu()
{
    auto menu = new QMenu();

    menu->setStyleSheet("QPushButton { "
                        "border:none;"
                        "width:30px;"
                        "height:30px;"
                        "border-radius: 5px;"
                        "}"
                        "QPushButton:hover {"
                        "cursor: pointer;"
                        "border-radius: 10px;"
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

    whiteButton->setCursor(Qt::PointingHandCursor);
    redButton->setCursor(Qt::PointingHandCursor);
    yellowButton->setCursor(Qt::PointingHandCursor);
    lightGreenButton->setCursor(Qt::PointingHandCursor);
    blueButton->setCursor(Qt::PointingHandCursor);

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

    purpleButton->setCursor(Qt::PointingHandCursor);
    pinkButton->setCursor(Qt::PointingHandCursor);
    orangeButton->setCursor(Qt::PointingHandCursor);
    greenButton->setCursor(Qt::PointingHandCursor);
    skyBlueButton->setCursor(Qt::PointingHandCursor);

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

    blackButton->setCursor(Qt::PointingHandCursor);
    darkRedButton->setCursor(Qt::PointingHandCursor);
    darkOrangeButton->setCursor(Qt::PointingHandCursor);
    darkGreenButton->setCursor(Qt::PointingHandCursor);
    darkBlueButton->setCursor(Qt::PointingHandCursor);

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

    menu->setLayout(menuLayout);

    ui->colorButton->setMenu(menu);
}

void MainWindow::setSizeMenu()
{
    auto menu = new QMenu();

    menu->setStyleSheet("QPushButton { "
                        "border:none;"
                        "qproperty-iconSize: 130px;"
                        "width:130px;"
                        "height:35px;"
                        "}"
                        "QPushButton:hover {"
                        "cursor: pointer;"
                        "border-radius: 5px;"
                        "background-color: #b3cccc;"
                        "}");

    auto menuLayout = new QGridLayout();

    auto smallButton = new QPushButton();
    auto normalButton = new QPushButton();
    auto mediumButton = new QPushButton();
    auto largeButton = new QPushButton();

    smallButton->setIcon(QIcon(":/icons/size/small.png"));
    normalButton->setIcon(QIcon(":/icons/size/normal.png"));
    mediumButton->setIcon(QIcon(":/icons/size/medium.png"));
    largeButton->setIcon(QIcon(":/icons/size/large.png"));

    smallButton->setCursor(Qt::PointingHandCursor);
    normalButton->setCursor(Qt::PointingHandCursor);
    mediumButton->setCursor(Qt::PointingHandCursor);
    largeButton->setCursor(Qt::PointingHandCursor);

    connect(smallButton, SIGNAL(clicked()), this, SLOT(onSmallSize()));
    connect(normalButton, SIGNAL(clicked()), this, SLOT(onNormalSize()));
    connect(mediumButton, SIGNAL(clicked()), this, SLOT(onMediumSize()));
    connect(largeButton, SIGNAL(clicked()), this, SLOT(onLargeSize()));

    connect(smallButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(normalButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(mediumButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(largeButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(smallButton, 0, 0, 1, 4);
    menuLayout->addWidget(normalButton, 1, 0, 1, 4);
    menuLayout->addWidget(mediumButton, 2, 0, 1, 4);
    menuLayout->addWidget(largeButton, 3, 0, 1, 4);

    menu->setLayout(menuLayout);

    ui->sizeButton->setMenu(menu);
}

void MainWindow::setSaveMenu()
{
    auto menu = new QMenu();

    menu->setStyleSheet("QPushButton {"
                        "padding: 5px 20px;"
                        "border:none;"
                        "qproperty-iconSize: 24px;"
                        "width:130px;"
                        "height:40px;"
                        "text-align: left;"
                        "font-weight: bold;"
                        "font-size:12px;"
                        "}"
                        "QPushButton:hover {"
                        "cursor: pointer;"
                        "border-radius: 5px;"
                        "background-color: #b3cccc;"
                        "}");

    auto menuLayout = new QGridLayout();

    auto imageButton = new QPushButton("Save as PNG");
    auto pdfButton = new QPushButton("Save as PDF");

    imageButton->setIcon(QIcon(":/icons/save/png.png"));
    pdfButton->setIcon(QIcon(":/icons/save/pdf.png"));

    imageButton->setCursor(Qt::PointingHandCursor);
    pdfButton->setCursor(Qt::PointingHandCursor);

    connect(imageButton, SIGNAL(clicked()), this, SLOT(saveAsImage()));
    connect(pdfButton, SIGNAL(clicked()), this, SLOT(saveAsPdf()));

    connect(imageButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(pdfButton, SIGNAL(clicked()), menu, SLOT(hide()));

    menuLayout->addWidget(imageButton, 0, 0, 1, 4);
    menuLayout->addWidget(pdfButton, 1, 0, 1, 4);

    menu->setLayout(menuLayout);

    ui->saveButton->setMenu(menu);
}

void MainWindow::onDraw(Phigure phigure, PhigureLine style, PhigureFill fill)
{
    PainterScene* scene = getCurrentScene();

    scene->setPhigure(phigure);
    scene->setPhigureLine(style);
    scene->setPhigureFill(fill);

    for (auto item : scene->items())
    {
        if(phigure == Phigure::Select)
        {
            item->setCursor(Qt::PointingHandCursor);

            item->setFlag(QGraphicsItem::ItemIsMovable);
            item->setFlag(QGraphicsItem::ItemIsSelectable);
        }
        else
        {
            item->unsetCursor();

            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        }
    }

    //undoStack->push(new SceneteItemCommand(scene, nullptr));

    reloadCustomCursor();
}

void MainWindow::setPenColor(QColor color)
{
    PainterScene* scene = getCurrentScene();

    scene->setPenColor(color);
}

void MainWindow::setPenSize(qreal size)
{
    PainterScene* scene = getCurrentScene();

    scene->setPenSize(size);

    reloadCustomCursor();
}

void MainWindow::saveCurrentScene(SaveType type)
{
    QString title("Save Scene ");
    QString fileName(("scene-" + std::to_string(sceneId + 1)).c_str());
    QString extensions("");

    switch (type)
    {
        case Image:
        {
            title.append("As Image");
            extensions.append("Images (*.png)");
            break;
        }

        case PDF:
        {
            title.append("As Pdf");
            extensions.append("PDF file (*.pdf)");
            break;
        }
    }

    PainterScene* scene = getCurrentScene();

    QString filePath = QFileDialog::getSaveFileName(this, title, fileName, extensions);

    saver->saveScene(scene, filePath, type);
}

void MainWindow::createUndoStackAndActions()
{
    undoStack = new QUndoStack(this);

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setIcon(QIcon(":/icons/undo.png"));

    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setIcon(QIcon(":/icons/redo.png"));

    ui->undoButton->setDefaultAction(undoAction);
    ui->redoButton->setDefaultAction(redoAction);
}

QPixmap MainWindow::drawPattern(int step, const QColor &color)
{
    QPixmap pixmap(step, step);
    QPainter painter;

    int pixmapWidth = pixmap.width() - 1;

    pixmap.fill(Qt::transparent);

    painter.begin(&pixmap);

    painter.setPen(color);
    painter.drawLine(0, 0, pixmapWidth, 0);
    painter.drawLine(0, 0, 0, pixmapWidth);

    return pixmap;
}

void MainWindow::slotTimer()
{
    timer->stop();
    getCurrentScene()->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
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
    onDraw(Phigure::Rectangle, PhigureLine::DashedLine);
}

void MainWindow::onDrawFillRectangle()
{
    onDraw(Phigure::Rectangle, PhigureLine::SolidLine, PhigureFill::Full);
}

void MainWindow::onDrawLine()
{
    onDraw(Phigure::Line);
}

void MainWindow::onDrawDashLine()
{
    onDraw(Phigure::Line, PhigureLine::DashedLine);
}

void MainWindow::onDrawDotLine()
{
    onDraw(Phigure::Line, PhigureLine::DotLine);
}

void MainWindow::onDrawCircle()
{
    onDraw(Phigure::Circle);
}

void MainWindow::onDrawDashCircle()
{
    onDraw(Phigure::Circle, PhigureLine::DashedLine);
}

void MainWindow::onDrawFillCircle()
{
    onDraw(Phigure::Circle, PhigureLine::SolidLine, PhigureFill::Full);
}

void MainWindow::onDrawTriangle()
{
    onDraw(Phigure::Triangle);
}

void MainWindow::onDrawDashedTriangle()
{
    onDraw(Phigure::Triangle, PhigureLine::DashedLine, PhigureFill::Border);
}

void MainWindow::onDrawFilledTriangle()
{
    onDraw(Phigure::Triangle, PhigureLine::SolidLine, PhigureFill::Full);
}

void MainWindow::onDrawRhombus()
{
    onDraw(Phigure::Rhombus);
}

void MainWindow::onDrawDashedRhombus()
{
    onDraw(Phigure::Rhombus, PhigureLine::DashedLine, PhigureFill::Border);
}

void MainWindow::onDrawFilledRhombus()
{
    onDraw(Phigure::Rhombus, PhigureLine::SolidLine, PhigureFill::Full);
}

void MainWindow::onSetWhiteColor()
{
    setPenColor(Qt::white);
}

void MainWindow::onSetRedColor()
{
    setPenColor(QColor(255, 25, 25, 255));
}

void MainWindow::onSetYellowColor()
{
    setPenColor(QColor(255, 222, 50, 255));
}

void MainWindow::onSetLightGreenColor()
{
    setPenColor(QColor(130, 199, 134, 255));
}

void MainWindow::onSetBlueColor()
{
    setPenColor(QColor(46, 140, 255, 255));
}

void MainWindow::onSetPurpleColor()
{
    setPenColor(QColor(180, 121, 255, 255));
}

void MainWindow::onSetPinkColor()
{
    setPenColor(QColor(255, 56, 199, 255));
}

void MainWindow::onSetOrangeColor()
{
    setPenColor(QColor(255, 138, 0, 255));
}

void MainWindow::onSetGreenColor()
{
    setPenColor(QColor(73, 214, 30, 255));
}

void MainWindow::onSetSkyBlueColor()
{
    setPenColor(QColor(81, 216, 235, 255));
}

void MainWindow::onSetBlackColor()
{
    setPenColor(QColor(0, 0, 0, 255));
}

void MainWindow::onSetDarkRedColor()
{
    setPenColor(QColor(127, 0, 0, 255));
}

void MainWindow::onSetDarkOrangeColor()
{
    setPenColor(QColor(119, 68, 8, 255));
}

void MainWindow::onSetDarkGreenColor()
{
    setPenColor(QColor(11, 114, 40, 255));
}

void MainWindow::onSetDarkBlueColor()
{
    setPenColor(QColor(20, 79, 195, 255));
}

void MainWindow::onSmallSize()
{
    setPenSize(1);
}

void MainWindow::onNormalSize()
{
    setPenSize(2);
}

void MainWindow::onMediumSize()
{
    setPenSize(3);
}

void MainWindow::onLargeSize()
{
    setPenSize(5);
}

void MainWindow::saveAsImage()
{
    saveCurrentScene(SaveType::Image);
}

void MainWindow::saveAsPdf()
{
    saveCurrentScene(SaveType::PDF);
}

void MainWindow::onSetGrid()
{
    if(isGridVisible)
    {
        // hide
        ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
        ui->showGridButton->setText("Show Grid");
    }
    else
    {
        // show
        ui->graphicsView->setBackgroundBrush(drawPattern(20, QColor(0, 0, 0, 30)));
        ui->showGridButton->setText("Hide Grid");
    }

    isGridVisible = !isGridVisible;
}

PainterScene* MainWindow::getCurrentScene()
{
    return scenes.at(sceneId);
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
    }
}

void MainWindow::nextScene()
{
    PainterScene* scene = getCurrentScene();

    QColor color = scene->getPenColor();
    int real = scene->getPenSize();
    Phigure phigure = scene->getPhigure();
    PhigureLine phigureLine = scene->getPhigureLine();
    PhigureFill phigureFill = scene->getPhigureFill();

    sceneId++;

    if(sceneId >= scenes.size())
    {
        PainterScene* nextScene = new PainterScene();

        nextScene->setSceneRect(scene->sceneRect());

        scenes.push_back(nextScene);
    }

    scene = getCurrentScene();

    scene->setPenSize(real);
    scene->setPenColor(color);
    scene->setPhigure(phigure);
    scene->setPhigureLine(phigureLine);
    scene->setPhigureFill(phigureFill);

    ui->graphicsView->setScene(scene);

    ui->sceneLabel->setText(tr((std::to_string(sceneId + 1)).c_str()));
}

void MainWindow::prevScene()
{
    if(sceneId > 0)
    {
        PainterScene* scene = getCurrentScene();

        QColor color = scene->getPenColor();
        int real = scene->getPenSize();
        Phigure phigure = scene->getPhigure();
        PhigureLine phigureLine = scene->getPhigureLine();
        PhigureFill phigureFill = scene->getPhigureFill();

        sceneId--;

        scene = getCurrentScene();

        scene->setPenSize(real);
        scene->setPenColor(color);
        scene->setPhigure(phigure);
        scene->setPhigureLine(phigureLine);
        scene->setPhigureFill(phigureFill);

        ui->graphicsView->setScene(scene);

        ui->sceneLabel->setText(tr((std::to_string(sceneId+1)).c_str()));
    }
}

void MainWindow::onSelect()
{
    onDraw(Phigure::Select, PhigureLine::SolidLine);
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
    else if(key == Qt::Key_Delete)
    {
        PainterScene* scene = getCurrentScene();

        Phigure phigure = scene->getPhigure();

        if(phigure == Phigure::Select)
        {
            for (auto selectedItem : scene->selectedItems())
            {
                scene->removeItem(selectedItem);
            }
        }
    }
}

void MainWindow::reloadCustomCursor()
{
    PainterScene* scene = getCurrentScene();

    Phigure phigure = scene->getPhigure();
    int size = scene->getPenSize();

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
            string filename = ":/cursors/";

            if(size == 1)
            {
                filename += "eraser_small.png";
            }
            else if(size == 2)
            {
                filename += "eraser_normal.png";
            }
            else if(size == 3)
            {
                filename += "eraser_medium.png";
            }
            else
            {
                filename += "eraser_large.png";
            }

            QImage image(filename.c_str());

            QSize imageSize = image.size();

            cursor = QCursor(QPixmap::fromImage(image), imageSize.width() / 2, imageSize.height() / 2);
            break;
        }

        case Phigure::Select:
        {
            cursor = Qt::ArrowCursor;
            break;
        }

        default:
        {
            cursor = Qt::CrossCursor;
            break;
        }
    }

    ui->graphicsView->viewport()->setCursor(cursor);
}
