//
//  Problem.h
//  cppipm
//
//  Created by Yiming Yan on 27/06/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#ifndef cppipm_Problem_h
#define cppipm_Problem_h

#include "linAlg/tnt.h"
using namespace TNT;

using mat = Array2D<double>;
using vec = Array1D<double>;

class Problem
{
private:
    vec optx;   // |
    vec opty;   // | Optimal solution
    vec opts;   // |

public:
    mat Q;     // |
    mat A;     // | Problem
    vec b;     // | Data
    vec c;     // |
    
    int    m;     // number of constraints
    int    n;     // number of variables
    
    // Constructors
    Problem(const mat &Q, const mat &A, const vec &b, const vec &c); // QP
    Problem(const mat &A, const vec &b, const vec &c);     // LP
    Problem(const Problem& otherProb);                              // Copy other problem
    ~Problem();
    
    // Methods
    void setOptx(const vec x);
    void setOpty(const vec y);
    void setOpts(const vec s);
    
    
    vec getOptx() const;
    vec getOpty() const;
    vec getOpts() const;
};

#endif
