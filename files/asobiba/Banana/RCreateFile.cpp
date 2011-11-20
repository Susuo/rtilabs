// RCreateFile.cpp: RCreateFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RCreateFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
void RStream::test()
{
	{
		RCreateFile file;
		file.Open("test/rstreamtest.txt" , GENERIC_READ);
		StreamCheckerForReturnCode returnCheck;
		
		char buffer[4];

		RStream stream;
		stream.Create( &file , 11);
		int readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("abc") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("bes") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("t!") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("tot") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("oro") == buffer);

		readSize = stream.Read(buffer , 3 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("") == buffer);
	}
	{
		RCreateFile file;
		file.Open("test/rstreamtest.txt" , GENERIC_READ);
		StreamCheckerForReturnCode returnCheck;
		
		char buffer[11];

		RStream stream;
		stream.Create( &file , 11);
		int readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("abc") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("best!") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("totoro") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("") == buffer);
	}
	{
		RCreateFile file;
		file.Open("test/rstreamtest.txt" , GENERIC_READ);
		StreamCheckerForReturnCode returnCheck;
		
		char buffer[11];

		RStream stream;
		stream.Create( &file );
		int readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("abc") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("best!") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("totoro") == buffer);

		readSize = stream.Read(buffer , 10 , &returnCheck);
		buffer[readSize] = '\0';
		ASSERT(string("") == buffer);
	}
}


int RStream::Read(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException)
{
	ASSERT(BufferLength >= inBufferSize);

	//�܂��A�O��ǂ݂������Ƃ���ɏI�[�����邩�`�F�b�N.
	if (! inStreamChecker->Check(Buffer , 0 , BufferUse) )
	{//����.
		int breakPoint = inStreamChecker->getSeparater();

		//�Ƃ肴��ɃR�s�[.
		int copy = breakPoint > inBufferSize ? inBufferSize : breakPoint;
		memcpy(outBuffer , Buffer , copy);
		//�ǂݍ��݂��������͐擪�ɉ�
		int skip = copy + (inStreamChecker->getNext() - inStreamChecker->getSeparater() ) + 1;
		memmove(Buffer , Buffer + skip  , BufferUse - skip );
		BufferUse = BufferUse - skip;

		return copy;
	}
	//���M�ȏ�̗ʂ��v�[�����Ă���?
	if (BufferUse > inBufferSize)
	{
		//�Ƃ肴��ɃR�s�[.
		int copy = inBufferSize;
		memcpy(outBuffer , Buffer , copy);
		//�ǂݍ��݂��������͐擪�ɉ�
		memmove(Buffer , Buffer + copy , BufferUse - copy);
		BufferUse = BufferUse - copy;

		return copy;
	}
	int sended;
	for(; BufferUse < BufferLength ; BufferUse += sended)
	{
//		TRACE("//RStream::Read  �ǂݍ��݂܂�." );
		//�ǂݍ���
		sended = Stream->Read(Buffer+BufferUse ,BufferLength - BufferUse);
//		TRACE("//RStream::Read  %d �ǂݍ��݂܂���." , sended);
		if ( sended == 0 )
		{//�ǂݍ��ݏI��
			break;
		}
		else if (sended < 0)
		{//�ُ펖��
			throw RException(EXCEPTIONTRACE + "�ǂݍ��ݎ��ɃG���[���������܂���");
		}
		//���ǂݍ��񂾒��ɏI�����������邩�`�F�b�N.
		if ( ! inStreamChecker->Check(Buffer , BufferUse , BufferUse+sended) )
		{	//�`�F�b�J�[�ɂ���~.
			BufferUse += sended;

			int breakPoint = inStreamChecker->getSeparater();

			//�Ƃ肴��ɃR�s�[.
			int copy = breakPoint > inBufferSize ? inBufferSize : breakPoint;
			memcpy(outBuffer , Buffer , copy);
			//�ǂݍ��݂��������͐擪�ɉ�
			int skip = copy + (inStreamChecker->getNext() - inStreamChecker->getSeparater() ) + 1;
			memmove(Buffer , Buffer + skip  , BufferUse - skip );
			BufferUse = BufferUse - skip;

			return copy;
		}
	}

	//���M�ɃR�s�[
	int copy = BufferUse > inBufferSize ? inBufferSize : BufferUse;
	memcpy(outBuffer , Buffer , copy);
	//�ǂݍ��݂��������͐擪�ɉ�
	memmove(Buffer , Buffer + copy , BufferUse - copy);
	BufferUse = BufferUse - copy;

	return copy;

}
