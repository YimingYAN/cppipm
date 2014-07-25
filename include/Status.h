//
//  Status.h
//  cppipm
//
//  Created by Yiming Yan on 07/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#ifndef cppipm_Status_h
#define cppipm_Status_h

class Status
{
private:
    int exitFlag;   // 0 - optimal
                    // 1 - terminated by maxIter
                    // 2 - infeasible
 
public:
    Status();
    
    void setExitFlag(int newFlag);
    int getExitFlag() const;
};


#endif
