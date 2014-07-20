//
//  Iterate.cpp
//  cppipm
//
//  Created by Yiming Yan on 07/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#include <cassert>
#include <iostream>
#include "Iterate.h"

Iterate::Iterate(){}

Iterate::Iterate(const Problem &prob)
{
    bc = std::max(norm(prob.b, 2), norm(prob.c, 2)) + 1;
    iter = 0;
    mu = 1000; // give a large number to initialize mu;
}

void Iterate::initialPoint(const Problem &prob)
{
    
    // Simple starting point
    //_initialPoint_simple(prob);
    _initialPoint_mehrotra(prob);
}

void Iterate::calResiduals(const Problem &prob)
{
    mu = dot(x,s) / prob.n;
    sigma = std::min(0.1,100*mu);
    
    Rp = prob.b - prob.A*x;
    Rd = prob.c - prob.A.t()*y - s + prob.Q*x;
    
    residual = ( norm(Rp,2) + norm(Rd,2) + prob.n*mu ) / bc;
}


bool Iterate::checkTermination(const Parameters &pars, Status &stat)
{
    bool check_maxIter  = iter > pars.maxIter;
    bool check_residual = residual < pars.tol;
    
    if ( check_maxIter )
        stat.setExitFlag(1);
    if ( check_residual )
        stat.setExitFlag(0);
    
    return check_maxIter || check_residual;
}

void Iterate::solveNewton_pathfollow(const Problem &prob)
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
    mat L(prob.m + prob.n, prob.m + prob.n);
    mat U(prob.m + prob.n, prob.m + prob.n);
    mat P(prob.m + prob.n, prob.m + prob.n);
    vec theta(prob.n);
    
    Rm = sigma*mu*ones(prob.n) - x%s;
    
    // format the coeff. matrix for (dx, dy) in the augmented system
    theta = s/x;
    
    M_R1 = join_horiz( -prob.Q-diagmat(theta), prob.A.t());
    M_R2 = join_horiz( prob.A,                 mat(prob.m, prob.m, fill::zeros) );
    M = join_vert(M_R1, M_R2);
    
    // factorise M
    lu(L,U,P,M);
    
    _getDirections(prob, Rp, Rd, Rm, L, U, P);
}

void Iterate::solveNewton_pc(const Problem &prob, const Parameters &pars)
{
    /*
     Predictor and corrector are used.
     
     Predictor step:
     
     [  A   0  0 ] [ dx_pred ] =   [ b - Ax           ]   [ Rp     ]
     [ -Q   A' I ] [ dy_pred ] =   [ c - A'y - s + Qx ] = [ Rd     ]
     [  S   0  X ] [ ds_pred ] =   [ - XSe            ]   [ Rm_pred ]
     
     Centring parameter:
     
     mu_pred = (x+alphax*dx_pred)'(s+alphas*ds_pred)/n
     sigma = (mu_pred/mu)^3
     
     
     Corrector step:
     
     [  A   0  0 ] [ dx_corr ] =   [ 0                              ]   [ 0       ]
     [ -Q   A' I ] [ dy_corr ] =   [ 0                              ] = [ 0       ]
     [  S   0  X ] [ ds_corr ] =   [ sigma*mu*e - dX_pred*dS_pred*e ]   [ Rm_corr ]
     
     Newton direction:
     
     dx = dx_pred + dx_corr;
     dy = dy_pred + dy_corr;
     ds = ds_pred + ds_corr;
     
     Note that in the implementation, we combine the Corrector step and Newton direction step,
     in the aim of saving memory. Namely we actually solve
     
     Corrector + centrality step:
     
     [  A   0  0 ] [ dx ] =   [ b - Ax                               ]   [ Rp       ]
     [ -Q   A' I ] [ dy ] =   [ c - A'y - s + Qx                     ] = [ Rd       ]
     [  S   0  X ] [ ds ] =   [ sigma*mu*e - dX_pred*dS_pred*e -  XSe]   [ Rm_corr2 ]
     
     */
    
    // form the augmented system
    mat M_R1(prob.n, prob.m+prob.n);
    mat M_R2(prob.m, prob.m+prob.n);
    mat M(prob.m + prob.n, prob.m + prob.n);
    mat L(prob.m + prob.n, prob.m + prob.n);
    mat U(prob.m + prob.n, prob.m + prob.n);
    mat P(prob.m + prob.n, prob.m + prob.n);
    vec theta(prob.n);
    
    // format the coeff. matrix for (dx, dy) in the augmented system
    theta = s/x;
    
    M_R1 = join_horiz( -prob.Q-diagmat(theta), prob.A.t());
    M_R2 = join_horiz( prob.A,                 mat(prob.m, prob.m, fill::zeros) );
    M = join_vert(M_R1, M_R2);
    
    // factorise M
    lu(L,U,P,M);
    
    // predictor step
    Rm = -x%s;
    _getDirections(prob, Rp, Rd, Rm, L, U, P);
    getStepSize(pars);
    
    // get sigma
    sigma = dot(x + alphax*dx, s + alphas*ds) / prob.n;
    sigma = pow( sigma/mu, 3.0);
    
    // corrector and centrality step
    Rm = sigma*mu*ones(prob.n) - dx%ds - x%s;
    _getDirections(prob, Rp, Rd, Rm, L, U, P);
}


