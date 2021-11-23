#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QPainter>
#include <QFileDialog>
#include <qpixmap.h>
#include <qdebug.h>
#include <qpdfwriter.h>
#include <qmessagebox.h>
#include <locale>

#include "saver.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene.push_back(new PainterScene());

    onDrawPen();
    onNormalSize();

    //Scene()->setPhigure(Phigure::Pen);
    //Scene()->setSize(2);
    Scene()->setColor(Qt::black);

    ui->graphicsView->setScene(Scene());
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);

    connect(ui->eraserButton, SIGNAL(clicked()), this, SLOT(Cleaner()));
    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->buttonLeft, SIGNAL(clicked()), this, SLOT(prevScene()));
    connect(ui->buttonRigth, SIGNAL(clicked()), this, SLOT(nextScene()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveInImage()));

    Scene()->setBackgroundBrush(QBrush(Qt::white));

    setDrawMenu();
    setSizeMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDrawMenu()
{
    QMenu *menu = new QMenu(this);

    menu->addAction(QIcon(":/icons/draw/pen.png"),
                    "Pen",
                    this,
                    SLOT(onDrawPen()));

    menu->addAction(QIcon(":/icons/draw/rectangle.png"),
                    "Rectangle",
                    this,
                    SLOT(onDrawRectangle()));

    menu->addAction(QIcon(":/icons/draw/rectangle_dashed.png"),
                    "Dashed rectangle",
                    this,
                    SLOT(onDrawDashRectangle()));

    menu->addAction(QIcon(":/icons/draw/line.png"),
                    "Line",
                    this,
                    SLOT(onDrawLine()));

    menu->addAction(QIcon(":/icons/draw/line_dashed.png"),
                    "Dashed Line",
                    this,
                    SLOT(onDrawDashLine()));

    menu->addAction(QIcon(":/icons/draw/circle.png"),
                    "Circle",
                    this,
                    SLOT(onDrawCircle()));

    menu->addAction(QIcon(":/icons/draw/circle_dashed.png"),
                    "Dashed Circle",
                    this,
                    SLOT(onDrawDashCircle()));

    ui->drawButton->setMenu(menu);
}

void MainWindow::setSizeMenu()
{
    QMenu *menu = new QMenu(this);

    menu->addAction(QIcon(":/icons/size/small.png"),
                    "Small",
                    this,
                    SLOT(onSmallSize()));

    menu->addAction(QIcon(":/icons/size/normal.png"),
                    "Normal",
                    this,
                    SLOT(onNormalSize()));

    menu->addAction(QIcon(":/icons/size/medium.png"),
                    "Medium",
                    this,
                    SLOT(onMediumSize()));

    menu->addAction(QIcon(":/icons/size/large.png"),
                    "Large",
                    this,
                    SLOT(onLargeSize()));

    ui->sizeButton->setMenu(menu);
}

void MainWindow::onDraw(Phigure phigure)
{
    Scene()->setPhigure(phigure);

    reloadCustomCursor();
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
    Scene()->setSize(1);
}

void MainWindow::onNormalSize()
{
    Scene()->setSize(2);
}

void MainWindow::onMediumSize()
{
    Scene()->setSize(5);
}

void MainWindow::onLargeSize()
{
    Scene()->setSize(10);
}

void MainWindow::slotTimer()
{
    timer->stop();
    Scene()->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void MainWindow::Cleaner(){

    Scene()->setPhigure(Phigure::Cleaner);

    reloadCustomCursor();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);

    ui->graphicsView->resize(event->size().width() - 20, event->size().height() - 144);

    QWidget::resizeEvent(event);
}

void MainWindow::setColor()
{
    QColor color = QColorDialog::getColor(Scene()->getColor(),
                                          this,
                                          "Color",
                                          QColorDialog::ColorDialogOption::ShowAlphaChannel);

    Scene()->setColor(color);
}

void MainWindow::clearScene()
{
    Scene()->clear();
    Scene()->initialize();
}

void MainWindow::nextScene()
{
    QColor color = Scene()->getColor();
    int real = Scene()->getSize();
    QRectF rect = Scene()->sceneRect();
    Phigure phigure = Scene()->getPhigure();

    sceneId++;

    if(sceneId >= scene.size())
    {
        scene.push_back(new PainterScene());

        Scene()->setSize(real);
        Scene()->setColor(color);

        Scene()->setSceneRect(rect);
        Scene()->setBackgroundBrush(scene.at(sceneId - 1)->backgroundBrush());
    }
    else
    {
        Scene()->setSize(real);
        Scene()->setColor(color);
    }

    Scene()->setPhigure(phigure);
    ui->graphicsView->setScene(Scene());

    ui->label_2->setText(tr((std::to_string(sceneId+1)).c_str()));
}

void MainWindow::prevScene()
{
    if(sceneId > 0)
    {
        QColor color = Scene()->getColor();
        int real = Scene()->getSize();
        Phigure phigure = Scene()->getPhigure();

        sceneId--;

        Scene()->setSize(real);
        Scene()->setColor(color);
        Scene()->setPhigure(phigure);
        Scene()->setSceneRect(Scene()->sceneRect());

        ui->graphicsView->setScene(Scene());

        ui->label_2->setText(tr((std::to_string(sceneId+1)).c_str()));
    }
}

void MainWindow::saveInImage()
{
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 "Save Image",
                                                 "untitled.png",
                                                 "Images (*.png)");

    Saver::SaveImage(Scene(), fileName, ui->graphicsView->sizeHint() * 2);
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

    //    if(event->key() == Qt::Key_Control){

    //      Scene()->phigure = Phigure::cleaner;

    //    }else
    //    if(event->key() == Qt::Key_Z){

    //      Scene()->phigure = Phigure::pen;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);
    //    }else
    //    if(event->key() == Qt::Key_X){

    //      Scene()->phigure = Phigure::rectangle;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);

    //    }else
    //    if(event->key() == Qt::Key_S){

    //      Scene()->phigure = Phigure::dash_rectangle;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);

    //    }else
    //    if(event->key() == Qt::Key_C){

    //      Scene()->phigure = Phigure::line;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);

    //    }else
    //    if(event->key() == Qt::Key_D){

    //      Scene()->phigure = Phigure::dash_line;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);

    //    }else
    //    if(event->key() == Qt::Key_V){

    //      Scene()->phigure = Phigure::circle;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);

    //    }else
    //    if(event->key() == Qt::Key_F){

    //      Scene()->phigure = Phigure::dash_circle;
    //      ui->comboBox->setCurrentIndex(Scene()->phigure);

    //    }else

    if(key == Qt::Key_Left){

        prevScene();
    }
    else if(key == Qt::Key_Right)
    {
        nextScene();
    }
    //    else
    //    if(event->key() >= 49 && event->key() < 55){

    //        int index = event->key() - 49;

    //        Scene()->size = ui->comboBox_2->itemText(index).toDouble();
    //        ui->comboBox_2->setCurrentIndex(index);
    //    }

    //    reloadCustomCursor();
}

void MainWindow::reloadCustomCursor()
{
    auto phigure = Scene()->getPhigure();

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
