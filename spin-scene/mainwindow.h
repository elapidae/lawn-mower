#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QListWidget>
#include "test_rounds_reader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_zoom_decrease_clicked();
    void on_zoom_increase_clicked();
    void on_holls_per_round_valueChanged(double arg1);

    void on_test_files_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_rebuild_btn_clicked();

    void on_paint_angles_clicked();

    void on_rebuild_by_all_clicked();

    void on_d2_paint_clicked();

    void on_d3_paint_clicked();

    void on_d4_paint_clicked();

    void on_d2_grad_valueChanged(double arg1);

    void on_d3_grad_valueChanged(double arg1);

    void on_d4_grad_valueChanged(double arg1);

private:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void rebuild();
    void rebuild_avg();
    void rebuild_all();

    void add_sonar_angle( qreal uz_x, qreal uz_y, qreal dist_R, qreal cur_angle_g );

    QString test_filename;
    double holls_per_round = 90;
    //std::shared_ptr<Test_Rounds_Reader> rr;

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H
