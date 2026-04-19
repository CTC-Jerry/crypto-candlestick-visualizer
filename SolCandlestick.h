// ===================================================================================
// My own code starts
#pragma once

#include <string>
#include "Candlestick.h"

class SolCandlestick:public Candlestick{
    public:
        SolCandlestick(std::string _time,
            double _open,
            double _high,
            double _low,
            double _close,
            unsigned int _volume);

        /** returns "open" value */  
        double getOpen();
        /** returns "high" value */
        double getHigh();
        /** returns "low" value */
        double getLow();
        /** returns "close" value */
        double getClose();
        /** returns "volume" value */
        unsigned int getVolume();

    private:
        unsigned int volume;
};
// My own code ends
// ===================================================================================