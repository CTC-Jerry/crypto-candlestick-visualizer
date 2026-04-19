// ===================================================================================
// My own code starts
#include "Candlestick.h"
#include <iostream>
#include <cmath> 

Candlestick::Candlestick(
            std::string _time,
            double _open,
            double _close,
            double _high,
            double _low)
:time(_time),
 open(_open),
 close(_close),
 high(_high),
 low(_low)
{
    
}

std::string Candlestick::getTime()
{
    return time;
}

std::vector<double> Candlestick::getData()
{
    std::vector<double> data;
    data.push_back(open);
    data.push_back(close);
    data.push_back(high);
    data.push_back(low);

    return data;
}
// My own code ends
// ===================================================================================