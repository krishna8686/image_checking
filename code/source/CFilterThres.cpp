#include <iostream>
#include "CFilterThres.h"
#include "QueueAndLock.h" 
#include "QueueAndLock.h"



std::condition_variable CondVar_12;      //extern lock
std::mutex Lock_2;                       // extern value

//int TValue[9] = { 1,1,1,1,1,1,1,1,1 };  //extra...........................
/*threshold constant value which are using calcuating the TV in block 2*/
double TValue[9] = { 0.00025177 ,0.008666992 ,0.078025818 ,0.24130249 ,0.343757629 ,0.24130249 ,0.078025818 ,0.008666992 ,0.000125885 };

CFilterThres* CFilterThres::m_ptrFilterThres=nullptr;
CFilterThres::CFilterThres()
{
	i_Colom_Count = 0; i_Row_Count = 0;
	b_vectorEmpty_check = true;
}

CFilterThres::~CFilterThres()
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
CFilterThres* CFilterThres::GetInstance()
{
	if (nullptr == m_ptrFilterThres)
	{
		m_ptrFilterThres = new CFilterThres();
	}
	return m_ptrFilterThres;
}

/**
@brief              this method gets 2 data from queue and when result put in next queue then notify 3 block.
@author				Krishna
@param [in]			void
@return             void
@pre				nothing
@post				nothing
*/
void CFilterThres::CalculateThres()
{
	//int i = 0;
	while (1)                                       // while loop for: in every iteration it call ThresholdCalculation();
	{                                               // and 2 data should extract in every iteration. 
		
		Processing2DataForThresh();                     //call for threshold calculation of 2 element


		Lock_2.lock();		
		if (QueueAndLock::queue_2(EMPTY_CHECK, ZERO))             // empty check of Queue_23(ref QueueAndLock.cpp)
		{                                           //notify 3rd block when the Queue_23 is not empty
			CondVar_23.notify_one();
		}

		Lock_2.unlock();
				
		if (i_Row_Count == i_RowCount)             //break the loop when we reach the one Row complete. 
			break;
		
	}
	
}

/**
@brief              this function call for processing only 2 element from Queue_12 for Threshold calculation.
@author				Krishna
@param [in]			void
@return
@pre				nothing
@post				nothing
*/
void CFilterThres::Processing2DataForThresh()
{
	
	for (int i = 0; i < 2; i++)
	{
		i_Colom_Count++;

		
		std::unique_lock<std::mutex> Ulock(Lock_1);
		if (!QueueAndLock::queue_1(EMPTY_CHECK,ZERO))                                  //check for Queue_12 is empty or not (refer QueueAndLock.cpp) 
		{			
			CondVar_12.wait(Ulock);			                              //wait until it did not get signal from previous thread.
		}

		double value = QueueAndLock::queue_1(QUEUE_PULL, ZERO);                         //get the data from Queue_12
		deq_BeforeThreshold.push_back(value);                            //push the data in a Deque(stl) for geting 9 value for calculation of TV

		if (b_vectorEmpty_check)                                        //for checking the first 4 data in each row
		{			
			b_vectorEmpty_check = false;                                // exicute once condition

			for (int j = 0; j < 4; j++)
			{
				QueueAndLock::queue_2(QUEUE_PUSH, 0);                            // push 4 zero(0) in next queue bcoz for that data we can not calculate
			}                                                           // threshold value.(we dont have all 9 neighbour data of these data)

		}


		if (deq_BeforeThreshold.size() < 9)                             // checking for that Deque is have 9 element or not for TV calcilation
			continue;
		
		int temp;
		temp = TreshholdCalculation();                                 //call this function for calculation threashhold value with tha help of (deq_BeforeThreshold)
		QueueAndLock::queue_2(QUEUE_PUSH, temp);                                // put the result in Queue_23

		deq_BeforeThreshold.pop_front();                              //pop one element from that Deque


		if (i_Colom_Count == i_RowLength)                             // chack if row is complete or not if complete then put 4 zero(0) for last
		{			                                                  // 4 data which we can not calculate TV
			for (int j = 0; j < 4; j++)
			{
				QueueAndLock::queue_2(QUEUE_PUSH, 0);
			}

			/*reset all value for next row start*/
			deq_BeforeThreshold.clear();
			b_vectorEmpty_check = true;
			i_Colom_Count = 0;

			i_Row_Count++;
			break;
		}

		
	}
}

/**
@brief              this function will calculate Threshold filter of each element with help of previos 4 <br>
                    after 4 data of these data.
@author				Krishna
@param [in]			void
@return             int : calculated threshold value
@pre				nothing
@post				nothing
*/
int CFilterThres::TreshholdCalculation()
{
	double sum = 0;;

	for (int i = 0; i < 9; i++)
	{
		sum += (deq_BeforeThreshold[i] * TValue[i]);
	}

	if (sum < d_ThresHoldValue)                                //check the Thresholdvalue check of each result.
		return 0;
	else
		return 1;
	


}