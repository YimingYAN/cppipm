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
#include <vector>
#include<armadillo>
using namespace arma;

class mpsReader
{

public:
    mpsReader(std::string fileName);
    
    std::string Name;
    std::string objsense = "MIN";
    
    double infty = 1e32;
    
    int n_rows;
    int n_rows_eq;
    int n_rows_inq;
    int n_cols;
    
    std::vector<std::string> row_labels;    // E, G, L
    std::vector<std::string> row_list;      // List of row names
    std::vector<std::string> col_list;      // List of column names
    
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
    // pisitions
    long col_pos;
    long rhs_pos;
    long bnd_pos;
    long qdo_pos;
    
    bool qdo_exist = false;
    bool bnd_exist = false;
    bool ranges_exist = false;
    bool objsense_exist = false;
    
    // Internal functions
    void _extractData(std::ifstream &readFile);
    /*
     * extract data from the mps file
     */
    void _getAraw(std::ifstream &readFile, mat &Araw);
    void _getbraw(std::ifstream &readFile, vec &braw);
    void _getBnds(std::ifstream &readFile);
    void _getQdo(std::ifstream &readFile);
    
    void _findPos2Start(std::ifstream &readFile);
    
    void _preprocScan(std::ifstream &readFile);
    /*
     This function scans the first string of each row in the
     mps file to determine the problem dimensions, problem name 
     and objective sense.
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
    
    void _initializeData();
    /*
     * Initialize the problem data Q, A ,b Aeq, beq, c, lb abd ub.
     */
    
    void _nextLine(std::ifstream &readFile);
    int _getIndex(std::vector<std::string> &list, std::string item) const;
    
};


#endif
