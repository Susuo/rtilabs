#include <iostream>
#include <string>

//
//�߂�l�ɃG���[�𗍂܂�����悤�ɂ��Ă݂����C�u����
//
//
namespace xreturn
{

//�G���[���グ�������Ɏg��
class error
{
	std::string    errormessage;
	int            errorcode;
public:
	error()
	{
		this->errormessage = "";
		this->errorcode = 0;
	}
	error(const std::string & errormessage , int errorcode = -1)
	{
		this->errormessage = errormessage;
		this->errorcode = errorcode;
	}
	std::string getErrorMessage() const
	{
		return this->errormessage;
	}
	int getErrorCode() const
	{
		return this->errorcode;
	}
};

enum enum_errortype{
	 errortype_no_error
	,errortype_error
	// ���G�ɂȂ邩���߂悤  ,errortype_ecetpyion 
};

//�G���[�����b�v����\��
template <typename TYPE,typename EXCEPTIONTYPE = std::exception>
class r
{
private:
	TYPE value;
	enum_errortype errortype;
	std::string    errormessage;
	int            errorcode;
public:
	typedef typename TYPE __TYPE;
	typedef typename EXCEPTIONTYPE __EXCEPTIONTYPE;

	r()
	{
		this->errormessage = "";
		this->errorcode = -1;
		this->errortype = errortype_error;
	}
	virtual ~r()
	{
	}

	//bool�ɃL���X�g����ꍇ
	operator bool()
	{
		return this->isOK();
	}
	operator TYPE()
	{
		if (! this->isOK() )
		{
			//�Ȃ�� std::exception���� std::string �������Ɏ��Ȃ��񂾂낤�l�B
			throw EXCEPTIONTYPE(this->errormessage.c_str() , this->errorcode);
		}
		return this->value;
	}
	r<TYPE,EXCEPTIONTYPE>& operator =(TYPE a)
	{
		this->errormessage = "";
		this->errorcode = 0;
		this->errortype = errortype_no_error;
		return *this;
	}
	r(TYPE a)
	{
		this->errortype = errortype_no_error;
		this->value = a;
	}

	r(error a)
	{
		this->errortype = errortype_error;
		this->errormessage = a.getErrorMessage();
		this->errorcode = a.getErrorCode();
	}

	bool isOK() const
	{
		return this->errortype == errortype_no_error;
	}
	bool isError() const
	{
		return ! this->isOK();
	}
	const char* getErrorMessage() const
	{
		return this->errormessage.c_str();
	}
	std::string getErrorMessageString() const
	{
		return this->errormessage;
	}
	int getErrorCode() const
	{
		return this->errorcode;
	}
};


//�G���[�����b�v����\��
//bool��p
template <typename EXCEPTIONTYPE>
class r<bool,EXCEPTIONTYPE>
{
private:
	bool value;
	enum_errortype errortype;
	std::string    errormessage;
	int            errorcode;
public:
	typedef typename bool __TYPE;
	typedef typename EXCEPTIONTYPE __EXCEPTIONTYPE;

	r()
	{
		this->errormessage = "";
		this->errorcode = -1;
		this->errortype = errortype_error;
	}
	virtual ~r()
	{
	}

	//bool�ɃL���X�g����ꍇ
	operator bool()
	{
		if (! this->isOK() )
		{
			//�Ȃ�� std::exception���� std::string �������Ɏ��Ȃ��񂾂낤�l�B
			throw EXCEPTIONTYPE(this->errormessage.c_str() , this->errorcode);
		}
		return this->value;
	}
	r<bool,EXCEPTIONTYPE>& operator =(bool a)
	{
		this->errormessage = "";
		this->errorcode = 0;
		this->errortype = errortype_no_error;
		return *this;
	}
	r(bool a)
	{
		this->errortype = errortype_no_error;
		this->value = a;
	}

	r(error a)
	{
		this->errortype = errortype_error;
		this->errormessage = a.getErrorMessage();
		this->errorcode = a.getErrorCode();
	}

