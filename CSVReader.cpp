#include "CSVReader.h"
#include <iostream>
#include <fstream>


CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            try {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }catch(const std::exception& e)
            {
                std::cout << "CSVReader::readCSV bad data"  << std::endl;
            }
        }// end of while
    }    

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries"  << std::endl;
    return entries; 
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
   std::vector<std::string> tokens;
   signed int start, end;
   std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do{
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
    start = end + 1;
    }while(end > 0);

   return tokens; 
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3]<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4]<< std::endl; 
        throw;        
    }

    OrderBookEntry obe{price, 
                        amount, 
                        tokens[0],
                        tokens[1], 
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe; 
}

// ===================================================================================
// My own code starts
std::vector<SolCandlestick> CSVReader::CSVToSolCandlestick(std::string csvFilename)
{
    // vector containing each row of the csv's data
    std::vector<SolCandlestick> sol_entries;

    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open())
    {
        while(std::getline(csvFile, line))
        {
            // try to get the SolCandlestick object
            try {
                SolCandlestick solCD = stringsToSolCD(tokenise(line, ','));
                sol_entries.push_back(solCD);
            }
            // warn the users if cannot get a SolCandlestick object
            catch(const std::exception& e)
            {
                std::cout << "CSVReader::CSVToSolCandlestick bad data"  << std::endl;
            }
        }// end of while
    }    

    std::cout << "CSVReader::CSVToSolCandlestick read " << sol_entries.size() << " entries"  << std::endl;
    return sol_entries; 
}

SolCandlestick CSVReader::stringsToSolCD(std::vector<std::string> tokens)
{
    double open,high,low,close;
    unsigned long volume;

    if(tokens.size()!=7){
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // try to get all properties required for creating a SolCandlestick object
    try{
        open=std::stod(tokens[1]);
        high=std::stod(tokens[2]);
        low=std::stod(tokens[3]);
        close=std::stod(tokens[4]);
        volume=std::stoul(tokens[6]);
    }
    // warn the users if cannot get all required properties
    catch(const std::exception& e){
        std::cout << "CSVReader::stringsToSolCD Bad input line! "<<tokens[0]<<std::endl;
        throw;
    }

    SolCandlestick solCD{tokens[0],open,high,low,close,volume};
    return solCD;
}
// My own code ends
// ===================================================================================

OrderBookEntry CSVReader::stringsToOBE(std::string priceString, 
                                    std::string amountString, 
                                    std::string timestamp, 
                                    std::string product, 
                                    OrderBookType orderType)
{
    double price, amount;
    try {
         price = std::stod(priceString);
         amount = std::stod(amountString);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString<< std::endl; 
        throw;        
    }
    OrderBookEntry obe{price, 
                    amount, 
                    timestamp,
                    product, 
                    orderType};
                
    return obe;
}
     