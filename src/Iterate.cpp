//
//  Iterate.cpp
//  cppipm
//
//  Created by Yiming Yan on 07/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include <cassert>
#include <iostream>
#include "Iterate.h"

Iterate::Iterate()
{
}

Iterate::Iterate(const Problem &prob)
{
    bc = std::max(norm(prob.b, 2), norm(prob.c, 2)) + 1;
    iter = 0;
    mu = 1000; // give a large number for initial mu;
}

void Iterate::initialPoint(const Problem &prob)
{
    // Simple starting point
    x.ones(prob.n);
    s.ones(prob.n);
    y.zeros(prob.m);
}

void Iterate::getResiduals(const Problem &prob)
{
    mu = dot(x,s) / prob.n;
    sigma = std::min(0.1,100*mu);
    
    Rp = prob.b - prob.A*x;
    prob.Q.print("getResidual Q = ");
    Rd = prob.c - prob.A.t()*y - s + prob.Q*x;
    Rm = sigma*mu*ones(prob.n) - x%s;
    
    residual = ( norm(Rp,2) + norm(Rd,2) + prob.n*mu ) / bc;
}


bool Iterate::checkTermination(const Parameters &pars)
{
    bool check_maxIter  = iter > pars.maxIter;
    bool check_residual = residual < pars.tol;
    
    return check_maxIter || check_residual;
}

void Iterate::solveNewton(const Problem &prob)
{
    /*
     The Newton's diections are obtained by solving the following system,
        [  A   0  0 ] [ dx ] =   [ b - Ax           ]   [ Rp ]
        [ -Q   A' I ] [ dy ] =   [ c - A'y - s + Qx ] = [ Rd ]
        [  S   0  X ] [ ds ] =   [ sigma*mu*e - XSe ]   [ Rm ]
     
      Augmented system is actually solved.

     */
    
    // form the augmented system
    mat M_R1(prob.n, prob.m+prob.n);
    mat M_R2(prob.m, prob.m+prob.n);
    mat M(prob.m + prob.n, prob.m + prob.n);
    vec rhs(prob.n,prob.m);
    mat R(prob.m + prob.n, prob.m + prob.n);
    mat Q(prob.m + prob.n, prob.m + prob.n);
    vec theta(prob.n);
    vec dxy(prob.m + prob.n);

    
    theta = s/x;
    
    M_R1 = join_horiz( -prob.Q-diagmat(theta), prob.A.t());
    M_R2 = join_horiz( prob.A,                 vec(prob.m, prob.m, fill::zeros) );
    M = join_vert(M_R1, M_R2);
    
    rhs = join_vert(Rd - Rm/x, Rp);
    
    // factorise M
    qr(Q,R,M);
    
    // solve for directions (dx,dy)
    dxy = solve(R, Q.t()*rhs);
    assert(norm(M*dxy - rhs,2) < 1e-12);
    
    dx = dxy.rows(0, prob.n-1);
    dy = dxy.rows(prob.n, prob.m+prob.n-1);
    ds = (Rm - s % dx) / x;
    
    assert(norm( prob.A*dx + (prob.A*x - prob.b),2 ) < 1e-12);
    assert(norm( -prob.Q*dx + prob.A.t()*dy + ds  - prob.c - prob.Q*x + prob.A.t()*y + s,2 ) < 1e-12);
    assert(norm( s%dx + x%ds - sigma*mu*ones(prob.n) + x%s,2 ) < 1e-12);
}

void Iterate::getStepSize(const Parameters &pars)
{
    alphax = 1;
    alphas = 1;
    
    for (int i=0; i<dx.n_rows; i++)
    {
        if (dx(i) < 0)
            if (alphax > -x(i)/dx(i))
                alphax = -x(i)/dx(i);
        
        if (ds(i) < 0)
            if (alphas > -s(i)/ds(i))
                alphas = -s(i)/ds(i);
    }
    
    alphax = pars.eta*alphax;
    alphas = pars.eta*alphas;

    assert( all(x+alphax*dx > 0) );
    assert( all(s+alphas*ds > 0) );
}

void Iterate::updateIter()
{
    x = x + alphax * dx;
    s = s + alphas * ds;
    y = y + alphas * dy;
}

void Iterate::iterIncrement()
{
    iter ++;
}

vec Iterate::getIterx()
{
    return x;
}

vec Iterate::getIters()
{
    return s;
}

vec Iterate::getItery()
{
    return y;
}