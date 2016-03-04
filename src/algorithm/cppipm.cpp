//
//  cppipm.cpp
//  cppipm
//
//  Created by Yiming Yan on 08/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#include <algorithm/cppipm.h>
#include <iostream>
#include <iomanip>


// Constructors

cppipm::cppipm(const Problem &iprob)
{
    m_prob = Problem(iprob);
}

cppipm::cppipm(const Problem &iprob, const Parameters &ipars)
{
    m_prob = Problem(iprob);
    m_params = Parameters(ipars);
}

cppipm::cppipm(const mat &A, const vec &b, const vec &c)
{
    m_prob = Problem(A, b, c);
}

cppipm::cppipm(const mat &Q, const mat &A, const vec &b, const vec &c)
{
    m_prob = Problem(Q, A, b, c);
}

cppipm::cppipm(const string fileName)
{
    mpsReader mpsQAFIRO(fileName);
    mat Q, A;
    vec b, c;

    mpsQAFIRO.trans2standardForm(Q, A, b, c);
    m_prob = Problem(Q, A, b, c);
}

void cppipm::initialize()
{
    iter = 0;
    totalTime = 0.0;
    residual = 0.0;
    
    bc = max(m_prob.b.norm(), m_prob.c.norm()) + 1;
}

void cppipm::initialPoint()
{
    vec e = vec::Ones(m_prob.n);
    
    double delta_x, delta_s, delta_x_c, delta_s_c, pdct;
    mat coeffM = m_prob.A*m_prob.A.transpose();
    
    // min norm(x) s.t. Ax = b
    x = m_prob.A.transpose() * coeffM.llt().solve(m_prob.b);
    //x = prob.A.transpose() * coeffM.jacobiSvd(ComputeThinU | ComputeThinV).solve(prob.b);
    
    // min norm(s) s.t. A'*y + s - Qx = c
    y = coeffM.llt().solve(m_prob.A*m_prob.c);
    //y = coeffM.jacobiSvd(ComputeThinU | ComputeThinV).solve(prob.A*prob.c);
    s = m_prob.c - m_prob.A.transpose()*y + m_prob.Q*x;
    
    // delta_x and delta_s
    delta_x = -1.5 * x.minCoeff();
    delta_s = -1.5 * s.minCoeff();
    
    if (delta_x < 0) delta_x = 0;
    if (delta_s < 0) delta_s = 0;
    
    // delta_x_c and delta_s_c
    vec temp = (x+delta_x * e);
    temp = temp.cwiseProduct(s+delta_s * e);
    pdct = 0.5 * temp.sum();
    delta_x_c = delta_x+pdct/(s.sum()+m_prob.n*delta_s);
    delta_s_c = delta_s+pdct/(x.sum()+m_prob.n*delta_x);
    
    x = x + delta_x_c * e;
    s = s + delta_s_c * e;
}

void cppipm::calResidual()
{
    mu = x.dot(s) / m_prob.n;
    Rp = m_prob.b - m_prob.A*x;
    Rd = m_prob.c - m_prob.A.transpose()*y - s + m_prob.Q*x;
    
    residual = ( Rp.norm() + Rd.norm() + m_prob.n*mu ) / bc;
}

bool cppipm::checkTermination()
{
    bool check_maxIter  = iter > m_params.maxIter();
    bool check_residual = residual < m_params.tol();
    
    if ( check_maxIter )
        m_stat.setExitFlag(1);
    if ( check_residual )
        m_stat.setExitFlag(0);
    
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
    vec e = vec::Ones(m_prob.n);
    
    // form the augmented system
    mat M(m_prob.m + m_prob.n, m_prob.m + m_prob.n);
    mat Theta(m_prob.n, m_prob.n);
    Theta = s.cwiseQuotient(x).asDiagonal();
    Theta = -m_prob.Q - Theta;
    M << Theta,  m_prob.A.transpose(),
         m_prob.A, mat::Zero(m_prob.m, m_prob.m);
    
    // factorise M
    Factorization factor = M.householderQr();
    
    // predictor step
    vec Rm = -x.cwiseProduct(s);
    getDirections(Rm, factor);
    calStepSize();
    
    // get sigma
    vec temp = x + alphax*dx;
    sigma = temp.dot(s + alphas*ds) / m_prob.n;
    sigma = pow( sigma/mu, 3.0);
    
    // corrector and centrality step
    Rm = sigma * mu * e - dx.cwiseProduct(ds) - x.cwiseProduct(s);
    getDirections(Rm, factor);
}

void cppipm::getDirections(vec& Rm, Factorization& factor)
{
    vec rhs(m_prob.n + m_prob.m);
    vec dxy(m_prob.n + m_prob.m);
    
    rhs << Rd - Rm.cwiseQuotient(x),
           Rp;
    
    // solve for directions
    dxy = factor.solve(rhs);
    dx = dxy.head(m_prob.n);
    dy = dxy.tail(m_prob.m);
    ds = (Rm - s.cwiseProduct(dx));
    ds = ds.cwiseQuotient(x);
}

void cppipm::calStepSize()
{
    alphax = (dx.cwiseQuotient(x)).minCoeff();
    alphas = (ds.cwiseQuotient(s)).minCoeff();
    alphax = -1.0/min(alphax,-1.0);
    alphas = -1.0/min(alphas,-1.0);
    alphax = min(0.9995*alphax, 1.0);
    alphas = min(0.9995*alphas, 1.0);
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
    if (m_params.verbose() > 0)
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
    if (m_params.verbose() > 0)
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
    
    if (m_params.verbose() >= 0)
    {
        ios::fmtflags old_settings = cout.flags();
        cout<<"----------------------------"<<endl;
        cout<< "CPPIPM Terminated. ";
        cout<< "Status : "<<m_stat.getExitFlag() << endl;
        cout<< "[Iters: "<< iter<< "] ";
        cout<<setprecision(2)<<scientific;
        cout<< "[Time: "<<totalTime << "s]";
        cout<< endl;
        cout.flags(old_settings);
    }
    
}

void cppipm::startTimer()
{
    m_timer.start();
}

void cppipm::endTimer()
{
    m_timer.stop();
    totalTime = m_timer.value();
}
