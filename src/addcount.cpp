/**
 * Copyright (c) 2013 Will Roberts
 * All Rights Reserved.
 *
 * Name:          addcount
 *
 * Author:        wildwilhelm@gmail.com (WKR)
 * Creation Date: 6 December 2013
 *
 * Purpose:       Sum two count files together
 *
 * Description:
 *    addcount sums the counts stored in two count files together,
 *    outputting the results to standard output (or to file, if
 *    specified).  The two input count files contain at least two
 *    tab-separated columns; the first specifying the count and the
 *    second the value.  The files must be sorted in alphabetical
 *    order on the second (value) column.
 *
 * Revision Information:
 *
 *    (WKR) 6 December 2013
 *          - Initial version.
 *
 * \file addcount.cpp
 */

//#define DEBUG

#include "config.h"
#include <getopt.h>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

void
printHelp()
{
    cout << "addcount - " << PACKAGE_STRING << endl << endl;
    cout << "addcount sums the counts stored in two count files together," << endl;
    cout << "outputting the results to standard output (or to the file OUTPUT, if" << endl;
    cout << "this is specified).  The two input count files INPUT1 and INPUT2" << endl;
    cout << "contain at least two tab-separated columns; the first specifying the" << endl;
    cout << "count and the second the value.  The files must be sorted in" << endl;
    cout << "alphabetical order on the second (value) column.  At most one of" << endl;
    cout << "INPUT1 and INPUT2 can be the character \"-\", indicating that this" << endl;
    cout << "stream should be read from standard input." << endl;
    cout << endl;
    cout << "Syntax:" << endl;
    cout << endl;
    cout << "   addcount [OPTIONS] INPUT1 INPUT2 [OUTPUT]" << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << endl;
    cout << "   -d      interpret counts as floating-point numbers" << endl;
    cout << "   -?      display this help message" << endl;
}

void
cleanup ( ifstream *inputFile1,
          ifstream *inputFile2,
          ofstream *outputFile )
{
    if (inputFile1)
    {
        inputFile1->close();
        delete inputFile1;
        inputFile1 = 0;
    }
    if (inputFile2)
    {
        inputFile2->close();
        delete inputFile2;
        inputFile2 = 0;
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
                string       &sValue,
                string       &sLastValue )
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
            cerr << sFileName << ":" << nLinesRead
                 << ": error: no tab character found on line" << endl;
            return true;
        }
        sValue = sLine.substr(nFirstTab + 1);
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

    if (nLinesRead > 1 && 0 <= sLastValue.compare(sValue))
    {
        cerr << sFileName << ":" << nLinesRead
             << ": error: file not sorted" << endl;
        return true;
    }
    sLastValue = sValue;

    return false;
}

