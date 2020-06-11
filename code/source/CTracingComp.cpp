
#include "CTracingComp.h"
#include "QueueAndLock.h"
#include <iostream>

std::condition_variable CondVar_34;
std::mutex Lock_4;

CTracingComp* CTracingComp::m_ptrTracingComp=nullptr;
CTracingComp::CTracingComp()
{


}

CTracingComp::~CTracingComp()
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
CTracingComp* CTracingComp::GetInstance()
{
	if (nullptr == m_ptrTracingComp)
	{
		m_ptrTracingComp = new CTracingComp();
	}
	return m_ptrTracingComp;
}

/**
@brief              this method will prosses the data accoring to the tracing flag.
                    1 for new element creation
					2 for old element data updatetion
					3 for row complete, then claculate other label data calculation
@author				Krishna
@param [in]			void
@return             void
@pre				nothing
@post				nothing
*/
void CTracingComp::StartTracing()
{
	
	while (1)
	{
		str_DataQueue_From3T4 temp_P;
		std::unique_lock<std::mutex> Ulock(Lock_3);
		if (!QueueAndLock::queue_3(3, temp_P).i_label)              //empty check for Queue_34
		{
			CondVar_34.wait(Ulock);                                 //if queue is empty then wait for getting signal from thread 3(labelling)
		}

				

		str_DataQueue_From3T4 temp_str_DataQueue_From3T4;
		temp_str_DataQueue_From3T4 = QueueAndLock::queue_3(2, temp_str_DataQueue_From3T4); //data pull from Queue

		if (temp_str_DataQueue_From3T4.i_SelectTracing == 1)  //tracing flag check for new element
		{
			/*this area is for all calculation of new label element*/
			/*data transformation from shared queue structure to class structure*/
			str_ForTracingComp temp_str_ForTracingComp;
			temp_str_ForTracingComp.i_left = temp_str_DataQueue_From3T4.i_colom;
			temp_str_ForTracingComp.i_right = temp_str_DataQueue_From3T4.i_colom;
			temp_str_ForTracingComp.i_top = temp_str_DataQueue_From3T4.i_row;
			temp_str_ForTracingComp.i_buttom = temp_str_DataQueue_From3T4.i_row;
			temp_str_ForTracingComp.i_root = temp_str_DataQueue_From3T4.i_label;

			/*put the class structure to the class's own map*/
			map_MaintainLabel.insert(std::pair<int, str_ForTracingComp>(temp_str_DataQueue_From3T4.i_label, temp_str_ForTracingComp));
			
		}
		else if (temp_str_DataQueue_From3T4.i_SelectTracing == 2) //tracing flag check for old element calculation
		{

			int i_temp = temp_str_DataQueue_From3T4.i_label;

			/*this loop for updating the coordinate when merge happen*/
			for (int i = 0; i < temp_str_DataQueue_From3T4.v_Neighbour.size(); i++)
			{
				int i_temp2 = FindRoot(temp_str_DataQueue_From3T4.v_Neighbour[i]);

				if (i_temp != i_temp2)
				{					
					map_MaintainLabel[i_temp2].i_root = i_temp;

					if (map_MaintainLabel[i_temp].i_left > map_MaintainLabel[i_temp2].i_left)
						map_MaintainLabel[i_temp].i_left = map_MaintainLabel[i_temp2].i_left;

					if (map_MaintainLabel[i_temp].i_right < map_MaintainLabel[i_temp2].i_right)
						map_MaintainLabel[i_temp].i_right = map_MaintainLabel[i_temp2].i_right;

					if (map_MaintainLabel[i_temp].i_top > map_MaintainLabel[i_temp2].i_top)
						map_MaintainLabel[i_temp].i_top = map_MaintainLabel[i_temp2].i_top;

					if (map_MaintainLabel[i_temp].i_buttom < map_MaintainLabel[i_temp2].i_buttom)
						map_MaintainLabel[i_temp].i_buttom = map_MaintainLabel[i_temp2].i_buttom;
				}


			}

			map_MaintainLabel[i_temp].i_size = temp_str_DataQueue_From3T4.i_size;// getting size of this label from queue structure
			
			/*update co ordinate for single element add in a label*/
			if (map_MaintainLabel[i_temp].i_left > temp_str_DataQueue_From3T4.i_colom)
				map_MaintainLabel[i_temp].i_left = temp_str_DataQueue_From3T4.i_colom;

			if (map_MaintainLabel[i_temp].i_right < temp_str_DataQueue_From3T4.i_colom)
				map_MaintainLabel[i_temp].i_right = temp_str_DataQueue_From3T4.i_colom;

			if (map_MaintainLabel[i_temp].i_top > temp_str_DataQueue_From3T4.i_row)
				map_MaintainLabel[i_temp].i_top = temp_str_DataQueue_From3T4.i_row;

			if (map_MaintainLabel[i_temp].i_buttom < temp_str_DataQueue_From3T4.i_row)
				map_MaintainLabel[i_temp].i_buttom = temp_str_DataQueue_From3T4.i_row;

			
		}
		else if (temp_str_DataQueue_From3T4.i_SelectTracing == 3)   //tracing flag check for row complete
		{
			/*this loop for printing the output of a label which has no connection with
			   other label in future*/
			if (temp_str_DataQueue_From3T4.deq_CompleteLabel.size() != 0)
			for (int i = 0; i < temp_str_DataQueue_From3T4.deq_CompleteLabel.size(); i++)
			{
				if (FindRoot(temp_str_DataQueue_From3T4.deq_CompleteLabel[i]) != temp_str_DataQueue_From3T4.deq_CompleteLabel[i])
				{
					map_MaintainLabel.erase(temp_str_DataQueue_From3T4.deq_CompleteLabel[i]);
					continue;
				}

				PrintOutput(temp_str_DataQueue_From3T4.deq_CompleteLabel[i]);

				map_MaintainLabel.erase(temp_str_DataQueue_From3T4.deq_CompleteLabel[i]);
			}


			if (i_row_count == i_RowCount - 1)  //check for last row condition
			{
				auto itr = map_MaintainLabel.begin();
				for (itr; itr != map_MaintainLabel.end(); itr++)  //print the all label information which are present in last row
				{
					if (itr->first == itr->second.i_root)
					{
						PrintOutput(itr->first);
					}

				}

				map_MaintainLabel.clear();  // clear the map
			}
			i_row_count++;

			
		}

		if (i_row_count == i_RowCount)
			break;
		

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
int CTracingComp::FindRoot(int x)
{
	
	while (x != map_MaintainLabel[x].i_root)      //if the label no is not same as its root then check for its root's root.
	{
		x = map_MaintainLabel[x].i_root;
	}
	return x;
}


/**
@brief              this method prints the one labels all data in console.
                    label no.
					size.
					co ordinates.
@author				Krishna
@param [in]			int : map key value;  in other wordslabel no.
@return             void
@pre				nothing
@post				nothing
*/
void CTracingComp::PrintOutput(int i)
{
	std::cout << "Label is : " << i << std::endl;
	std::cout << "Size is : " << map_MaintainLabel[i].i_size << std::endl;
	std::cout << "Co-ordinates are : " << "(" << map_MaintainLabel[i].i_left+1 << "," << map_MaintainLabel[i].i_top+1 << "), ";
	std::cout << "(" << map_MaintainLabel[i].i_right+1 << "," << map_MaintainLabel[i].i_top+1 << "), ";
	std::cout << "(" << map_MaintainLabel[i].i_right+1 << "," << map_MaintainLabel[i].i_buttom+1 << "), ";
	std::cout << "(" << map_MaintainLabel[i].i_left+1 << "," << map_MaintainLabel[i].i_buttom+1 << ")" << std::endl;
}

