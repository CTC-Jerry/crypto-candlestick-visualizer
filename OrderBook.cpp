#include "OrderBook.h"
#include "CSVReader.h"
#include "CandlestickPlotting.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}
/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp )
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}


double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                 product, 
                                                 timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                 product, 
                                                    timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales; 

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].price << std::endl;
    
    for (OrderBookEntry& ask : asks)
    {
        // for bid in bids:
        for (OrderBookEntry& bid : bids)
        {
            // if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
                // sale = new order()
                // sale.price = ask.price
            OrderBookEntry sale{ask.price, 0, timestamp, 
                product, 
                OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType =  OrderBookType::asksale;
                }
            
                // # now work out how much was sold and 
                // # create new bids and asks covering 
                // # anything that was not sold
                // if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
                    // sale.amount = ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    // # can do no more with this ask
                    // # go onto the next ask
                    // break
                    break;
                }
                // if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
                    // sale.amount = ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // # we adjust the bid in place
                    // # so it can be used to process the next ask
                    // bid.amount = bid.amount - ask.amount
                    bid.amount =  bid.amount - ask.amount;
                    // # ask is completely gone, so go to next ask                
                    // break
                    break;
                }


                // if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount && 
                   bid.amount > 0)
                {
                    // sale.amount = bid.amount
                    sale.amount = bid.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // # update the ask
                    // # and allow further bids to process the remaining amount
                    // ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
                    // bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    // # some ask remains so go to the next bid
                    // continue
                    continue;
                }
            }
        }
    }
    return sales;             
}

// ===================================================================================
// My own code starts
void OrderBook::getCandlesticksData(std::vector<std::string> knownProducts)
{
    std::cout<<"View candlestick data - enter: order type, product, eg  ask,ETH/BTC"<<std::endl;
    std::string input;
    std::getline(std::cin,input);
    std::vector<std::string> tokens=CSVReader::tokenise(input,',');

    if(tokens.size()==2){
        // try to get a vector of Candlestick object
        try{
            candlesticks=CandlestickCreator::generateCandlesticks(orders,tokens[0],tokens[1],knownProducts);
        }
        // throw an error if cannot get a vector of Candlestick object
        catch(std::exception& e){
            throw;
        }
    }
    else{
        throw std::exception{};
    }
}

void OrderBook::printCandlesticksData()
{
    // the index of the first printed candlestick data
    int startPrintIndex=0;

    // user input
    std::string input="";
    // number of candlestick data to be printed
    int displayedDataNum=5;

    while(true){
        // when users type "leave", break the loop and go back to menu page 
        if(input=="leave"){
            break;
        }
        // when users type "next"
        else if(input=="next"){
            // there are 5 more candlesticks next
            if(startPrintIndex!=candlesticks.size()-1&&startPrintIndex+9<=candlesticks.size()-1){
                displayedDataNum=5;
                startPrintIndex+=5;
            }
            // there are less than 5 more candlesticks next
            else if(startPrintIndex!=candlesticks.size()-1&&startPrintIndex+9>candlesticks.size()-1){
                displayedDataNum=candlesticks.size()-(startPrintIndex+5);
                startPrintIndex+=5;
            }
            // there is no more candlstick
            else{
                std::cout<<"No more data to show"<<std::endl;
                std::cout<<"Please type \"leave\" to go back to the menu or type \"prev\" to view previous page"<<std::endl;
                std::getline(std::cin,input);
                continue;
            }
        }
        // when users type "prev"
        else if(input=="prev"){
            // there are more than 5 candlesticks before
            if(startPrintIndex!=0&&startPrintIndex-5>=0){
                displayedDataNum=5;
                startPrintIndex-=5;
            }
            // there are less than 5 candlesticks before
            else if(startPrintIndex!=0&&startPrintIndex-5<0){
                displayedDataNum=startPrintIndex;
                startPrintIndex=0;
            }
            // there is no more candlestick before
            else{
                std::cout<<"No more data to show"<<std::endl;
                std::cout<<"Please type \"leave\" to go back to the menu or type \"next\" to view previous page"<<std::endl;
                std::getline(std::cin,input);
                continue;
            }
        }
        // show users what they can do after plotting the graph for the first time
        else if(input!=""){
            std::cout<<"Please enter valid input"<<std::endl;
            std::cout<<"Please type: \"prev\"-go to previous page, \"next\"-go to next page, \"leave\"-to go back to the menu"<<std::endl;
            std::getline(std::cin,input);
            continue;
        }

        // print table header
        std::cout<<std::endl;
        std::cout<<std::setw(30)<<std::left<<"Time";
        std::cout<<std::setw(15)<<std::left<<"Open";
        std::cout<<std::setw(15)<<std::left<<"Close";
        std::cout<<std::setw(15)<<std::left<<"High";
        std::cout<<std::setw(15)<<std::left<<"Low"<<std::endl;

        // print table data
        for(unsigned int i=startPrintIndex;i<startPrintIndex+displayedDataNum;++i){
            std::string time=candlesticks[i].getTime();
            std::vector<double> data=candlesticks[i].getData();

            // time
            std::cout<<std::setw(30)<<std::left<<time;
            // open
            std::cout<<std::setw(15)<<std::left<<data[0];
            // close
            std::cout<<std::setw(15)<<std::left<<data[1];
            // high
            std::cout<<std::setw(15)<<std::left<<data[2];
            // low
            std::cout<<std::setw(15)<<std::left<<data[3]<<std::endl;
        }
        std::cout<<std::endl;

        std::cout<<"Please type: \"prev\"-go to previous page, \"next\"-go to next page, \"leave\"-to go back to the menu"<<std::endl;
        std::getline(std::cin,input);
    }
}

