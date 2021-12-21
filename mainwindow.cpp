#include <string>
#include <QPainter>
#include <QFileDialog>
#include <QActionGroup>
#include <QWidgetAction>
#include <QFontDatabase>

#include <qmessagebox.h>

#include "ui_mainwindow.h"
#include "mainwindow.h"
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

    connect(ui->previousSceneButton, SIGNAL(clicked()), this, SLOT(prevScene()));
    connect(ui->nextSceneButton, SIGNAL(clicked()), this, SLOT(nextScene()));
    connect(ui->createSceneButton, SIGNAL(clicked()), this, SLOT(createScene()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));

    QActionGroup* menuActionGroup = new QActionGroup(this);

    selectAction = menuActionGroup->addAction(QIcon(":/icons/select.png"), "Select");
    drawAction = menuActionGroup->addAction(QIcon(":/icons/draw.png"), "Draw");
    eraserAction = menuActionGroup->addAction(QIcon(":/icons/eraser.png"), "Eraser");

    showGridlinesAction = new QAction(QIcon(":/icons/grid.png"), "Show Grid", this);

    selectAction->setCheckable(true);
    drawAction->setCheckable(true);
    eraserAction->setCheckable(true);
    showGridlinesAction->setCheckable(true);

    drawAction->setChecked(true);

    connect(selectAction, SIGNAL(triggered()), this, SLOT(onSelect()));
    connect(eraserAction, SIGNAL(triggered()), this, SLOT(onClean()));
    connect(showGridlinesAction, SIGNAL(triggered()), this, SLOT(onSetGrid()));

    ui->selectButton->setDefaultAction(selectAction);
    ui->drawButton->setDefaultAction(drawAction);
    ui->eraserButton->setDefaultAction(eraserAction);
    ui->showGridButton->setDefaultAction(showGridlinesAction);

    setDrawMenu();
    setColorMenu();
    setSizeMenu();
    setSaveMenu();
    createUndoStackAndActions();

    setCentralWidget(ui->graphicsView);

    ui->headerGroupBox->setParent(centralWidget());
    ui->footerGroupBox->setParent(centralWidget());

    goToScene(0);
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

    menu->setStyleSheet("QToolButton { "
                        "border:none;"
                        "qproperty-iconSize: 24px;"
                        "width:35px;"
                        "height:35px;"
                        "}"
                        "QToolButton:hover {"
                        "border-radius: 5px;"
                        "background-color: rgba(179, 204, 204, 0.5);"
                        "}"
                        "QToolButton:checked {"
                        "border-radius: 5px;"
                        "background-color: rgba(179, 204, 204, 1);"
                        "}");

    auto menuLayout = new QGridLayout();

    auto penButton = new QToolButton();
    auto rectangleButton = new QToolButton();
    auto dashedRectangleButton = new QToolButton();
    auto fillRectangleButton = new QToolButton();
    auto lineButton = new QToolButton();
    auto dashedLineButton = new QToolButton();
    auto dotedLineButton = new QToolButton();
    auto circleButton = new QToolButton();
    auto dashedCircleButton = new QToolButton();
    auto fillCircleButton = new QToolButton();
    auto triangleButton = new QToolButton();
    auto dashedTriangleButton = new QToolButton();
    auto filledTriangleButton = new QToolButton();
    auto rhombusButton = new QToolButton();
    auto dashedRhombusButton = new QToolButton();
    auto filledRhombusButton = new QToolButton();
    auto parallelogramButton = new QToolButton();
    auto dashedParallelogramButton = new QToolButton();
    auto filledParallelogramButton = new QToolButton();

    drawActionGroup = new QActionGroup(this);

    QAction* penAction = drawActionGroup->addAction(QIcon(":/icons/draw/pen.png"), "");

    QAction* rectangleAction = drawActionGroup->addAction(QIcon(":/icons/draw/rectangle.png"), "");
    QAction* rectangleDashedAction = drawActionGroup->addAction(QIcon(":/icons/draw/rectangle_dashed.png"), "");
    QAction* rectangleFilledAction = drawActionGroup->addAction(QIcon(":/icons/draw/rectangle_filled.png"), "");

    QAction* lineAction = drawActionGroup->addAction(QIcon(":/icons/draw/line.png"), "");
    QAction* lineDashedAction = drawActionGroup->addAction(QIcon(":/icons/draw/line_dashed.png"), "");
    QAction* lineDotedAction = drawActionGroup->addAction(QIcon(":/icons/draw/line_doted.png"), "");

    QAction* circleAction = drawActionGroup->addAction(QIcon(":/icons/draw/circle.png"), "");
    QAction* circleDashedAction = drawActionGroup->addAction(QIcon(":/icons/draw/circle_dashed.png"), "");
    QAction* circleFilledAction = drawActionGroup->addAction(QIcon(":/icons/draw/circle_filled.png"), "");

    QAction* triangleAction = drawActionGroup->addAction(QIcon(":/icons/draw/triangle.png"), "");
    QAction* triangleDashedAction = drawActionGroup->addAction(QIcon(":/icons/draw/triangle_dashed.png"), "");
    QAction* triangleFilledAction = drawActionGroup->addAction(QIcon(":/icons/draw/triangle_filled.png"), "");

    QAction* rhombusAction = drawActionGroup->addAction(QIcon(":/icons/draw/rhombus.png"), "");
    QAction* rhombusDashedAction = drawActionGroup->addAction(QIcon(":/icons/draw/rhombus_dashed.png"), "");
    QAction* rhombusFilledAction = drawActionGroup->addAction(QIcon(":/icons/draw/rhombus_filled.png"), "");

    QAction* parallelogramAction = drawActionGroup->addAction(QIcon(":/icons/draw/parallelogram.png"), "");
    QAction* parallelogramDashedAction = drawActionGroup->addAction(QIcon(":/icons/draw/parallelogram_dashed.png"), "");
    QAction* parallelogramFilledAction = drawActionGroup->addAction(QIcon(":/icons/draw/parallelogram_filled.png"), "");

    penAction->setCheckable(true);

    rectangleAction->setCheckable(true);
    rectangleDashedAction->setCheckable(true);
    rectangleFilledAction->setCheckable(true);

    lineAction->setCheckable(true);
    lineDashedAction->setCheckable(true);
    lineDotedAction->setCheckable(true);

    circleAction->setCheckable(true);
    circleDashedAction->setCheckable(true);
    circleFilledAction->setCheckable(true);

    triangleAction->setCheckable(true);
    triangleDashedAction->setCheckable(true);
    triangleFilledAction->setCheckable(true);

    rhombusAction->setCheckable(true);
    rhombusDashedAction->setCheckable(true);
    rhombusFilledAction->setCheckable(true);

    parallelogramAction->setCheckable(true);
    parallelogramDashedAction->setCheckable(true);
    parallelogramFilledAction->setCheckable(true);

    penAction->setChecked(true);

    connect(penAction, SIGNAL(triggered()), this, SLOT(onDrawPen()));

    connect(rectangleAction, SIGNAL(triggered()), this, SLOT(onDrawRectangle()));
    connect(rectangleDashedAction, SIGNAL(triggered()), this, SLOT(onDrawDashRectangle()));
    connect(rectangleFilledAction, SIGNAL(triggered()), this, SLOT(onDrawFillRectangle()));

    connect(lineAction, SIGNAL(triggered()), this, SLOT(onDrawLine()));
    connect(lineDashedAction, SIGNAL(triggered()), this, SLOT(onDrawDashLine()));
    connect(lineDotedAction, SIGNAL(triggered()), this, SLOT(onDrawDotLine()));

    connect(circleAction, SIGNAL(triggered()), this, SLOT(onDrawCircle()));
    connect(circleDashedAction, SIGNAL(triggered()), this, SLOT(onDrawDashCircle()));
    connect(circleFilledAction, SIGNAL(triggered()), this, SLOT(onDrawFillCircle()));

    connect(triangleAction, SIGNAL(triggered()), this, SLOT(onDrawTriangle()));
    connect(triangleDashedAction, SIGNAL(triggered()), this, SLOT(onDrawDashedTriangle()));
    connect(triangleFilledAction, SIGNAL(triggered()), this, SLOT(onDrawFilledTriangle()));

    connect(rhombusAction, SIGNAL(triggered()), this, SLOT(onDrawRhombus()));
    connect(rhombusDashedAction, SIGNAL(triggered()), this, SLOT(onDrawDashedRhombus()));
    connect(rhombusFilledAction, SIGNAL(triggered()), this, SLOT(onDrawFilledRhombus()));

    connect(parallelogramAction, SIGNAL(triggered()), this, SLOT(onDrawParallelogram()));
    connect(parallelogramDashedAction, SIGNAL(triggered()), this, SLOT(onDrawDashedParallelogram()));
    connect(parallelogramFilledAction, SIGNAL(triggered()), this, SLOT(onDrawFilledParallelogram()));

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
    connect(parallelogramButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(dashedParallelogramButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(filledParallelogramButton, SIGNAL(clicked()), menu, SLOT(hide()));

    penButton->setDefaultAction(penAction);

    rectangleButton->setDefaultAction(rectangleAction);
    dashedRectangleButton->setDefaultAction(rectangleDashedAction);
    fillRectangleButton->setDefaultAction(rectangleFilledAction);

    lineButton->setDefaultAction(lineAction);
    dashedLineButton->setDefaultAction(lineDashedAction);
    dotedLineButton->setDefaultAction(lineDotedAction);

    circleButton->setDefaultAction(circleAction);
    dashedCircleButton->setDefaultAction(circleDashedAction);
    fillCircleButton->setDefaultAction(circleFilledAction);

    triangleButton->setDefaultAction(triangleAction);
    dashedTriangleButton->setDefaultAction(triangleDashedAction);
    filledTriangleButton->setDefaultAction(triangleFilledAction);

    rhombusButton->setDefaultAction(rhombusAction);
    dashedRhombusButton->setDefaultAction(rhombusDashedAction);
    filledRhombusButton->setDefaultAction(rhombusFilledAction);

    parallelogramButton->setDefaultAction(parallelogramAction);
    dashedParallelogramButton->setDefaultAction(parallelogramDashedAction);
    filledParallelogramButton->setDefaultAction(parallelogramFilledAction);

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
    menuLayout->addWidget(parallelogramButton, 0, 6);
    menuLayout->addWidget(dashedParallelogramButton, 1, 6);
    menuLayout->addWidget(filledParallelogramButton, 2, 6);

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

    menu->setLayout(menuLayout);

    ui->colorButton->setMenu(menu);
}

void MainWindow::setSizeMenu()
{
    auto menu = new QMenu();

    menu->setStyleSheet("QToolButton { "
                        "border:none;"
                        "qproperty-iconSize: 130px;"
                        "width:130px;"
                        "height:35px;"
                        "}"
                        "QToolButton:hover {"
                        "border-radius: 5px;"
                        "background-color: rgba(179, 204, 204, 0.5);"
                        "}"
                        "QToolButton:checked {"
                        "border-radius: 5px;"
                        "background-color: rgba(179, 204, 204, 1);"
                        "}");

    auto menuLayout = new QGridLayout();

    auto smallButton = new QToolButton();
    auto normalButton = new QToolButton();
    auto mediumButton = new QToolButton();
    auto largeButton = new QToolButton();

    QActionGroup* menuActionGroup = new QActionGroup(this);

    QAction* smallAction = menuActionGroup->addAction(QIcon(":/icons/size/small.png"), "");
    QAction* normalAction = menuActionGroup->addAction(QIcon(":/icons/size/normal.png"), "");
    QAction* mediumAction = menuActionGroup->addAction(QIcon(":/icons/size/medium.png"), "");
    QAction* largeAction = menuActionGroup->addAction(QIcon(":/icons/size/large.png"), "");

    smallAction->setCheckable(true);
    normalAction->setCheckable(true);
    mediumAction->setCheckable(true);
    largeAction->setCheckable(true);

    normalAction->setChecked(true);

    connect(smallAction, SIGNAL(triggered()), this, SLOT(onSmallSize()));
    connect(normalAction, SIGNAL(triggered()), this, SLOT(onNormalSize()));
    connect(mediumAction, SIGNAL(triggered()), this, SLOT(onMediumSize()));
    connect(largeAction, SIGNAL(triggered()), this, SLOT(onLargeSize()));

    connect(smallButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(normalButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(mediumButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(largeButton, SIGNAL(clicked()), menu, SLOT(hide()));

    smallButton->setDefaultAction(smallAction);
    normalButton->setDefaultAction(normalAction);
    mediumButton->setDefaultAction(mediumAction);
    largeButton->setDefaultAction(largeAction);

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
                        "width:60px;"
                        "height:40px;"
                        "text-align: left;"
                        "font-weight: bold;"
                        "font-size:12px;"
                        "}"
                        "QPushButton:hover {"
                        "border-radius: 5px;"
                        "background-color: rgba(179, 204, 204, 0.5);"
                        "}");

    auto menuLayout = new QGridLayout();

    auto currentScenePNGButton = new QPushButton("PNG");
    auto currentScenePDFButton = new QPushButton("PDF");
    auto allScenePNGButton = new QPushButton("PNG");
    auto allScenePDFButton = new QPushButton("PDF");

    currentScenePNGButton->setIcon(QIcon(":/icons/save/png.png"));
    currentScenePDFButton->setIcon(QIcon(":/icons/save/pdf.png"));
    allScenePNGButton->setIcon(QIcon(":/icons/save/png.png"));
    allScenePDFButton->setIcon(QIcon(":/icons/save/pdf.png"));

    connect(currentScenePNGButton, SIGNAL(clicked()), this, SLOT(saveAsImage()));
    connect(currentScenePDFButton, SIGNAL(clicked()), this, SLOT(saveAsPdf()));
    connect(allScenePNGButton, SIGNAL(clicked()), this, SLOT(saveAllAsImage()));
    connect(allScenePDFButton, SIGNAL(clicked()), this, SLOT(saveAllAsPdf()));

    connect(currentScenePNGButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(currentScenePDFButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(allScenePNGButton, SIGNAL(clicked()), menu, SLOT(hide()));
    connect(allScenePDFButton, SIGNAL(clicked()), menu, SLOT(hide()));

    auto currentSceneLabel = new QLabel("Current scene:");
    auto allSceneLabel = new QLabel("All scenes:");

    currentSceneLabel->setStyleSheet("font-weight: bold;");
    allSceneLabel->setStyleSheet("font-weight: bold;");

    menuLayout->addWidget(currentSceneLabel, 0, 0, 1, 2);
    menuLayout->addWidget(currentScenePNGButton, 1, 0, 1, 1);
    menuLayout->addWidget(currentScenePDFButton, 1, 1, 1, 1);
    menuLayout->addWidget(allSceneLabel, 2, 0, 1, 2);
    menuLayout->addWidget(allScenePNGButton, 3, 0, 1, 1);
    menuLayout->addWidget(allScenePDFButton, 3, 1, 1, 1);

    menu->setLayout(menuLayout);

    ui->saveButton->setMenu(menu);
}

void MainWindow::onDraw(Phigure phigure, PhigureLine style, PhigureFill fill)
{
    if(phigure == Phigure::Select ||
       phigure == Phigure::Cleaner)
    {
        for (QAction* action : drawActionGroup->actions())
        {
            action->setChecked(false);
        }
    }

    if(phigure == Phigure::Select)
    {
        selectAction->setChecked(true);
        ui->colorButton->setDisabled(true);
        ui->sizeButton->setDisabled(true);
    }
    else if(phigure == Phigure::Cleaner)
    {
        eraserAction->setChecked(true);
        ui->colorButton->setDisabled(true);
        ui->sizeButton->setDisabled(false);
    }
    else
    {
        drawAction->setChecked(true);
        ui->colorButton->setDisabled(false);
        ui->sizeButton->setDisabled(false);
    }

    PainterScene* scene = getCurrentScene();

    scene->setPhigure(phigure);
    scene->setPhigureLine(style);
    scene->setPhigureFill(fill);

    for (auto item : scene->items())
    {
        if(phigure == Phigure::Select)
        {
            item->setCursor(Qt::SizeAllCursor);

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
    PainterScene* scene = getCurrentScene();

    undoAction = scene->undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setIcon(QIcon(":/icons/undo.png"));

    redoAction = scene->undoStack->createRedoAction(this, tr("&Redo"));
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
    getCurrentScene()->setSceneRect(0, 0, centralWidget()->width() - 2, centralWidget()->height() - 2);

    ui->headerGroupBox->move(centralWidget()->width() / 2 - ui->headerGroupBox->width() / 2,
                             10);

    ui->footerGroupBox->move(centralWidget()->width() - ui->footerGroupBox->width() - 10,
                             centralWidget()->height() - ui->footerGroupBox->height() - 10);
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

void MainWindow::onDrawParallelogram()
{
    onDraw(Phigure::Parallelogram);
}

void MainWindow::onDrawDashedParallelogram()
{
    onDraw(Phigure::Parallelogram, PhigureLine::DashedLine);
}

void MainWindow::onDrawFilledParallelogram()
{
    onDraw(Phigure::Parallelogram, PhigureLine::SolidLine, PhigureFill::Full);
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

void MainWindow::saveAllAsImage()
{
    QString title("Save All Scenes As Images");

    QString folderPath = QFileDialog::getExistingDirectory(this, title);

    saver->saveScenes(scenes, folderPath, SaveType::Image);
}

void MainWindow::saveAllAsPdf()
{
    QString title("Save All Scenes As Pdf");
    QString fileName("scenes");
    QString extensions("PDF file (*.pdf)");

    QString filePath = QFileDialog::getSaveFileName(this, title, fileName, extensions);

    saver->saveScenes(scenes, filePath, SaveType::PDF);
}

void MainWindow::onSetGrid()
{
    if(isGridVisible)
    {
        // hide
        ui->graphicsView->setBackgroundBrush(Qt::NoBrush);
        showGridlinesAction->setText("Show Grid");
    }
    else
    {
        // show
        ui->graphicsView->setBackgroundBrush(drawPattern(20, QColor(0, 0, 0, 30)));
        showGridlinesAction->setText("Hide Grid");
    }

    isGridVisible = !isGridVisible;

    showGridlinesAction->setChecked(isGridVisible);
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
        getCurrentScene()->clearScene();
    }
}

void MainWindow::goToScene(int sceneId)
{
    PainterScene* scene = getCurrentScene();

    QColor color = scene->getPenColor();
    int real = scene->getPenSize();
    Phigure phigure = scene->getPhigure();
    PhigureLine phigureLine = scene->getPhigureLine();
    PhigureFill phigureFill = scene->getPhigureFill();

    this->sceneId = sceneId;

    scene = getCurrentScene();

    scene->clearSelection();

    scene->setPenSize(real);
    scene->setPenColor(color);
    scene->setPhigure(phigure);
    scene->setPhigureLine(phigureLine);
    scene->setPhigureFill(phigureFill);

    ui->graphicsView->setScene(scene);

    bool isPrevNextButtonsVisible = scenes.size() != 1;

    ui->previousSceneButton->setVisible(isPrevNextButtonsVisible);
    ui->nextSceneButton->setVisible(isPrevNextButtonsVisible);
    ui->sceneLabel->setVisible(isPrevNextButtonsVisible);

    if(isPrevNextButtonsVisible)
    {
        int sceneNumber = sceneId + 1;

        ui->sceneLabel->setText(tr((std::to_string(sceneNumber)).c_str()));

        ui->previousSceneButton->setEnabled(sceneNumber > 1);
        ui->nextSceneButton->setEnabled(sceneNumber < scenes.size());

        ui->footerGroupBox->setMinimumWidth(215);

        ui->footerGroupBox->move(centralWidget()->width() - ui->footerGroupBox->width() - 10,
                                 centralWidget()->height() - ui->footerGroupBox->height() - 10);
    }

    createUndoStackAndActions();
}

void MainWindow::nextScene()
{
    if(sceneId + 1 < scenes.size())
    {
        goToScene(sceneId + 1);
    }
}

void MainWindow::prevScene()
{
    if(sceneId > 0)
    {
        goToScene(sceneId - 1);
    }
}

void MainWindow::createScene()
{
    PainterScene* currentScene = getCurrentScene();
    PainterScene* nextScene = new PainterScene();

    nextScene->setSceneRect(currentScene->sceneRect());

    scenes.push_back(nextScene);

    goToScene(scenes.size() - 1);
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
    auto modifiers = event->modifiers();

    PainterScene* scene = getCurrentScene();

    // previous scene
    if(key == Qt::Key_Left)
    {
        prevScene();
    }
    // next scene
    else if(key == Qt::Key_Right)
    {
        nextScene();
    }
    // delete
    else if(key == Qt::Key_Delete)
    {
        Phigure phigure = scene->getPhigure();

        if(phigure == Phigure::Select)
        {
            for (auto selectedItem : scene->selectedItems())
            {
                scene->removeItem(selectedItem);
            }
        }
    }
    // undo
    else if(modifiers == Qt::ControlModifier && key == Qt::Key_Z)
    {
        scene->undoStack->undo();
    }
    // redo
    else if(modifiers == Qt::ControlModifier && key == Qt::Key_Y)
    {
        scene->undoStack->redo();
    }
    // copy
    else if(modifiers == Qt::ControlModifier && key == Qt::Key_C)
    {
        this->selectedItemsToCopy.clear();

        for (QGraphicsItem* item : scene->selectedItems())
        {
            QGraphicsItem* copyItem = scene->createCopy(item);

            if(copyItem)
            {
                this->selectedItemsToCopy.append(copyItem);
            }
        }
    }
    // paste
    else if(modifiers == Qt::ControlModifier && key == Qt::Key_V)
    {
        if(!this->selectedItemsToCopy.isEmpty())
        {
            scene->clearSelection();

            QList<QGraphicsItem*> copiedItems;

            for (QGraphicsItem* item : this->selectedItemsToCopy)
            {
                QGraphicsItem* copyItem = scene->createCopy(item);

                if(copyItem)
                {
                    scene->addItem(copyItem);

                    copiedItems.append(copyItem);
                }
            }

            onSelect();

            for (QGraphicsItem* item : copiedItems)
            {
                item->moveBy(20, 30);
                item->setSelected(true);
            }

            scene->undoStack->push(new PasteSceneCommand(scene, copiedItems));
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
