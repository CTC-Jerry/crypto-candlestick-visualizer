// ===================================================================================
// My own code starts
#include "SolOrder.h"
#include <iostream>
#include "CSVReader.h"
#include <iomanip>
#include <cmath>

SolOrder::SolOrder(std::string filename)
{
    sol_orders=CSVReader::CSVToSolCandlestick(filename);

    maxChartNum=std::ceil(sol_orders.size()/dataToShow);
}

void SolOrder::plot(std::string mode)
{
    // throw an error if the mode users selected does not exist
    if(mode!="open"&&mode!="high"&&mode!="low"&&mode!="close"&&mode!="volume"){
        throw std::exception{};
    }

    // user input
    std::string input="";
    // set the chart index to 1 so that everytime users select a new mode, the chart is printed from the first date
    currentChartIndex=1;

    while(true){
        // when users type "leave", break the loop and go back to menu page 
        if(input=="leave"){
            break;
        }
        // when users type "next"
        else if(input=="next"){
            // check if there is a next graph
            if(currentChartIndex<=maxChartNum){
                // go to the next graph
                currentChartIndex++;
            }
            // warn the users if there is no next graph
            else{
                std::cout<<"No more data to show"<<std::endl;
                std::cout<<"Please type \"leave\" to go back to the menu or type \"prev\" to view previous page"<<std::endl;
                std::getline(std::cin,input);
                continue;
            }
        }
        // when users type "prev"
        else if(input=="prev"){
            // check if there is a previous graph 
            if(currentChartIndex>1){
                // go back to the previous graph
                currentChartIndex--;
            }
            // warn the users if there is not previous graph
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

        // clear the posToDraw variable if there is data inside already
        if(!posToDraw.empty()){
            posToDraw.clear();
        }

        // when users select to view "volume"
        if(mode=="volume"){
            // declare and initialise the top and bottom y-axis labels
            std::map<std::string,unsigned long> top_bottom_YLables;
            top_bottom_YLables=getYLabel();

            // declare and initialise all the y-axis labels 
            std::vector<unsigned long> yLabels;
            yLabels=getYLabels(top_bottom_YLables);

            // update the "posToDraw" variable to get where to print the points
            posToDraw=update_posToDraw(yLabels);

            // draw the graph
            draw(posToDraw,yLabels);
        }
        // when users select to view "open", "close", "high", and "low"
        else{
            // declare and initialise the top and bottom y-axis labels
            std::map<std::string,double> top_bottom_YLables;
            top_bottom_YLables=getYLabel(mode);

            // declare and initialise all the y-axis labels 
            std::vector<double> yLabels;
            yLabels=getYLabels(top_bottom_YLables);

            // update the "posToDraw" variable to get where to print the points
            posToDraw=update_posToDraw(mode,yLabels);

            // draw the graph
            draw(posToDraw,yLabels);
        }

        std::cout<<"Please type: \"prev\"-go to previous page, \"next\"-go to next page, \"leave\"-to go back to the menu"<<std::endl;
        std::getline(std::cin,input);
    }
}

std::map<std::string,double> SolOrder::getYLabel(std::string mode)
{
    // variables store the max and min values found
    double max;
    double min;

    // when users select "open"
    if(mode=="open"){
        // update max and min
        max=sol_orders[0].getOpen();
        min=sol_orders[0].getOpen();

        for(int i=1;i<sol_orders.size();++i){
            if(sol_orders[i].getOpen()>max){
                max=sol_orders[i].getOpen();
            }
            if(sol_orders[i].getOpen()<min){
                min=sol_orders[i].getOpen();
            }
        }
    }
    // when users select "high"
    else if(mode=="high"){
        // update max and min
        max=sol_orders[0].getHigh();
        min=sol_orders[0].getHigh();

        for(int i=1;i<sol_orders.size();++i){
            if(sol_orders[i].getHigh()>max){
                max=sol_orders[i].getHigh();
            }
            if(sol_orders[i].getHigh()<min){
                min=sol_orders[i].getHigh();
            }
        }
    }
    // when users select "low"
    else if(mode=="low"){
        // update max and min
        max=sol_orders[0].getLow();
        min=sol_orders[0].getLow();

        for(int i=1;i<sol_orders.size();++i){
            if(sol_orders[i].getLow()>max){
                max=sol_orders[i].getLow();
            }
            if(sol_orders[i].getLow()<min){
                min=sol_orders[i].getLow();
            }
        }
    }
    // when users select "close"
    else{
        // update max and min
        max=sol_orders[0].getClose();
        min=sol_orders[0].getClose();

        for(int i=1;i<sol_orders.size();++i){
            if(sol_orders[i].getClose()>max){
                max=sol_orders[i].getClose();
            }
            if(sol_orders[i].getClose()<min){
                min=sol_orders[i].getClose();
            }
        }
    }

    std::map<std::string, double> top_bottom_yLabel;
    top_bottom_yLabel["top"]=max;
    top_bottom_yLabel["bottom"]=min;

    return top_bottom_yLabel;
}

std::map<std::string,unsigned long> SolOrder::getYLabel()
{
    // variables store the max and min values found
    unsigned long max=sol_orders[0].getVolume();
    unsigned long min=sol_orders[0].getVolume();

    // update max and min
    for(int i=1;i<sol_orders.size();++i){
        if(sol_orders[i].getVolume()>max){
            max=sol_orders[i].getVolume();
        }
        if(sol_orders[i].getVolume()<min){
            min=sol_orders[i].getVolume();
        }
    }

    std::map<std::string,unsigned long> top_bottom_yLabel;
    top_bottom_yLabel["top"]=max;
    top_bottom_yLabel["bottom"]=min;

    return top_bottom_yLabel;
}

std::vector<double> SolOrder::getYLabels(std::map<std::string,double> top_bottom_YLabels)
{
    // the range of the top and bottom y-axis labels
    double range=top_bottom_YLabels["top"]-top_bottom_YLabels["bottom"];
    // the gap between 2 y-axis labels
    double gap=range/intervalNum;

    // get all y-axis labels
    std::vector<double> yLabels;
    for(int i=0;i<intervalNum;++i){
        yLabels.push_back(top_bottom_YLabels["top"]-gap*i);
    }

    return yLabels;
}

std::vector<unsigned long> SolOrder::getYLabels(std::map<std::string,unsigned long> top_bottom_YLabels)
{
    // the range of the top and bottom y-axis labels
    unsigned long range=top_bottom_YLabels["top"]-top_bottom_YLabels["bottom"];
    // the gap between 2 y-axis labels
    unsigned long gap=range/intervalNum;

    // get all y-axis labels
    std::vector<unsigned long> yLabels;
    for(int i=0;i<intervalNum;++i){
        yLabels.push_back(top_bottom_YLabels["top"]-gap*i);
    }

    return yLabels;
}

std::vector<std::vector<unsigned int>> SolOrder::update_posToDraw(std::string mode,std::vector<double> yLabels)
{
    int displayedDataNum;
    int startIndex=(currentChartIndex-1)*dataToShow;
    // get the number of date to print out
    if(dataToShow*currentChartIndex<=sol_orders.size()){
        displayedDataNum=dataToShow;
    }
    else{
        displayedDataNum=sol_orders.size()-(currentChartIndex-1)*dataToShow;
    }

    std::vector<std::vector<unsigned int>> positions;
    unsigned int cloestToIndex=0;
    unsigned long cloestDifference;

    // find the positions of where to print the point
    // when users select "open"
    if(mode=="open"){
        for(unsigned int i=0;i<displayedDataNum;++i){
            cloestDifference=sol_orders[i+startIndex].getOpen()-yLabels[0];
            for(unsigned int j=1;j<yLabels.size();++j){
                double difference=sol_orders[i+startIndex].getOpen()-yLabels[j];

                if(difference<cloestDifference){
                    cloestDifference=difference;
                    cloestToIndex=j;
                }
            }

            std::vector<unsigned int> pos={i,cloestToIndex};
            positions.push_back(pos);
        }
    }
    // when users select "high"
    else if(mode=="high"){
        for(unsigned int i=0;i<displayedDataNum;++i){
            cloestDifference=sol_orders[i+startIndex].getHigh()-yLabels[0];
            for(unsigned int j=1;j<yLabels.size();++j){
                double difference=sol_orders[i+startIndex].getHigh()-yLabels[j];

                if(difference<cloestDifference){
                    cloestDifference=difference;
                    cloestToIndex=j;
                }
            }

            std::vector<unsigned int> pos={i,cloestToIndex};
            positions.push_back(pos);
        }
    }
    // when users select "low"
    else if(mode=="low"){
        for(unsigned int i=0;i<displayedDataNum;++i){
            cloestDifference=sol_orders[i+startIndex].getLow()-yLabels[0];
            for(unsigned int j=1;j<yLabels.size();++j){
                double difference=sol_orders[i+startIndex].getLow()-yLabels[j];

                if(difference<cloestDifference){
                    cloestDifference=difference;
                    cloestToIndex=j;
                }
            }

            std::vector<unsigned int> pos={i,cloestToIndex};
            positions.push_back(pos);
        }
    }
    // when users select "close"
    else{
        for(unsigned int i=0;i<displayedDataNum;++i){
            cloestDifference=sol_orders[i+startIndex].getClose()-yLabels[0];
            for(unsigned int j=1;j<yLabels.size();++j){
                double difference=sol_orders[i+startIndex].getClose()-yLabels[j];

                if(difference<cloestDifference){
                    cloestDifference=difference;
                    cloestToIndex=j;
                }
            }

            std::vector<unsigned int> pos={i,cloestToIndex};
            positions.push_back(pos);
        }
    }

    return positions;
}

std::vector<std::vector<unsigned int>> SolOrder::update_posToDraw(std::vector<unsigned long> yLabels)
{
    int displayedDataNum;
    int startIndex=(currentChartIndex-1)*dataToShow;
    // get the number of date to print out
    if(dataToShow*currentChartIndex<=sol_orders.size()){
        displayedDataNum=dataToShow;
    }
    else{
        displayedDataNum=sol_orders.size()-(currentChartIndex-1)*dataToShow;
    }

    // find the positions of where to print the point
    std::vector<std::vector<unsigned int>> positions;
    for(unsigned int i=0;i<displayedDataNum;++i){
        unsigned int cloestToIndex=0;
        unsigned long cloestDifference=sol_orders[i+startIndex].getVolume()-yLabels[0];
        for(unsigned int j=1;j<yLabels.size();++j){
            unsigned long difference=sol_orders[i+startIndex].getVolume()-yLabels[j];

            if(difference<cloestDifference){
                cloestDifference=difference;
                cloestToIndex=j;
            }
        }

        std::vector<unsigned int> pos={i,cloestToIndex};
        positions.push_back(pos);
    }

    return positions;
}

void SolOrder::draw(std::vector<std::vector<unsigned int>> positions,std::vector<double> yLabels)
{
    // print the y-axis labels and points
    std::cout<<std::endl;
    for(unsigned int i=0;i<yLabels.size();++i){
        std::cout<<std::setw(14)<<std::left<<yLabels[i];
        for(unsigned int j=0;j<positions.size();++j){
            if(i==positions[j][1]){
                std::cout<<LIGHT_BLUE<<"*";
            }
            else{
                std::cout<<" ";
            }
        }
        std::cout<<RESET<<std::endl;
    }
   
    int startIndex=(currentChartIndex-1)*dataToShow;
    // positions for where to print print date as x-axis label
    std::map<int,std::string> posForDates;
    // print "|" for date and get corresponding the positions and dates 
    std::cout<<std::setw(14)<<std::left<<" ";
    for(int i=0;i<positions.size();++i){
        std::string date=sol_orders[i+startIndex].getTime();
        std::vector<std::string> date_tokens=CSVReader::tokenise(date,'/');

        if(date_tokens[0]=="01"||date_tokens[0]=="15"){
            std::cout<<"|";
            posForDates[i-4]=date;
        }
        else{
            std::cout<<" ";
        }
    }
    std::cout<<std::endl;

    // print all dates as x-axis label
    int pos_index=0;
    std::cout<<std::setw(14)<<std::right<<"Dates->";
    while(pos_index<positions.size()){
        if(posForDates.count(pos_index)==1){
            std::cout<<posForDates[pos_index];
            pos_index+=10;
        }
        else{
            std::cout<<" ";
            pos_index++;
        }
    }
    std::cout<<std::endl;
}

void SolOrder::draw(std::vector<std::vector<unsigned int>> positions,std::vector<unsigned long> yLabels)
{
    // print the y-axis labels and points
    std::cout<<std::endl;
    for(unsigned int i=0;i<yLabels.size();++i){
        std::cout<<std::setw(14)<<std::left<<yLabels[i];
        for(unsigned int j=0;j<positions.size();++j){
            if(i==positions[j][1]){
                std::cout<<LIGHT_BLUE<<"*";
            }
            else{
                std::cout<<" ";
            }
        }
        std::cout<<RESET<<std::endl;
    }


    int startIndex=(currentChartIndex-1)*dataToShow;
    // positions for where to print print date as x-axis label
    std::map<int,std::string> posForDates;
    // print "|" for date and get corresponding the positions and dates 
    std::cout<<std::setw(14)<<std::left<<" ";
    for(int i=0;i<positions.size();++i){
        std::string date=sol_orders[i+startIndex].getTime();
        std::vector<std::string> date_tokens=CSVReader::tokenise(date,'/');

        if(date_tokens[0]=="01"||date_tokens[0]=="15"){
            std::cout<<"|";
            posForDates[i-4]=date;
        }
        else{
            std::cout<<" ";
        }
    }
    std::cout<<std::endl;

    // print all dates as x-axis label
    int pos_index=0;
    std::cout<<std::setw(14)<<std::right<<"Dates->";
    while(pos_index<positions.size()){
        if(posForDates.count(pos_index)==1){
            std::cout<<posForDates[pos_index];
            pos_index+=10;
        }
        else{
            std::cout<<" ";
            pos_index++;
        }
    }
    std::cout<<std::endl;
}
// My own code ends
// ===================================================================================
