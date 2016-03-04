#include <iostream>

int main (int argc, char *argv[])
{
    if(argc < 2) {
        std::cout << "Usage: executable <algorithm_name> <problem_file>" << std::endl;
        return 0;
    }
    std::cout << "Running the app" << std::endl;
    return 0;
}
