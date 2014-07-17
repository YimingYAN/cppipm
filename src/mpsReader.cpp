//
//  mpsReader.cpp
//  cppipm
//
//  Created by Yiming Yan on 11/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#include "mpsReader.h"
#include <fstream>
#include <cassert>
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
        //_printData();
    }
    else
    {
        std::cout<<"Error: MPSREADER - File not found"<<std::endl;
        time  = 0.0;
    }
    
    readFile.close();
}

/*
 * Public functions
 */
int mpsReader::trans2standardForm(mat &Qs, mat &As, vec &bs, vec &cs)
{
    //cout<< "Tranforming to the standard form..."<<endl;
    assert( lb.min() > - infty);
    
    mat tmp;
    
    int n_ubounds = 0;
    for (int i=0;i < ub.size(); i++)
    {
        if (fabs(ub(i) - infty) > 1)
         n_ubounds ++;
    }
    
    As = mat(A.n_rows + n_ubounds, A.n_cols + n_ubounds, fill::zeros);
    bs = vec(b.n_rows + n_ubounds);
    Qs = mat(Q.n_cols + n_ubounds, Q.n_cols + n_ubounds, fill::zeros);
    cs = vec(c.n_rows+n_ubounds);
    
    bs.rows(0, b.n_rows - 1) = b - A*lb;
    As(0, 0, size(A)) = A;
    Qs(0, 0, size(Q)) = Q;
    cs.rows(0,c.n_rows-1) = c + 2*Q*lb;
    
    // append [0 0 0 ... 0 1 0 ...0] to As
    // and ub_i- lb+i to bs;
    int counter = 0;
    for (int i = 0; i < ub.size(); i++)
    {
        if (fabs(ub(i) - infty) > 1)
        {
            As(A.n_rows + counter, i) = 1;
            As(A.n_rows + counter, A.n_cols + counter) = 1;
            bs(b.n_rows + counter) = ub(i) - lb(i);
            counter ++;
        }
        
    }
    
    //Qs.print("Qs = "); As.print("As = "); bs.print("bs = "); cs.print("cs = ");
    
    return 0;
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
    _splitRaw(Araw, braw, c, A, b);
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
            // ======= check termination ======
            if ( _checkSectionName(firstWord) == 10 )
                break;
            
            if ( _checkSectionName(firstWord) == 8)
            {
                std::cout<< "Error: MPSREASER - Currently cannot handle SOS"<<std::endl;
                break;
            }
            
            if ( _checkSectionName(firstWord) == 9)
            {
                std::cout<< "Error: MPSREASER - Currently cannot handle RANGES"<<std::endl;
                break;
            }
            // ======= end check termination ======
            
            // ======= preprocess sections ======
            if ( _checkSectionName(firstWord) == 1 ) // name
            {
                readFile >> Name;
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkSectionName(firstWord) == 2 ) //rows
            {
                // update firstWord and row_list
                readFile >> firstWord;

                while (_checkSectionName(firstWord) == -1)
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
            else if ( _checkSectionName(firstWord) == 3 ) // cols
            {
                // get postion
                col_pos = readFile.tellg();

                readFile >> firstWord;
                
                while (_checkSectionName(firstWord) == -1) // continue if the keyword is not a feild name
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
            else if ( _checkSectionName(firstWord) == 4) //rhs
            {
                rhs_pos = readFile.tellg();
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkSectionName(firstWord) == 5) //bounds
            {
                bnd_exist = true;
                
                bnd_pos = readFile.tellg();
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkSectionName(firstWord) == 6) //quadobj
            {
                qdo_exist = true;
                
                qdo_pos = readFile.tellg();
                _nextLine(readFile);
                readFile >> firstWord;
            }
            else if ( _checkSectionName(firstWord) == 7) // objsense
            {
                objsense = true;
                
                _nextLine(readFile);
                readFile >> objsense;
                readFile >> firstWord;
            }
            else                                        // not a keywod of sections
            {
                _nextLine(readFile);
                readFile >> firstWord;
            }
            // ======= end preprocess sections termination ======
            
        }
    }
    
}


int mpsReader::_checkSectionName(std::string checkWord) const
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
    Q   = mat(n_cols+n_rows_inq   , n_cols+n_rows_inq, fill::zeros);
    A   = mat(n_rows_inq+n_rows_eq, n_cols+n_rows_inq, fill::zeros);
    b   = vec(n_rows_inq+n_rows_eq,                    fill::zeros);
    c   = vec(n_cols+n_rows_inq   ,         fill::zeros);
    lb  = vec(n_cols+n_rows_inq   ,         fill::zeros);
    ub  = vec(n_cols+n_rows_inq);  ub.fill(infty);
    
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
        
        // break if get to next section
        if (_checkSectionName(colName) != -1)
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
        
        if ( _checkSectionName(colName) != -1 )
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
        else
        {
            std::cout<<"Error: MPSREADER only accept LO and UP for Bounds"<<std::endl;
            break;
        }
        
    } while (_checkSectionName(label) == -1);
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
        
        if (_checkSectionName(colName1) != -1)
            break;
        
        colIdx2 = _getIndex(col_list, colName2);
        
        Q(colIdx1,colIdx2) = value;
        
        if (colIdx1 != colIdx2)
            Q(colIdx2,colIdx1) = value;
    }
}

void mpsReader::_splitRaw(mat &Araw, vec &braw, vec &c, mat &A, vec &b)
{
    int counter = 0, counter_inq = 0;
    
    for (int i=0; i< n_rows; i++)
    {
        if (row_labels[i] == "N")
            c.rows(0,n_cols-1) = trans( Araw.row(i) );
        else if (row_labels[i] == "E")
        {
            A.submat(counter, 0, counter, n_cols-1) = Araw.row(i);
            b.row(counter) = braw.row(i);
            counter ++;
        }
        else if (row_labels[i] == "L")
        {
            A.submat(counter, 0, counter, n_cols-1) = Araw.row(i);
            A(counter, n_cols + counter_inq) = 1;
            b.row(counter) = braw.row(i);
            counter ++;
            counter_inq ++;
        }
        else if (row_labels[i] == "G")
        {
            A.submat(counter, 0, counter, n_cols-1) = Araw.row(i);
            A(counter, n_cols + counter_inq) = -1;
            b.row(counter) = braw.row(i);
            counter ++;
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
    lb.print("lb: ");
    ub.print("ub: ");
    
    std::cout<<"---------------------"<<std::endl;
    std::cout<<"MPS file read. Time = [ "<<time<<"s ]"<<std::endl;
}