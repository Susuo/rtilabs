
#include <windows.h>
#include <process.h>
#include <Dbghelp.h>

#include <stdio.h>

#define ASSERT
#define TRACE

class StackTrace
{
public:
	StackTrace()
	{
		this->CurrentProccessHandle = NULL;
		this->CurrentThreadHandle = NULL;
		this->IsGet = false;
	}
	~StackTrace()
	{
		//����͕��Ȃ��Ă�����.
		this->CurrentProccessHandle = NULL;

		//�������͕���
		if (this->CurrentThreadHandle)
		{
			CloseHandle(this->CurrentThreadHandle);
			this->CurrentThreadHandle = NULL;
		}

	}
private:
	//���݂̃v���Z�X�n���h��
	HANDLE CurrentProccessHandle;
	//���݂̃X���b�h�n���h��
	HANDLE CurrentThreadHandle;

	//������������?
	bool IsGet;
private:
//	static unsigned int __stdcall _Start(void* inThis)
	static unsigned long __stdcall _Start(void* inThis)
	{
		return ((StackTrace*)inThis)->Run();
	}

	//�X�^�b�N�擾�X���b�h�̒�
	unsigned int Run()
	{
		ASSERT(this->CurrentProccessHandle == NULL);
		ASSERT(this->CurrentThreadHandle == NULL);

		//�܂��A�����������X���b�h���~������.
		::SuspendThread(this->CurrentThreadHandle);

		StackDump();

		//���������������̂ŕ��A������.
		::ResumeThread(this->CurrentThreadHandle);

		return 0;
	}

