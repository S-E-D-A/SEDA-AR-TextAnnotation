#include "MLDriver.h"

using namespace std;

MLDriver::MLDriver()
{

}

ARSummary* MLDriver::GetSummary(int nChapter)
{

    int nPad = 3;
    ostringstream ss;
    ss << setw(nPad) << setfill('0') << nChapter;
    string sChapter = "Summaries/" + ss.str() + ".txt";

    vector<string> vLines; //4 lines
    string sLine;
    ifstream fsSummary(sChapter.c_str());
    if (fsSummary.is_open())
    {
        while ( getline(fsSummary, sLine) )
            vLines.push_back(sLine);
        fsSummary.close();
    }

    ARSummary* pChapSummary = new ARSummary;
    vector<string> vStrings;

    //First line: top word frequencies
    vStrings = ParseLine(vLines[0]);
    double maxfreq = 0;
    double freq;
    for (int i=0; i<vStrings.size(); i++)
    {
        freq = atoi(vStrings[i].c_str());
        pChapSummary->vTopWordFreqs.push_back(freq);
        if (freq > maxfreq)
            maxfreq = freq;
    }
    for (int i=0; i<vStrings.size(); i++)
        pChapSummary->vTopWordFreqs[i] /= maxfreq; //normalize to 1

    //Second line: top words
    vStrings = ParseLine(vLines[1]);
    pChapSummary->vTopWords = vStrings;

    //Third line: number of words in summary
    pChapSummary->nNumSumWords = atoi(vLines[2].c_str());

    //Fourth line: summary
    vStrings = ParseLine(vLines[3]);
    for (int i=0; i<vStrings.size(); i++)
        pChapSummary->vSummary.push_back(vStrings[i].c_str());

    //Store the chapter number in the Chapter Object
    pChapSummary->nChapter = nChapter;

    return pChapSummary;

}

vector<string> MLDriver::ParseLine(string sInput)
{
    stringstream ss(sInput);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vOutput(begin, end);
    return vOutput;
}







