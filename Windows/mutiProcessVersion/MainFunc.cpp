/*
	This is the main function.
	Initializing Producers & Consumers Process.

	一个大小为4的缓冲区，初始为空

	3个生产者
	-随机等待一段时间，向缓冲区添加姓名首字母
	-若缓冲区已满，等待消费者取走数据后再添加
	-重复4次

	4个消费者
	-随机等待一段时间，从缓冲区读取数据
	-若缓冲区为空，等待生产者添加数据后再读取
	-重复3次

	需打印每次操作内容 & 需打印缓冲区 & 按先生产的商品先消费原则 & 生产者消费者用进程模拟
*/
#include "Public.h"

int main()
{
	HANDLE FileMapping = CreateFileMapping(
		HANDLE(0xFFFFFFFF),							/* 0xFFFFFFFF indicates that this area is shared by process*/
		NULL,
		PAGE_READWRITE,								/* shared read and write*/
		0,											/* the high 32 digits of max length*/
		sizeof(Buffer) + 10,						/* the low 32 digits of max length, in this case, is a little larger than the Buffer Size*/
		FILEMAPPING_NAME);							/* the only identification of this area*/
	
	PutBufferIntoMappingFile(FileMapping);

	HANDLE BufferMutex = CreateMutex(				/* restrict the entry into Buffer*/
		NULL,
		TRUE,
		BUFFERMUTEX_NAME);
	
	HANDLE ProductNumSem = CreateSemaphore(			/* restrict the Consumer to get the product*/
		NULL,
		0,											/* at the begining, there is no product*/
		BUFFER_SIZE,
		PRODUCT_NUM_SEM);
	
	HANDLE EmptyAreaSem = CreateSemaphore(			/* restrict the Producer to produce*/
		NULL,
		BUFFER_SIZE,								/* at the begining, the buffer are all empty*/
		BUFFER_SIZE,
		EMPTY_AREA_SEM);

	
	

	cout << DIVIDER_LINE << endl << endl;
	cout << "   Producer & Consumer process as shown below..." << endl << endl;
	cout << DIVIDER_LINE << endl << endl;

	STARTUPINFO producers_si[PRODUCER_NO];
	STARTUPINFO consumers_si[CONSUMER_NO];
	PROCESS_INFORMATION producers_pi[PRODUCER_NO];
	PROCESS_INFORMATION consumers_pi[CONSUMER_NO];

	MyCreateProcess(producers_si, consumers_si, producers_pi, consumers_pi);
	
	ReleaseMutex(BufferMutex);
	
	
	MyCloseHandle(producers_pi, consumers_pi);

	cout << DIVIDER_LINE << endl << endl;
	cout << "	The End." << endl << endl;
	cout << DIVIDER_LINE << endl << endl;

	CloseHandle(FileMapping);
	CloseHandle(BufferMutex);
	CloseHandle(ProductNumSem);
	CloseHandle(EmptyAreaSem);

	system("pause");
	return 0;
}