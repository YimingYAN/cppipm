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
    bc = max(prob.b.norm(), prob.c.norm()) + 1;
}

cppipm::cppipm(const Problem &iprob, const Parameters &ipars)
{
    prob = Problem(iprob);
    pars = Parameters(ipars);
    bc = max(prob.b.norm(), prob.c.norm()) + 1;
}

cppipm::cppipm(const mat &A, const vec &b, const vec &c)
{
    prob = Problem(A, b, c);
    bc = max(prob.b.norm(), prob.c.norm()) + 1;
}

cppipm::cppipm(const mat &Q, const mat &A, const vec &b, const vec &c)
{
    prob = Problem(Q, A, b, c);
    bc = max(prob.b.norm(), prob.c.norm()) + 1;
}

void cppipm::initialPoint()
{
    vec e = vec::Ones(prob.n);
    
    double delta_x, delta_s, delta_x_c, delta_s_c, pdct;
    mat coeffM = prob.A*prob.A.transpose();
    
    // min norm(x) s.t. Ax = b
    x = prob.A.transpose() * coeffM.llt().solve(prob.b);
    //x = prob.A.transpose() * coeffM.jacobiSvd(ComputeThinU | ComputeThinV).solve(prob.b);
    
    // min norm(s) s.t. A'*y + s - Qx = c
    y = coeffM.llt().solve(prob.A*prob.c);
    //y = coeffM.jacobiSvd(ComputeThinU | ComputeThinV).solve(prob.A*prob.c);
    s = prob.c - prob.A.transpose()*y + prob.Q*x;
    
    // delta_x and delta_s
    delta_x = -1.5 * x.minCoeff();
    delta_s = -1.5 * s.minCoeff();
    
    if (delta_x < 0) delta_x = 0;
    if (delta_s < 0) delta_s = 0;
    
    // delta_x_c and delta_s_c
    vec temp = (x+delta_x * e);
    temp = temp.cwiseProduct(s+delta_s * e);
    pdct = 0.5 * temp.sum();
    delta_x_c = delta_x+pdct/(s.sum()+prob.n*delta_s);
    delta_s_c = delta_s+pdct/(x.sum()+prob.n*delta_x);
    
    x = x + delta_x_c * e;
    s = s + delta_s_c * e;
}

void cppipm::calResidual()
{
    mu = x.dot(s) / prob.n;
    Rp = prob.b - prob.A*x;
    Rd = prob.c - prob.A.transpose()*y - s + prob.Q*x;
    
    residual = ( Rp.norm() + Rd.norm() + prob.n*mu ) / bc;
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
    vec e = vec::Ones(prob.n);
    
    // form the augmented system
    mat M_R1(prob.n, prob.m+prob.n);
    mat M_R2(prob.m, prob.m+prob.n);
    mat M(prob.m + prob.n, prob.m + prob.n);
    vec theta(prob.n);
    
    // format the coeff. matrix for (dx, dy) in the augmented system
    mat Theta(prob.n, prob.n);
    Theta = s.cwiseQuotient(x).asDiagonal();
    Theta = -prob.Q - Theta;
    
    M_R1 << Theta,  prob.A.transpose();
    M_R2 << prob.A, mat::Zero(prob.m, prob.m);
    M << M_R1,
         M_R2;
    
    // factorise M
    Factorization factor = M.householderQr();
    
    // predictor step
    vec Rm = -x.cwiseProduct(s);
    _getDirections(Rm, factor);
    getStepSize();
    
    // get sigma
    vec temp = x + alphax*dx;
    sigma = temp.dot(s + alphas*ds) / prob.n;
    sigma = pow( sigma/mu, 3.0);
    
    // corrector and centrality step
    Rm = sigma * mu * e - dx.cwiseProduct(ds) - x.cwiseProduct(s);
    _getDirections(Rm, factor);
}

void cppipm::_getDirections(vec& Rm, Factorization& factor)
{
    vec rhs(prob.n + prob.m);
    vec dxy(prob.n + prob.m);
    
    rhs << Rd - Rm.cwiseQuotient(x),
           Rp;
    
    // solve for directions (dx,dy)
    dxy = factor.solve(rhs);
    
    dx = dxy.head(prob.n);
    dy = dxy.tail(prob.m);
    ds = (Rm - s.cwiseProduct(dx));
    ds = ds.cwiseQuotient(x);
}

void cppipm::getStepSize()
{
    // Initialize the stepsize
    alphax = 10.0;
    alphas = 10.0;
    
    for (int i=0; i<dx.rows(); i++)
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
    timer.start();
}

void cppipm::endTimer()
{
    timer.stop();
    totalTime = timer.value();
}
