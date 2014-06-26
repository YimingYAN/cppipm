#include <iostream>

int main (int argc, char* argv[])
{
	using namespace std;
    int m = 1, n = 2;
	double A[m][n];
    double b[m];
    double c[n];
    
    A[0][0] = 1;
    A[0][1] = 2;
    
    c[0] = 1;
    c[1] = 0;
    
    b[0] = 2;
    
    cout<< "Test file for classes" << endl;
    for (int i=0; i<m; i++)
    {
        for (int j=0; j<n; j++)
        {
            cout << "A[" << i << "]" << "[" << j << "] = " << A[i][j] << endl;
        }
    }
    
	return 0;
}
