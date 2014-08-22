#include <iostream>
#include "cppipm.h"
#include "mpsReader.h"
#include "include_libs.h"

int main ( )
{

    // test direct input
    cout << "\nTest random problems: "<<endl;
    int m, n;

    m = 2; n = 5;

    mat Q = mat::Random(n,n);
    mat A = mat::Random(m,n);
    vec b = vec::Random(m);
    vec c = vec::Random(n);

    Q = Q.transpose()*Q;

    // qp test
    cout<<"\nQP problem:"<<endl;
    Algorithm* qp_test = new cppipm(Q, A, b, c);
    qp_test->solve();
    
    cout<<"\nLP problem:"<<endl;
    Algorithm* lp_test = new cppipm(A, b, c);
    lp_test->solve();
    
    
    // test read from QPS
    cout << "\nTest qps input : "<<endl;

    mat Qs, As;
    vec bs, cs;
    
    try
    {
        cout<< "\n++++++ TESTPROB " <<endl;
        mpsReader mpsTESTPROB("TESTPROB.QPS");
        mpsTESTPROB.trans2standardForm(Qs, As, bs, cs);
        Algorithm* mps_testprob_test = new cppipm(Qs, As, bs, cs);
        mps_testprob_test->solve();
    }
    catch (...)
    {
        cout<< "ah....TESTPROB"<<endl;
    }
        
    try {
        cout<< "\n++++++ TESTPROB2" <<endl;
        mpsReader mpsTESTPROB2("TESTPROB2.QPS");
        mpsTESTPROB2.trans2standardForm(Qs, As, bs, cs);
        Algorithm* mps_testprob2_test = new cppipm(Qs, As, bs, cs);
        mps_testprob2_test->solve();
    }
    catch (...)
    {
        cout<< "ah....TESTPROB2"<<endl;
    }
    
    try
    {
        cout<< "\n+++++ QAFIRO " <<endl;
        mpsReader mpsQAFIRO("QAFIRO.QPS");
        mpsQAFIRO.trans2standardForm(Qs, As, bs, cs);
        Algorithm* mps_qafiro_test = new cppipm(Qs, As, bs, cs);
        mps_qafiro_test->solve();
    }
    catch (...)
    {
        cout<< "ah....QAFIRO"<<endl;
    }
    
    try
    {
        cout<< "\n+++++ SIMPLE" <<endl;
        mpsReader mpsSIMPLE("SIMPLE.QPS");
        mpsSIMPLE.trans2standardForm(Qs, As, bs, cs);
        Algorithm* mps_simple_test = new cppipm(Qs, As, bs, cs);
        mps_simple_test->solve();
    }
    catch (...)
    {
        cout<< "ah....SIMPLE"<<endl;
    }

    
	return 0;
}
