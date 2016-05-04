//
//  Public.h
//  exp_3_linux
//
//  Created by bit_zt on 16/4/11.
//  Copyright (c) 2016年 bit_zt. All rights reserved.
//
//
//#ifndef exp_3_linux_Public_h
//#define exp_3_linux_Public_h

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <cstdlib>

using namespace std;

typedef struct sembuf SemConfiguration;

const int BUFFER_SIZE = 4;
const int PRODUCER_NO = 3;
const int CONSUMER_NO = 4;
const int MAX_PRODUCT_NO = 4;
const int MAX_CONSUME_NO = 3;

const int BUFFERMUTEX_ID = 254;
const int PRODUCT_NUM_SEM_ID = 255;
const int EMPTY_AREA_SEM_ID = 256;
const int SHARED_MEMORY_AREA_ID = 110;

const int SHARED_MEMORY_AREA_SIZE = 1024;

const int AUTHORITY_SEM = 0666;
const int AUTHORITY_SMA = 0666;


const string DIVIDER_LINE = "-------------------------------------------------------";

void Set_sem(int,int);
void MyCreateProcess(const char*, const int);
void WaitForChildProcess();
void CreateSMA(int,int,int);					/* get product from buffer*/
void ReleaseSMA();

SemConfiguration Get_Pconf();
SemConfiguration Get_Vconf();

void P_src(int);
void V_src(int);
void P_Mutex();
void V_Mutex();
void P_ProductNum();
void V_ProductNum();
void P_EmptyArea();
void V_EmptyArea();
/*
	define the expression of one single product
 */
class ProductRecord
{
private:
    int Producer_ID;
    int Product_ID;								/* producer's ith product*/
public:
    ProductRecord()	{}
    ProductRecord(int per_ID, int p_ID) : Producer_ID(per_ID), Product_ID(p_ID)	{}
    void Print()
    {
        cout << "From [P" << Producer_ID << "]  good No:[" << Product_ID << "] ";
    }
};

/*
	define the buffer area
 */
class Buffer
{
protected:
    ProductRecord products[BUFFER_SIZE];		/* record the product info, use array to simulate a queue for FIFO requirement*/
    int curProductNum;							/* current product number*/
    int productHeadPointer;						/* the head position of the queue*/
    int productRearPointer;						/* the rear position of the queue*/
public:
    Buffer() :
    curProductNum(0),
    productHeadPointer(0),
    productRearPointer(0)
    {}
    
    void PrintBuffer();							/* print all products info in the queue*/
    
    int GetcurProductNum()
    {
        return curProductNum;
    }
    
    void PutProduct(ProductRecord);
    ProductRecord GetProduct();
    
    virtual ~Buffer()
    {}
};

void PutProduct(int, int);							/* put product into buffer*/
ProductRecord GetProduct(int);

union semun {
             int val;                  /* value for SETVAL */
             struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
             unsigned short *array;    /* array for GETALL, SETALL */
                                       /* Linux specific part: */
             struct seminfo *__buf;    /* buffer for IPC_INFO */
       };

//#endif
