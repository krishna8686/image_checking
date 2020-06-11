#ifndef TRACING_COMPUTATION
#define TRACING_COMPUTATION

#include "QueueAndLock.h"

/**---------------------------------------------------------------------
 Project		:	krishna_cynlr     <br>
 Filename     	:	CTracingComp.h    <br>

 @details           handle the all function which helping tracing computation block
 @date         		30/01/2020
 @author       		Krishna
*/
class CTracingComp
{
	//CTracingComp();
public:
	CTracingComp();
	~CTracingComp();

	static CTracingComp* GetInstance();
	void StartTracing();
	void PrintOutput(int i);
	int FindRoot(int x);
private:
	static CTracingComp* m_ptrTracingComp;
	std::map<int, str_ForTracingComp> map_MaintainLabel;
	int i_row_count = 0, i_colom_count = 0, i_label_count = 0;


};
#endif 