int main ( int argc, char **argv )
{
#ifdef DEBUG
    cout << "Hello, world!" << endl;
#endif // DEBUG

    bool       fFloatingPoint      = false;
    int        c;
    while ((c = getopt(argc, argv, "d?")) != -1)
    {
        switch(c)
        {
        case 'd':
            fFloatingPoint = true;
            break;
        case '?':
            printHelp();
            exit(1);
            break;
        default:
            break;
        }
    }

    string    sFile1Name      = "";
    string    sFile2Name      = "";
    string    sOutputFileName = "";

    ifstream *inputFile1      = 0;
    ifstream *inputFile2      = 0;
    ofstream *outputFile      = 0;

    if ((argc - optind) < 2)
    {
        cerr << "ERROR: Missing input arguments." << endl;
        printHelp();
        exit(1);
    }

    sFile1Name = argv[optind];
    if (sFile1Name.compare("-") == 0)
    {
#ifdef DEBUG
        cout << "file1 stdin" << endl;
#endif // DEBUG
        inputFile1 = 0;
        sFile1Name = "<stdin>";
    }
    else
    {
#ifdef DEBUG
        cout << "file1 " << sFile1Name << endl;
#endif // DEBUG
        inputFile1 = new ifstream();
        inputFile1->open(sFile1Name.c_str());
        if (!*inputFile1)
        {
            cerr << "ERROR: Could not open file " << sFile1Name << endl;
            exit(1);
        }
    }

    sFile2Name = argv[optind + 1];
    if (sFile2Name.compare("-") == 0)
    {
        if (!inputFile1)
        {
            cerr << "ERROR: only one input file can be standard input" << endl;
            cleanup(inputFile1, inputFile2, outputFile);
            exit(1);
        }
#ifdef DEBUG
        cout << "file2 stdin" << endl;
#endif // DEBUG
        inputFile2 = 0;
        sFile2Name = "<stdin>";
    }
    else
    {
#ifdef DEBUG
        cout << "file2 " << sFile2Name << endl;
#endif // DEBUG
        inputFile2 = new ifstream();
        inputFile2->open(sFile2Name.c_str());
        if (!*inputFile2)
        {
            cerr << "ERROR: Could not open file " << sFile2Name <<endl;
            cleanup(inputFile1, inputFile2, outputFile);
            exit(1);
        }
    }
    if ((argc - optind) == 2)
    {
#ifdef DEBUG
        cout << "output to stdout" << endl;
#endif // DEBUG
        outputFile      = 0;
        sOutputFileName = "<stdout>";
    }
    else
    {
        sOutputFileName = argv[optind + 2];
#ifdef DEBUG
        cout << "output to " << sOutputFileName << endl;
#endif // DEBUG
        outputFile = new ofstream(sOutputFileName.c_str());
        if (!*outputFile)
        {
            cerr << "ERROR: Could not open file " << sOutputFileName <<endl;
            cleanup(inputFile1, inputFile2, outputFile);
            exit(1);
        }
    }

    int    nLineNum1 = 0;
    int    nLineNum2 = 0;
    bool   fReadLine1;
    int    nCount1;
    double nFloatCount1;
    string sValue1;
    string sLastValue1;
    bool   fReadLine2;
    int    nCount2;
    double nFloatCount2;
    string sValue2;
    string sLastValue2;
    if (ReadCountLine ( sFile1Name,
                        inputFile1,
                        fFloatingPoint,
                        fReadLine1,
                        nLineNum1,
                        nCount1,
                        nFloatCount1,
                        sValue1,
                        sLastValue1 ))
    {
        cleanup(inputFile1, inputFile2, outputFile);
        exit(1);
    }
    if (ReadCountLine ( sFile2Name,
                        inputFile2,
                        fFloatingPoint,
                        fReadLine2,
                        nLineNum2,
                        nCount2,
                        nFloatCount2,
                        sValue2,
                        sLastValue2 ))
    {
        cleanup(inputFile1, inputFile2, outputFile);
        exit(1);
    }

#ifdef DEBUG
    if (fReadLine1)
    {
        cout << "nCount1 \"" << nCount1 << "\"" << endl;
        cout << "sValue1 \"" << sValue1 << "\"" << endl;
    }
#endif // DEBUG

    while (fReadLine1 && fReadLine2)
    {
        int nCompare = sValue1.compare(sValue2);
        if (nCompare < 0)
        {
            if (fFloatingPoint)
            {
                if (outputFile)
                    *outputFile << nFloatCount1 << "\t" << sValue1 << endl;
                else
                    cout << nFloatCount1 << "\t" << sValue1 << endl;
            }
            else
            {
                if (outputFile)
                    *outputFile << nCount1 << "\t" << sValue1 << endl;
                else
                    cout << nCount1 << "\t" << sValue1 << endl;
            }
            // process 1
            if (ReadCountLine ( sFile1Name,
                                inputFile1,
                                fFloatingPoint,
                                fReadLine1,
                                nLineNum1,
                                nCount1,
                                nFloatCount1,
                                sValue1,
                                sLastValue1 ))
            {
                cleanup(inputFile1, inputFile2, outputFile);
                exit(1);
            }
        }
        else if (nCompare == 0)
        {
            if (fFloatingPoint)
            {
                if (outputFile)
                    *outputFile << nFloatCount1 + nFloatCount2 << "\t" << sValue1 << endl;
                else
                    cout << nFloatCount1 + nFloatCount2 << "\t" << sValue1 << endl;
            }
            else
            {
                if (outputFile)
                    *outputFile << nCount1 + nCount2 << "\t" << sValue1 << endl;
                else
                    cout << nCount1 + nCount2 << "\t" << sValue1 << endl;
            }
            // process 1
            if (ReadCountLine ( sFile1Name,
                                inputFile1,
                                fFloatingPoint,
                                fReadLine1,
                                nLineNum1,
                                nCount1,
                                nFloatCount1,
                                sValue1,
                                sLastValue1 ))
            {
                cleanup(inputFile1, inputFile2, outputFile);
                exit(1);
            }
            // process 2
            if (ReadCountLine ( sFile2Name,
                                inputFile2,
                                fFloatingPoint,
                                fReadLine2,
                                nLineNum2,
                                nCount2,
                                nFloatCount2,
                                sValue2,
                                sLastValue2 ))
            {
                cleanup(inputFile1, inputFile2, outputFile);
                exit(1);
            }
        }
        else
        {
            if (fFloatingPoint)
            {
                if (outputFile)
                    *outputFile << nFloatCount2 << "\t" << sValue2 << endl;
                else
                    cout << nFloatCount2 << "\t" << sValue2 << endl;
            }
            else
            {
                if (outputFile)
                    *outputFile << nCount2 << "\t" << sValue2 << endl;
                else
                    cout << nCount2 << "\t" << sValue2 << endl;
            }
            // process 2
            if (ReadCountLine ( sFile2Name,
                                inputFile2,
                                fFloatingPoint,
                                fReadLine2,
                                nLineNum2,
                                nCount2,
                                nFloatCount2,
                                sValue2,
                                sLastValue2 ))
            {
                cleanup(inputFile1, inputFile2, outputFile);
                exit(1);
            }
        }
    }
    while (fReadLine1)
    {
        if (fFloatingPoint)
        {
            if (outputFile)
                *outputFile << nFloatCount1 << "\t" << sValue1 << endl;
            else
                cout << nFloatCount1 << "\t" << sValue1 << endl;
        }
        else
        {
            if (outputFile)
                *outputFile << nCount1 << "\t" << sValue1 << endl;
            else
                cout << nCount1 << "\t" << sValue1 << endl;
        }
        // process 1
        if (ReadCountLine ( sFile1Name,
                            inputFile1,
                            fFloatingPoint,
                            fReadLine1,
                            nLineNum1,
                            nCount1,
                            nFloatCount1,
                            sValue1,
                            sLastValue1 ))
        {
            cleanup(inputFile1, inputFile2, outputFile);
            exit(1);
        }
    }
    while (fReadLine2)
    {
        if (fFloatingPoint)
        {
            if (outputFile)
                *outputFile << nFloatCount2 << "\t" << sValue2 << endl;
            else
                cout << nFloatCount2 << "\t" << sValue2 << endl;
        }
        else
        {
            if (outputFile)
                *outputFile << nCount2 << "\t" << sValue2 << endl;
            else
                cout << nCount2 << "\t" << sValue2 << endl;
        }
        // process 2
        if (ReadCountLine ( sFile2Name,
                            inputFile2,
                            fFloatingPoint,
                            fReadLine2,
                            nLineNum2,
                            nCount2,
                            nFloatCount2,
                            sValue2,
                            sLastValue2 ))
        {
            cleanup(inputFile1, inputFile2, outputFile);
            exit(1);
        }
    }

    cleanup(inputFile1, inputFile2, outputFile);

    return 0;
}
