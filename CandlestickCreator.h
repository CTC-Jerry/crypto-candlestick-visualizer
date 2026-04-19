// ===================================================================================
// My own code starts
#pragma once

#include "Candlestick.h"
#include "OrderBookEntry.h"
#include <vector>

class CandlestickCreator
{
    public:
        CandlestickCreator()=default;

        /** returns a vector of Candlestick objects */
        static std::vector<Candlestick> generateCandlesticks(std::vector<OrderBookEntry>& orders,
                                                             std::string orderTypeStr,
                                                             std::string product,
                                                             std::vector<std::string> productsList);

        /** returns a vector containing OrderBookEntry with the same timestamp organised in another vector  */
        static std::vector<std::vector<OrderBookEntry>> getOrderSets(std::vector<OrderBookEntry>& orders,
                                                                     std::string product,
                                                                     OrderBookType orderType);

        /** returns a vector containing "open", "close", "high", and "low" */
        static std::vector<double>parseOrderSet(std::vector<OrderBookEntry>& prevOrderSet,
                                                std::vector<OrderBookEntry>& nowOrderSet);
        
        /** returns "open" */
        static double getOpen(std::vector<OrderBookEntry>& prevOrderSet);
        /** returns "close" */
        static double getClose(std::vector<OrderBookEntry>& nowOrderSet);
        /** returns "high" */
        static double getHigh(std::vector<OrderBookEntry>& nowOrderSet);
        /** returns "low" */
        static double getLow(std::vector<OrderBookEntry>& nowOrderSet);

        /** returns a Candlestick object */
        static Candlestick toCandlestick(std::string time,
                                         double open,
                                         double close,
                                         double high,
                                         double low);
};
// My own code ends
// ===================================================================================