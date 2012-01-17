#include <iostream>

//�֐��I�u�W�F�N�g�⃁�\�b�h���֐��|�C���^�ɕϊ�����B
//c_function.hpp�����Ɏ����ł�����Ă݂��B
//�Ⴂ  1.�֐��I�u�W�F�N�g�����ł͂Ȃ��A���\�b�h���n����
//      2.boost�s�v :p)
//      3.stdcall �ɂ��Ή��ł���B
//
//����m�F VS2010

//���\�b�h�ɑ΂���R�[���o�b�N
namespace methodcallback
{
	//�W���Ăяo��
	namespace detail
	{
		template<typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD>
		struct callcdecl
		{
			static _CLASS*  _class;
			static _METHOD _method;
			template<typename _RET,typename _P1> static _RET callback(_P1 p1){	return (_class->*_method)(p1);	}
			template<typename _RET,typename _P1,typename _P2> static _RET callback(_P1 p1,_P2 p2){	return (_class->*_method)(p1,p2);	}
			template<typename _RET,typename _P1,typename _P2,typename _P3> static _RET callback(_P1 p1,_P2 p2,_P3 p3){	return (_class->*_method)(p1,p2,p3);	}
			template<typename _RET,typename _P1,typename _P2,typename _P3,typename _P4> static _RET callback(_P1 p1,_P2 p2,_P3 p3,_P4 p4){	return (_class->*_method)(p1,p2,p3,p4);	}
			template<typename _RET,typename _P1,typename _P2,typename _P3,typename _P4,typename _P5> static _RET callback(_P1 p1,_P2 p2,_P3 p3,_P4 p4,_P5 p5){	return (_class->*_method)(p1,p2,p3,p4,p5);	}
		};
		template <typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD> _CLASS* callcdecl<_GUID,FUNC_FUNC,_CLASS,_METHOD>::_class = NULL;
		template <typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD> _METHOD callcdecl<_GUID,FUNC_FUNC,_CLASS,_METHOD>::_method = NULL;
	}
	//�R�[���o�b�N�o�^
	template<typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD>
	FUNC_FUNC regist(_CLASS* _class,_METHOD _method)
	{
		typedef detail::callcdecl<_GUID,FUNC_FUNC,_CLASS,_METHOD> DETAIL;
		DETAIL::_class = _class;
		DETAIL::_method = _method;
		return (FUNC_FUNC)(DETAIL::callback);
	}
	//METHOD���ȗ������ꍇ�� operator() ���Ă΂��.
	template<typename _GUID,typename FUNC_FUNC,typename _CLASS>
	FUNC_FUNC regist(_CLASS* _class)
	{
		return regist<_GUID,FUNC_FUNC>(_class,&_CLASS::operator());
	}

	//stdcall
	namespace detail
	{
		template<typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD>
		struct callstdcall
		{
			static _CLASS*  _class;
			static _METHOD _method;
			template<typename _RET,typename _P1> static _RET __stdcall callback(_P1 p1){	return (_class->*_method)(p1);	}
			template<typename _RET,typename _P1,typename _P2> static _RET __stdcall callback(_P1 p1,_P2 p2){	return (_class->*_method)(p1,p2);	}
			template<typename _RET,typename _P1,typename _P2,typename _P3> static _RET __stdcall callback(_P1 p1,_P2 p2,_P3 p3){	return (_class->*_method)(p1,p2,p3);	}
			template<typename _RET,typename _P1,typename _P2,typename _P3,typename _P4> static _RET __stdcall callback(_P1 p1,_P2 p2,_P3 p3,_P4 p4){	return (_class->*_method)(p1,p2,p3,p4);	}
			template<typename _RET,typename _P1,typename _P2,typename _P3,typename _P4,typename _P5> static _RET __stdcall callback(_P1 p1,_P2 p2,_P3 p3,_P4 p4,_P5 p5){	return (_class->*_method)(p1,p2,p3,p4,p5);	}
		};
		template <typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD> _CLASS* callstdcall<_GUID,FUNC_FUNC,_CLASS,_METHOD>::_class = NULL;
		template <typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD> _METHOD callstdcall<_GUID,FUNC_FUNC,_CLASS,_METHOD>::_method = NULL;
	}
	//�R�[���o�b�N�o�^
	template<typename _GUID,typename FUNC_FUNC,typename _CLASS,typename _METHOD>
	FUNC_FUNC registstdcall(_CLASS* _class,_METHOD _method)
	{
		typedef detail::callstdcall<_GUID,FUNC_FUNC,_CLASS,_METHOD> DETAIL;
		DETAIL::_class = _class;
		DETAIL::_method = _method;
		return (FUNC_FUNC)(DETAIL::callback);
	}
	//METHOD���ȗ������ꍇ�� operator() ���Ă΂��.
	template<typename _GUID,typename FUNC_FUNC,typename _CLASS>
	FUNC_FUNC registstdcall(_CLASS* _class)
	{
		return registstdcall<_GUID,FUNC_FUNC>(_class,&_CLASS::operator());
	}
};

struct AAA
{
	int Method(int ccc)
	{
		return 12 + ccc;
	}
};

struct BBB
{
	int operator() (int ccc)
	{
		return 12 + ccc;
	}
};

int main()
{
	int ans;

	//�N���X���\�b�h AAA:Method
	AAA aaa;
	FUNC f = methodcallback::regist<struct a123,FUNC>(&aaa , &AAA::Method);
	ans = f(22);
	std::cout << "12+22=" << ans << std::endl;

	//�֐��I�u�W�F�N�g operator() 
	BBB bbb;
	FUNC f2 = methodcallback::regist<struct a123,FUNC>(&bbb);
	ans = f2(33);
	std::cout << "12+23=" << ans << std::endl;

	//stdcall ���ĂԂɂ́A registstdcall ���g���Ă��������B
}

