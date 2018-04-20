cppipm
=====
C++ implementation of the Interior Point Method (CPPIPM)

## Requirements
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page), "*a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms*". 
Eigen forms the linear algebra core of this software.

## Compilation 
1. Compile the project using the qmake and the project file (cppipm) provided in the repo. This will require instllation of the qmake toolchain. The solver itself has NO dependency on Qt libraries. 
2. A simple way to compile and run the exmaple: 
  ```
      cd examples
      g++ -std=c++14 -I ../lib/ -I ../include/   ../src/cppipm.cpp ../src/mpsReader.cpp test.cpp -o test
      ./test
  ```

## How to use
QP:

```
  Algorithm* test = new cppipm(Q,A,b,c); test -> solve();
```

LP:

```
  Algorithm* test = new cppipm(A,b,c);  test -> solve(); 
```

Or:

```
  Algorithm* test = new cppipm(otherProblem);  test -> solve();
```

## Example
See test.cpp

## TODO
This is a very old piece of work from my PhD time. As you can see my understanding of C++ at that time was not very upto the professional standard. ;) 
Will spend some time in the summer to refactor it. 
 - [ ] Replace qmake project file with CMake
 - [ ] Refactor the code to comply with the SOLID principles at least
 - [ ] Setup continuous integration
 - [ ] Use c++11/14 features 


----
Yiming Yan
