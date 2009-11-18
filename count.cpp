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

#include <string>
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

int
main ( int    argc,
       char **argv )
{
    int             nNumLines = 0;
    map<string,int> LineDict;
    while ( cin.good() )
    {
        string sLine;
        getline ( cin, sLine );
        LineDict[sLine] += 1;
        nNumLines       += 1;
    }
    //int nWidth = 0;
    //while ( nNumLines > 0 )
    //{
    //    nWidth++;
    //    nNumLines /= 10;
    //}
    for ( map<string,int>::iterator iterator = LineDict.begin();
          iterator != LineDict.end(); iterator++ )
    {
        //cout << setw(nWidth) << iterator->second
        //         << "  " << iterator->first << endl;
        cout << iterator->second << "\t" << iterator->first << endl;
    }
    return 0;
}
