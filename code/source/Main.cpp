#include <stdio.h>
#include <string.h>
#include <iostream>
#include <condition_variable>
#include <thread>

#include "CDataGen.h"
#include "CFilterThres.h"
#include "CLabelling.h"
#include "CTracingComp.h"


extern std::condition_variable CondVar_01;

int main()
{
	/*create thread for indivisual block which has different class*/
	std::thread ThreadObj1(&CDataGen::GeneratingData, CDataGen::GetInstance());
	std::thread ThreadObj2(&CFilterThres::CalculateThres, CFilterThres::GetInstance());
	std::thread ThreadObj3(&CLabelling::StartLabelling, CLabelling::GetInstance());
	std::thread ThreadObj4(&CTracingComp::StartTracing, CTracingComp::GetInstance());

	std::this_thread::sleep_for(std::chrono::nanoseconds(100));      

	CondVar_01.notify_one();      // notify the thread 1 for start its processing

	/*thread joins*/
	ThreadObj1.join();
	ThreadObj2.join();
	ThreadObj3.join();
	ThreadObj4.join();
	   	 


	std::getchar();
	return 0;
}