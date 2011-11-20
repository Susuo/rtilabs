// RStdioSerialPort.cpp: RStdioSerialPort �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RStdioSerialPort.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RStdioSerialPort::RStdioSerialPort()
{

}

RStdioSerialPort::~RStdioSerialPort()
{

}

void RStdioSerialPort::Open(int inPortNumber,int speed , int inTimeOut) throw (RWin32Exception)
{

	File.Open( string("COM") + num2str(inPortNumber) + ":" ,
			GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DCB dcb = {0};
	if ( GetCommState(File.getHandle(), &dcb) == FALSE )
	{
		throw RWin32Exception("GetCommState �Ɏ��s");
	}
	dcb.BaudRate = speed ;
	dcb.ByteSize = 8 ;
	dcb.StopBits = ONESTOPBIT ;
	dcb.Parity = NOPARITY ;

	if ( SetCommState( File.getHandle() ,&dcb) == FALSE )
	{
		throw RWin32Exception("SetCommState �Ɏ��s");
	}

	//��M�L���[�̃N���A.
	QueeClear();
}

//��M�L���[�̃N���A.
void RStdioSerialPort::QueeClear()
{
	char buffer[1024];
	COMSTAT		comstate;
	DWORD		errorMask;

//	TRACE("//��M�L���[�̃N���A.");
	while(1)
	{
		ClearCommError(File.getHandle(), &errorMask, &comstate);
		if (comstate.cbInQue == 0) break;

		Read(buffer , 1024);
	}
//	TRACE("//��M�L���[�̃N���A �I��.");
}



int RStdioSerialPort::Read(void* inPtr , DWORD inSize ) throw(RWin32Exception)
{
	COMSTAT		comstate;
	DWORD		errorMask;
	
//	TRACE("//COM����ǂݍ��݂܂�.");
	int first = ::GetTickCount();
	while( 1 )
	{
		if (! ClearCommError(File.getHandle(), &errorMask, &comstate) )
		{
			throw RWin32Exception(EXCEPTIONTRACE + ":ClearCommError");
		}

		//��M�o�b�t�@�ɂȂ񂩓����Ă�?
		if ( comstate.cbInQue > 0) break;
		//60�b�Ń^�C���A�E�g.
		if (::GetTickCount() - first > 1000 * 60)
		{
//			TRACE("//COM����ǂݍ��ݒ��Ƀ^�C���A�E�g.");
			return 0;
		}
//		TRACE("//COM����ǂݍ��߂Ȃ��悤�Ȃ̂ł��΂炭�x�݂܂�.");
		//������Ƃ��x��
		::Sleep(1000);
	}
//	TRACE("//COM���� %d �ǂݍ��݂܂�." , (int)comstate.cbInQue);
	return File.Read(inPtr,inSize < comstate.cbInQue ? inSize : comstate.cbInQue);
}
int RStdioSerialPort::Write(const void* inPtr , int inSize) throw(RWin32Exception)
{
	return File.Write(inPtr,inSize);
}
