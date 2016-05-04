//
//  Consumer.cpp
//  
//
//  Created by bit_zt on 16/4/11.
//
//

#include "Public.h"

int main()
{
    int execution_count = MAX_CONSUME_NO;
    
    while (execution_count > 0) {
        P_ProductNum();
        P_Mutex();
        
        ProductRecord product = GetProduct(getpid());
        execution_count--;
        
        sleep(0.3);
        V_Mutex();
        V_EmptyArea();
    }
    
    return 0;
}
