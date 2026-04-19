// ===================================================================================
// My own code starts
#include "CandlestickPlotting.h"
#include <iostream>
#include <iomanip>
#include <cmath> 

void CandlestickPlotting::plot(std::vector<std::string>& timeList,
                               std::vector<double>& highList,
                               std::vector<double>& lowList,
                               std::vector<double>& openList,
                               std::vector<double>& closeList,
                               unsigned int candlestickNum)
{
    // get highest "high" among the highlist
    double highest_high=getHighestHigh(highList);
    // get lowest "low" among the lowlist
    double lowest_low=getLowestLow(lowList);
    // get the range between highest "high" and lowest "low"
    double high_low_range=getHighLowRange(highest_high,lowest_low);

    // get all the y-axis labels
    std::vector<double> indexList=getPlotIndex(highest_high,high_low_range);
    std::vector<PriceResult> priceResult;

    // get the candlesticks' properties
    std::vector<std::vector<std::string>> candlestickProperty=getCandlestickProperty(priceResult,indexList,highList,lowList,openList,closeList);

    // plot the candlestick graph
    plotCandlesticks(timeList,priceResult,indexList,candlestickProperty);
}

double CandlestickPlotting::getHighestHigh(std::vector<double>& highList)
{
    double highest_high=highList[0];
    for(unsigned int i=1;i<highList.size();++i){
        if(highList[i]>highest_high){
            highest_high=highList[i];
        }
    }
    return highest_high;
}

double CandlestickPlotting::getLowestLow(std::vector<double>& lowList)
{
    double lowest_low=lowList[0];
    for(unsigned int i=1;i<lowList.size();++i){
        if(lowList[i]<lowest_low){
            lowest_low=lowList[i];
        }
    }
    return lowest_low;
}

double CandlestickPlotting::getHighLowRange(double highest_high, double lowest_low)
{
    return highest_high-lowest_low;
}

std::vector<double> CandlestickPlotting::getPlotIndex(double highest_high,double range)
{
    // how many y-axis label to show
    unsigned int gapNumber=32;
    // the gap between 2 y-axis labels
    double gap=range/gapNumber;
    std::vector<double> indexList;

    for(int i=0;i<gapNumber+16;++i){
        indexList.push_back(highest_high-gap*(i-8));
    }

    return indexList;
}

std::vector<std::vector<std::string>> CandlestickPlotting::getCandlestickProperty(std::vector<PriceResult>& priceResult,
                                                                                  const std::vector<double>& indexList,
                                                                                  const std::vector<double>& highList,
                                                                                  const std::vector<double>& lowList,
                                                                                  const std::vector<double>& openList,
                                                                                  const std::vector<double>& closeList)
{
    std::vector<std::vector<std::string>> candlesticksProperty;

    for(unsigned int i=0;i<highList.size();++i){
        // update high
        unsigned int closestToIndex_high=0;
        double closestDifference_high=fabs(highList[i]-indexList[0]);
        for(unsigned j=0;j<indexList.size();++j){
            double difference=fabs(highList[i]-indexList[j]);

            if(difference<closestDifference_high){
                closestToIndex_high=j;
                closestDifference_high=difference;
            }
        }

        // update low
        unsigned int closestToIndex_low=0;
        double closestDifference_low=fabs(lowList[i]-indexList[0]);
        for(unsigned j=0;j<indexList.size();++j){
            double difference=fabs(lowList[i]-indexList[j]);

            if(difference<closestDifference_low){
                closestToIndex_low=j;
                closestDifference_low=difference;
            }
        }

        // update oepn
        unsigned int closestToIndex_open=0;
        double closestDifference_open=fabs(openList[i]-indexList[0]);
        for(unsigned j=0;j<indexList.size();++j){
            double difference=fabs(openList[i]-indexList[j]);

            if(difference<closestDifference_open){
                closestToIndex_open=j;
                closestDifference_open=difference;
            }
        }

        // update close
        unsigned int closestToIndex_close=0;
        double closestDifference_close=fabs(closeList[i]-indexList[0]);
        for(unsigned j=0;j<indexList.size();++j){
            double difference=fabs(closeList[i]-indexList[j]);

            if(difference<closestDifference_close){
                closestToIndex_close=j;
                closestDifference_close=difference;
            }
        }

        // update priceresult
        if(closeList[i]-openList[i]>0){
            priceResult.push_back(PriceResult::GoesUp);
        }
        else if(closeList[i]-openList[i]<0){
            priceResult.push_back(PriceResult::GoesDown);
        }
        else{
            priceResult.push_back(PriceResult::Remain);
        }

        // get the properties for one candlestick
        std::vector<std::string> candlestickProperty;
        for(unsigned int j=0;j<indexList.size();++j){
            if(j<closestToIndex_high){
                candlestickProperty.push_back("                 ");
            }
            else if(j>=closestToIndex_high&&(j<closestToIndex_open&&j<closestToIndex_close)){
                candlestickProperty.push_back("        |        ");
            }
            else if((j>=closestToIndex_open&&j<=closestToIndex_close)||(j>=closestToIndex_close&&j<=closestToIndex_open)){
                candlestickProperty.push_back("   |||||||||||   ");
            }
            else if((j>closestToIndex_open&&j>closestToIndex_close)&&j<=closestToIndex_low){
                candlestickProperty.push_back("        |        ");
            }
            else{
                candlestickProperty.push_back("                 ");
            }
        }
        candlesticksProperty.push_back(candlestickProperty);
    }

    return candlesticksProperty;
}

void CandlestickPlotting::plotCandlesticks(std::vector<std::string>& timeList,
                                           std::vector<PriceResult>& priceResult,
                                           std::vector<double>& indexList,
                                           std::vector<std::vector<std::string>>& candlestickProperty)
{
    std::cout<<std::endl;
    std::cout<<"Price: "<<std::endl;

    // print y-axis labels candlesticks
    for(unsigned int i=0;i<indexList.size();++i){
        std::cout<<WHITE<<std::setw(15)<<std::left<<indexList[i];
        for(unsigned int j=0;j<candlestickProperty.size();++j){
            std::string colour;
            if(priceResult[j]==PriceResult::GoesUp){
                colour=GREEN;
            }
            else if(priceResult[j]==PriceResult::GoesDown){
                colour=RED;
            }
            else{
                colour=YELLOW;
            }
            std::cout<<colour<<candlestickProperty[j][i];
        }
        std::cout<<std::endl;
    }

    // print x-axis labels
    std::cout<<RESET<<std::setw(15)<<std::right<<"Time -> ";
    for(std::string& time:timeList){
        std::cout<<"     "<<time<<"    ";
    }
    std::cout<<std::endl;
}
// My own code ends
// ===================================================================================