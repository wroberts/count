/**
 * Copyright (c) 2013 Will Roberts
 * All Rights Reserved.
 *
 * Name:          sortalph
 *
 * Author:        wildwilhelm@gmail.com (WKR)
 * Creation Date: 15 May 2014
 *
 * Purpose:       Sorts a count files alphabetically
 *
 * Description:
 *    sortalph reads in a count files, sorts it alphabetically, and
 *    sums duplicate entries if they are present, outputting the
 *    results to standard output (or to file, if specified).  The
 *    input count file contains at least two tab-separated columns;
 *    the first specifying the count and the second the value.
 *
 * Revision Information:
 *
 *    (WKR) 15 May 2014
 *          - Initial version.
 *
 * \file sortalph.cpp
 */

//#define DEBUG

#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

void
printHelp()
{
    cout << "sortalph reads in a count file, sorts it alphabetically, and sums" << endl;
    cout << "duplicate entries if they are present, outputting the results to" << endl;
    cout << "standard output (or to the file OUTPUT, if this is specified).  The" << endl;
    cout << "input count file INPUT contains at least two tab-separated columns;" << endl;
    cout << "the first specifying the count and the second the value.  If INPUT is" << endl;
    cout << "not specified, or is given as the character \"-\", sortalph will read " << endl;
    cout << "from standard input." << endl;
    cout << endl;
    cout << "Syntax:" << endl;
    cout << endl;
    cout << "   sortalph [OPTIONS] [INPUT [OUTPUT]]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << endl;
    cout << "   -f      sort output in order of descending frequency" << endl;
    cout << "   -d      interpret counts as floating-point numbers" << endl;
    cout << "   -?      display this help message" << endl;
}

void
cleanup ( ifstream *inputFile,
          ofstream *outputFile )
{
    if (inputFile)
    {
        inputFile->close();
        delete inputFile;
        inputFile = 0;
    }
    if (outputFile)
    {
        outputFile->close();
        delete outputFile;
        outputFile = 0;
    }
}

