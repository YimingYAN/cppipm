cppipm
=====
C++ implementation of the Interior Point Method (CPPIPM)

###Requirements
* [Armadillo](http://arma.sourceforge.net/), C++ linear algebra library
* LAPACK
* BLAS


###How to use
For QP:

```Algorithm* test = new cppipm(Q,A,b,c); test -> solve();```

For LP:

```Algorithm* test = new cppipm(A,b,c);  test -> solve(); ```

Or:

```Algorithm* test = new cppipm(otherProblem);  test -> solve();```



###Example
See test.cpp

###Matlab Interface

###Technical details


###Todo
- [ ] Multiple centrality correctors from J. Gondizo
- [ ] Cholesky decomposition for symmetric indefinite matrix (MA27 or MA57)
- [ ] Active-set preidiction class
- [ ] Preprocessor
- [ ] Matlab interface
- [ ] Constraints reduction class??? Not sure...


----
Yiming Yan
