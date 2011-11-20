// RCacheFS.cpp: RCacheFS �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RCacheFS.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RCacheFS::RCacheFS()
{

}

RCacheFS::~RCacheFS()
{

}

RCacheFS* RCacheFS::getInstance()
{
	static RCacheFS cfs;
	return &cfs;
}


//�L���b�V���̌���.
//������Ȃ������� NULL
Rfiles* RCacheFS::Search(__RFILETYPE inRfileType , string inPath)
{
	for(RFILESCACHE::iterator it = Cache.begin(); it != Cache.end() ; ++it)
	{
		if ( ((*it)->getRfileType() == inRfileType) && ((*it)->getPath()) == inPath )
		{
			(*it)->Inc();
			return *it;
		}
	}
	return NULL;
}

//�L���b�V���̊i�[.
void RCacheFS::Append(Rfiles* inRfiles)
{
	ASSERT(inRfiles != NULL);
	Cache.push_back(inRfiles);
}

//�L���b�V���̃N���A.
void RCacheFS::Remove(Rfiles* ioRfiles)
{
	for(RFILESCACHE::iterator it = Cache.begin() ; it != Cache.end() ; ++it)
	{
		if ( *it == ioRfiles) 
		{
			if ( ioRfiles->Dec() )
			{
				Cache.erase(it);
			}
			return;
		}
	}
}

