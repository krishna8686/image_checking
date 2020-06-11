
#include "CUtils.h"
#include <sstream>


using namespace std;

int CUtils::StrToInt(const char* p_cptr)
{
	std::istringstream s_str(p_cptr);
	int i;
	s_str >> i;
	return i;
}

float CUtils::StrToFloat(const char* p_cptr)
{
	std::istringstream s_str(p_cptr);
	float f;
	s_str >> f;
	return f;
}

double CUtils::StrToDouble(const char* p_cptr)
{
	std::istringstream s_str(p_cptr);
	double d;
	s_str >> d;
	return d;
}

string CUtils::IntToString(const int p_iInt)
{
	stringstream ss;
	ss << p_iInt;
	string str = ss.str();
	return str;
}

string CUtils::LongToString(const long p_long)
{
	stringstream ss;
	ss << p_long;
	string str = ss.str();
	return str;
}

string CUtils::FloatToString(const float p_float)
{
	stringstream ss;
	ss << p_float;
	string str = ss.str();
	return str;
}