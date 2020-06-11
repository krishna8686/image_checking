

#include "QueueAndLock.h"
#include "CUtils.h"
#include "InputFile.h"
#include <fstream>


std::queue<double>Queue_12;
std::queue<int> Queue_23;
std::queue<str_DataQueue_From3T4>Queue_34;


double d_ThresHoldValue, d_ProcessTime;      //


/**
@brief              this method is for accessing the Queue_12.
                    only one process(pull/push/empty check) can possible at a time in one thread
					x:1 for push in the queue
					x:2 for pull from the queue
					x:3 for empty check 
@author				Krishna
@param [in]			int : process cossing flag, means which operation we want
@param [in]         int : for pushing operating it carrys the push data, other than this no use of this value.
@return             int : for pull it will return the pop value, other than this it will return 0.
@pre				nothing
@post				nothing
*/
double QueueAndLock::queue_1(int x, double z)
{
	std::mutex Lock_00;
	std::lock_guard<std::mutex> guard(Lock_00);  //lock for single use of thread.
	if (x == 1)
	{
		Queue_12.push(z);                        // push in the queue
		return (z);
	}
	else if (x == 2)
	{	
		int y;
		y = Queue_12.front();
		Queue_12.pop();                          //pop fromthe queue
		return(y);
	}
	else if (x == 3)
	{			
		if (Queue_12.size() > 0)                 //empty check
		{			
			return 1;
		}
		else
		{			
			return 0;
		}
		
	}
	else
		return 0;
}

/**
@brief              this method is for accessing the Queue_23.
					only one process(pull/push/empty check) can possible at a time in one thread
					x:1 for push in the queue
					x:2 for pull from the queue
					x:3 for empty check
@author				Krishna
@param [in]			int : process cossing flag, means which operation we want
@param [in]         int : for pushing operating it carrys the push data, other than this no use of this value.
@return             int : for pull it will return the pop value, other than this it will return 0.
@pre				nothing
@post				nothing
*/
int QueueAndLock::queue_2(int x, int z)
{
	std::mutex Lock_22;
	std::lock_guard<std::mutex> guard(Lock_22);
	if (x == 1)
	{
		Queue_23.push(z);
		return (z);
	}
	else if (x == 2)
	{	
		int y;				
		y = Queue_23.front();
		Queue_23.pop();
		return(y);
	}
	else if (x == 3)
	{		
		if (Queue_23.size() > 0)    //check
		{			
			return 1;
		}
		else
		{			
			return 0;
		}

	}
	else
		return 0;

}

/**
@brief              this method is for accessing the Queue_34.
					only one process(pull/push/empty check) can possible at a time in one thread
					x:1 for push in the queue
					x:2 for pull from the queue
					x:3 for empty check
@author				Krishna
@param [in]			int : process cossing flag, means which operation we want
@param [in]         int : for pushing operating it carrys the push data, other than this no use of this value.
@return             int : for pull it will return the pop value, other than this it will return 0.
@pre				nothing
@post				nothing
*/
str_DataQueue_From3T4 QueueAndLock::queue_3(int x, str_DataQueue_From3T4 y)
{
	std::mutex Lock_33;
	std::lock_guard<std::mutex> guard(Lock_33);
	if (x == 1)
	{
		Queue_34.push(y);

		return (y);
	}
	else if (x == 2)
	{
		str_DataQueue_From3T4 a;
	    a = Queue_34.front();
		Queue_34.pop();
		return(a);
	}
	else if (x == 3)
	{
		str_DataQueue_From3T4 a;
		if (Queue_34.size() > 0)
		{
			y.i_label = 1;
			return y;
		}
		else
		{
			y.i_label = 0;
			return y;
		}

	}
	else
		return y;

}


/**
@brief              this method will extract the value from"Input.txt".
@author				Krishna
@param [in]			void
@return             void
@pre				nothing
@post				nothing
*/
void QueueAndLock::ExtractTimeNThreshold()
{
	std::ifstream InputBuffer(INPUT);

	char tempStr[20];
	InputBuffer.getline(tempStr, 20);
	i_RowLength = CUtils::StrToInt(tempStr);       //extraction the row length.
	
	InputBuffer.getline(tempStr, 20);
	i_RowCount = CUtils::StrToInt(tempStr);        //extraction the no. of row.

	InputBuffer.getline(tempStr, 20);
	d_ThresHoldValue = CUtils::StrToDouble(tempStr); //extraction of threshold check value.

	InputBuffer.getline(tempStr, 20);
	d_ProcessTime = CUtils::StrToFloat(tempStr);       //extraction of process time.

	InputBuffer.close();

}



