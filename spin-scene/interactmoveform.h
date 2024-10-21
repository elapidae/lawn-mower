#ifndef INTERACTMOVEFORM_H
#define INTERACTMOVEFORM_H

#include <QWidget>
#include "engine.h"
#include "usonic.h"
#include <memory>

namespace Ui {
class InteractMoveForm;
}

class InteractMoveForm : public QWidget
{
    Q_OBJECT

public:
    explicit InteractMoveForm(QWidget *parent = nullptr);
    ~InteractMoveForm();

private slots:
    void on_btn_reset_engines_clicked();
    void on_ck_poll_sonics_clicked();

    void sl_left_engine_holl (int level, quint32 tick);
    void sl_right_engine_holl(int level, quint32 tick);

    // first pair
    void sl_us1_measured(double d, quint32, quint32);
    void sl_us4_measured(double d, quint32, quint32);
    // second pair
    void sl_us2_measured(double d, quint32, quint32);
    void sl_us3_measured(double d, quint32, quint32);

private:
    Ui::InteractMoveForm *ui;
    Engine left_engine, right_engine;
    int left_holls = 0, right_holls = 0;

    void send_us1_pair();
    void send_us2_pair();
    USonic us1, us2, us3, us4;
    int us_measured = 0;
};

#endif // INTERACTMOVEFORM_H
