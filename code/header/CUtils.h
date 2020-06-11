#ifndef UTILS
#define UTILS


#include <string>

/**---------------------------------------------------------------------
 Project		:	krishna_cynlr   <br>
 Filename     	:	CUtils.h        <br>

 @details           it is handling all conversion between string to any type or any type to string.
 @date         		30/01/2020
 @author       		Krishna
*/
class CUtils
{
public:
	static std::string IntToString(const int p_iInt);
	static std::string LongToString(const long p_long);
	static std::string  FloatToString(const float p_float);
	static double StrToDouble(const char* p_cptr);
	static float StrToFloat(const char* p_cptr);
	static int StrToInt(const char* p_cptr);
};
#endif // !UTILS


