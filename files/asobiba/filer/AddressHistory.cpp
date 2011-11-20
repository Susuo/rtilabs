// AddressHistory.cpp: AddressHistory �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "AddressHistory.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

AddressHistory::AddressHistory()
{
	HistoryPointer = -1;
}

AddressHistory::~AddressHistory()
{
}

//�i�߂��?
bool AddressHistory::IsNext() const
{
	if (History.size() <= 0) return false;
	return ( (HistoryPointer+1) < History.size());
}

//�߂��?
bool AddressHistory::IsBack() const
{
	if (History.size() <= 0) return false;
	return ((HistoryPointer) > 0 );
}

//�߂�!
//! �߂�Ȃ��Ƃ��ɂ� RNotFoundException  �𓊂���.
string AddressHistory::Back() throw(RNotFoundException)
{
	if ( !IsBack() )
	{
		throw RNotFoundException("����ȏ�߂邱�Ƃ͂ł��܂���");
	}
	return History[--HistoryPointer];
}

//�i��!
//! �i�߂Ȃ��Ƃ��ɂ� RNotFoundException  �𓊂���.
string AddressHistory::Next() throw(RNotFoundException)
{
	if ( !IsNext() ) 
	{
		throw RNotFoundException("����ȏ�i�ނ��Ƃ͂ł��܂���");
	}
	return History[++HistoryPointer];
}

//��O�̃p�X���擾���܂��B
//! �擾�ł��Ȃ��Ƃ��� RNotFoundException  �𓊂���.
string AddressHistory::getLastPath() const throw(RNotFoundException)
{
	if ( !IsBack() )
	{
		throw RNotFoundException("����ȏ���͂���܂���");
	}
	return History[HistoryPointer - 1];
}


//�p�X�̒ǉ�
void AddressHistory::Append(string inAddress)
{
	//�O��ǉ������v�f�͒ǉ����Ȃ�
	if (HistoryPointer >= 0 && History[HistoryPointer] == inAddress) return;


	//�ǉ�
	if ( HistoryPointer+1 < History.size() )
	{
		History.resize(HistoryPointer + 1);
	}
	History.push_back( inAddress );
	HistoryPointer++;
/*
	puts("==============dump================");
	for (HISTORY::iterator it = History.begin() ; it != History.end() ; ++it)
	{
		puts((*it).c_str());
	}
	puts("==============================");
*/
}


void AddressHistory::test()
{
	AddressHistory p;

	ASSERT( ! p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == -1 );

	//--------------------------------
	p.Append("c:\\");
	p.Append("c:\\");	//�����Ȃ̂Œǉ�����Ȃ�

	ASSERT( p.History.size() == 1);

	ASSERT( ! p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 0 );

	//--------------------------------
	p.Append("d:\\");

	ASSERT( p.History.size() == 2);

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//���� c:\\ d:\\ �Ƌl�܂�Ă���͂�.
	p.Append("e:\\");

	ASSERT( p.History.size() == 3);

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//���� c:\\ d:\\ e:\\ �Ƌl�܂�Ă���͂�.
	//               ^-------HistoryPointer �͂������w���Ă܂��B

	//�� e:\\ �̈�O���ق����̂� d:\\ ��Ԃ��܂��B
	//�ʏ�X�^�b�N�Ə������삪�Ⴄ�̂Œ���.
	ASSERT( p.Back() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//���� c:\\ d:\\ e:\\ �Ƌl�܂�Ă���͂�.
	//          ^-------HistoryPointer �͂������w���Ă܂��B

	//�i��ł݂܂��B
	ASSERT( p.Next() == "e:\\" );

	ASSERT( p.IsBack() );
	ASSERT(! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//���� c:\\ d:\\ e:\\ �Ƌl�܂�Ă���͂�.
	//                ^-------HistoryPointer �͂������w���Ă܂��B

	//����ς�߂�܂�
	ASSERT( p.Back() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//����ɖ߂�܂�
	ASSERT( p.Back() == "c:\\" );

	ASSERT( !p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 0 );

	//--------------------------------
	//���� c:\\ d:\\ e:\\ �Ƌl�܂�Ă���͂�.
	//     ^-------HistoryPointer �͂������w���Ă܂��B

	//��i�݂܂��B
	ASSERT( p.Next() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//���� c:\\ d:\\ e:\\ �Ƌl�܂�Ă���͂�.
	//          ^-------HistoryPointer �͂������w���Ă܂��B

	//F5�����[�h���܂����B
	p.Append("d:\\");	//�ǉ�����Ȃ�

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//���� c:\\ d:\\ e:\\ �Ƌl�܂�Ă���͂�.
	//          ^-------HistoryPointer �͂������w���Ă܂��B

	//�ʂ̂Ƃ���ɐ����܂��B
	p.Append("x:\\");	

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//���� c:\\ d:\\ x:\\ �Ƌl�܂�Ă���͂�.
	//               ^-------HistoryPointer �͂������w���Ă܂��B

	//�ʂ̂Ƃ���ɐ����܂��B
	p.Append("z:\\");	

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 3 );

	//--------------------------------
	//���� c:\\ d:\\ x:\\ z:\\ �Ƌl�܂�Ă���͂�.
	//                    ^-------HistoryPointer �͂������w���Ă܂��B

	//�߂�܂�
	ASSERT( p.Back() == "x:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//������߂�܂�
	ASSERT( p.Back() == "d:\\" );

	ASSERT( p.IsBack() );
	ASSERT( p.IsNext() );
	ASSERT( p.HistoryPointer == 1 );

	//--------------------------------
	//���� c:\\ d:\\ x:\\ z:\\ �Ƌl�܂�Ă���͂�.
	//          ^-------HistoryPointer �͂������w���Ă܂��B

	p.Append("p:\\");	

	ASSERT( p.IsBack() );
	ASSERT( ! p.IsNext() );
	ASSERT( p.HistoryPointer == 2 );

	//--------------------------------
	//���� c:\\ d:\\ p:\\ �Ƌl�܂�Ă���͂�.
	//               ^-------HistoryPointer �͂������w���Ă܂��B
}

