#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/icon.png"));

    MainWindow w;

    w.setWindowTitle("Whiteboard");
    w.showMaximized();

    return a.exec();
}
