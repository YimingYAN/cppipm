//
//  Problem.cpp
//  cppipm
//
//  Created by Yiming Yan on 27/06/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include <cmath>
#include "Problem.h"
#include <cassert>

Problem::Problem(const mat &iQ, const mat &iA, const vec &ib, const vec &ic)
{
    m = iA.dim1();
    n = iA.dim2();
    
    Q = iQ.copy();
    A = iA.copy();
    b = ib.copy();
    c = ic.copy();
}

Problem::Problem(const mat &iA, const vec &ib, const vec &ic)
{
    m = iA.dim1();
    n = iA.dim2();
    
    A = iA.copy();
    b = ib.copy();
    c = ic.copy();
    
    Q = mat(m,n) = 0.0;
}


Problem::Problem(const Problem& otherProb)
{
    m = otherProb.m;
    n = otherProb.n;
    
    for (int i=0; i<m; i++)
    {
        b[i] = otherProb.b[i];
        c[i] = otherProb.c[i];
        for(int j=0; j<n; j++)
        {
            A[i][j] = otherProb.A[i][j];
        }
        
    }
    
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<n; j++) {
            Q[i][j] = otherProb.Q[i][j];
        }
    }
    
}

Problem::~Problem()
{
    
}

void Problem::setOptx(const vec x)
{
    optx = x.copy();
}

void Problem::setOpts(const vec s)
{
    optx = s.copy();
}

void Problem::setOpty(const vec y)
{
    optx = y.copy();
}
