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


Iterate::Iterate(const Problem &prob, const Parameters &pars, const Status &stat)
{
    
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
    Rd = prob.c - prob.A.t()*y - s + prob.Q*x;
    Rm = sigma*mu*ones(prob.n) - x%s;
}


bool Iterate::checkTermination(const Parameters &pars)
{
    return residual < pars.tol;
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

void Iterate::getStepSize()
{

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