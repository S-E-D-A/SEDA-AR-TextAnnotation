#ifndef __ML_DRIVER_H
#define __ML_DRIVER_H

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <iterator>

struct ARSummary
{
    int nChapter;
    std::vector<double> vTopWordFreqs;
    std::vector<std::string> vTopWords;
    int nNumSumWords;
    std::vector<std::string> vSummary;
};

class MLDriver
{
public:
    MLDriver();
    ARSummary* GetSummary(int nChapter);

protected:
    std::vector<std::string> ParseLine(std::string sInput);

};



#endif
