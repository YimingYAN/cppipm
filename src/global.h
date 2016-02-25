//
//  global.h
//  cppipm
//
//  Created by Yiming Yan on 25/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#ifndef cppipm_global_h
#define cppipm_global_h

#if defined(_MSC_VER) //Microsoft Visual Studio
  #define DLL_EXPORT __declspec(dllexport)
#else
  #define DLL_EXPORT
#endif

#include <iostream>
using namespace std;

#include <Eigen/Dense>
#include <Eigen/bench/BenchTimer.h>

typedef Eigen::MatrixXd mat;
typedef Eigen::VectorXd vec;
typedef Eigen::HouseholderQR<mat> Factorization;

#endif
