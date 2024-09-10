#include "test_rounds_reader.h"

#include "vlog.h"

Test_Rounds_Reader::Test_Rounds_Reader(QString fname)
{
    QFile f(fname);
    if ( !f.open(QIODevice::ReadOnly) )
        throw verror << fname;

    auto all = f.readAll();
    auto strings = all.split('\n');
    for ( auto str: strings )
    {
        auto vals = str.split(';');
        if (vals.size() != 3) break;
        Step step;
        step.distance = vals.at(0).toDouble();
        step.l_stop = vals.at(1).toInt();
        step.r_stop = vals.at(2).toInt();
        steps.append(step);
        //vdeb << step.distance << step.l_stop << step.r_stop;
    }
    next();
    prev = cur;
}
//=======================================================================================
void Test_Rounds_Reader::next()
{
    prev = cur;
    cur = steps.takeFirst();
}
//=======================================================================================
bool Test_Rounds_Reader::finished() const
{
    return steps.isEmpty();
}
//=======================================================================================
QList<Test_Rounds_Reader::Step> Test_Rounds_Reader::next_serie_by_left()
{
    QList<Test_Rounds_Reader::Step> res;
    res << cur;
    while ( !finished() )
    {
        next();
        res << cur;
        if ( prev.l_stop != cur.l_stop ) break;
    }
    return res;
}
//=======================================================================================

