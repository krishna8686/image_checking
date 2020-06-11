#include <iostream>
#include <fstream>

#include "QueueAndLock.h"
#include "CDataGen.h"
#include "CUtils.h"


std::condition_variable CondVar_01;
int i_RowLength;
std::mutex Lock_1;


CDataGen* CDataGen::m_ptrDataGen= nullptr;
CDataGen::CDataGen()
{

	WaitCheck_1 = true;
}

CDataGen::~CDataGen()
{

}

/**
@brief              this function is used for singleton purpose of tha class 
@author				Krishna
@param [in]			void
@return             class pointer
@pre				nothing
@post				nothing
*/
CDataGen* CDataGen::GetInstance()
{
	if (nullptr == m_ptrDataGen)
	{
		m_ptrDataGen = new CDataGen();
	}
	return m_ptrDataGen;
}

/**
@brief				it extract tha data from array.txt file and put it in shared queue.
@author				Krishna
@param [in]			void
@return				void
@pre				nothing
@post				nothing
*/
void CDataGen::GeneratingData()
{
	
	QueueAndLock::ExtractTimeNThreshold();                          // extract the other value from Input.txt file and put in glabal variable 
	//int i = 0;
	int i_Colom_Count = 0, i_Row_Count=0;                           //hold the curent thread colom_count and row_count.   
	std::ifstream InputBuffer(INPUT_ARRAY);                         // input buffer for extracting value from Array.txt
	char tempStr[20];
	while (1)
	{
		if (WaitCheck_1)                                            //check for once. for getting signal from main function
		{
			std::mutex Lock_1;
			std::unique_lock<std::mutex> Ulock(Lock_1);
			CondVar_01.wait(Ulock);                                 //condition variable...for waiting tread till the signal not come from main function.
			WaitCheck_1=false;
		}


	
		for (int j = 0; j < 2; j++)                                // every iteration 2 data will extract and put in queue
		{
			i_Colom_Count++;
			
			if (i_Colom_Count == i_RowLength)                     // condition for check last data in evrey row bcoz after data no "," is there
			{
				i_Colom_Count = 0;
				i_Row_Count++;

				InputBuffer.getline(tempStr, 10);                 // extract data from file
				int i_Array_value = CUtils::StrToDouble(tempStr);				

				QueueAndLock::queue_1(QUEUE_PUSH, i_Array_value);          // put the value in queue_1,...argument 1 for put value in queue(refer QueueAndLock.cpp)
				break;
			}

			InputBuffer.getline(tempStr, 10,',');                 //  extract data from file till "," not came
			int i_Array_value = CUtils::StrToDouble(tempStr);
			QueueAndLock::queue_1(QUEUE_PUSH, i_Array_value);

			Lock_1.lock();                                       // lock for data rachandlinge
			
			if (QueueAndLock::queue_1(EMPTY_CHECK, ZERO))                     //  empty check of queue_1....argument 3 for empty check of queue(refer QueueAndLock.cpp)
			{
				CondVar_12.notify_one();                         // if upper codition satisfy then notify 2nd thread 
			}

			Lock_1.unlock();

		}		
		
		
		if (i_Row_Count == i_RowCount)                           // if it is last row then exit the loop.
		{
			InputBuffer.close();
			break;
		}
	}
	
	
}