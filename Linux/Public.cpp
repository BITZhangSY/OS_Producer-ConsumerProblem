//
//  main.cpp
//  exp_3_linux
//
//  Created by bit_zt on 16/4/10.
//  Copyright (c) 2016年 bit_zt. All rights reserved.
//
#include "Public.h"

void Buffer :: PrintBuffer() {
    cout << DIVIDER_LINE << endl;
    if (curProductNum == 0)
    {
        cout << "Current buffer area is empty." << endl;
    }
    else
    {
        cout << curProductNum << endl;
        cout << "Current buffer area used as below: " << endl;
        for (int i = productHeadPointer, j = 0; j < curProductNum; j++)
        {
            products[i].Print();
            if (j == curProductNum - 1)
            {
                cout << endl;
            }
            else
            {
                cout << "	";
            }
            i = (i + 1) % BUFFER_SIZE;
        }
    }
    cout << DIVIDER_LINE << endl;
}

void Buffer::PutProduct(ProductRecord product)
{
    products[productRearPointer] = product;
    productRearPointer = (productRearPointer + 1) % BUFFER_SIZE;
    curProductNum++;
}

ProductRecord Buffer::GetProduct()
{
    ProductRecord product = products[productHeadPointer];
    productHeadPointer = (productHeadPointer + 1) % BUFFER_SIZE;
    curProductNum--;
    return product;
}


ProductRecord GetProduct(int Consumer_ID){
    int shmid = shmget(SHARED_MEMORY_AREA_ID, SHARED_MEMORY_AREA_SIZE, AUTHORITY_SMA);
    Buffer *buffer_pointer = (Buffer *)shmat(shmid, 0, 0);
    
    Buffer buffer = buffer_pointer[0];
    
    ProductRecord product = buffer.GetProduct();
    product.Print();
    cout << "has been taken by [" << Consumer_ID << "]." <<endl;
    buffer.PrintBuffer();
    
    buffer_pointer[0] = buffer;
    shmdt(buffer_pointer);
    
    return product;
}

void PutProduct(int producer_ID, int product_ID){
    int shmid = shmget(SHARED_MEMORY_AREA_ID, SHARED_MEMORY_AREA_SIZE, AUTHORITY_SMA);
    Buffer *buffer_pointer = (Buffer *)shmat(shmid, 0, 0);
    
    Buffer buffer = buffer_pointer[0];
    
    ProductRecord product(producer_ID, product_ID);
    buffer.PutProduct(product);
    
    product.Print();
    cout << "has been placed." << endl;
    buffer.PrintBuffer();
    
    buffer_pointer[0] = buffer;
    shmdt(buffer_pointer);
}

void Set_sem(int ID, int initial_val)
{
    int sem_id;
    union semun sem_set;
    int rc;
    sem_id = semget(ID, 1, IPC_CREAT|AUTHORITY_SEM);      /* set sem */
    if(sem_id == -1){
        perror("main's semget error");
        exit(1);
    
    }
    
    sem_set.val = initial_val;              /* initialize sem value */
    rc = semctl(sem_id, 0, SETVAL, sem_set);
    if(rc == -1){
        perror("main's semctl error");
    }
}

void MyCreateProcess(const char* path, const int num)
{
    for (int i=0; i<num; i++) {
        int pid_producer = fork();
        if(pid_producer < 0){
            perror("fork() error.");
            exit(1);
        }
        if(pid_producer == 0){
            if(execl(path,0) < 0){
                perror("create process error.");
            }
        }
    }
}

void WaitForChildProcess()
{
    int status;
    while (-1 != wait(&status)) {
        ;
    }
}

void CreateSMA(int ID, int size, int authority){
    shmget(ID, size, IPC_CREAT | authority);
}

void ReleaseSMA()
{
    int shmid = shmget(SHARED_MEMORY_AREA_ID, SHARED_MEMORY_AREA_SIZE, AUTHORITY_SMA);
    shmctl(shmid, IPC_RMID, 0);
}

SemConfiguration Get_Pconf(){
    SemConfiguration sem_conf;
    sem_conf.sem_num = 0;
    sem_conf.sem_op = -1;
    sem_conf.sem_flg = 0;
    return sem_conf;
}

SemConfiguration Get_Vconf(){
    SemConfiguration sem_conf;
    sem_conf.sem_num = 0;
    sem_conf.sem_op = 1;
    sem_conf.sem_flg = 0;
    return sem_conf;
}

void P_src(int ID){
    int sem_id = semget(ID, 1, IPC_CREAT | AUTHORITY_SEM);
    SemConfiguration sem_conf = Get_Pconf();
    semop(sem_id, &sem_conf, 1);
}

void V_src(int ID){
    int sem_id = semget(ID, 1, IPC_CREAT | AUTHORITY_SEM);
    SemConfiguration sem_conf = Get_Vconf();
    semop(sem_id, &sem_conf, 1);
}

void P_Mutex(){
    P_src(BUFFERMUTEX_ID);
}

void V_Mutex(){
    V_src(BUFFERMUTEX_ID);
}

void P_ProductNum(){
    P_src(PRODUCT_NUM_SEM_ID);
}

void V_ProductNum(){
    V_src(PRODUCT_NUM_SEM_ID);
}

void P_EmptyArea(){
    P_src(EMPTY_AREA_SEM_ID);
}

void V_EmptyArea(){
    V_src(EMPTY_AREA_SEM_ID);
}