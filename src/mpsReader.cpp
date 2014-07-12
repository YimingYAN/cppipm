//
//  mpsReader.cpp
//  cppipm
//
//  Created by Yiming Yan on 11/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#include "mpsReader.h"
#include <fstream>

/*
 * Constructor
 */
mpsReader::mpsReader(std::string fileName)
{
    std::ifstream readFile(fileName);

    if (readFile.is_open())
    {
        // get rid of comments or blank line
        _findPos2Start(readFile);
        
        std::cout<<"+ Reading mps file"<<std::endl;
        // get problem dimention
        _preprocScan(readFile);
        
        // initilize matrices and vecrors
        _initializeData();
        
        // extract data
        _extractData(readFile);
        
        readFile.close();
        
        // Output data
        
        // verbose > 0
        std::cout<<"Name       : "<<Name<<std::endl;
        std::cout<<"objsense   : "<<objsense<<std::endl;
        std::cout<<"n_rows_eq  : "<<n_rows_eq<<std::endl;
        std::cout<<"n_rows_inq : "<<n_rows_inq<<std::endl;
        std::cout<<"n_cols     : "<<n_cols<<std::endl;
        // verbose > 1
        Q.print("Q = ");
        A.print("A = ");        b.print("b = ");
        Aeq.print("Aeq = ");    beq.print("beq = ");
        c.print("c = ");
        lb.print("lb = ");      ub.print("ub = ");
        
    }
    else
    {
        std::cout<<"Error: MPSREADER - File not found"<<std::endl;
    }
    
    readFile.close();
}


/*
 * Internal functions
 */

void mpsReader::_extractData(std::ifstream &readFile)
{
    std::cout<< "  + Extracting data..." << std::endl;

}

void mpsReader::_preprocScan(std::ifstream &readFile)
{
    n_rows = 0;
    n_rows_eq = 0;
    n_rows_inq = 0;
    n_cols = 0;
    objsense = "";
    
    std::string tmp = "_";
    // get first word of each line
    std::string firstWord;
    readFile >> firstWord;
    //cout <<"1+" << firstWord << endl;
    
    while (!readFile.eof())
    {
        if ( !firstWord.empty() && firstWord.find("*") != 0)
        {
            if ( _checkFieldName(firstWord) == 10 )
                break;
            
            if ( _checkFieldName(firstWord) == 8)
                std::cout<< "Warning: MPSREASER - Currently cannot handle SOS"<<std::endl;
            
            if ( _checkFieldName(firstWord) == 9)
                std::cout<< "Warning: MPSREASER - Currently cannot handle RANGES"<<std::endl;
            
            if ( _checkFieldName(firstWord) == 1 ) // name
            {
                readFile >> Name;
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkFieldName(firstWord) == 2 ) //rows
            {
                readFile >> firstWord;
                //cout <<"2+"<< firstWord << endl;

                // count n_rows, n_rows_eq, n_rows_inq
                while (_checkFieldName(firstWord) == -1)
                {
                    if ( firstWord.compare("E") == 0 )
                    {
                        n_rows++;
                        n_rows_eq++;
                    }
                    else if ( firstWord.compare("L") == 0 )
                    {
                        n_rows++;
                        n_rows_inq++;
                    }
                    else if ( firstWord.compare("G") == 0 )
                    {
                        n_rows++;
                        n_rows_inq++;
                    }
                    else if ( firstWord.compare("N") == 0 )
                    {
                        n_rows++;
                    }
                    _nextLine(readFile);
                    readFile >> firstWord;
                    //cout <<"22+"<< firstWord << endl;
                }
            }
            else if ( _checkFieldName(firstWord) == 3 ) // cols
            {
                // count columns
                readFile >> firstWord;
                //cout <<"3+"<< firstWord << endl;
                
                while (_checkFieldName(firstWord) == -1)
                {
                    if ( firstWord.compare(tmp) != 0 )
                    {
                        n_cols ++;
                        tmp = firstWord;
                    }
                    _nextLine(readFile);
                    readFile >> firstWord;
                    //cout <<"33+"<< firstWord << endl;
                }
                
            }
            else if ( _checkFieldName(firstWord) == 7) // objsense
            {
                _nextLine(readFile);
                readFile >> objsense;
                readFile >> firstWord;
                //cout <<"7+"<< firstWord << endl;
            }
            else
            {
                _nextLine(readFile);
                readFile >> firstWord;
                //cout<<"else: "<<firstWord<<endl;
            }
            
        }
    }
    
}


int mpsReader::_checkFieldName(std::string checkWord) const
{
    
    if (checkWord.compare("NAME") == 0)
        return 1;
    else if (checkWord.compare("ROWS") == 0)
        return 2;
    else if (checkWord.compare("COLUMNS") == 0)
        return 3;
    else if (checkWord.compare("RHS") == 0)
        return 4;
    else if (checkWord.compare("BOUNDS") == 0)
        return 5;
    else if (checkWord.compare("QUADOBJ") == 0)
        return 6;
    else if (checkWord.compare("OBJSENSE") == 0)
        return 7;
    else if (checkWord.compare("SOS") == 0)
        return 8;
    else if (checkWord.compare("RANGES") == 0)
        return 9;
    else if (checkWord.compare("ENDATA") == 0)
        return 10;
    else
        return -1;
}

void mpsReader::_findPos2Start(std::ifstream &readFile)
{
    std::streampos pos;
    std::string line;
    std::string firdWord;
    while (true)
    {
        pos = readFile.tellg();
        getline(readFile, line);
        if ( line.empty() )
            continue;
        else if ( line.find("*") == 0)
            continue;
        else
        {
            readFile.seekg(pos, std::ios::beg);   // go back one line
            break;
        }
    }
}

void mpsReader::_nextLine(std::ifstream &readFile)
{
    readFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mpsReader::_initializeData()
{
    Q   = mat(n_cols    , n_cols, fill::zeros);
    A   = mat(n_rows_inq, n_cols, fill::zeros);
    b   = vec(n_rows_inq,         fill::zeros);
    Aeq = mat(n_rows_eq , n_cols, fill::zeros);
    beq = vec(n_rows_eq ,         fill::zeros);
    c   = vec(n_cols    ,         fill::zeros);
    lb  = vec(n_cols    ,         fill::zeros);
    ub  = vec(n_cols    ,         fill::zeros);
}