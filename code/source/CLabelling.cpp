#include "CLabelling.h"

#include <iostream>
#include "QueueAndLock.h"


std::condition_variable CondVar_23;
int i_RowCount;
std::mutex Lock_3;

CLabelling* CLabelling::m_ptrLabelling=nullptr;
CLabelling::CLabelling()
{

}

CLabelling::~CLabelling()
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
CLabelling* CLabelling::GetInstance()
{
	if (nullptr == m_ptrLabelling)
	{
		m_ptrLabelling = new CLabelling();
	}
	return m_ptrLabelling;
}

/**
@brief              this method call for processing 2 data for labelling purpose in every iteration. 
@author				Krishna
@param [in]			void
@return             void
@pre				nothing
@post				nothing
*/
void CLabelling::StartLabelling()
{
	
	while (1)
	{
		
		Labelling();                                                 // for processing 2 data from Queue_23 for labelling


		Lock_3.lock();

		str_DataQueue_From3T4 temp_P;
		if (QueueAndLock::queue_3(EMPTY_CHECK, temp_P).i_label)     //empty check of Queue_34
		{
			CondVar_34.notify_one();                                //if upper queue is empty then notyfy block 4 for start its processing
		}

		Lock_3.unlock();

		
		
		if (i_row_count == i_RowCount)                              //row no is last then loop will break.
			break;
	}
}


/**
@brief              Mainly this method call for getting data from Queue_23 and reset all internal data
                    after every row complete.
@author				Krishna
@param [in]			void
@return             void
@pre				nothing
@post				nothing
*/
void CLabelling::Labelling()
{
	/* loop for processing 2 data at every iteration*/
	for (int i = 0; i < 2; i++)
	{	
		
		std::unique_lock<std::mutex> Ulock(Lock_2);
		if (!QueueAndLock::queue_2(EMPTY_CHECK, ZERO))            //emtpy check the Queue_23
		{
			CondVar_23.wait(Ulock);                               //if the previous queue is empty then it will wait till  
		}                                                                 //signal receive from thread 2.

		int value = QueueAndLock::queue_2(QUEUE_PULL, ZERO);      //geting data from Queu_23

		LabelASingleElement(value);                               // this is the main method, which process each data for labelling

		i_colom_count++;
		if (i_colom_count == i_RowLength)                         // row complition check
		{
			str_DataQueue_From3T4 temp;                           // structure for queue which is shared between block 3 and 4.

			temp.i_SelectTracing = ROW_COMPLETE;                  //set row complition flag 
			CompleteLabelCheck(temp.deq_CompleteLabel);           // the argument "deq_CompleteLabel" which carry the label no. 
			                                                      //    which has no future conection in any label(means complete label no)
			QueueAndLock::queue_3(QUEUE_PUSH, temp);			 // push the structure in Queue_34

			/*reset all internal data for next row*/
			pre_vector = prs_vector;  //shift present_row_vector to Previous_row_vector
			prs_vector.clear();       //clear the present_row_vector for carring next row data
			i_colom_count = 0;
			i_row_count++;

			break;
			
		}
	}

}

/**
@brief              this method mainly separate "1" for processing for labelling and update the present_row_vector,
                    and a set which carry all label no that is present in pesent row.
@author				Krishna
@param [in]			int  : raw data which was received from previous queue.
@return             void
@pre				nothing
@post				nothing
*/
void CLabelling::LabelASingleElement(int value)
{
	if (value == 0)
	{
		prs_vector.push_back(0);                      //check the data is 0 or 1 ,if zero thenpush it in present row vector and do nothing
		return;
	}

	int i_temp = ProcessingLabel(value);                     //send "1" for processing label



	//QueueAndLock::Neighbour_calculation(5, 0);  //check
	prs_vector.push_back(i_temp);
	set_LabelsInPresentVector.insert(i_temp);
	v_Neighbour_ForLabellingClass.clear();

}

