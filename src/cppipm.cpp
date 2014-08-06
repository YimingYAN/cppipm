//
//  cppipm.cpp
//  cppipm
//
//  Created by Yiming Yan on 08/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#include "cppipm.h"
#include <iostream>
#include <iomanip>

// Constructors
cppipm::cppipm()
{
    cout<<"No data input\n";
}

cppipm::cppipm(const Problem &iprob)
{
    prob = Problem(iprob);
    bc = max(norm(prob.b, 2), norm(prob.c, 2)) + 1;
}

cppipm::cppipm(const Problem &iprob, const Parameters &ipars)
{
    prob = Problem(iprob);
    pars = Parameters(ipars);
    bc = max(norm(prob.b, 2), norm(prob.c, 2)) + 1;
}

cppipm::cppipm(const mat &A, const vec &b, const vec &c)
{
    prob = Problem(A, b, c);
    bc = max(norm(prob.b, 2), norm(prob.c, 2)) + 1;
}

cppipm::cppipm(const mat &Q, const mat &A, const vec &b, const vec &c)
{
    prob = Problem(Q, A, b, c);
    bc = max(norm(prob.b, 2), norm(prob.c, 2)) + 1;
}

void cppipm::initialPoint()
{
    double delta_x, delta_s, delta_x_c, delta_s_c, pdct;
    
    // min norm(x) s.t. Ax = b
    x = prob.A.t() * arma::solve(prob.A*prob.A.t(), prob.b);
    
    // min norm(s) s.t. A'*y + s - Qx = c
    y = arma::solve(prob.A*prob.A.t(), prob.A*prob.c);
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

void cppipm::calResidual()
{
    mu = dot(x,s) / prob.n;
    Rp = prob.b - prob.A*x;
    Rd = prob.c - prob.A.t()*y - s + prob.Q*x;
    
    residual = ( norm(Rp,2) + norm(Rd,2) + prob.n*mu ) / bc;
}

bool cppipm::checkTermination()
{
    bool check_maxIter  = iter > pars.maxIter;
    bool check_residual = residual < pars.tol;
    
    if ( check_maxIter )
        stat.setExitFlag(1);
    if ( check_residual )
        stat.setExitFlag(0);
    
    return check_maxIter || check_residual;
}

void cppipm::calSearchDriection()
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
    vec Rm = -x%s;
    _getDirections(Rm, L, U, P);
    getStepSize();
    
    // get sigma
    sigma = dot(x + alphax*dx, s + alphas*ds) / prob.n;
    sigma = pow( sigma/mu, 3.0);
    
    // corrector and centrality step
    Rm = sigma*mu*ones(prob.n) - dx%ds - x%s;
    _getDirections(Rm, L, U, P);
}

void cppipm::_getDirections(vec& Rm, mat& L, mat& U, mat& P)
{
    vec rhs(prob.n + prob.m);
    vec dxy(prob.n + prob.m);
    
    rhs = join_vert(Rd - Rm/x, Rp);
    
    // solve for directions (dx,dy)
    dxy = arma::solve(P.t(), rhs);
    dxy = arma::solve(trimatl(L),dxy);
    dxy = arma::solve(trimatu(U), dxy);
    
    dx = dxy.rows(0, prob.n-1);
    dy = dxy.rows(prob.n, prob.m+prob.n-1);
    ds = (Rm - s % dx) / x;

}

void cppipm::getStepSize()
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
    
    alphax = min(pars.eta*alphax, 1.0);
    alphas = min(pars.eta*alphas, 1.0);
}

void cppipm::updateIterates()
{
    x = x + alphax * dx;
    s = s + alphas * ds;
    y = y + alphas * dy;
}

// Print info
void cppipm::printHeader()
{
    if (pars.verbose > 0)
    {
        ios::fmtflags old_settings = cout.flags();
        cout<<setw(5)<< "Iter";
        cout<<setw(10)<<"Residual";
        cout<<setw(10)<<"Mu";
        cout<<setw(10)<<"Alphax";
        cout<<setw(10)<<"Alphas"<< "\n";
        cout.flags(old_settings);
    }
}

void cppipm::printIter()
{
    if (pars.verbose > 0)
    {
        ios::fmtflags old_settings = cout.flags();
        cout<<setw(5)<< iter;
        cout<<setprecision(2)<<scientific;
        cout<<setw(10)<< residual;
        cout<<setw(10)<< mu;
        if (iter == 0)
        {
            cout<<setw(10)<<"---";
            cout<<setw(10)<<"---";
        }
        else
        {
            cout<<setw(10)<<alphax;
            cout<<setw(10)<<alphas;
        }
        cout<<endl;
        cout.flags(old_settings);
    }
}

void cppipm::printFooter()
{
    using namespace std;
    
    if (pars.verbose >= 0)
    {
        ios::fmtflags old_settings = cout.flags();
        cout<<"----------------------------"<<endl;
        cout<< "CPPIPM Terminated. ";
        cout<< "Status : "<<stat.getExitFlag() << endl;
        cout<< "[Iters: "<< iter<< "] ";
        cout<<setprecision(2)<<scientific;
        cout<< "[Time: "<<totalTime << "s]";
        cout<< endl;
        cout.flags(old_settings);
    }
    
}

void cppipm::startTimer()
{
    timer.tic();
}

void cppipm::endTimer()
{
    totalTime = timer.toc();
}
