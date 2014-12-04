/**
 * Copyright (c) 2013 Will Roberts
 * All Rights Reserved.
 *
 * Name:          threshcount
 *
 * Author:        wildwilhelm@gmail.com (WKR)
 * Creation Date: 8 December 2013
 *
 * Purpose:       Thresholds a count file
 *
 * Description:
 *    threshcount reads a count file on standard input, discarding
 *    those lines whose counts are less than or equal to the threshold
 *    argument passed on the command line.  It outputs the result to
 *    standard output.  The input contains at least two tab-separated
 *    columns; the first specifying the count and the second the
 *    value.
 *
 * Revision Information:
 *
 *    (WKR) 8 December 2013
 *          - Initial version.
 *
 * \file threshcount.cpp
 */

//#define DEBUG

#include "config.h"
#include <iostream>
#include <sstream>
using namespace std;

void
printHelp()
{
    cout << "threshcount - " << PACKAGE_STRING << endl << endl;
    cout << "threshcount reads a count file on standard input, discarding those" << endl;
    cout << "lines whose counts are less than or equal to the threshold argument" << endl;
    cout << "passed on the command line.  It outputs the result to standard output." << endl;
    cout << "The input contains at least two tab-separated columns; the first" << endl;
    cout << "specifying the count and the second the value." << endl;
    cout << endl;
    cout << "Syntax:" << endl;
    cout << endl;
    cout << "   threshcount [OPTIONS] THRESHOLD" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << endl;
    cout << "   -?      display this help message" << endl;
}

bool
ReadCountLine ( bool   &fDidRead,
                int    &nLinesRead,
                int    &nCount,
                string &sValue )
{
    fDidRead     = false;
    string sLine = "";
    if (cin)
    {
        fDidRead    = true;
        nLinesRead += 1;
        getline ( cin, sLine );
        if (!cin && sLine.length() == 0)
        {
            fDidRead = false;
            return false;
        }
        size_t nFirstTab = sLine.find_first_of('\t');
        if (nFirstTab == string::npos)
        {
            cerr << nLinesRead
                 << ": error: no tab character found on line" << endl;
            return true;
        }
        sValue = sLine.substr(nFirstTab + 1);
        istringstream iss(sLine.substr(0, nFirstTab));
        iss >> nCount;
        if (iss.fail())
        {
            cerr << nLinesRead
                 << ": error: could not read count field" << endl;
            return true;
        }
    }

    return false;
}

int main ( int argc, char **argv )
{
#ifdef DEBUG
    cout << "Hello, world!" << endl;
#endif // DEBUG

    int        c;
    while ((c = getopt(argc, argv, "?")) != -1)
    {
        switch(c)
        {
        case '?':
            printHelp();
            exit(1);
            break;
        default:
            break;
        }
    }

    if ((argc - optind) < 1)
    {
        cerr << "ERROR: Missing threshold argument." << endl;
        printHelp();
        exit(1);
    }

    int           nThreshold = 0;
    istringstream iss(argv[optind]);
    iss >> nThreshold;
    if (iss.fail())
    {
        cerr << "ERROR: Invalid threshold argument " << argv[optind] << endl;
        printHelp();
        exit(1);
    }
    if (nThreshold <= 0)
    {
        cerr << "ERROR: Threshold must be positive: " << nThreshold << endl;
        printHelp();
        exit(1);
    }

    int    nLineNum = 0;
    bool   fReadLine;
    int    nCount;
    string sValue;
    if (ReadCountLine ( fReadLine,
                        nLineNum,
                        nCount,
                        sValue ))
    {
        exit(1);
    }

#ifdef DEBUG
    if (fReadLine)
    {
        cout << "nCount \"" << nCount << "\"" << endl;
        cout << "sValue \"" << sValue << "\"" << endl;
    }
#endif // DEBUG

    while (fReadLine)
    {
        if (nThreshold < nCount)
        {
            cout << nCount << "\t" << sValue << endl;
        }
        // process
        if (ReadCountLine ( fReadLine,
                            nLineNum,
                            nCount,
                            sValue ))
        {
            exit(1);
        }
    }

    return 0;
}

