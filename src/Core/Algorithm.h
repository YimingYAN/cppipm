/*
 *  Algorithm.h
 *  Abstract Algorithm Framework
 *
 *  Created by Yiming Yan on 06/08/2014.
 *  Copyright (c) 2014 Yiming. All rights reserved.
 */

#ifndef Algorithm_h
#define Algorithm_h

#include "global.h"
/*!
 * \brief Generic tempalte class for convex optimization solvers
 */
class DLL_EXPORT Algorithm
{
public:
    virtual void solve() = 0;
};


#endif
