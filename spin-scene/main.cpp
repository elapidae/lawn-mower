#include "mainwindow.h"

#include <QApplication>
//#include "test_rounds_reader.h"
#include "vlog.h"
#include "vcat.h"

#include <csignal>
#include <thread>
#include "interactmoveform.h"

using namespace std::chrono_literals;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    InteractMoveForm w;
    w.resize(1200, 800);
    w.show();
    return a.exec();
}
