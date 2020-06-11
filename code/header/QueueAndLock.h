#ifndef QUEUE_LOCK
#define QUEUE_LOCK

#include <queue>
#include <map>
#include <set>
#include <mutex>
#include <condition_variable>

#include "Structures.h"
#include "InputFile.h"


extern std::condition_variable CondVar_01, CondVar_12, CondVar_23, CondVar_34;
extern int i_RowLength, i_RowCount;
extern double d_ThresHoldValue, d_ProcessTime;

extern std::mutex Lock_1, Lock_2, Lock_3, Lock_4;


/**---------------------------------------------------------------------
 Project		:	krishna_cynlr               <br>
 Filename     	:	 <br>

 @details           handle the all function which helping shared queue operation
 @date         		30/01/2020
 @author       		Krishna
*/
class QueueAndLock
{
public:
	static double queue_1(int x, double y);
	static int queue_2(int x, int y);
	static str_DataQueue_From3T4 queue_3(int x, str_DataQueue_From3T4 y);

	static void ExtractTimeNThreshold();

private:
	
	


};

#endif 