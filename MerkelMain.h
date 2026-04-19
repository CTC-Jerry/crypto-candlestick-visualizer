#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "SolOrder.h"

class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);

        // ===================================================================================
        // My own code starts
        /* Midterm part 1 */ 
        void showCandlestickData();
        /* Midterm part 2 */
        void plotCandlestickGraph();
        /* Midterm part 3 */
        void plotSol();
        // My own coee ends
        // ===================================================================================

        std::string currentTime;

        // OrderBook orderBook{"20200317.csv"};
	    OrderBook orderBook{"20200601.csv"};
        SolOrder solOrder{"SOL-USD.csv"};

        Wallet wallet;

};
