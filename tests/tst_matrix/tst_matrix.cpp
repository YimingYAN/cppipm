#include <QString>
#include <QtTest>

#include <iostream>
#include <algorithm/cppipm.h>
#include <core/Vector.h>
#include <core/Operations.h>

using namespace std;

class Tst_Matrix : public QObject
{
    Q_OBJECT

public:
    Tst_Matrix(){ }

private Q_SLOTS:
    void vectorConstructors();
    void vectorOperators();
};

void Tst_Matrix::vectorConstructors()
{
    Vector vec1;
    cout << vec1 << endl;

    Vector vec2(2);
    cout << vec2 << endl;

    Vector vec3(3, 1);
    cout << vec3 << endl;

    std::vector<double> vec;
    vec.push_back(0);
    vec.push_back(2.21);
    vec.push_back(1.3);
    vec.push_back(6);

    Vector vec4(vec);
    cout << vec4 << endl;
}

void Tst_Matrix::vectorOperators()
{
    vector<double> vec1, vec2;
    vec1.push_back(0.0); vec1.push_back(2.0);
    vec1.push_back(3.0); vec1.push_back(7.5);

    vec2.push_back(1.0); vec2.push_back(3.78);
    vec2.push_back(6.0); vec2.push_back(7.5);
    Vector v1(vec1), v2(vec2);
    cout << v1 << endl;
    cout << v2 << endl;

    cout << "v1 + v2: " << v1 + v2 << endl;
    cout << "v1 - v2: " << v1 - v2 << endl;
    cout << "v1 * v2: " << v1 * v2 << endl;
    cout << "v1 / v2: " << v1 / v2 << endl;

    v1 = v2;
    cout << "v1 = v2: " << v1 << endl;
}

QTEST_APPLESS_MAIN(Tst_Matrix)

#include "tst_matrix.moc"
