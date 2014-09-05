#include <iostream>
#include "cppipm.h"

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

	return 0;
}
