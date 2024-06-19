/*
https://libgpiod-dlang.dpldocs.info/gpiod.gpiod_chip_open_by_name.html
*/

#include "mainwindow.h"

#include <QApplication>
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include "vlog.h"

using namespace std;
using namespace std::chrono_literals;


struct InitCleanGPIO final
{
    InitCleanGPIO()
    {

    }
};


void test_UZ()
{}

void test_chip()
{
    const char *chipname = "gpiochip0";

    struct gpiod_line *lineRed;    // Red LED
    struct gpiod_line *lineGreen;  // Green LED
    struct gpiod_line *lineYellow; // Yellow LED
    struct gpiod_line *lineButton; // Pushbutton
    int i, val;

    // Open GPIO chip
    auto chip = gpiod_chip_open_by_name(chipname);
    vdeb << "chip initialized:" << chip;

    auto line_trig = gpiod_chip_get_line(chip, 15);
    auto rcode = gpiod_line_request_output(line_trig, "example1", 0);
    vdeb << "req output" << rcode;
    if (rcode < 0) throw verror;

    for (int i = 0; i < 100; ++i)
    {
        vdeb << 0;
        gpiod_line_set_value(line_trig, 0);
        this_thread::sleep_for(1s);
        vdeb << 1;
        gpiod_line_set_value(line_trig, 1);
        this_thread::sleep_for(1s);
    }
    // Open GPIO lines
//    lineRed = gpiod_chip_get_line(chip, 24);
//    lineGreen = gpiod_chip_get_line(chip, 25);
//    lineYellow = gpiod_chip_get_line(chip, 5);
//    lineButton = gpiod_chip_get_line(chip, 6);

//    // Open LED lines for output
//    gpiod_line_request_output(lineRed, "example1", 0);
//    gpiod_line_request_output(lineGreen, "example1", 0);
//    gpiod_line_request_output(lineYellow, "example1", 0);

    // Open switch line for input
//    gpiod_line_request_input(lineButton, "example1");

    // Blink LEDs in a binary pattern
//    i = 0;
//    while (true) {
//      gpiod_line_set_value(lineRed, (i & 1) != 0);
//      gpiod_line_set_value(lineGreen, (i & 2) != 0);
//      gpiod_line_set_value(lineYellow, (i & 4) != 0);

//      // Read button status and exit if pressed
//      val = gpiod_line_get_value(lineButton);
//      if (val == 0) {
//        break;
//      }

//      usleep(100000);
//      i++;
//    }

    // Release lines and chip
//    gpiod_line_release(lineRed);
//    gpiod_line_release(lineGreen);
//    gpiod_line_release(lineYellow);
//    gpiod_line_release(lineButton);

    gpiod_chip_close(chip);
}

int main(int argc, char *argv[])
{
    test_chip();
    return 0;


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
