/*structure which is used in 4th block(tracing and comparision)
 for label operation*/
struct str_ForTracingComp
{
	str_ForTracingComp() :i_size(1)
	{

	}
	int i_left, i_right, i_top, i_buttom;  //for storing co ordinates of each one pixel

	int i_root;                            //for carring root of that labele
	int i_size;                            //size of that label with adding this pixel
};

/*structure for which used in block 3(labelling)
  */
struct str_ForLabelingClass
{
	str_ForLabelingClass() :i_size(1)
	{

	}
	
	int i_root;    //root of that label at present time when that was setting
	int i_size;    //size of the label of that pixel
};

/*structure is using communicating between block 3 & 4*/
struct str_DataQueue_From3T4
{
	str_DataQueue_From3T4()
	{
		i_colom = 0;
		i_row = 0;
		
	}
	int i_colom;                         //present pixel's colom
	int i_row;                           //present pixel's row
	int i_SelectTracing;                 //tracing flag: what we want its a new label ,old label or row complete
	int i_label;                         //label no.
	int i_size;                          //label size
	std::vector<int>v_Neighbour;         //neighbours vector
	std::deque<int>deq_CompleteLabel;    //deque carring the label no. which has no future connection
	
};
