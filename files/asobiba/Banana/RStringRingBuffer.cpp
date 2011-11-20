// RStringRingBuffer.cpp: RStringRingBuffer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RStringRingBuffer.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

void RStringRingBuffer::Push(const char* inStr , int inSize) throw(RException)
{
	if (inSize <= 0)
	{
		return ;
	}

	if ( this->NowPos + inSize > this->BufferSize )
	{
		if (inSize > this->OneSize)
		{
			throw RException(EXCEPTIONTRACE + "�o�b�t�@�ȏ�̃f�[�^���n����܂���" );
		}
		
		//�V��������������Ă����ꂾ���c����.
		int nokori = this->OneSize - inSize ;
		//�c���镔����擪�Ɏ����Ă���.
		memmove(this->Buffer , this->Buffer + (this->NowPos - nokori ), nokori);
		//�����̌��ɐV������������R�s�[
		memcpy(this->Buffer + nokori , inStr , inSize);
		ASSERT(nokori + inSize <= this->OneSize);
		this->Buffer[nokori + inSize] = '\0';
		this->NowPos = nokori + inSize;
	}
	else
	{
		memcpy(this->Buffer + this->NowPos , inStr , inSize);
		this->Buffer[this->NowPos + inSize] = '\0';
		this->NowPos += inSize;
	}
}
const string RStringRingBuffer::getBuffer() const
{
	if (this->NowPos > this->OneSize)
	{
		return string( this->Buffer ,this->NowPos - this->OneSize , this->OneSize);
	}
	else
	{
		return string( this->Buffer , 0 , this->NowPos);
	}
}

void RStringRingBuffer::test()
{
	string r;
	RStringRingBuffer p;
	p.Create(10);

	string t = "totoro";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "totoro" ) ;

	t = "3";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "totoro3" ) ;

	t = "mahoro";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "oro3mahoro" ) ;

	t = "minawa";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "horominawa" ) ;

	t = "meido";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "inawameido" ) ;

	t = "0123456789";
	p.Push(t.c_str() ,t.size() );
	ASSERT( ( r = p.getBuffer() )  == "0123456789" ) ;
}
