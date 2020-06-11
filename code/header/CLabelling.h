#ifndef LABELLING
#define LABELLING

#include "QueueAndLock.h"

/**---------------------------------------------------------------------
 Project		:	krishna_cynlr   <br>
 Filename     	:	CLabelling.h    <br>

 @details           handle the all function which helping data labelling block
 @date         		30/01/2020
 @author       		Krishna
*/
class CLabelling
{
	CLabelling();
public:
	
	~CLabelling();

	static CLabelling* GetInstance();
	void StartLabelling();
	void Labelling();
	void LabelASingleElement(int value);
	int ProcessingLabel(int x);
	//void ConVar40WaitState_WaitCheck_4();
	int NewLabelName();
	int OldLabelName();
	int FindRoot(int x);
	void CompleteLabelCheck(std::deque<int> &CLabel);
	

private:
	std::vector<int>pre_vector, prs_vector;
	static CLabelling* m_ptrLabelling;
	int i_row_count=0, i_colom_count=0, i_label_count;

	std::deque<int>deq_RecycleLabel, deq_recycle;
	std::map<int, str_ForLabelingClass>map_Label_ForLabellingClass;
	std::vector<int>v_Neighbour_ForLabellingClass;

	int i_SelectTracing, i_NewLabel_Row, i_NewLabel_Colom;
	std::set<int>set_LabelsInPresentVector;
	std::set<int>set_LabelsInPreviousVector;



};
#endif 
