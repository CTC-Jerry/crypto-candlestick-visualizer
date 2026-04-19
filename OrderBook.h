#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>
#include "Candlestick.h"
#include "CandlestickCreator.h"

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);
        /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
        /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
         * sent time in the orderbook  
         * If there is no next timestamp, wraps around to the start
         **/
        std::string getNextTime(std::string timestamp);

        void insertOrder(OrderBookEntry& order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        // ===================================================================================
        // My own code starts

        // setup the variable "candlesticks"
        void getCandlesticksData(std::vector<std::string> knownProducts);
        // for midterm part 1, print the candlestick data based on the variable "candlesticks"
        void printCandlesticksData();
        // for midterm part 2, draw the candlestick graph based on the variable "candlesticks"
        void drawCandlestickgraph();

        // My own code ends
        // ===================================================================================

    private:
        std::vector<OrderBookEntry> orders;
        // ===================================================================================
        // My own code starts
        std::vector<Candlestick> candlesticks;
        // My own code ends
        // ===================================================================================
};
