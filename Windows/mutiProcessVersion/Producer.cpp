/*
	This is the Producer program.
*/
#include "Public.h"

int main()
{
	HANDLE BufferMutex = GetBufferMutex();
	HANDLE EmptyAreaSem = GetEmptyAreaSem();
	HANDLE ProductNumSem = GetProductNumSem();
	
	int execution_count = MAX_PRODUCT_NO;

	while (execution_count > 0)
	{
		WaitForSingleObject(EmptyAreaSem, INFINITE);		/* P(empty) */
		WaitForSingleObject(BufferMutex, INFINITE);			/* P(mutex) */

		PutProduct(GetCurrentProcessId(), (5 - execution_count));
		execution_count--;

		Sleep(300);
		ReleaseMutex(BufferMutex);							/* V(mutex) */
		ReleaseSemaphore(									/* V(product) */
			ProductNumSem,
			1,
			NULL);
	}

	return 0;
}