void Iterate::solveNewton_mcc(const Problem &prob, const Parameters &pars)
{
    std::cout<<"Not implemented yet."<<endl;
}


void Iterate::_getDirections(const Problem& prob, const vec& Rp, const vec& Rd, const vec& Rm, const mat& L, const mat& U, const mat& P)
{
    vec rhs(prob.n + prob.m);
    vec dxy(prob.n + prob.m);

    rhs = join_vert(Rd - Rm/x, Rp);
    
    // solve for directions (dx,dy)
    dxy = solve(P.t(), rhs);
    dxy = solve(trimatl(L),dxy);
    dxy = solve(trimatu(U), dxy);
    
    dx = dxy.rows(0, prob.n-1);
    dy = dxy.rows(prob.n, prob.m+prob.n-1);
    ds = (Rm - s % dx) / x;
    
    //assert(norm(M*dxy - rhs,2) < 1e-09);
    //assert(norm( prob.A*dx + (prob.A*x - prob.b),2 ) < 1e-09);
    //assert(norm( -prob.Q*dx + prob.A.t()*dy + ds  - prob.c - prob.Q*x + prob.A.t()*y + s,2 ) < 1e-09);
    //assert(norm( s%dx + x%ds - sigma*mu*ones(prob.n) + x%s,2 ) < 1e-09);
}

void Iterate::getStepSize(const Parameters &pars)
{
    // Initialize the stepsize
    alphax = 10.0;
    alphas = 10.0;
    
    for (int i=0; i<dx.n_rows; i++)
    {
        if (dx(i) < 0)
            if (alphax > -x(i)/dx(i))
                alphax = -x(i)/dx(i);
        
        if (ds(i) < 0)
            if (alphas > -s(i)/ds(i))
                alphas = -s(i)/ds(i);
    }
    
    alphax = std::min(pars.eta*alphax, 1.0);
    alphas = std::min(pars.eta*alphas, 1.0);

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

vec Iterate::getIterx() const
{
    return x;
}

vec Iterate::getIters() const
{
    return s;
}

vec Iterate::getItery() const
{
    return y;
}

int Iterate::getIterNum() const
{
    return iter;
}

double Iterate::getMu() const
{
    return mu;
}

double Iterate::getRes() const
{
    return residual;
}


// Iternal functions
void Iterate::_initialPoint_simple(const Problem &prob)
{
    // Simple starting point
    x.ones(prob.n);
    s.ones(prob.n);
    y.zeros(prob.m);
}

void Iterate::_initialPoint_mehrotra(const Problem &prob)
{
    double delta_x, delta_s, delta_x_c, delta_s_c, pdct;
    
    // min norm(x) s.t. Ax = b
    x = prob.A.t() * solve(prob.A*prob.A.t(), prob.b);
    
    // min norm(s) s.t. A'*y + s - Qx = c
    y = solve(prob.A*prob.A.t(), prob.A*prob.c);
    s = prob.c - prob.A.t()*y + prob.Q*x;
    
    
    // delta_x and delta_s
    delta_x = -1.5 * x.min();
    delta_s = -1.5 * s.min();
    
    if (delta_x < 0) delta_x = 0;
    if (delta_s < 0) delta_s = 0;
    
    // delta_x_c and delta_s_c
    pdct = 0.5 * sum((x+delta_x) % (s+delta_s));
    delta_x_c = delta_x+pdct/(sum(s)+prob.n*delta_s);
    delta_s_c = delta_s+pdct/(sum(x)+prob.n*delta_x);
    
    x = x + delta_x_c;
    s = s + delta_s_c;
}

double Iterate::getAlphx() const
{
    return alphax;
}

double Iterate::getAlphs() const
{
    return alphas;
}