bool
ReadCountLine ( const string &sFileName,
                ifstream     *inputFile,
                bool          fFloatingPoint,
                bool         &fDidRead,
                int          &nLinesRead,
                int          &nCount,
                double       &nFloatCount,
                string       &sValue )
{
    fDidRead     = false;
    string sLine = "";
    if ((inputFile && *inputFile) ||
        (!inputFile && cin))
    {
        fDidRead    = true;
        nLinesRead += 1;
        if (inputFile)
            getline ( *inputFile, sLine );
        else
            getline ( cin, sLine );
        if (((inputFile && !*inputFile) ||
             (!inputFile && !cin)) && sLine.length() == 0)
        {
            fDidRead = false;
            return false;
        }
        size_t nFirstTab = sLine.find_first_of('\t');
        if (nFirstTab == string::npos)
        {
            sValue    = "";
            nFirstTab = sLine.length();
        }
        else
        {
            sValue = sLine.substr(nFirstTab + 1);
        }
        istringstream iss(sLine.substr(0, nFirstTab));
        if (fFloatingPoint)
        {
            iss >> nFloatCount;
        }
        else
        {
            iss >> nCount;
        }
        if (iss.fail())
        {
            cerr << sFileName << ":" << nLinesRead
                 << ": error: could not read count field" << endl;
            return true;
        }
    }

    return false;
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

int main ( int argc, char **argv )
{
#ifdef DEBUG
    cout << "Hello, world!" << endl;
#endif // DEBUG

    bool       fFloatingPoint      = false;
    bool       fSortDecreasingFreq = false;
    int        c;
    while ((c = getopt(argc, argv, "df?")) != -1)
    {
        switch(c)
        {
        case 'd':
            fFloatingPoint = true;
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

    string    sInputFileName  = "";
    string    sOutputFileName = "";

    ifstream *inputFile       = 0;
    ofstream *outputFile      = 0;

    if ((argc - optind) < 1)
    {
        sInputFileName = "-";
    }
    else
    {
        sInputFileName = argv[optind++];
    }

    if (sInputFileName.compare("-") == 0)
    {
#ifdef DEBUG
        cout << "input from stdin" << endl;
#endif // DEBUG
        inputFile      = 0;
        sInputFileName = "<stdin>";
    }
    else
    {
#ifdef DEBUG
        cout << "input from " << sInputFileName << endl;
#endif // DEBUG
        inputFile = new ifstream();
        inputFile->open(sInputFileName.c_str());
        if (!*inputFile)
        {
            cerr << "ERROR: Could not open file " << sInputFileName << endl;
            exit(1);
        }
    }

    if ((argc - optind) == 0)
    {
#ifdef DEBUG
        cout << "output to stdout" << endl;
#endif // DEBUG
        outputFile      = 0;
        sOutputFileName = "<stdout>";
    }
    else
    {
        sOutputFileName = argv[optind];
#ifdef DEBUG
        cout << "output to " << sOutputFileName << endl;
#endif // DEBUG
        outputFile = new ofstream(sOutputFileName.c_str());
        if (!*outputFile)
        {
            cerr << "ERROR: Could not open file " << sOutputFileName << endl;
            cleanup(inputFile, outputFile);
            exit(1);
        }
    }

    int                nLineNum = 0;
    map<string,int>    LineDict;
    map<string,double> LineDictFloat;
    bool               fReadLine;
    int                nCount;
    double             nFloatCount;
    string             sValue;

    if (ReadCountLine ( sInputFileName,
                        inputFile,
                        fFloatingPoint,
                        fReadLine,
                        nLineNum,
                        nCount,
                        nFloatCount,
                        sValue ))
    {
        cleanup(inputFile, outputFile);
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
        if (fFloatingPoint)
            LineDictFloat[sValue] += nFloatCount;
        else
            LineDict[sValue] += nCount;
        // process 1
        if (ReadCountLine ( sInputFileName,
                            inputFile,
                            fFloatingPoint,
                            fReadLine,
                            nLineNum,
                            nCount,
                            nFloatCount,
                            sValue ))
        {
            cleanup(inputFile, outputFile);
            exit(1);
        }
    }

    if (fSortDecreasingFreq)
    {
        if (fFloatingPoint)
        {
            map<double,string> FlippedLineDictFloat = flip_map(LineDictFloat);
            for ( map<double,string>::reverse_iterator iterator =
                      FlippedLineDictFloat.rbegin();
                  iterator != FlippedLineDictFloat.rend(); iterator++ )
            {
                if (outputFile)
                    *outputFile << iterator->first << "\t" << iterator->second << endl;
                else
                    cout << iterator->first << "\t" << iterator->second << endl;
            }
        }
        else
        {
            map<int,string> FlippedLineDict = flip_map(LineDict);
            for ( map<int,string>::reverse_iterator iterator =
                      FlippedLineDict.rbegin();
                  iterator != FlippedLineDict.rend(); iterator++ )
            {
                if (outputFile)
                    *outputFile << iterator->first << "\t" << iterator->second << endl;
                else
                    cout << iterator->first << "\t" << iterator->second << endl;
            }
        }
    }
    else
    {
        if (fFloatingPoint)
        {
            for ( map<string,double>::iterator iterator = LineDictFloat.begin();
                  iterator != LineDictFloat.end(); iterator++ )
            {
                if (outputFile)
                    *outputFile << iterator->second << "\t" << iterator->first << endl;
                else
                    cout << iterator->second << "\t" << iterator->first << endl;
            }
        }
        else
        {
            for ( map<string,int>::iterator iterator = LineDict.begin();
                  iterator != LineDict.end(); iterator++ )
            {
                if (outputFile)
                    *outputFile << iterator->second << "\t" << iterator->first << endl;
                else
                    cout << iterator->second << "\t" << iterator->first << endl;
            }
        }
    }

    cleanup(inputFile, outputFile);

    return 0;
}
