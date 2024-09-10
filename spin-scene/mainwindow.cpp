#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsView>

#include <math.h>
#include <QGraphicsEllipseItem>
#include "vlog.h"

//=======================================================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;//(-1000, -1000, 2000, 2000);
    auto view = ui->view;
    view->setScene(scene);
    view->setInteractive(true);
    view->setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );
    view->scale(3, 3);
    view->rotate(-90);

    ui->test_files_list->addItem( "../40-150.txt" );
    ui->test_files_list->addItem( "../40-250.txt" );
    ui->test_files_list->addItem( "../45-200.txt" );
    ui->test_files_list->addItem( "../50-200.txt" );
    ui->test_files_list->addItem( "../80-200.txt" );

    ui->test_files_list->setCurrentRow(0);
    ui->holls_per_round->setValue(90);

    rebuild();
}
//=======================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//=======================================================================================
//=======================================================================================
void MainWindow::on_zoom_decrease_clicked()
{
    ui->view->scale( .9, .9 );
}
//=======================================================================================
void MainWindow::on_zoom_increase_clicked()
{
    ui->view->scale( 1.1, 1.1 );
}
//=======================================================================================
void MainWindow::on_holls_per_round_valueChanged( double arg1 )
{
    holls_per_round = arg1;
    rebuild();
}
//=======================================================================================
void MainWindow::on_test_files_list_currentItemChanged(QListWidgetItem *current,
                                                       QListWidgetItem *previous)
{
    test_filename = current->text();
}
//=======================================================================================
void MainWindow::on_rebuild_btn_clicked()
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_paint_angles_clicked()
{
    rebuild();
}
//=======================================================================================


//=======================================================================================
void MainWindow::rebuild()
{
    scene->clear();

    double pok_radius = 19;
    scene->addEllipse( -pok_radius, -pok_radius,
                       pok_radius * 2, pok_radius * 2,
                       QPen(Qt::red) );

    Test_Rounds_Reader rr( test_filename );

    const auto dangle = 2. * M_PI / holls_per_round;
    const auto dangle_g = 360. / holls_per_round;
    double cur_angle = 0;
    double cur_angle_g = 0;

    QColor color( Qt::blue );
    color.setAlphaF( .3 );
    QPen pen( color, .3 );

    int cnt = 0;
    while ( !rr.finished() )
    {
        auto candle = rr.next_left_candle();
        auto avg = candle.avg;
        auto dist = avg + pok_radius;
        vdeb << avg << cur_angle;

        auto uz_x = pok_radius * cos( cur_angle );
        auto uz_y = pok_radius * sin( cur_angle );

        auto x = dist * cos( cur_angle );
        auto y = dist * sin( cur_angle );

        QLineF dist_line( uz_x, uz_y, x, y );
        auto dist_R = dist_line.length();

        if ( ui->paint_angles->isChecked() )
        {
            auto item = new QGraphicsEllipseItem( uz_x - dist_R, uz_y - dist_R,
                                                  dist_R * 2, dist_R * 2 );
            item->setPen( pen );
            item->setStartAngle( (cur_angle_g - 7.5) * 16 );
            item->setSpanAngle( 15 * 16 );
            scene->addItem( item );
        }

        scene->addEllipse( x, y, 0.1, 0.1 );
        scene->addLine( dist_line, QPen(Qt::green, .2) );

        cur_angle -= dangle;
        cur_angle_g += dangle_g;
    }
}
//=======================================================================================

