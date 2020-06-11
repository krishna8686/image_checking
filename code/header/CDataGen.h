#ifndef DATA_GEN
#define DATA_GEN

/**---------------------------------------------------------------------
 Project		:	krishna_cynlr   <br>
 Filename     	:	CDataGen.h      <br>

 @details			handle the all function which helping data generation block
 @date         		30/01/2020
 @author       		Krishna
*/
class CDataGen
{
	CDataGen();
public:
	
	~CDataGen();

	static CDataGen* GetInstance();
	void GeneratingData();
	
private:
	static CDataGen* m_ptrDataGen;
	bool WaitCheck_1;

};
#endif 