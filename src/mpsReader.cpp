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
        wall_clock timer;
        timer.tic();
        
        // get rid of comments or blank line
        _findPos2Start(readFile);
        
        // get problem dimention
        _preprocScan(readFile);
        
        // initilize matrices and vecrors
        _initializeData();
        
        // extract data
        _extractData(readFile);
        
        readFile.close();
        time = timer.toc();
        
        // output
        _printData();
        

    }
    else
    {
        std::cout<<"Error: MPSREADER - File not found"<<std::endl;
        time  = 0.0;
    }
    
    readFile.close();
}


/*
 * Internal functions
 */

void mpsReader::_extractData(std::ifstream &readFile)
{
    mat Araw(n_rows, n_cols, fill::zeros);
    vec braw(n_rows, fill::zeros);
    
    // get Araw
    _getAraw(readFile, Araw);
    //Araw.print("Araw: ");
    
    // get rhs
    _getbraw(readFile, braw);
    //braw.print("braw: ");
    
    // get bounds
    if (bnd_exist)
        _getBnds(readFile);
    
    
    // get quadobj
    if (qdo_exist)
        _getQdo(readFile);
    
    // split Araw to A, Aeq, c
    // and splict braw to b and beq
    _splitRaw(Araw, braw, c, A, b, Aeq, beq);
}

