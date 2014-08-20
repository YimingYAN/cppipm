/*
 *  mpsReader.h
 *  cppipm
 *
 *  Created by Yiming Yan on 11/07/2014.
 *  Copyright (c) 2014 Yiming Yan. All rights reserved.
 *
 * =================================================================
 *
 * Problem format
 * min 1/2 x'Qx + c'x
 * s.t. 
 *      Ax = b
 *      lb <= x <= ub
 *
 * Note that in order to have the above format, slack variables will 
 * be added if needed.
 *
 * After call trans2standardForm() function, we get
 * min 1/2 x'Qx + c'x
 * s.t.
 *      Ax = b
 *      x >= 0
 *
 * =================================================================
 * Accepted format: mps, qps, free fromatted mps, 
 * free formatted qps
 *  
 *
 * In the ROWS section, each row of the constraint matrix must have a
 * row type and a row name specified. The code for indicating row type
 * is as follows:
 *
 *      type        meaning
 * ---------------------------
 *      E           equality
 *      L           less than or equal
 *      G           greater than or equal
 *      N           objective
 *
 * *** N will only be recognised as objective function.
 * 
 * RANGES and SOS are not accepted currently.
 *
 * For BOUNDS, we accept only 
 *      type            meaning
 *  ---------------------------------------------------
 *      LO              lower bound        lb <= x (< +inf)
 *      UP              upper bound        (0 <=) x <= ub
 *
 * *** Thus lb is always finite.
 *
 * For details about MPS format, see
 *      http://lpsolve.sourceforge.net/5.5/mps-format.htm
 *
 */

#ifndef cppipm_mpsReader_h
#define cppipm_mpsReader_h

#include<iostream>
#include <vector>

#include "include_libs.h"

class mpsReader
{

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    mpsReader(std::string fileName);
    
    double time;
    
    std::string Name;
    std::string objsense; //default "MIN"
    
    double infty;         // large number, representing infinity
    
    int n_rows;
    int n_rows_eq;
    int n_rows_inq;
    int n_cols;

    std::vector<std::string> row_labels;    // E, G, L
    std::vector<std::string> row_list;      // List of row names
    std::vector<std::string> col_list;      // List of column names
    
    mat Q;
    mat A;
    vec b;
    vec lb;
    vec ub;
    vec c;
    
    //public functions
    int trans2standardForm(mat& Qs, mat& As, vec& bs, vec &cs);
    /*
     * trans2standardForm - new data will be stored in (Qs, As, bs, cs)
     * return 0 if successful.
     * If x* is the optimal solution of the standard form, then x* + lb
     * is an optimal solution of the original problem 
     */
    
private:
    // internal properties
    // pisitions
    long col_pos;
    long rhs_pos;
    long bnd_pos;
    long qdo_pos;
    
    //bool flags, default false
    bool qdo_exist;
    bool bnd_exist;
    bool objsense_exist;
    
    // Internal functions
    void _extractData(std::ifstream &readFile);
    /*
     * extract data from the mps file
     */
    void _getAraw(std::ifstream &readFile, mat &Araw);
    void _getbraw(std::ifstream &readFile, vec &braw);
    void _getBnds(std::ifstream &readFile);
    void _getQdo(std::ifstream &readFile);
    void _splitRaw(mat &Araw, vec &braw, vec &c, mat &A, vec &b);
    
    void _findPos2Start(std::ifstream &readFile);
    
    void _preprocScan(std::ifstream &readFile);
    /*
     This function scans the first string of each row in the
     mps file to determine the problem dimensions, problem name 
     and objective sense.
     */
    
    
    int _checkSectionName(std::string checkWord) const;
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
    void _printData();
};


#endif
