#include <iostream>

using std::cout;
using std::endl;
int main (int argc, char *argv[])
{
    if(argc < 3) {
        cout << "Usage: executable <algorithm_name> <problem_file>" << endl;
        return 0;
    }
    cout << "Running the app."       << endl;
    cout << "Algorithm: " << argv[1] << endl;
    cout << "Problem  : " << argv[2] << endl;
    return 0;
}
