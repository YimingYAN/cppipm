//
//  Parameters.h
//  cppipm
//
//  Created by Yiming Yan on 06/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#ifndef cppipm_Parameters_h
#define cppipm_Parameters_h
#include "global.h"

class DLL_EXPORT Parameters
{
public:
    Parameters()
        : m_maxIter(50),
          m_verbose(2),
          m_tol(1e-08),
          m_eta(0.99995) { }
    
    Parameters(int otherMaxIter, double otherTol, int otherVerbose)
        : m_maxIter(otherMaxIter),
          m_verbose(otherVerbose),
          m_tol(otherTol),
          m_eta(0.99995) { }
    
    void setMaxIter(int otherMaxIter)  { m_maxIter = otherMaxIter; }
    void setTolerance(double otherTol) { m_tol = otherTol;         }
    void setVerbose(int otherVerbose)  { m_verbose = otherVerbose; }
    int maxIter() const {return m_maxIter;}
    int verbose() const {return m_verbose;}
    double tol() const {return m_tol;}
    double eta() const {return m_eta;}

private:
    int m_maxIter;
    int m_verbose;
    double m_tol;
    double m_eta;     // ratio for stepsizes
};

#endif