/**
@brief              This method call for processing all "1".
                     
@author				Krishna
@param [in]			int :raw data "1"
@return             int : label no
@pre				nothing
@post				nothing
*/
int CLabelling::ProcessingLabel(int x)
{
	if (i_row_count==0)                                                 //data processing block for 1st row.
	{
		//RowCheck = 0;
		if (i_colom_count == 0)                                         //data processing for 1st data in every row.
		{
			/*this area for new label only:(1st row and 1st colom)*/

			str_DataQueue_From3T4 temp;                                //structure for Queue_34
			str_ForLabelingClass Temp_str_ForLabelingClass;            //structure for which is used for labelling class only.

			/* set structure for Queue_34 */
			temp.i_SelectTracing = NEW_LABEL;               //set new label flag
			temp.i_row = i_row_count;                       //set present row no.
			temp.i_colom = i_colom_count;                   //set present colom no
			temp.i_label = NewLabelName();                  //label no which was calculated by "NewLabelName" for new label.

			Temp_str_ForLabelingClass.i_root = temp.i_label; //set structure for calulation only in this class.


			/* map: which carry every new label created.  key: label no, key_value: structure of "str_ForLabelingClass" */
			map_Label_ForLabellingClass.insert(std::pair<int, str_ForLabelingClass>(temp.i_label, Temp_str_ForLabelingClass));
			
			QueueAndLock::queue_3(QUEUE_PUSH,temp);          // push in Queue_34
						
			return temp.i_label;                            //return label no of present data
		}
		///////////

		if (prs_vector[i_colom_count-1] != 0)                    // check for previous position value is 0 or 1 in first row
		{
			/*this area for 1st row data except 1st colom element
			  for other things refer upper comment.
			  */
			v_Neighbour_ForLabellingClass.push_back(prs_vector[i_colom_count - 1]);//this data has one neighour which is in previous position in present row

			str_DataQueue_From3T4 temp;		

			temp.i_SelectTracing = OLD_LABEL;
			temp.i_row = i_row_count;
			temp.i_colom = i_colom_count;
			temp.i_label = OldLabelName();                  //label no which was calculated by "OldLabelName" for old label(means label which was already created).
			temp.v_Neighbour = v_Neighbour_ForLabellingClass;  //store the all neighbour which was labelled previously.
			temp.i_size = map_Label_ForLabellingClass[temp.i_label].i_size;    // store the size of that label which was allocated to this data.			

			
			QueueAndLock::queue_3(QUEUE_PUSH, temp);		
		   			
			return temp.i_label;                            //return label no.
		}
		
		/*this area for 1st row data except 1st colom element and element which data have no neighbour
		  manily it will apply for only new label.  for other things refer upper comment.
			  */
		str_DataQueue_From3T4 temp;

		str_ForLabelingClass Temp_str_ForLabelingClass;

		temp.i_SelectTracing = NEW_LABEL;
		temp.i_row = i_row_count;
		temp.i_colom = i_colom_count;
		temp.i_label = NewLabelName();

		Temp_str_ForLabelingClass.i_root = temp.i_label;

		map_Label_ForLabellingClass.insert(std::pair<int, str_ForLabelingClass>(temp.i_label, Temp_str_ForLabelingClass));

		QueueAndLock::queue_3(QUEUE_PUSH, temp);             //push the result in queue for next block 4

		return temp.i_label;                                 //return label no.

	}

	/////////////

	/*this area for all element except data of 1st row*/
	if (i_colom_count == 0)
	{
		/*this area for first colom data only whose neighbour are only top and top right corner */
		if (pre_vector[i_colom_count + 1]!=0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count + 1]);
		if (pre_vector[i_colom_count] != 0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count]);

		if (v_Neighbour_ForLabellingClass.size() == 0)  // those data has no neighbour element that will goes for new label
		{
			/*new label area*/
			str_DataQueue_From3T4 temp;

			str_ForLabelingClass Temp_str_ForLabelingClass;

			temp.i_SelectTracing = NEW_LABEL;
			temp.i_row = i_row_count;
			temp.i_colom = i_colom_count;
			temp.i_label = NewLabelName();

			Temp_str_ForLabelingClass.i_root = temp.i_label;

			map_Label_ForLabellingClass.insert(std::pair<int, str_ForLabelingClass>(temp.i_label, Temp_str_ForLabelingClass));
			QueueAndLock::queue_3(QUEUE_PUSH, temp);                //push in queue

			return temp.i_label;

		}


		/*this area for whose has atleast one neighbour*/
		str_DataQueue_From3T4 temp;

		temp.i_SelectTracing = OLD_LABEL;
		temp.i_row = i_row_count;
		temp.i_colom = i_colom_count;
		temp.i_label = OldLabelName();
		temp.v_Neighbour = v_Neighbour_ForLabellingClass;
		temp.i_size = map_Label_ForLabellingClass[temp.i_label].i_size;


		QueueAndLock::queue_3(QUEUE_PUSH, temp);

		return temp.i_label;

		
	}
	else if (i_colom_count == (i_RowLength-1))
	{
		/*this area for last element in row those has possible 3 neighbour. that is top, topleft corner and left*/
		if (pre_vector[i_colom_count - 1] != 0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count - 1]);
		if (pre_vector[i_colom_count] != 0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count]);
		if (prs_vector[i_colom_count - 1] != 0)
			v_Neighbour_ForLabellingClass.push_back(prs_vector[i_colom_count - 1]);

		if (v_Neighbour_ForLabellingClass.size() == 0)     // whose has no neighboue will goes for new label calculation
		{
			str_DataQueue_From3T4 temp;

			str_ForLabelingClass Temp_str_ForLabelingClass;

			temp.i_SelectTracing = NEW_LABEL;
			temp.i_row = i_row_count;
			temp.i_colom = i_colom_count;
			temp.i_label = NewLabelName();

			Temp_str_ForLabelingClass.i_root = temp.i_label;

			map_Label_ForLabellingClass.insert(std::pair<int, str_ForLabelingClass>(temp.i_label, Temp_str_ForLabelingClass));

			QueueAndLock::queue_3(QUEUE_PUSH, temp);

			return temp.i_label;
		}

		/*those has atlest one labelled neighbour*/
		str_DataQueue_From3T4 temp;

		temp.i_SelectTracing = OLD_LABEL;
		temp.i_row = i_row_count;
		temp.i_colom = i_colom_count;
		temp.i_label = OldLabelName();
		temp.v_Neighbour = v_Neighbour_ForLabellingClass;
		temp.i_size = map_Label_ForLabellingClass[temp.i_label].i_size;


		QueueAndLock::queue_3(QUEUE_PUSH, temp);

		return temp.i_label;

	}
	else
	{
		/*this area will for rest of all element which is not coming in any condition. 
		neighbours are top, top left corner, top right corner and left side */
		if (pre_vector[i_colom_count - 1] != 0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count - 1]);
		if (pre_vector[i_colom_count] != 0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count]);
		if (pre_vector[i_colom_count + 1] != 0)
			v_Neighbour_ForLabellingClass.push_back(pre_vector[i_colom_count + 1]);
		if (prs_vector[i_colom_count - 1] != 0)
			v_Neighbour_ForLabellingClass.push_back(prs_vector[i_colom_count - 1]);

		if (v_Neighbour_ForLabellingClass.size() == 0)                   // neighbours check
		{
			/*this are for new label which has no neighbour*/
			str_DataQueue_From3T4 temp;

			str_ForLabelingClass Temp_str_ForLabelingClass;

			temp.i_SelectTracing = NEW_LABEL;
			temp.i_row = i_row_count;
			temp.i_colom = i_colom_count;
			temp.i_label = NewLabelName();

			Temp_str_ForLabelingClass.i_root = temp.i_label;

			map_Label_ForLabellingClass.insert(std::pair<int, str_ForLabelingClass>(temp.i_label, Temp_str_ForLabelingClass));

			QueueAndLock::queue_3(QUEUE_PUSH, temp);

			return temp.i_label;
		}

		/*those have atleast one neighbour.*/
		str_DataQueue_From3T4 temp;

		temp.i_SelectTracing = OLD_LABEL;
		temp.i_row = i_row_count;
		temp.i_colom = i_colom_count;
		temp.i_label = OldLabelName();
		temp.v_Neighbour = v_Neighbour_ForLabellingClass;
		temp.i_size = map_Label_ForLabellingClass[temp.i_label].i_size;


		QueueAndLock::queue_3(QUEUE_PUSH, temp);

		return temp.i_label;

	}

}