	void StackDump()
	{
		ASSERT(this->CurrentProccessHandle == NULL);
		ASSERT(this->CurrentThreadHandle == NULL);

		this->CurrentProccessHandle = ::GetCurrentProcess();

		BOOL r;

		//�X���b�h�̃R���e�L�X�g�Q�b�g
		CONTEXT threadContext = {0};
		threadContext.ContextFlags = CONTEXT_FULL;
		r = ::GetThreadContext(this->CurrentThreadHandle , &threadContext);
		if (!r)
		{
			ASSERT(0);
			return ;
		}

		//�V���{���G���W���̏�����
		r = ::SymInitialize(this->CurrentProccessHandle , NULL , TRUE);
		if (!r)
		{
			ASSERT(0);
			return ;
		}

		//�s�ԍ��܂Ń��[�h���Ăق����ȁ[
		DWORD option = ::SymGetOptions();
		option |= SYMOPT_LOAD_LINES;
		option &= ~SYMOPT_UNDNAME;
		::SymSetOptions(option);


		//�����������X���b�h�̃X�^�b�N�̏��͂���Ȋ���.
		STACKFRAME stackFrame = {0};
		stackFrame.AddrPC.Offset    = threadContext.Eip;
		stackFrame.AddrStack.Offset = threadContext.Esp;
		stackFrame.AddrFrame.Offset = threadContext.Ebp;

		stackFrame.AddrPC.Mode      = AddrModeFlat;
		stackFrame.AddrStack.Mode   = AddrModeFlat;
		stackFrame.AddrFrame.Mode   = AddrModeFlat;
		stackFrame.AddrReturn.Mode  = AddrModeFlat;
		stackFrame.AddrBStore.Mode  = AddrModeFlat;

		while(1)
		{
			//�X�^�b�N��������܂�.
			r =	StackWalk(IMAGE_FILE_MACHINE_I386 , 
					this->CurrentProccessHandle , this->CurrentThreadHandle ,
					&stackFrame, &threadContext , NULL, 
					SymFunctionTableAccess, SymGetModuleBase, NULL);
			if (!r)
			{
				break;
			}
			if ( stackFrame.AddrPC.Offset == 0 )
			{
				break;
			}

			IMAGEHLP_MODULE imageModule = { sizeof(IMAGEHLP_MODULE) };
			r = SymGetModuleInfo(this->CurrentProccessHandle , stackFrame.AddrPC.Offset , &imageModule);
			if (!r)
			{
				printf("%p:no module:no symbol\r\n" , stackFrame.AddrPC.Offset );
			}
			else
			{
				//�V���{�����i�[�o�b�t�@.
				IMAGEHLP_SYMBOL * imageSymbol;
				char buffer[6000 + sizeof(IMAGEHLP_SYMBOL) ] = {0};
				imageSymbol = (IMAGEHLP_SYMBOL*)buffer;
				imageSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
				imageSymbol->MaxNameLength = 6000 - sizeof(IMAGEHLP_SYMBOL);

				//�֐����̎擾...
				DWORD disp = 0;
				r = ::SymGetSymFromAddr(this->CurrentProccessHandle , stackFrame.AddrPC.Offset , &disp , imageSymbol );
				if (!r)
				{//�֐������킩��܂���.
					printf("%p:%s:no symbol\r\n" , stackFrame.AddrPC.Offset , imageModule.ModuleName );
				}
				else
				{
					//�s�ԍ��̎擾
					IMAGEHLP_LINE line ={0};
					r = ::SymGetLineFromAddr
						(this->CurrentProccessHandle , stackFrame.AddrPC.Offset , &disp , &line);
					if (!r)
					{//�s�ԍ���������܂���
						printf("%p:%s:%s\r\n" , stackFrame.AddrPC.Offset , imageModule.ModuleName , imageSymbol->Name);
					}
					else
					{//�s�ԍ����킩��܂���.
						printf("%p:%s:%s %s:%d\r\n" , stackFrame.AddrPC.Offset , 
										imageModule.ModuleName , imageSymbol->Name , 
										line.FileName , line.LineNumber	);
					}
				}
			}
		}
		//�V���{���G���W���̏I��
		SymCleanup(this->CurrentProccessHandle);

		this->IsGet = true;


	}
public:
	bool Here()
	{
		ASSERT(this->CurrentProccessHandle == NULL);
		ASSERT(this->CurrentThreadHandle == NULL);

		HANDLE currentProccessHandle = ::GetCurrentProcess();

		//���݂̃X���b�h�̃n���h�����擾...
		//�ł��A GetCurrentThread �Ŏ擾�ł���̂́A���̃v���Z�X���ŗL���ȃ_�~�[��
		//���͂����Ƃ�������ق����񂾂�
		//����́A DuplicateHandle �ŕ�������Ǝ�ɓ���!!
		HANDLE currentThreadHandleDummy = ::GetCurrentThread();
		HANDLE currentThreadHandle;
		BOOL dup = 
			::DuplicateHandle(	currentProccessHandle , currentThreadHandleDummy , 
								currentProccessHandle , &currentThreadHandle , 
								0 , FALSE , DUPLICATE_SAME_ACCESS);
		if (!dup)
		{
			TRACE("StackTrace::Here:DuplicateHandle error:%d" , ::GetLastError() );
			ASSERT(0);
			return false;
		}
		//�O��̃X���b�h�n���h������ɓ��ꂽ��!!
		this->CurrentProccessHandle = currentProccessHandle;
		this->CurrentThreadHandle = currentThreadHandle;

		//���݂̃X���b�h�̃X�^�b�N�����������g�̃X���b�h�Ō��邱�Ƃ͏o���܂���
		//�����āA�X�^�b�N�擾�̏����𑖂点����A�X�^�b�N�ɗ]�v�ɉו����l�܂�邵
		//����āA�ʃX���b�h���쐬���܂�.
//		unsigned int threadID = 0;
//		HANDLE stackTrackerThread = (HANDLE)_beginthreadex(NULL , 0 , _Start , (void*) this ,  0 , &threadID);
		unsigned long threadID = 0;
		HANDLE stackTrackerThread = CreateThread(NULL , 0 ,_Start , (VOID*) this , 0 , &threadID);
		if (stackTrackerThread == NULL)
		{
			TRACE("StackTrace::Here:_beginthreadex error:%d" , ::GetLastError() );
			ASSERT(0);

			return false;
		}

		//�X�^�b�N�擾�X���b�h���I���܂őҋ@.
		while( WaitForSingleObject(stackTrackerThread , 0 ) != WAIT_OBJECT_0 )
		{
		}

		//�X�^�b�N�擾�X���b�h���~�A�����l
		::CloseHandle(stackTrackerThread);
		stackTrackerThread = NULL;

		return this->IsGet;
	}
};


int sum(int a , int b)
{
	StackTrace st;
	st.Here();

	return a + b;
}

void main()
{
	int ans = sum(1,2);

	printf("ans:%d\r\n" , ans);
}