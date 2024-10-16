#include "test_rounds_reader.h"

#include "vlog.h"

static QList<QString> split_by_hands(QString str)
{
    QList<QString> res;
    if ( str.size() != 28 )
    {
        //vdeb << str.size();
        return res;
    }

    while ( res.size() < 4 )
    {
        QString num;
        num.append( str.at(0) );
        num.append( str.at(1) );
        num.append( str.at(2) );
        num.append( str.at(3) );
        num.append( str.at(4) );
        num.append( str.at(5) );
        num.append( str.at(6) );
        res << num;
        str = str.right(str.size() - 7);
    }

    return res;
}

static Test_Rounds_Reader::Step read_by_4( QString str )
{
    Test_Rounds_Reader::Step res;
    if ( !str.contains('|') ) throw verror;

    auto ll = str.split('|');
    if (ll.size() != 2) throw verror;
    auto stops = ll.at(1).split(';');
    res.l_stop = stops.at(0).toInt();
    res.r_stop = stops.at(1).toInt();

    auto dists = ll.at(0).split(';');
    if ( dists.size() == 1 )
        dists = split_by_hands( dists.first() );

    if (dists.isEmpty()) return res;
    res.distance = dists.at(0).toDouble();
    res.d2 = dists.at(1).toDouble();
    res.d3 = dists.at(2).toDouble();
    res.d4 = dists.at(3).toDouble();

    return res;
}

Test_Rounds_Reader::Test_Rounds_Reader(QString fname)
{
    QFile f(fname);
    if ( !f.open(QIODevice::ReadOnly) )
        throw verror << fname;

    auto all = f.readAll();
    auto strings = all.split('\n');
    for ( auto str: strings )
    {
        Step step;
        if ( str.contains('|') )
        {
            auto step = read_by_4( str );
            if ( step.d2 > 0 )
                steps.append( step );
            continue;
        }
        auto vals = str.split(';');
        if (vals.size() != 3) break;
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

