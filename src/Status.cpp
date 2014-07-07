//
//  Status.cpp
//  cppipm
//
//  Created by Yiming Yan on 07/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include "Status.h"
#include <cassert>

Status::Status()
{
    exitFlag = 0;
}

void Status::setExitFlag(int newFlag)
{
    exitFlag = newFlag;
}

double Status::getExitFlag()
{
    return exitFlag;
}