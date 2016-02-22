//
//  global.h
//  cppipm
//
//  Created by Yiming Yan on 25/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#ifndef cppipm_global_h
#define cppipm_global_h

#include <iostream>
using namespace std;

#include <Eigen/Dense>
#include <Eigen/bench/BenchTimer.h>

using namespace Eigen;

typedef MatrixXd mat;
typedef VectorXd vec;
typedef HouseholderQR<mat> Factorization;

#endif
