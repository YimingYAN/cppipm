#include <QString>
#include <QtTest>

#include <iostream>
#include <Algorithm/cppipm.h>

using namespace std;

class Tst_Algrun : public QObject
{
    Q_OBJECT

public:
    Tst_Algrun(){ }

private Q_SLOTS:
    void testRandomQP();
    void testRandomLP();
    void testAfiroQP();
};


void Tst_Algrun::testRandomQP()
{
    cout << "\nTest random QP problem: "<<endl;
    int m, n;

    m = 20; n = 50;

    mat Q = mat::Random(n,n);
    mat A = mat::Random(m,n);
    vec b = vec::Random(m);
    vec c = vec::Random(n);

    Q = Q.transpose()*Q;

    Algorithm* qp_test = new cppipm(Q, A, b, c);
    qp_test->solve();

    QCOMPARE(true, true);
}

void Tst_Algrun::testRandomLP()
{
    cout << "\nTest random LP problem: "<<endl;
    int m, n;

    m = 2; n = 5;

    mat A = mat::Random(m,n);
    vec x = vec::Random(n);
    vec b = A*x;
    vec c = vec::Random(n);

    Algorithm* lp_test = new cppipm(A, b, c);
    lp_test->solve();

    QCOMPARE(true, true);
}

void Tst_Algrun::testAfiroQP()
{


}

QTEST_APPLESS_MAIN(Tst_Algrun)

#include "tst_algrun.moc"
