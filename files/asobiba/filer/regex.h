#ifdef DEBUG
	#undef new								//�f�o�b�O�p�� new ���E��

	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
	#include <boost/regex.hpp>

	#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)		//�E�����̂ōĒ�`
#else
	#pragma warning (disable: 4786) 
	#pragma warning (disable: 4503) 
	#include <boost/regex.hpp>
#endif
