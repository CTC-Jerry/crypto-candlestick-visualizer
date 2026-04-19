// ===================================================================================
// My own code starts
#include "CandlestickCreator.h"
#include <cmath>
#include <iostream>

std::vector<Candlestick> CandlestickCreator::generateCandlesticks(std::vector<OrderBookEntry>& orders,
                                                                  std::string orderTypeStr,
                                                                  std::string product,
                                                                  std::vector<std::string> productsList)
{
    std::vector<Candlestick> candlesticks;
    // vector contaning vectors of OrderBookEntry in different timestamp
    std::vector<std::vector<OrderBookEntry>> orderSets;

    // if users select a known product
    bool productKnown=false;
    for(std::string p:productsList){
        if(product==p){
            productKnown=true;
            break;
        }
    }

    // update "orderSets" based on user input
    if(productKnown&&(orderTypeStr=="ask"||orderTypeStr=="bid")){
        try{
            if(orderTypeStr=="ask"){
                orderSets=getOrderSets(orders,product,OrderBookType::ask);
            }
            else{
                orderSets=getOrderSets(orders,product,OrderBookType::bid);
            }
        }
        catch(std::exception& e){
            throw;
        }
    }
    else{
        throw std::exception{};
    }

    // get a vector of Candlestick objects
    for(int i=1;i<orderSets.size();++i){
        std::vector<double> candlestickData=parseOrderSet(orderSets[i-1],orderSets[i]);

        std::string time=orderSets[i][0].timestamp;
        Candlestick cd=toCandlestick(time,
                                     candlestickData[0],
                                     candlestickData[1],
                                     candlestickData[2],
                                     candlestickData[3]);

        candlesticks.push_back(cd);
    }

    return candlesticks;
}

std::vector<std::vector<OrderBookEntry>> CandlestickCreator::getOrderSets(std::vector<OrderBookEntry>& orders,std::string product,OrderBookType orderType)
{
    // store all the "orderSet" variable 
    std::vector<std::vector<OrderBookEntry>> orderSets;
    // store OrderBookEntry with the same timestamp
    std::vector<OrderBookEntry> orderSet;

    for(OrderBookEntry& order:orders){

        if(order.product==product&&order.orderType==orderType){
            if(orderSet.size()==0){
                orderSet.push_back(order);
            }
            // order has the same timestamp
            else if(orderSet[0].timestamp==order.timestamp){
                orderSet.push_back(order);
            }
            // order has the next timestamp
            else{
                orderSets.push_back(orderSet);
                orderSet.clear();
            }
        }
    }
    orderSets.push_back(orderSet);

    return orderSets;
}

std::vector<double> CandlestickCreator::parseOrderSet(std::vector<OrderBookEntry>& prevOrderSet, std::vector<OrderBookEntry>& nowOrderSet)
{
    std::vector<double> parsedData;

    // calculate open
    double open=getOpen(prevOrderSet);
    parsedData.push_back(open);

    // calculate close
    double close=getClose(nowOrderSet);
    parsedData.push_back(close);

    // calculate high
    double high=getHigh(nowOrderSet);
    parsedData.push_back(high);

    // calculate low 
    double low=getLow(nowOrderSet);
    parsedData.push_back(low);

    return parsedData;
}
        
double CandlestickCreator::getOpen(std::vector<OrderBookEntry>& prevOrderSet)
{
    double totalPrevAmount=0;
    double totalPrevValue=0;
    for(OrderBookEntry& order:prevOrderSet){
        totalPrevAmount+=order.amount;
        totalPrevValue+=order.price*order.amount;
    }
    double prevAverageUnitPrice=(totalPrevValue/totalPrevAmount);

    return prevAverageUnitPrice;
}

double CandlestickCreator::getClose(std::vector<OrderBookEntry>& nowOrderSet)
{
    double totalNowAmount=0;
    double totalNowValue=0;
    for(OrderBookEntry& order:nowOrderSet){
        totalNowAmount+=order.amount;
        totalNowValue+=order.price*order.amount;
    }
    double nowAverageUnitPrice=(totalNowValue/totalNowAmount);

    return nowAverageUnitPrice;
}

double CandlestickCreator::getHigh(std::vector<OrderBookEntry>& nowOrderSet)
{
    double highestPrice=nowOrderSet[0].price;
    for(int i=1;i<nowOrderSet.size();++i){
        if(nowOrderSet[i].price>highestPrice){
            highestPrice=nowOrderSet[i].price;
        }
    }

    return highestPrice;
}

double CandlestickCreator::getLow(std::vector<OrderBookEntry>& nowOrderSet)
{
    double lowestPrice=nowOrderSet[0].price;
    for(int i=1;i<nowOrderSet.size();++i){
        if(nowOrderSet[i].price<lowestPrice){
            lowestPrice=nowOrderSet[i].price;
        }
    }

    return lowestPrice;
}

Candlestick CandlestickCreator::toCandlestick(std::string time,
                                              double open,
                                              double close,
                                              double high,
                                              double low)
{
    Candlestick candlestick{time,open,close,high,low};

    return candlestick;
}
// My own code ends
// ===================================================================================