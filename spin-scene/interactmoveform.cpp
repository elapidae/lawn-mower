#include "interactmoveform.h"
#include "ui_interactmoveform.h"

#include "vlog.h"
#include <QMetaObject>
#include <QThread>



//=======================================================================================
static void invoke( QObject *obj, const char *slot,
                    QGenericArgument a1,
                    QGenericArgument a2,
                    QGenericArgument a3 = QGenericArgument() )
{
    bool ok = QMetaObject::invokeMethod( obj, slot,
                                         Qt::BlockingQueuedConnection,
                                         a1, a2, a3 );
    Q_ASSERT(ok);
}
//=======================================================================================


//=======================================================================================
InteractMoveForm::InteractMoveForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InteractMoveForm)
    , left_engine( Engine::left_engine() )
    , right_engine( Engine::right_engine() )
    , us1( USonic::first() )
    , us2( USonic::second() )
    , us3( USonic::three() )
    , us4( USonic::four() )
{
    ui->setupUi(this);

    left_engine.set_holl_signal();
    right_engine.set_holl_signal();

    left_engine.holl_signal += [this](int level, uint32_t tick) {
        invoke( this, "sl_left_engine_holl", Q_ARG(int,level), Q_ARG(quint32,tick) );
    };
    right_engine.holl_signal += [this](int level, uint32_t tick) {
        invoke( this, "sl_right_engine_holl", Q_ARG(int,level), Q_ARG(quint32,tick) );
    };

    // first pair
    us1.measured += [this](double d, uint32_t a1, uint32_t a2) {
        invoke( this, "sl_us1_measured",
                Q_ARG(double,d), Q_ARG(quint32,a1), Q_ARG(quint32,a2) );
    };
    us4.measured += [this](double d, uint32_t a1, uint32_t a2) {
        invoke( this, "sl_us4_measured",
                Q_ARG(double,d), Q_ARG(quint32,a1), Q_ARG(quint32,a2) );
    };
    // second pair
    us2.measured += [this](double d, uint32_t a1, uint32_t a2) {
        invoke( this, "sl_us2_measured",
                Q_ARG(double,d), Q_ARG(quint32,a1), Q_ARG(quint32,a2) );
    };
    us3.measured += [this](double d, uint32_t a1, uint32_t a2) {
        invoke( this, "sl_us3_measured",
                Q_ARG(double,d), Q_ARG(quint32,a1), Q_ARG(quint32,a2) );
    };
}
//=======================================================================================
InteractMoveForm::~InteractMoveForm()
{
    delete ui;
}
//=======================================================================================


//=======================================================================================
void InteractMoveForm::on_btn_reset_engines_clicked()
{
    left_holls = 0;
    right_holls = 0;
    ui->le_left_engine->setText("");
    ui->le_right_engine->setText("");
}
//=======================================================================================
void InteractMoveForm::on_ck_poll_sonics_clicked()
{
    vdeb << "us checked" << ui->ck_poll_sonics->isChecked();
    if ( !ui->ck_poll_sonics->isChecked() ) return;

    us_measured = 0;
    us1.trigger();
    us4.trigger();
}
//=======================================================================================

//=======================================================================================
//      ENGINES
//=======================================================================================
void InteractMoveForm::sl_left_engine_holl(int level, uint32_t tick)
{
    ++left_holls;
    ui->le_left_engine->setText( QString("%1").arg(left_holls) );
}
//=======================================================================================
void InteractMoveForm::sl_right_engine_holl(int level, uint32_t tick)
{
    ++right_holls;
    ui->le_right_engine->setText( QString("%1").arg(right_holls) );
}
//=======================================================================================

//=======================================================================================
//  SONICS
//=======================================================================================
void InteractMoveForm::send_us1_pair()
{
    if ( ++us_measured < 2 ) return;
    if ( !ui->ck_poll_sonics->isChecked() ) return;

    QThread::msleep(5);
    us_measured = 0;
    us1.trigger();
    us4.trigger();
}
//=======================================================================================
void InteractMoveForm::send_us2_pair()
{
    if ( ++us_measured < 2 ) return;
    if ( !ui->ck_poll_sonics->isChecked() ) return;

    QThread::msleep(5);
    us_measured = 0;
    us2.trigger();
    us3.trigger();
}
//=======================================================================================
void InteractMoveForm::sl_us1_measured(double d, uint32_t, uint32_t)
{
    ui->le_us_d1->setText( QString("%1").arg(d) );
    send_us2_pair();
}
//=======================================================================================
void InteractMoveForm::sl_us4_measured(double d, uint32_t, uint32_t)
{
    ui->le_us_d4->setText( QString("%1").arg(d) );
    send_us2_pair();
}
//=======================================================================================
void InteractMoveForm::sl_us2_measured(double d, uint32_t, uint32_t)
{
    ui->le_us_d2->setText( QString("%1").arg(d) );
    send_us1_pair();
}
//=======================================================================================
void InteractMoveForm::sl_us3_measured(double d, uint32_t, uint32_t)
{
    ui->le_us_d3->setText( QString("%1").arg(d) );
    send_us1_pair();
}
//=======================================================================================