void mpsReader::_preprocScan(std::ifstream &readFile)
{
    n_rows = 0;
    n_rows_eq = 0;
    n_rows_inq = 0;
    n_cols = 0;
    objsense = "";
    
    std::string tmp = "_";
    std::string tmpItem = "";
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
                // update firstWord and row_list
                readFile >> firstWord;

                while (_checkFieldName(firstWord) == -1)
                {
   
                    // count n_rows, n_rows_eq, n_rows_inq
                    if ( firstWord.compare("E") == 0 )
                    {
                        n_rows++;
                        n_rows_eq++;
                    }
                    else if ( firstWord.compare("L") == 0 || firstWord.compare("G") == 0)
                    {
                        n_rows++;
                        n_rows_inq++;
                    }
                    else if ( firstWord.compare("N") == 0 )
                    {
                        n_rows++;
                    }
                    // store row labels
                    row_labels.push_back(firstWord);
                    
                    // get row_list
                    readFile >> tmpItem;
                    row_list.push_back(tmpItem);
                    
                    _nextLine(readFile);
                    readFile >> firstWord;
                }
            }
            else if ( _checkFieldName(firstWord) == 3 ) // cols
            {
                // get postion
                col_pos = readFile.tellg();

                readFile >> firstWord;
                
                while (_checkFieldName(firstWord) == -1) // continue if the keyword is not a feild name
                {
                    if ( firstWord.compare(tmp) != 0 )
                    {
                        // update column list
                        col_list.push_back(firstWord);
                        
                        // count columns
                        n_cols ++;
                        tmp = firstWord;
                    }
                    _nextLine(readFile);
                    readFile >> firstWord;
                }
                
            }
            else if ( _checkFieldName(firstWord) == 4) //rhs
            {
                rhs_pos = readFile.tellg();
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkFieldName(firstWord) == 5) //bounds
            {
                bnd_exist = true;
                
                bnd_pos = readFile.tellg();
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkFieldName(firstWord) == 6) //quadobj
            {
                qdo_exist = true;
                
                qdo_pos = readFile.tellg();
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkFieldName(firstWord) == 7) // objsense
            {
                objsense = true;
                
                _nextLine(readFile);
                readFile >> objsense;
                readFile >> firstWord;
            }
            else
            {
                _nextLine(readFile);
                readFile >> firstWord;
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
    long pos;
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

void mpsReader::_initializeData()
{
    Q   = mat(n_cols    , n_cols, fill::zeros);
    A   = mat(n_rows_inq, n_cols, fill::zeros);
    b   = vec(n_rows_inq,         fill::zeros);
    Aeq = mat(n_rows_eq , n_cols, fill::zeros);
    beq = vec(n_rows_eq ,         fill::zeros);
    c   = vec(n_cols    ,         fill::zeros);
    lb  = vec(n_cols    ,         fill::zeros);
    ub  = vec(n_cols);  ub.fill(infty);
    
    objsense = "MIN";
}

void mpsReader::_getAraw(std::ifstream &readFile, mat &Araw)
{
    std::string line, colName, rowName;
    double value;
    int colIdx = 0, rowIdx = 0;
    
    // go to the the postion of cols
    readFile.seekg(col_pos, std::ios::beg);
    _nextLine(readFile);
    do
    {
        // clear
        colName = "";  rowName = ""; value = 0.0;
        
        // read one line
        getline(readFile, line);
        std::istringstream thisLine(line);
        
        thisLine >> colName;
        
        // break if get to next field
        if (_checkFieldName(colName) != -1)
            break;
        
        // get col index
        colIdx = _getIndex(col_list, colName);
        
        while (thisLine >> rowName >> value)
        {
            // get row index
            rowIdx = _getIndex(row_list, rowName);
            
            Araw(rowIdx,colIdx) = value;
        }
        
    } while (true);
}

void mpsReader::_getbraw(std::ifstream &readFile, vec &braw)
{
    std::string line, colName, rowName;
    double value;
    int rowIdx = 0;
    // go to the position of rhs
    readFile.seekg(rhs_pos, std::ios::beg);
    _nextLine(readFile);
    
    do
    {
        // read one line
        getline(readFile, line);
        std::istringstream thisLine(line);
        
        thisLine >> colName;
        
        if ( _checkFieldName(colName) != -1 )
            break;
        
        while (thisLine >> rowName >> value)
        {
            // get row index
            rowIdx = _getIndex(row_list, rowName);
            
            braw(rowIdx) = value;
        }
        
    } while (true);
}

void mpsReader::_getBnds(std::ifstream &readFile)
{
    std::string label, colName, rowName;
    
    double value;
    
    int colIdx = 0;

    readFile.seekg(bnd_pos, std::ios::beg);
    _nextLine(readFile);
    
    do
    {
        readFile >> label >> rowName >> colName >> value;
        colIdx = _getIndex(col_list, colName);
            
        if (label == "LO")
            lb(colIdx) = value;
        else if (label == "UP")
            ub(colIdx) = value;
        else if (label == "FX") // fixed value
        {
            lb(colIdx) = value;
            ub(colIdx) = value;
        }
        else if (label == "FR") // free variables
        {
            lb(colIdx) = -infty;
            ub(colIdx) = infty;
        }
        
    } while (_checkFieldName(label) == -1);
}

void mpsReader::_getQdo(std::ifstream &readFile)
{
    std::string colName1, colName2;
    
    double value;
    
    int colIdx1 = 0, colIdx2 = 0;

    readFile.seekg(qdo_pos, std::ios::beg);
    _nextLine(readFile);
    
    while (true)
    {
        readFile >> colName1 >> colName2 >> value;
        colIdx1 = _getIndex(col_list, colName1);
        
        if (_checkFieldName(colName1) != -1)
            break;
        
        colIdx2 = _getIndex(col_list, colName2);
        
        Q(colIdx1,colIdx2) = value;
        
        if (colIdx1 != colIdx2)
            Q(colIdx2,colIdx1) = value;
    }
}

void mpsReader::_splitRaw(mat &Araw, vec &braw, vec &c, mat &A, vec &b, mat &Aeq, vec &beq)
{
    int counter_eq = 0, counter_inq = 0;
    for (int i=0; i< n_rows; i++)
    {
        if (row_labels[i] == "N")
            c = trans( Araw.row(i) );
        else if (row_labels[i] == "E")
        {
            Aeq.row(counter_eq) = Araw.row(i);
            beq.row(counter_eq) = braw.row(i);
            counter_eq ++;
        }
        else if (row_labels[i] == "L")
        {
            A.row(counter_inq) = Araw.row(i);
            b.row(counter_inq) = braw.row(i);
            counter_inq ++;
        }
        else if (row_labels[i] == "G")
        {
            A.row(counter_inq) = -Araw.row(i);
            b.row(counter_inq) = -braw.row(i);
            counter_inq ++;
        }
        
    }
}

void mpsReader::_nextLine(std::ifstream &readFile)
{
    readFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int mpsReader::_getIndex(std::vector<std::string> &list, std::string item) const
{
    int idx = (int) (std::find(list.begin(), list.end(), item) - list.begin());
    
    if( idx >= list.size() )
        idx = -1;
    
    return idx;
}

void mpsReader::_printData()
{
    
    // Output data
    std::cout<<"\tName       : "<<Name<<std::endl;
    std::cout<<"\tobjsense   : "<<objsense<<std::endl;
    std::cout<<"\tn_rows_eq  : "<<n_rows_eq<<std::endl;
    std::cout<<"\tn_rows_inq : "<<n_rows_inq<<std::endl;
    std::cout<<"\tn_cols     : "<<n_cols<<std::endl;
    for (int i = 0; i <row_labels.size(); i++)
        std::cout<<"\trow_labels["<<i<<"] <-> row_list["<<i<<"] : " <<row_labels[i]<< " <-> "<< row_list[i] << std::endl;
    for (int i = 0; i <col_list.size(); i++)
        std::cout<<"\tcol_list["<<i<<"] :"<<col_list[i]<< std::endl;
    
    Q.print("Q: ");
    c.print("c: ");
    A.print("A: ");
    b.print("b ");
    Aeq.print("Aeq: ");
    beq.print("beq: ");
    lb.print("lb: ");
    ub.print("ub: ");
    
    std::cout<<"---------------------"<<std::endl;
    std::cout<<"MPS file read. Time = [ "<<time<<"s ]"<<std::endl;
}