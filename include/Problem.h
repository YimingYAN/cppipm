//
//  Problem.h
//  cppipm
//
//  Created by Yiming Yan on 27/06/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#ifndef cppipm_Problem_h
#define cppipm_Problem_h

#include "include_libs.h"
#include <cassert>

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
    
    long m;     // number of constraints
    long n;     // number of variables
    
    // Constructors
    Problem() {}
    
    /* QP */
    Problem(const mat &iQ, const mat &iA, const vec &ib, const vec &ic)
    {
        assert(iA.rows() > 0 && iA.cols() > 0);
        m = iA.rows();
        n = iA.cols();
        
        Q = iQ;
        
        A = iA;
        b = ib;
        c = ic;
    }
    
    /* LP */
    Problem(const mat &iA, const vec &ib, const vec &ic)
    {
        assert(iA.rows() > 0 && iA.cols() > 0);
        
        m = iA.rows();
        n = iA.cols();
        
        A = iA;
        b = ib;
        c = ic;
        
        Q = mat::Zero(n,n);
    }
    
    /* Copy from other problem */
    Problem(const Problem& otherProb)
    {
        assert(otherProb.n > 0 && otherProb.m > 0);
        
        m = otherProb.m;
        n = otherProb.n;
        
        Q = otherProb.Q;
        
        A = otherProb.A;
        b = otherProb.b;
        c = otherProb.c;
        
    }
    // Methods
    void setOptx(const vec x) { optx = x; }
    void setOpty(const vec y) { opty = y; }
    void setOpts(const vec s) { opts = s; }
    
    
    vec getOptx() const { return optx; }
    vec getOpty() const { return opty; };
    vec getOpts() const { return opts; };
};

#endif
