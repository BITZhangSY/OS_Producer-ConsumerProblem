//
//  Producer.cpp
//  
//
//  Created by bit_zt on 16/4/11.
//
//

#include "Public.h"

int main(){
    
    int execution_count = MAX_PRODUCT_NO;
    
    while (execution_count > 0) {
        P_EmptyArea();
        P_Mutex();
        
        PutProduct(getpid(), (5 - execution_count));
        execution_count--;
        
        sleep(0.3);
        V_Mutex();
        V_ProductNum();
    }
    
    return 0;
}