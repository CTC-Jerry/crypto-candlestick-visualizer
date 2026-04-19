#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>
#include "SolCandlestick.h"


class CSVReader
{
    public:
        CSVReader();

        static std::vector<OrderBookEntry> readCSV(std::string csvFile);

        static std::vector<std::string> tokenise(std::string csvLine, char separator);
        
        static OrderBookEntry stringsToOBE(std::string price, 
                                           std::string amount, 
                                           std::string timestamp, 
                                           std::string product, 
                                           OrderBookType OrderBookType);

        // ===================================================================================
        // My own code starts
        /** read "SOL-USD.csv" data and store it into a vector of SolCandlestick objects */
        static std::vector<SolCandlestick> CSVToSolCandlestick(std::string csvFile);
        /** returns a SolCandlestick object based on the given string */
        static SolCandlestick stringsToSolCD(std::vector<std::string> strings);
        // My own code ends
        // ===================================================================================

    private:
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
     
};