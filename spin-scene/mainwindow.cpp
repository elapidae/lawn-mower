#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsView>

#include <math.h>
#include <QGraphicsEllipseItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include "vlog.h"

#define qdeb qDebug() << __LINE__


double pok_radius = 64;


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
    view->installEventFilter(this);

    ui->test_files_list->addItem( "../40-150.txt" );
    ui->test_files_list->addItem( "../40-250.txt" );
    ui->test_files_list->addItem( "../45-200.txt" );
    ui->test_files_list->addItem( "../50-200.txt" );
    ui->test_files_list->addItem( "../80-200.txt" );
    ui->test_files_list->addItem( "../38-550.txt" );
    ui->test_files_list->addItem( "../40-550.txt" );

    ui->d2_grad->setMinimum(-180);
    ui->d2_grad->setMaximum(180);
    ui->d3_grad->setMinimum(-180);
    ui->d3_grad->setMaximum(180);
    ui->d4_grad->setMinimum(-180);
    ui->d4_grad->setMaximum(180);

    ui->test_files_list->setCurrentRow(6);
    ui->holls_per_round->setValue(90);
    ui->rebuild_by_all->setChecked( true );
    ui->d2_grad->setValue(5);
    ui->d3_grad->setValue(90);
    ui->d4_grad->setValue(-90);
}
//=======================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//=======================================================================================
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui->view) return false;

    if (event->type() == QEvent::Wheel)
    {
        auto wheel = dynamic_cast<QWheelEvent*>(event);
        if (!wheel) throw verror;

        auto delta = wheel->angleDelta().y();
        if (delta > 0)
            on_zoom_increase_clicked();
        else
            on_zoom_decrease_clicked();

        return true;
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        auto mouse = dynamic_cast<QMouseEvent*>(event);
        if (!mouse) throw verror;
        vdeb << mouse->pos() << mouse->localPos() << mouse->button();

        return true;
    }

    //vdeb << event->type();



    return false;
    //GraphicsSceneMouseMove
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
    rebuild();
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
void MainWindow::on_rebuild_by_all_clicked()
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_d2_paint_clicked()
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_d3_paint_clicked()
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_d4_paint_clicked()
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_d2_grad_valueChanged(double arg1)
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_d3_grad_valueChanged(double arg1)
{
    rebuild();
}
//=======================================================================================
void MainWindow::on_d4_grad_valueChanged(double arg1)
{
    rebuild();
}
//=======================================================================================
void MainWindow::rebuild()
{
    if ( ui->rebuild_by_all->isChecked() )
        rebuild_all();
    else
        rebuild_avg();
}
//=======================================================================================


//=======================================================================================
void MainWindow::rebuild_avg()
{
    scene->clear();

    scene->addEllipse( -pok_radius, -pok_radius,
                       pok_radius * 2, pok_radius * 2,
                       QPen(Qt::red) );

    Test_Rounds_Reader rr( test_filename );

    const auto dangle = 2. * M_PI / holls_per_round;
    const auto dangle_g = 360. / holls_per_round;
    double cur_angle = 0;
    double cur_angle_g = 0;

    int cnt = 0;
    while ( !rr.finished() )
    {
        auto candle = rr.next_left_candle();
        auto avg = candle.avg;
        auto dist = avg + pok_radius;
        //vdeb << avg << cur_angle;

        auto uz_x = pok_radius * cos( cur_angle );
        auto uz_y = pok_radius * sin( cur_angle );

        auto x = dist * cos( cur_angle );
        auto y = dist * sin( cur_angle );

        QLineF dist_line( uz_x, uz_y, x, y );
        auto dist_R = dist_line.length();

        add_sonar_angle( uz_x, uz_y, dist_R, cur_angle_g );

        scene->addEllipse( x, y, 0.1, 0.1 );
        scene->addLine( dist_line, QPen(Qt::green, .2) );

        cur_angle -= dangle;
        cur_angle_g += dangle_g;
    }
    vdeb << scene->items().size();
}
//=======================================================================================
void MainWindow::rebuild_all()
{
    scene->clear();

    scene->addEllipse( -pok_radius, -pok_radius,
                       pok_radius * 2, pok_radius * 2,
                       QPen(Qt::red) );

    Test_Rounds_Reader rr( test_filename );

    const auto dAngle = 2. * M_PI / holls_per_round;
    const auto dAngle_g = 360. / holls_per_round;

    int cnt = 0;
    double cur_angle = 0;
    double cur_angle_g = 0;
    while ( !rr.finished() )
    {
        auto fix_list = rr.next_serie_by_left();
        const auto dangle = dAngle / fix_list.size();
        const auto dangle_g = dAngle_g / fix_list.size();

        auto do_paint = [&]( double distance, double angle_grad, QPen pen )
        {
            auto angle_rad = angle_grad * M_PI / 180.;

            auto avg = distance;
            auto dist = avg + pok_radius;

            auto ccos = cos( cur_angle + angle_rad );
            auto ssin = sin( cur_angle + angle_rad );

            auto uz_x = pok_radius * ccos;
            auto uz_y = pok_radius * ssin;

            auto x = dist * ccos;
            auto y = dist * ssin;

            QLineF dist_line( uz_x, uz_y, x, y );
            auto dist_R = dist_line.length();

            add_sonar_angle( uz_x, uz_y, dist_R, cur_angle_g + angle_grad );

            scene->addEllipse( x, y, 0.1, 0.1 );
            scene->addLine( dist_line, pen );
        };

        auto green = QPen(Qt::green, .2);
        auto blue  = QPen(Qt::blue, .2);
        auto magenta = QPen(Qt::magenta, .2);
        auto darkCyan = QPen(Qt::darkCyan, .2);
        for ( auto step: fix_list )
        {
            do_paint( step.distance, 0, green );

            if ( ui->d2_paint->isChecked() )
                do_paint( step.d2, ui->d2_grad->value(), blue );

            if ( ui->d3_paint->isChecked() )
                do_paint( step.d3, ui->d3_grad->value(), magenta );

            if ( ui->d4_paint->isChecked() )
                do_paint( step.d4, ui->d4_grad->value(), darkCyan );

            cur_angle -= dangle;
            cur_angle_g += dangle_g;
        } // for each element in list
    } // for rr.finished()

    vdeb << scene->items().size();
}
//=======================================================================================
void MainWindow::add_sonar_angle( qreal uz_x, qreal uz_y,
                                  qreal dist_R,
                                  qreal cur_angle_g )
{
    if ( !ui->paint_angles->isChecked() )
        return;

    QColor color( Qt::blue );
    color.setAlphaF( .03 );
    QBrush brush( color );

    auto item = new QGraphicsEllipseItem( uz_x - dist_R, uz_y - dist_R,
                                          dist_R * 2, dist_R * 2 );
    item->setPen( Qt::NoPen );
    item->setBrush( brush );
    item->setStartAngle( (cur_angle_g - 7.5) * 16 );
    item->setSpanAngle( 15 * 16 );
    scene->addItem( item );
}
//=======================================================================================
