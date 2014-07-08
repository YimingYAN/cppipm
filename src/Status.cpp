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
    assert(newFlag >= 0);
    
    exitFlag = newFlag;
}

int Status::getExitFlag()
{
    return exitFlag;
}