	bool isOK() const
	{
		return this->errortype == errortype_no_error;
	}
	bool isError() const
	{
		return ! this->isOK();
	}
	const char* getErrorMessage() const
	{
		return this->errormessage.c_str();
	}
	std::string getErrorMessageString() const
	{
		return this->errormessage;
	}
	int getErrorCode() const
	{
		return this->errorcode;
	}
};


//////////////////////////////////////////////////////////
//�����Ȑ�g���\��
//////////////////////////////////////////////////////////

//OK?
template<typename XRETURN> bool isOK(const XRETURN & a)
{
	return a.isOK();
}
//���߂�?
template<typename XRETURN> bool isError(const XRETURN & a)
{
	return ! isOK(a);
}



//�⑫�����G���[�̓��e�ŗ�O���グ��
template<typename __EXCEPTIONTYPE,typename XRETURN> 
void xthrow(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	//ASSERT( ! a.isOK() );
	if ( a.isOK() )
	{
		//OK�Ȃ̂ɌĂяo���₪�����B�ǂ����悤���ˁE�E�E
		//ASSERT�������B
		abort();
	}

	if ( ! a.isOK() )
	{
		//�{����exception���K�w�\���ɂ������ȁ[�E�E�E�E
		if (errormessage == "" && errorcode == -1) 
		{
			//���[�U�������w�肵�Ȃ��Ƃ��́A
			//���ʃ��C���[���Ԃ������e�ŗ�O���グ��.
			throw __EXCEPTIONTYPE(a.getErrorMessage() , a.getErrorCode() );
		}
		else
		{
			//���[�U�[���w�肵���ꍇ�͂��̓��e�ŗ�O���グ��
			throw __EXCEPTIONTYPE(errormessage.c_str() , errorcode);
		}
	}
}
//�⑫�����G���[�̓��e�ŗ�O���グ�� �̓������O�ȗ��o�[�W����
template<typename XRETURN> 
void xthrow(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	xthrow< XRETURN::__EXCEPTIONTYPE,XRETURN>(a,errormessage,errorcode);
}


//�G���[�����������O�����āA�G���[����Ȃ�������l����
template<typename EXCEPTIONTYPE,typename XRETURN>XRETURN
	check(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	if ( ! a.isOK() )
	{
		xthrow<EXCEPTIONTYPE,XRETURN>(a,errormessage , errorcode);
	}
	return a;
}

//�G���[�����������O�����āA�G���[����Ȃ�������l���� �̓������O�ȗ��o�[�W����
template<typename XRETURN>XRETURN
	check(const XRETURN & a,const std::string & errormessage = "" , int errorcode = -1)
{
	return check<XRETURN::__EXCEPTIONTYPE,XRETURN>(a,errormessage , errorcode);
}

}; //namespace xreturn

///////
//�������牺�͌��؃R�[�h
////////


//�I���I����O
class myexception
{
	std::string message;
	int code;
public:
	myexception( const std::string message , int code)
	{
		this->message = message;
		this->code = code;
	}
	virtual ~myexception()
	{
	}

	std::string what()
	{
		return this->message;
	}
};


xreturn::r<int> div10(int a)
{
	if (a == 0)
	{
		return xreturn::error("�[���ł�������߁[",123);
	}
	return a / 10;
}

xreturn::r<bool,myexception> taechan(std::string fueruyo)
{
	if (fueruyo == "�Ƒ�")
	{
		return xreturn::error("�����A��߂�",123);
	}
	if (fueruyo == "false")
	{
		return false;
	}
	return true;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//�ӂ[�Ɏg��
	int r1 = div10(100);
	std::cout << r1 << std::endl; 

	//�G���[�`�F�b�N���Ďg��.
	if (!div10(0))
	{
		std::cout << "���߁[��" << std::endl; 
	}

	//�G���[�`�F�b�N�̓��e������.
	auto r = div10(0);
	if (!r)
	{
		std::cout << "���߁[��2 ���R:" << r.getErrorMessage() << std::endl; 
	}

	//�E���ɂ��G���[�`�F�b�N�����Ȃ������ꍇ�L���X�g�����u�ԂɎE��.
	try
	{
		//�G���[�`�F�b�N�Ȃ�Ă��Ȃ��Ŏg����<�A
		//int �ɃL���X�g�����u�ԂɎE�����B
		int r99 = div10(0);
		std::cout << r99 << std::endl; 
	}
	catch(std::exception & )
	{
		std::cout << "��O99����1" << std::endl; 
	}


	//�����ŃG���[���n���h�����O����ꍇ
	try
	{
		auto r99 = div10(0);

		//�G���[���������O�𓊂���
		if  ( isError(r99) )
		{
			//��O�ɕϊ�����
			xreturn::xthrow(r99);
		}
	}
	catch(std::exception & )
	{
		std::cout << "��O99����2" << std::endl; 
	}


	//�����ŃG���[���n���h�����O���āA���O exception�ɂ���ꍇ
	try
	{
		auto r99 = div10(0);

		if  ( isError(r99) )
		{
			xreturn::xthrow<myexception>(r99);
		}
	}
	catch(myexception & e )	//std::exception�ł͂Ȃ����Ƃɒ���.
	{
		std::cout << "��O99����3" << e.what() << std::endl; 
	}


	//�G���[��������A�w�肵����O��Ԃ��āA
	//�����łȂ���Βl��Ԃ���!
	try
	{
		int r99 = xreturn::check<myexception>(div10(0) , "�G���[�������炱�̕�����" , 9999);
	}
	catch(myexception & e )	//std::exception�ł͂Ȃ����Ƃɒ���.
	{
		std::cout << "��O99����4" << e.what() << std::endl; 
	}

	//true ���ς��Ă���ꍇ
	if ( taechan("true") )
	{
		std::cout << "bool�e�X�g ����1" << std::endl; 
	}

	//false ���ς��Ă���ꍇ
	if ( ! taechan("false") )
	{
		std::cout << "bool�e�X�g ����2" << std::endl; 
	}

	//�E�����ꍇ
	try
	{
		if ( ! taechan("�Ƒ�") )
		{
		}
	}
	catch(myexception & e )	//std::exception�ł͂Ȃ����Ƃɒ���.
	{
		std::cout << "bool�e�X�g�B��O" << e.what() << std::endl; 
	}


	return 0;
}

