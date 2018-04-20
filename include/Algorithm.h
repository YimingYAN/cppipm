/*
 *  Algorithm.h
 *  Abstract Algorithm Framework
 *
 *  Created by Yiming Yan on 06/08/2014.
 *  Copyright (c) 2014 Yiming. All rights reserved.
 */


#ifndef Algorithm_h
#define Algorithm_h

class Algorithm
{
    
public:
	/*
	 * Template method
	 */    
    void solve()
    {
        initialize();
        
        printHeader();
        startTimer();
        
        initialPoint();
        while (true) {
            calResidual();
            
            printIter();

            if ( checkTermination() )
                break;
            
            calSearchDriection();
            getStepSize();
            updateIterates();

            iter++;
        }
        endTimer();
        printFooter();
    }
    
protected:
    int iter;
    double residual;
    double totalTime;
    
    virtual void initialize()               = 0;
    virtual void initialPoint()             = 0;
    virtual void calResidual()              = 0;
    virtual bool checkTermination()         = 0;
    virtual void calSearchDriection()       = 0;
    virtual void getStepSize()              = 0;
    virtual void updateIterates()           = 0;

    virtual void startTimer()               = 0;
    virtual void endTimer()                 = 0;
    virtual void printHeader()              = 0;
    virtual void printIter()                = 0;
    virtual void printFooter()              = 0;
    
};


#endif
