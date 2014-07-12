//
//  mpsReader.h
//  cppipm
//
//  Created by Yiming Yan on 11/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//
/* Problem format
 * min 1/2 x'Qx + c'x
 * s.t. 
 *      Ax <= b
 *      Aeq x = beq
 *      lb <= x <= ub
 */

#ifndef cppipm_mpsReader_h
#define cppipm_mpsReader_h

#include<iostream>
#include<armadillo>
using namespace arma;

class mpsReader
{

public:
    mpsReader(std::string fileName);
    std::string Name;
    std:: string objsense;
    
    std::string rows;
    int n_rows;
    int n_rows_eq;
    int n_rows_inq;
    
    std::string cols;
    int n_cols;
    
    std::string row_labels;
    std::string col_labels;
    mat Q;
    mat A;
    mat Aeq;
    vec b;
    vec beq;
    vec lb;
    vec ub;
    vec c;
    
private:
    // internal properties
    
    
    // Internal functions
    void _extractData(std::ifstream &readFile);
    /*
     * extract data from the mps file
     */
    
    void _nextLine(std::ifstream &readFile);
    
    void _findPos2Start(std::ifstream &readFile);
    
    void _preprocScan(std::ifstream &readFile);
    /*
     This function scans the first string of each row in the
     mps file to determine the problem dimensions, problem name 
     and objective sense.
     */
    
    void _initializeData();
    /*
     * Initialize the problem data Q, A ,b Aeq, beq, c, lb abd ub.
     */
    
    int _checkFieldName(std::string checkWord) const;
    /* This function checks the field names. 
     * Return an interge ranging from 1 to 8
     * 1 - NAME
     * 2 - ROWS
     * 3 - COLUMNS
     * 4 - RHS
     * 5 - BOUNDS
     * 6 - QUADOBJ
     * 7 - OBJSENSE
     * 8 - SOS
     * 9 - RANGES
     *10 - ENDATA
     *-1 - unknown field
     */

    
    
};


#endif
