#ifndef ABSTRACTCONVEXOPTIMISATIONSOLVER
#define ABSTRACTCONVEXOPTIMISATIONSOLVER

#include <core/Algorithm.h>
/*!
 * \brief Generic tempalte class for convex optimization solvers
 */
class AbsractConvexOptimizationSolver : public Algorithm
{
public:
    void solve()
    {
        initialize();
        printHeader();
        startTimer();

        initialPoint();
        while (true) {
            calResidual();
            printIter();

            if (checkTermination()) {
                break;
            }

            calSearchDriection();
            calStepSize();
            updateIterates();

            iter++;
        }
        endTimer();
        printFooter();
    }

protected:
    int    iter;
    double residual;
    double totalTime;

private:
    virtual void initialize()               = 0;
    virtual void initialPoint()             = 0;
    virtual void calResidual()              = 0;
    virtual bool checkTermination()         = 0;
    virtual void calSearchDriection()       = 0;
    virtual void calStepSize()              = 0;
    virtual void updateIterates()           = 0;

    virtual void startTimer()               = 0;
    virtual void endTimer()                 = 0;
    virtual void printHeader()              = 0;
    virtual void printIter()                = 0;
    virtual void printFooter()              = 0;

};

#endif // ABSTRACTCONVEXOPTIMISATIONSOLVER