/**
@brief              this method create a new label.
                    if any previous reuseable label no. is present or not other wise create a new label no.
@author				Krishna
@param [in]			void
@return             int: new label no
@pre				nothing
@post				nothing
*/
int CLabelling::NewLabelName()
{
	if (deq_RecycleLabel.empty())    //check the recycle label no deque is empty or not
	{
		return ++i_label_count;      //if empty then create a new label no by increament a global variable
	}
	else
	{
		int a = deq_RecycleLabel[0];  //if the deque has value then pop a label no from that deque.
		deq_RecycleLabel.pop_front();
		return a;
	}
}

/**
@brief              calculate label no. for which data has atlest one neighbour.
                    if it has more than one neighbour then calculate most sized label no and return that label no.
					and rest set label no.s parent to that most sized label no.
@author				Krishna
@param [in]			void
@return             int : calculated label no 
@pre				nothing
@post				nothing
*/
int CLabelling::OldLabelName()
{
	int i_temp = FindRoot(v_Neighbour_ForLabellingClass[0]);                       // take root of first element from neighbour vector.

	/*find the most sized label no from neighbour vector */
	for (int i = 1; i < v_Neighbour_ForLabellingClass.size(); i++)
	{
		int i_temp2 = FindRoot(v_Neighbour_ForLabellingClass[i]);                 // take root of next label no from neighbour vector
		if (map_Label_ForLabellingClass[i_temp].i_size < map_Label_ForLabellingClass[i_temp2].i_size)
			i_temp = i_temp2;                                   

	}


	/*replace the root of all member to the top most sized label no
	and calculate the size of that label with sum of all other label no. size*/
	for (int i = 0; i < v_Neighbour_ForLabellingClass.size(); i++)
	{
		int i_temp2 = FindRoot(v_Neighbour_ForLabellingClass[i]);

		if (i_temp != i_temp2)
		{
			map_Label_ForLabellingClass[i_temp].i_size += map_Label_ForLabellingClass[i_temp2].i_size;  //add the size in that label no. size
			map_Label_ForLabellingClass[i_temp2].i_root = i_temp;                                      //put that label nos root as other label no's root
			
		}
	}

	map_Label_ForLabellingClass[i_temp].i_size++;      // increase that label no's size by one becoz 
	                                                   //we add one present element under that label no. 

	return i_temp;

}

