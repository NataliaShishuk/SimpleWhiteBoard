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

    Scene()->setPhigure(Phigure::Pen);
    Scene()->setSize(2);
    Scene()->setColor(Qt::black);

    ui->graphicsView->setScene(Scene());
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    standart_cursor.load(":/new/cursor/cursor.png");
    circle_cursor.load(":/new/cursor/circle_cursor.png");
    cleaner_cursor.load(":/icons/eraser.png");
    rectangle_cursor.load(":/new/cursor/rectangle_cursor.png");
    line_cursor.load(":/new/cursor/line_cursor.png");

    custom_cursor = QCursor(QPixmap::fromImage(standart_cursor), 0, 32);

    ui->graphicsView->viewport()->setCursor(custom_cursor);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);

    connect(ui->eraserButton, SIGNAL(clicked()), this, SLOT(Cleaner()));
    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->buttonLeft, SIGNAL(clicked()), this, SLOT(prevScene()));
    connect(ui->buttonRigth, SIGNAL(clicked()), this, SLOT(nextScene()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveInImage()));
    connect(ui->comboBox_2, SIGNAL(activated(int)), this, SLOT(resizePen(int)));

    Scene()->setBackgroundBrush(QBrush(Qt::white));

    QMenu *menu = new QMenu(this);

    menu->addAction(QIcon(":/icons/pen.png"),
                    "Pen",
                    this,
                    SLOT(onDrawPen()));

    menu->addAction(QIcon(":/icons/rectangle.png"),
                    "Rectangle",
                    this,
                    SLOT(onDrawRectangle()));

    menu->addAction(QIcon(":/icons/rectangle_dashed.png"),
                    "Dashed rectangle",
                    this,
                    SLOT(onDrawDashRectangle()));

    menu->addAction(QIcon(":/icons/line.png"),
                    "Line",
                    this,
                    SLOT(onDrawLine()));

    menu->addAction(QIcon(":/icons/line_dashed.png"),
                    "Dashed Line",
                    this,
                    SLOT(onDrawDashLine()));

    menu->addAction(QIcon(":/icons/circle.png"),
                    "Circle",
                    this,
                    SLOT(onDrawCircle()));

    menu->addAction(QIcon(":/icons/circle_dashed.png"),
                    "Dashed Circle",
                    this,
                    SLOT(onDrawDashCircle()));

    ui->drawButton->setMenu(menu);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::resizePen(int value)
{
    auto size = ui->comboBox_2->itemText(value).toInt();

    Scene()->setSize(size);
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
                                                 "Сохранить",
                                                 "untitled.png",
                                                 "Images (*.png)");

    Saver::SaveImage(Scene(), fileName, ui->graphicsView->sizeHint() * 2);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    auto result = QMessageBox::question(
                this,
                "Выход",
                "Вы дейсвительно хотите выйти?",
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

void MainWindow::keyReleaseEvent(QKeyEvent *event){

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
    if(event->key() == Qt::Key_Left){

        prevScene();
    }else
    if(event->key() == Qt::Key_Right){

        nextScene();

    }
//    else
//    if(event->key() >= 49 && event->key() < 55){

//        int index = event->key() - 49;

//        Scene()->size = ui->comboBox_2->itemText(index).toDouble();
//        ui->comboBox_2->setCurrentIndex(index);
//    }


    reloadCustomCursor();

}

void MainWindow::reloadCustomCursor(){
    switch (Scene()->getPhigure()) {

    case Phigure::Pen:
        custom_cursor = QCursor(QPixmap::fromImage(standart_cursor),0,32);
        break;
    case Phigure::Circle:
        custom_cursor = QCursor(QPixmap::fromImage(circle_cursor),16,16);
        break;
    case Phigure::Cleaner:
        custom_cursor = QCursor(QPixmap::fromImage(cleaner_cursor),0,0);
        break;
    case Phigure::DashCircle:
        custom_cursor = QCursor(QPixmap::fromImage(circle_cursor),16,16);
        break;
    case Phigure::Line:
        custom_cursor = QCursor(QPixmap::fromImage(line_cursor),2,2);
        break;
    case Phigure::DashLine:
        custom_cursor = QCursor(QPixmap::fromImage(line_cursor),2,2);
        break;
    case Phigure::Rectangle:
        custom_cursor = QCursor(QPixmap::fromImage(rectangle_cursor),0,0);
        break;
    case Phigure::DashRectangle:
        custom_cursor = QCursor(QPixmap::fromImage(rectangle_cursor),0,0);
        break;

    }

           ui->graphicsView->viewport()->setCursor(custom_cursor);

}
