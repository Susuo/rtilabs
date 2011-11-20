// RAlphabetHash.cpp: RAlphabetHash �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RAlphabetHash.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

void RAlphabetHash::test()
{
	{
		RAlphabetHash	hash;
		hash = "anime";
		ASSERT (hash == RAlphabetHash("anime") );
	}
	{
		RAlphabetHash	hash;
		hash = "rti";
		ASSERT (hash == RAlphabetHash("rti") );
	}
	{
		RAlphabetHash	hash;
		hash = "a";
		ASSERT (hash == RAlphabetHash("a") );
		ASSERT (hash != RAlphabetHash("anime") );
	}
	{
		RAlphabetHash	hash;
		hash = "anime";
		ASSERT (hash == "anime" );
	}
	{
		RAlphabetHash	hash;
		hash = "anime";
		ASSERT (hash == "anime" );
	}
}
