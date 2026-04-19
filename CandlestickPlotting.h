// ===================================================================================
// My own code starts
#pragma once

#include "Candlestick.h"

// define the colour used to colour the text displayed in terminal
#define RESET  "\033[0m"
#define WHITE  "\033[37m"
#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"

/** the price from "close" to "open" */
enum class PriceResult{GoesUp, GoesDown, Remain};

class CandlestickPlotting
{
    public:
        CandlestickPlotting()=default;
        static void plot(std::vector<std::string>& timeList,
                         std::vector<double>& highList,
                         std::vector<double>& lowList,
                         std::vector<double>& openList,
                         std::vector<double>& closeList,
                         unsigned int candlestickNum);

    private:
        /** find the highest "high" value among the "high" list */
        static double getHighestHigh(std::vector<double>& highList);
        /** find the lowest "low" value among the "low" list */
        static double getLowestLow(std::vector<double>& lowList);
        /** get the range between highest "high" and lowest "low" */
        static double getHighLowRange(double highest_high, double lowest_low);
        /** returns a vector containing all the y-axis labels */
        static std::vector<double> getPlotIndex(double highest_high,double range);

        /** draw the candlestick graph */
        static void plotCandlesticks(std::vector<std::string>& timeList,
                                     std::vector<PriceResult>& closeGreaterThanOpen,
                                     std::vector<double>& indexList,
                                     std::vector<std::vector<std::string>>& candlestickProperty);

        /** returns a vector containing the appearance of each candlestick(one vector containing strings for each candlestick) */
        static std::vector<std::vector<std::string>> getCandlestickProperty(std::vector<PriceResult>& closeGreaterThanOpen,
                                                                            const std::vector<double>& indexList,
                                                                            const std::vector<double>& highList,
                                                                            const std::vector<double>& lowList,
                                                                            const std::vector<double>& openList,
                                                                            const std::vector<double>& closeList);

};
// My own code ends
// ===================================================================================