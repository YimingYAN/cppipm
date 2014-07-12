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
    
    cppipm lp_test = cppipm(A, b, c);
    lp_test.solve();
    
    
    // test read from QPS
    cout<<endl;
    cout << "test qps reader : "<<endl;
    mpsReader fileInput("/Users/yimingyan/Dropbox/Github/cppipm/examples/TESTPROB2.QPS");
    
	return 0;
}
