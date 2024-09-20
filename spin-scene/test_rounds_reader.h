#ifndef TEST_ROUNDS_READER_H
#define TEST_ROUNDS_READER_H

#include <QFile>
#include <QList>

class Test_Rounds_Reader
{
public:
    struct Step
    {
        double distance = -1;
        int l_stop = -1, r_stop = -1;
        double d2 = -1, d3 = -1, d4 = -1;
    };

    Test_Rounds_Reader(QString fname);

    Step prev, cur;
    void next();
    bool finished() const;

    QList<Step> next_serie_by_left();

    QList<Step> steps;

    struct Candle
    {
        Candle( QList<Step> steps )
        {
            assert( !steps.isEmpty() );
            min = max = steps.at(0).distance;
            auto sum = 0.0;
            for ( auto s: steps )
            {
                min = std::min(s.distance, min);
                sum += s.distance;
                max = std::max(s.distance, max);
            }
            avg = sum / steps.size();
        }
        double min, avg, max;
    };

    Candle next_left_candle()
    {
        return next_serie_by_left();
    }
};

#endif // TEST_ROUNDS_READER_H