/**
@brief              this method calculate the label no whose has no fissibe posibility to connect any other
                    label in future.
@author				Krishna
@param [in, out]    std::deque :which carry the all  label no which has no possibly connection in future.
@return             void
@pre				nothing
@post				nothing
*/
void CLabelling::CompleteLabelCheck(std::deque<int> &CLabel)
{

	if (i_row_count == 0)     // for first row no need to calculate this method
	{
		/*set: means which carry all label no.present in present/previous row.*/
		set_LabelsInPreviousVector = set_LabelsInPresentVector;  //replace the previous label set to present label set
		set_LabelsInPresentVector.clear();                       // and clear the present label set 
	}
	else
	{
		/*check the each element of previous set which is not present in present set*/
		auto itr = set_LabelsInPreviousVector.begin();     //iterator for previous set
		for (itr; itr != set_LabelsInPreviousVector.end(); itr++)
		{
			if (set_LabelsInPresentVector.count(*itr) == 0)    //check this element from previous set is present in present set or not.
			{
				deq_recycle.push_back(*itr);                   //if not present put that value in to 2 deques.
				deq_RecycleLabel.push_back(*itr);                          //bcoz 1 for future label naming purpose and 2nd for delete that label from map.
			}
		}

		CLabel = deq_recycle;   //copy that deque in to [in/out] variable.

		/*delete the collected label no. from map  */
		if (deq_recycle.size() != 0)
			for (int i = 0; i < deq_recycle.size(); i++)
			{
				map_Label_ForLabellingClass.erase(deq_recycle[i]);
			}


		set_LabelsInPreviousVector = set_LabelsInPresentVector; // replace previos set with present vector
		set_LabelsInPresentVector.clear();                      //clear present set for next row.
		deq_recycle.clear();	                                //clear the deque

	}


	if (i_row_count == i_RowCount - 1)      //if last row reached then clear the map.
	{
		map_Label_ForLabellingClass.clear();
	}
}

/**
@brief              thas method for calculating root of label with the halp of map.
@author				Krishna
@param [in/out]		int:  label no whose root we want to find	
@return             void
@pre				nothing
@post				nothing
*/
int CLabelling::FindRoot(int x)
{
	
	while (x != map_Label_ForLabellingClass[x].i_root) //if the label no is not same as its root then check for its root's root.
	{
		x = map_Label_ForLabellingClass[x].i_root;
	}
	return x;
}
