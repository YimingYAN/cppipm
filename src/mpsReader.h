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
    int n; // number of variables
    int nrows; // number of rows
    int ncols; // number of columns
    
    
    // Internal functions
    void _findPos2Start(std::ifstream &readFile);
    
    int _checkFieldName(std::ifstream &readFile);
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
