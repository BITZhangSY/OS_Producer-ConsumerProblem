//
//  MainFunc.cpp
//  
//
//  Created by bit_zt on 16/4/11.
//
//

#include "Public.h"

int main()
{
    Set_sem(BUFFERMUTEX_ID, 1);                 /* create and initialize sem */
    Set_sem(PRODUCT_NUM_SEM_ID, 0);
    Set_sem(EMPTY_AREA_SEM_ID, BUFFER_SIZE);
    
    CreateSMA(SHARED_MEMORY_AREA_ID, SHARED_MEMORY_AREA_SIZE, AUTHORITY_SMA);
    
    
    cout << DIVIDER_LINE << endl << endl;
    cout << "   Producer & Consumer process as shown below..." << endl << endl;
    cout << DIVIDER_LINE << endl << endl;
    
    
    /* debug path on mac os:
        /Users/bit_zt/Documents/exp_3/Build/Products/Debug/Consumer
        /Users/bit_zt/Documents/exp_3/Build/Products/Debug/Producer
     */
    const char* PRODUCER_EXE_PATH = "/home/bitzt/Documents/exp_3/Producer";
    const char* CONSUMER_EXE_PATH = "/home/bitzt/Documents/exp_3/Consumer";
    
    MyCreateProcess(PRODUCER_EXE_PATH, PRODUCER_NO);         /* create producer process */
    MyCreateProcess(CONSUMER_EXE_PATH, CONSUMER_NO);         /* create consumer process */
    
    WaitForChildProcess();
    
    ReleaseSMA();
    
    cout << DIVIDER_LINE << endl << endl;
    cout << "	The End." << endl << endl;
    cout << DIVIDER_LINE << endl << endl;
    
    system("pause");
    return 0;
}
