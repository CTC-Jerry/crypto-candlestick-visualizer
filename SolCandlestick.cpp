// ===================================================================================
// My own code starts
#include "SolCandlestick.h"
#include <string>

SolCandlestick::SolCandlestick(
    std::string _time,
            double _open,
            double _high,
            double _low,
            double _close,
            unsigned int _volume)
    :Candlestick(_time,_open,_close,_high,_low), volume(_volume)
{

}

double SolCandlestick::getOpen()
{
    return open;
}

double SolCandlestick::getHigh()
{
    return high;
}

double SolCandlestick::getLow()
{
    return low;
}

double SolCandlestick::getClose()
{
    return close;
}

unsigned int SolCandlestick::getVolume()
{
    return volume;
}
// My own code ends
// ===================================================================================