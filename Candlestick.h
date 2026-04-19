// ===================================================================================
// My own code starts
#pragma once

#include <string>
#include <vector>

class Candlestick
{
    public:
        Candlestick(
            std::string _time,
            double _open,
            double _close,
            double _high,
            double _low
        );

        /** returns the time protected variable*/
        std::string getTime();
        /** returns the protected variables, open, close, high, and low in a vector({open,close,high,low}). */
        std::vector<double> getData();

    protected:
        std::string time;
        double open;
        double close;
        double high;
        double low;
};
// My own code ends
// ===================================================================================