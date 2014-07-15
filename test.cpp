#include <iostream>
#include "src/cppipm.h"
#include "src/mpsReader.h"

using namespace arma;
int main (int argc, char* argv[])
{
    mat Q(2,2);
    mat A(1,2);
    vec b(1);
    vec c(2);
    
    Q(0,0) = 1.0; Q(0,1) = 0.0;
    Q(1,0) = 0; Q(1,1) = 0.5;
    
    A(0,0) = 1; A(0,1) = 1;
    
    b(0) = 1;
    
    c(0) = 1; c(1) = 0.5;

    // Test the cppipm as a whole
    cppipm qp_test = cppipm(Q, A, b, c);
    qp_test.solve();
    
    cout<<endl;
    
    cppipm lp_test = cppipm(A, b, c);
    lp_test.solve();
    
    try
    {
        // test read from QPS
        cout << "\nTest qps reader : "<<endl;
    
        mat Qs, As;
        vec bs, cs;
    
        cout<< "\n++++++ TESTPROB " <<endl;
        mpsReader mpsTESTPROB("/Users/yimingyan/Dropbox/Github/cppipm/examples/TESTPROB.QPS");
        mpsTESTPROB.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_testprob_test = cppipm(Qs, As, bs, cs);
        mps_testprob_test.solve();
    

        cout<< "\n++++++ TESTPROB2" <<endl;
        mpsReader mpsTESTPROB2("/Users/yimingyan/Dropbox/Github/cppipm/examples/TESTPROB2.QPS");
        mpsTESTPROB2.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_testprob2_test = cppipm(Qs, As, bs, cs);
        mps_testprob2_test.solve();
    
        cout<< "\n+++++ QAFIRO " <<endl;
        mpsReader mpsQAFIRO("/Users/yimingyan/Dropbox/Github/cppipm/examples/QAFIRO.QPS");
        mpsQAFIRO.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_qafiro_test = cppipm(Qs, As, bs, cs);
        mps_qafiro_test.solve();
        
        cout<< "\n+++++ SIMPLE" <<endl;
        mpsReader mpsSIMPLE("/Users/yimingyan/Dropbox/Github/cppipm/examples/SIMPLE.QPS");
        mpsSIMPLE.trans2standardForm(Qs, As, bs, cs);
        cppipm mps_simple_test = cppipm(Qs, As, bs, cs);
        mps_simple_test.solve();
    }
    catch (...)
    {
        cout<< "ah...."<<endl;
    }
    
	return 0;
}
