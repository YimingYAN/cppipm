#ifndef GENERICCONVEXOPTIMISATIONSOLVER
#define GENERICCONVEXOPTIMISATIONSOLVER

#include <core/Algorithm.h>
/*!
 * \brief Generic tempalte class for convex optimization solvers
 */
class GenericConvexOptimizationSolver : public Algorithm
{
public:
    void solve()
    {
        initialize();
        startTimer();

        initialPoint();
        while (true) {
            calResidual();
            if (checkTermination()) {
                break;
            }

            calSearchDriection();
            calStepSize();
            updateIterates();

            iter++;
        }
        endTimer();
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
};

#endif // GENERICCONVEXOPTIMISATIONSOLVER

