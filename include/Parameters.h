//
//  Parameters.h
//  cppipm
//
//  Created by Yiming Yan on 06/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#ifndef cppipm_Parameters_h
#define cppipm_Parameters_h

class Parameters
{
public:
    int maxIter;
    int verbose;
    double tol;
    double eta = 0.99995;     // ratio for stepsizes
    
    Parameters()
    {
        maxIter = 50;
        tol = 1e-08;
        verbose = 2;
    }
    
    Parameters(int otherMaxIter, double otherTol, int otherVerbose)
    {
        maxIter = otherMaxIter;
        tol = otherTol;
        verbose = otherVerbose;
    }
    
    void set_maxIter(int otherMaxIter) { maxIter = otherMaxIter; }
    void set_setTol(double otherTol)   { tol = otherTol;         }
    void set_verbose(int otherVerbose) { verbose = otherVerbose; }
};

#endif
