#ifndef FILTER_THRES
#define FILTER_THRES

#include <deque>

/**
 Project		:	krishna_cynlr    <br>
 Filename     	:	CFilterThres.h   <br>

 @details           handle the all function which helping filter threshold block
 @date         		30/01/2020
 @author       		Krishna
*/
class CFilterThres
{
	CFilterThres();
public:
	
	~CFilterThres();

	static CFilterThres* GetInstance();
	void CalculateThres();

private:
	static CFilterThres* m_ptrFilterThres;
	void Processing2DataForThresh();
	int TreshholdCalculation();
	std::deque<double> deq_BeforeThreshold;
	int i_Colom_Count, i_Row_Count;
	bool b_vectorEmpty_check;

};
#endif
