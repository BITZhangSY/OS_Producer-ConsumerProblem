/*
	This is the Consumer program.
*/
#include "Public.h"

int main()
{
	HANDLE BufferMutex = GetBufferMutex();
	HANDLE EmptyAreaSem = GetEmptyAreaSem();
	HANDLE ProductNumSem = GetProductNumSem();

	int execution_count = MAX_CONSUME_NO;

	while (execution_count > 0)
	{
		WaitForSingleObject(ProductNumSem, INFINITE);		/* P(product) */
		WaitForSingleObject(BufferMutex, INFINITE);			/* P(mutex) */

		ProductRecord product = GetProduct();				/* do some work with the product*/
		execution_count--;

		Sleep(300);
		ReleaseMutex(BufferMutex);							/* V(mutex) */
		ReleaseSemaphore(									/* V(empty) */
			EmptyAreaSem,
			1,
			NULL);
	}

	return 0;
}