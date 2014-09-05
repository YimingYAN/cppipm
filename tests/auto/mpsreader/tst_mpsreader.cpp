#include <iostream>
#include "cppipm.h"
#include "include_libs.h"

int main()
{
    // test read from QPS
    cout << "\nTest qps input : "<<endl;

    try
    {
        cout<< "\n++++++ TESTPROB " <<endl;
        Algorithm* mps_testprob_test = new cppipm("TESTPROB.QPS");
        mps_testprob_test->solve();
    }
    catch (...)
    {
        cout<< "ah....TESTPROB"<<endl;
    }

    try {
        cout<< "\n++++++ TESTPROB2" <<endl;
        Algorithm* mps_testprob2_test = new cppipm("TESTPROB2.QPS");
        mps_testprob2_test->solve();
    }
    catch (...)
    {
        cout<< "ah....TESTPROB2"<<endl;
    }

    try
    {
        cout<< "\n+++++ QAFIRO " <<endl;
        Algorithm* mps_qafiro_test = new cppipm("QAFIRO.QPS");
        mps_qafiro_test->solve();
    }
    catch (...)
    {
        cout<< "ah....QAFIRO"<<endl;
    }

    try
    {
        cout<< "\n+++++ SIMPLE" <<endl;
        Algorithm* mps_simple_test = new cppipm("SIMPLE.QPS");
        mps_simple_test->solve();
    }
    catch (...)
    {
        cout<< "ah....SIMPLE"<<endl;
    }
}
