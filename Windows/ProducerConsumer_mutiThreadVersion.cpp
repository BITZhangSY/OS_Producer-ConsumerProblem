#include "Public.h"

class ProducerConsumer
{
	protected:
		HANDLE Producer_th[PRODUCER_NO];
		HANDLE Consumer_th[CONSUMER_NO];
		HANDLE Buffer_Mutex;
		ProductRecord products[BUFFER_SIZE];
		int current_product_num;
		int product_head_pointer;
		int product_rear_pointer;
	public:
		ProducerConsumer() : 
			current_product_num(0),
			product_head_pointer(0),
			product_rear_pointer(0)
		{
			Buffer_Mutex = CreateMutex(						//创建互斥体用于访问缓冲区
				NULL,										//缺省的安全性
				TRUE,										//初始时，互斥体处于无信号状态
				NULL);										//匿名的
			for (int i = 0; i < PRODUCER_NO; i++)
			{
				Producer_th[i] = CreateThread(
					NULL,
					0,
					ProduceThreadProc,
					reinterpret_cast<LPVOID>(this),
					0,
					NULL);
			}
			for (int i = 0; i < CONSUMER_NO; i++)
			{
				Consumer_th[i] = CreateThread(
					NULL,
					0,
					ConsumeThreadProc,
					reinterpret_cast<LPVOID>(this),
					0,
					NULL);
			}
			ReleaseMutex(Buffer_Mutex);
		}

		virtual ~ProducerConsumer()
		{
			CloseHandle(Buffer_Mutex);
			for (int i = 0; i < PRODUCER_NO; i++)
			{
				CloseHandle(Producer_th[i]);
			}
			for (int i = 0; i < CONSUMER_NO; i++)
			{
				CloseHandle(Consumer_th[i]);
			}
		}

		virtual void WaitForThreads()
		{
			for (int i = 0; i < PRODUCER_NO; i++)
			{
				if(Producer_th[i] != INVALID_HANDLE_VALUE)
					WaitForSingleObject(Producer_th[i], INFINITE);
			}
			for (int i = 0; i < CONSUMER_NO; i++)
			{
				if (Consumer_th[i] != INVALID_HANDLE_VALUE)
					WaitForSingleObject(Consumer_th[i], INFINITE);
			}
		}
	protected:
		virtual void DoWork(int step, bool isProducer)
		{
			int count = isProducer ? MAX_PRODUCT_NO : MAX_CONSUME_NO;

			while (count > 0)
			{
				WaitForSingleObject(Buffer_Mutex, INFINITE);
				if (isProducer && current_product_num < 4)												//生产者
				{
					ProductRecord product(GetCurrentThreadId(), current_product_num+1);
					products[product_rear_pointer] = product;
					product_rear_pointer = (product_rear_pointer + 1) % BUFFER_SIZE;
					current_product_num++;
					count--;										//已完成任务次数-1
					product.Print();
					cout<< "has been placed." << endl;
					PrintBuffer();
				}
				else if(current_product_num > 0)
				{
					ProductRecord product = products[product_head_pointer];
					product_head_pointer = (product_head_pointer + 1) % BUFFER_SIZE;
					current_product_num--;
					count--;										//已完成任务次数-1
					product.Print();
					cout << "has been taken." << endl;
					PrintBuffer();
				}
				Sleep(300);
				ReleaseMutex(Buffer_Mutex);
			}
		}
		void PrintBuffer()
		{
			cout << "--------------------------------------------" << endl;
			if (current_product_num == 0)
			{
				cout << "Current buffer area is empty." << endl;
			}
			else
			{
				cout << "Current buffer area used as below: " << endl;
				for (int i = product_head_pointer, j = 0; j < current_product_num; j++)
				{
					products[i].Print();
					if (j == current_product_num - 1)
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
			cout << "--------------------------------------------" << endl;
		}
		static DWORD WINAPI ProduceThreadProc(LPVOID LpParam)
		{
			ProducerConsumer* pThis = reinterpret_cast<ProducerConsumer*>(LpParam);
			pThis->DoWork(1, true);
			return 0;
		}
		static DWORD WINAPI ConsumeThreadProc(LPVOID LpParam)
		{
			ProducerConsumer* pThis = reinterpret_cast<ProducerConsumer*>(LpParam);
			pThis->DoWork(-1, false);
			return 0;
		}
};

int main()
{
	ProducerConsumer p_c;
	p_c.WaitForThreads();
	system("pause");
}

