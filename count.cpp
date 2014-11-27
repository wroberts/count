/**
 * Copyright (c) 2008 Will Roberts
 * All Rights Reserved.
 *
 * Name:          count
 *
 * Author:        wildwilhelm@gmail.com (WKR)
 * Creation Date: 11 November 2008
 *
 * Purpose:       Standard input line counter
 *
 * Description:
 *    count counts the number of unique lines on the standard input,
 *    outputting the counts after all the input is read.  The output
 *    of the program can be piped through sort -nr to sort it in order
 *    of descending frequency.
 *
 * Revision Information:
 *
 *    (WKR) 11 November 2008
 *          - Initial version.
 *
 * \file count.cpp
 */

#include <unistd.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

void
printHelp()
{
    cout << "Counts unique lines on standard input, outputting the counts after all" << endl;
    cout << "input is read.  Output is sorted in alphabetical order of lines.  The" << endl;
    cout << "output of the program can be sort it in order of descending frequency" << endl;
    cout << "using the -f option or by piping through sort -nr." << endl;
    cout << endl;
    cout << "Syntax:" << endl;
    cout << endl;
    cout << "   count [OPTIONS]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << endl;
    cout << "   -e      always count the last line of the input, even if it is" << endl;
    cout << "           empty" << endl;
    cout << "   -f      sort output in order of descending frequency" << endl;
    cout << "   -?      display this help message and exit" << endl;
}

template<typename A, typename B>
pair<B,A> flip_pair(const pair<A,B> &p)
{
    return pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
map<B,A> flip_map(const map<A,B> &src)
{
    map<B,A> dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()),
              flip_pair<A,B>);
    return dst;
}

int
main ( int    argc,
       char **argv )
{
    bool       fIncludeLastLine    = false;
    bool       fSortDecreasingFreq = false;
    int        c;
    while ((c = getopt(argc, argv, "ef?")) != -1)
    {
        switch(c)
        {
        case 'e':
            fIncludeLastLine = true;
            break;
        case 'f':
            fSortDecreasingFreq = true;
            break;
        case '?':
            printHelp();
            exit(1);
            break;
        default:
            break;
        }
    }

    int             nNumLines = 0;
    map<string,int> LineDict;
    while ( cin.good() )
    {
        string sLine;
        getline ( cin, sLine );
        if (fIncludeLastLine || !cin.eof() || sLine.length() != 0)
        {
            LineDict[sLine] += 1;
        }
        nNumLines       += 1;
    }
    //int nWidth = 0;
    //while ( nNumLines > 0 )
    //{
    //    nWidth++;
    //    nNumLines /= 10;
    //}
    if (fSortDecreasingFreq)
    {
        map<int,string> FlippedLineDict = flip_map(LineDict);
        for ( map<int,string>::reverse_iterator iterator =
                  FlippedLineDict.rbegin();
              iterator != FlippedLineDict.rend(); iterator++ )
        {
            cout << iterator->first << "\t" << iterator->second << endl;
        }
    }
    else
    {
        for ( map<string,int>::iterator iterator = LineDict.begin();
              iterator != LineDict.end(); iterator++ )
        {
            cout << iterator->second << "\t" << iterator->first << endl;
        }
    }
    return 0;
}