void OrderBook::drawCandlestickgraph()
{
    // the index of the first printed candlestick plot
    int startPrintIndex=0;

    // user input
    std::string input="";
    // number of candlestick plot to be printed
    int displayedDataNum=5;

    while(true){
        // when users type "leave", break the loop and go back to menu page 
        if(input=="leave"){
            break;
        }
        // when users type "next"
        else if(input=="next"){
            // there are 5 more candlesticks next
            if(startPrintIndex!=candlesticks.size()-1&&startPrintIndex+9<=candlesticks.size()-1){
                displayedDataNum=5;
                startPrintIndex+=5;
            }
            // there are less than 5 more candlesticks next
            else if(startPrintIndex!=candlesticks.size()-1&&startPrintIndex+9>candlesticks.size()-1){
                displayedDataNum=candlesticks.size()-(startPrintIndex+5);
                startPrintIndex+=5;
            }
            // there is no more candlstick
            else{
                std::cout<<"No more data to show"<<std::endl;
                std::cout<<"Please type \"leave\" to go back to the menu or type \"prev\" to view previous page"<<std::endl;
                std::getline(std::cin,input);
                continue;
            }
        }
        // when users type "prev"
        else if(input=="prev"){
            // there are more than 5 candlesticks before
            if(startPrintIndex!=0&&startPrintIndex-5>=0){
                displayedDataNum=5;
                startPrintIndex-=5;
            }
            // there are less than 5 candlesticks before
            else if(startPrintIndex!=0&&startPrintIndex-5<0){
                displayedDataNum=startPrintIndex;
                startPrintIndex=0;
            }
            // there is no more candlestick before
            else{
                std::cout<<"No more data to show"<<std::endl;
                std::cout<<"Please type \"leave\" to go back to the menu or type \"next\" to view previous page"<<std::endl;
                std::getline(std::cin,input);
                continue;
            }
        }
        // show users what they can do after plotting the graph for the first time
        else if(input!=""){
            std::cout<<"Please enter valid input"<<std::endl;
            std::cout<<"Please type: \"prev\"-go to previous page, \"next\"-go to next page, \"leave\"-to go back to the menu"<<std::endl;
            std::getline(std::cin,input);
            continue;
        }

        // vector containing all "timestamp" of candlesticks
        std::vector<std::string> timeList;
        // vector containing all "high" of candlesticks
        std::vector<double> highList;
        // vector containing all "low" of candlesticks
        std::vector<double> lowList;
        // vector containing all "open" of candlesticks
        std::vector<double> openList;
        // vector containing all "close" of candlesticks
        std::vector<double> closeList;
        // setup "timeList", "highList", "lowList", "openList", "closeList"
        for(unsigned int i=startPrintIndex;i<startPrintIndex+displayedDataNum;++i){
            std::string trimmed_time=CSVReader::tokenise(candlesticks[i].getTime(),' ')[1];
            std::string double_trimmed_time=CSVReader::tokenise(trimmed_time,'.')[0];
            timeList.push_back(double_trimmed_time);
            std::vector<double> candlestickData=candlesticks[i].getData();
            openList.push_back(candlestickData[0]);
            closeList.push_back(candlestickData[1]);
            highList.push_back(candlestickData[2]);
            lowList.push_back(candlestickData[3]);
        }
        // draw the candlestick graph
        CandlestickPlotting::plot(timeList,highList,lowList,openList,closeList,displayedDataNum);

        std::cout<<"Please type: \"prev\"-go to previous page, \"next\"-go to next page, \"leave\"-to go back to the menu"<<std::endl;
        std::getline(std::cin,input);
    }
}
// My own code ends
// ===================================================================================