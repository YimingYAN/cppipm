#include <iostream>
#include "cppipm.h"
#include "mpsReader.h"

using namespace arma;
int main (int argc, char* argv[])
{
    // test direct input
    cout << "\nTest direct input: "<<endl;
    
    mat Q(2,2);
    mat A(1,2);
    vec b(1);
    vec c(2);
    
    Q(0,0) = 1.0; Q(0,1) = 0.0;
    Q(1,0) = 0; Q(1,1) = 0.5;
    
    A(0,0) = 1; A(0,1) = 1;
    
    b(0) = 1;
    
    c(0) = 1; c(1) = 0.5;

    // qp test
    cout<<"\nQP problem:"<<endl;
    cppipm qp_test = cppipm(Q, A, b, c);
    qp_test.solve();
    
    cout<<"\nLP problem:"<<endl;
    cppipm lp_test = cppipm(A, b, c);
    lp_test.solve();
    
    
    // test read from QPS
    cout << "\nTest qps input : "<<endl;

    mat Qs, As;
    vec bs, cs;
    
    try
    {
        cout<< "\n++++++ TESTPROB " <<endl;
        mpsReader mpsTESTPROB("/Users/yimingyan/Dropbox/Github/cppipm/examples/TESTPROB.QPS");
        mpsTESTPROB.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_testprob_test = cppipm(Qs, As, bs, cs);
        mps_testprob_test.solve();
    }
    catch (...)
    {
        cout<< "ah....TESTPROB"<<endl;
    }
        
    try {
        cout<< "\n++++++ TESTPROB2" <<endl;
        mpsReader mpsTESTPROB2("/Users/yimingyan/Dropbox/Github/cppipm/examples/TESTPROB2.QPS");
        mpsTESTPROB2.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_testprob2_test = cppipm(Qs, As, bs, cs);
        mps_testprob2_test.solve();
    }
    catch (...)
    {
        cout<< "ah....TESTPROB2"<<endl;
    }
    
    try
    {
        cout<< "\n+++++ QAFIRO " <<endl;
        mpsReader mpsQAFIRO("/Users/yimingyan/Dropbox/Github/cppipm/examples/QAFIRO.QPS");
        mpsQAFIRO.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_qafiro_test = cppipm(Qs, As, bs, cs);
        mps_qafiro_test.solve();
    }
    catch (...)
    {
        cout<< "ah....QAFIRO"<<endl;
    }
    
    try
    {
        cout<< "\n+++++ SIMPLE" <<endl;
        mpsReader mpsSIMPLE("/Users/yimingyan/Dropbox/Github/cppipm/examples/SIMPLE.QPS");
        mpsSIMPLE.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_simple_test = cppipm(Qs, As, bs, cs);
        mps_simple_test.solve();
    }
    catch (...)
    {
        cout<< "ah....SIMPLE"<<endl;
    }
    
	return 0;
}
