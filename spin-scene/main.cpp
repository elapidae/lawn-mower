#include "mainwindow.h"

#include <QApplication>
//#include "test_rounds_reader.h"
#include "vlog.h"
#include "vcat.h"

#include <csignal>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char *argv[])
{

    //Test_Rounds_Reader rr("../40-150.txt");

    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    return a.exec();
}
