#pragma once

#include<iostream>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<string>

using namespace std;

const int BUFFER_SIZE = 4;						
const int PRODUCER_NO = 3;
const int CONSUMER_NO = 4;
const int MAX_PRODUCT_NO = 4;
const int MAX_CONSUME_NO = 3;
const LPCTSTR FILEMAPPING_NAME = "ProcessSharedBuffer";
const LPCTSTR BUFFERMUTEX_NAME = "BufferMutex";
const LPCTSTR PRODUCT_NUM_SEM = "ProductNumSem";
const LPCTSTR EMPTY_AREA_SEM = "EmptyAreaSem";
const LPUTSTR PRODUCER_EXE_PATH = "C:\\Users\\bit_zt\\documents\\visual studio 2015\\Projects\\test\\Debug\\\Producer.exe";
const LPUTSTR CONSUMER_EXE_PATH = "C:\\Users\\bit_zt\\documents\\visual studio 2015\\Projects\\test\\Debug\\\Consumer.exe";
const string DIVIDER_LINE = "-------------------------------------------------------";
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


void PutBufferIntoMappingFile(HANDLE);				/* initialize the buffer in the mapping file area*/

void MyCreateProcess(								/* create all Producers & Consumers process*/
	STARTUPINFO(&producers_si)[PRODUCER_NO],
	STARTUPINFO(&consumers_si)[CONSUMER_NO],
	PROCESS_INFORMATION(&producers_pi)[PRODUCER_NO],
	PROCESS_INFORMATION(&consumers_pi)[CONSUMER_NO]);

void MyCloseHandle(									/* close all process & thread*/
	PROCESS_INFORMATION(&producers_pi)[PRODUCER_NO],
	PROCESS_INFORMATION(&consumers_pi)[CONSUMER_NO]);

void PutProduct(int, int);							/* put product into buffer*/
ProductRecord GetProduct();							/* get product from buffer*/
HANDLE GetBufferMutex();							/* return the buffer mutex*/
HANDLE GetProductNumSem();							/* return the Semaphore of the Product num*/
HANDLE GetEmptyAreaSem();							/* return the Semaphore of the Empty area*/




