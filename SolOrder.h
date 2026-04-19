// ===================================================================================
// My own code starts
#pragma once

#include "SolCandlestick.h"
#include <vector> 
#include <map>

// define the colour used to colour the text displayed in terminal
#define RESET "\033[0m"
#define LIGHT_BLUE "\033[96m"

class SolOrder
{
    public:
        SolOrder(std::string filename);

        void plot(std::string mode);

    private:
        /** the number of y-axis labels to display */
        int intervalNum=20;
        /** the number of maximum dates to show at a time */
        int dataToShow=106;

        /** the maximum number of charts that users can use "next" and "prev" command to switch between */
        int maxChartNum;
        /** the current index of the chart */
        int currentChartIndex=1;

        /** a vector containing points' potitions(vector of 2 unsigned int) for where to draw them inside a nested for loop */
        std::vector<std::vector<unsigned int>> posToDraw;

        /** a vector storing all the "SOL-USD.csv" data as SolCandlestick data type objects */
        std::vector<SolCandlestick> sol_orders;

        /** returns a map containing the top and bottom values of the y-axis labels of "open", "close", "high", and "low" */
        std::map<std::string,double> getYLabel(std::string mode);
        /** returns a map containing the top and bottom values of the y-axis labels of "volume" */
        std::map<std::string,unsigned long> getYLabel();

        /** returns a vector containing all the y-axis labels of "open", "close", "high", and "low" */
        std::vector<double> getYLabels(std::map<std::string,double> top_bottom_YLabels);
        /** returns a vector containing all the y-axis labels of "volume" */
        std::vector<unsigned long> getYLabels(std::map<std::string,unsigned long> top_bottom_YLabels);

        /** update the "posToDraw" variable for the next chart to be printed by returning a vector of potitions(vector of 2 unsigned int) 
            used for "open", "close", "high", and "low" */
        std::vector<std::vector<unsigned int>> update_posToDraw(std::string mode,std::vector<double> yLabels);
        /** update the "posToDraw" variable for the next chart to be printed by returning a vector of potitions(vector of 2 unsigned int) 
            used for "volume" */
        std::vector<std::vector<unsigned int>> update_posToDraw(std::vector<unsigned long> yLabels);

        /** the actual plotting function, used for "open", "close", "high", and "low" */
        void draw(std::vector<std::vector<unsigned int>> positions,std::vector<double> yLabels);
        /** the actual plotting function, used for "volume" */
        void draw(std::vector<std::vector<unsigned int>> positions,std::vector<unsigned long> yLabels);
};
// My own code ends
// ===================================================================================