#include "Public.h"

void Buffer :: PrintBuffer() {
	cout << DIVIDER_LINE << endl;
	if (curProductNum == 0)
	{
		cout << "Current buffer area is empty." << endl;
	}
	else
	{
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

void PutBufferIntoMappingFile(HANDLE FileMapping)
{
	HANDLE MappingView = MapViewOfFile(
		FileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);

	Buffer* buffer_pointer = (Buffer*)MappingView;

	Buffer buffer;
	buffer_pointer[0] = buffer;

	UnmapViewOfFile(MappingView);
}

void MyCreateProcess(STARTUPINFO (&producers_si)[PRODUCER_NO],
					 STARTUPINFO (&consumers_si)[CONSUMER_NO],
					 PROCESS_INFORMATION (&producers_pi)[PRODUCER_NO],
					 PROCESS_INFORMATION (&consumers_pi)[CONSUMER_NO])
{
	for (int i = 0; i < PRODUCER_NO; i++)
	{
		ZeroMemory(reinterpret_cast<void*>(&producers_si[i]), sizeof(producers_si[i]));
		producers_si[i].cb = sizeof(producers_si[i]);

		BOOL CreateOK = CreateProcess(
			PRODUCER_EXE_PATH,
			NULL,
			NULL,
			NULL,
			FALSE,
			NULL,
			NULL,
			NULL,
			&producers_si[i],
			&producers_pi[i]);

		if (!CreateOK)
		{
			cout << "create producer " << i << " fail." << endl;
		}
	}

	for (int i = 0; i < CONSUMER_NO; i++)
	{
		ZeroMemory(reinterpret_cast<void*>(&consumers_si[i]), sizeof(consumers_si[i]));
		consumers_si[i].cb = sizeof(consumers_si[i]);

		BOOL CreateOK = CreateProcess(
			CONSUMER_EXE_PATH,
			NULL,
			NULL,
			NULL,
			FALSE,
			NULL,
			NULL,
			NULL,
			&consumers_si[i],
			&consumers_pi[i]);

		if (!CreateOK)
		{
			cout << "create consumer " << i << " fail." << endl;
		}
	}
}


void MyCloseHandle(
	PROCESS_INFORMATION(&producers_pi)[PRODUCER_NO],
	PROCESS_INFORMATION(&consumers_pi)[CONSUMER_NO])
{
	for (int i = 0; i < PRODUCER_NO; i++)
	{
		WaitForSingleObject(producers_pi[i].hProcess, INFINITE);
		CloseHandle(producers_pi[i].hProcess);
		CloseHandle(producers_pi[i].hThread);
	}

	for (int i = 0; i < CONSUMER_NO; i++)
	{
		WaitForSingleObject(consumers_pi[i].hProcess, INFINITE);
		CloseHandle(consumers_pi[i].hProcess);
		CloseHandle(consumers_pi[i].hThread);
	}
}

void PutProduct(int producer_ID, int product_ID)
{
	HANDLE FileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		FILEMAPPING_NAME);

	HANDLE MappingView = MapViewOfFile(
		FileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);

	Buffer* buffer_pointer = (Buffer*)MappingView;
	Buffer buffer = buffer_pointer[0];

	ProductRecord product(producer_ID, product_ID);
	buffer.PutProduct(product);

	product.Print();
	cout << "has been placed." << endl;
	buffer.PrintBuffer();

	buffer_pointer[0] = buffer;

	UnmapViewOfFile(MappingView);
	CloseHandle(FileMapping);
}

ProductRecord GetProduct()
{
	HANDLE FileMapping = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		FILEMAPPING_NAME);

	HANDLE MappingView = MapViewOfFile(
		FileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);

	Buffer* buffer_pointer = (Buffer*)MappingView;
	Buffer buffer = buffer_pointer[0];

	ProductRecord product = buffer.GetProduct();

	product.Print();
	cout << "has been taken." << endl;
	buffer.PrintBuffer();

	buffer_pointer[0] = buffer;

	UnmapViewOfFile(MappingView);
	CloseHandle(FileMapping);

	return product;
}

HANDLE GetBufferMutex()
{
	HANDLE BufferMutex = OpenMutex(
		MUTEX_ALL_ACCESS,
		FALSE,
		BUFFERMUTEX_NAME);
	return BufferMutex;
}

HANDLE GetProductNumSem()
{
	HANDLE ProductNumSem = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		PRODUCT_NUM_SEM);
	return ProductNumSem;
}

HANDLE GetEmptyAreaSem()
{
	HANDLE EmptyAreaSem = OpenSemaphore(
		SEMAPHORE_ALL_ACCESS,
		FALSE,
		EMPTY_AREA_SEM);
	return EmptyAreaSem;
}