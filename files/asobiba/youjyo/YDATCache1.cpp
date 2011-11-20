// YDATCache1.cpp: YDATCache �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YDATCache1.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YDATCache::YDATCache()
{
	BoardName = "";
	UnHit = 0;
}

YDATCache::~YDATCache()
{
	//���ׂĕۑ����A�������.
	for(DATCACHE::iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		try
		{
			ydat->Save(this->BoardName);
		}
		catch(RException e)
		{
			printf("(%s:%d)�̃L���b�V�����t���b�V�����Ă���Ƃ��ɗ�O�����m:%s\n",
				this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
		}
		delete ydat;
	}
}

YDAT* YDATCache::getDAT(DATNUMBER inDATNumber) throw(RIOCannotOpenException)
{
	ASSERT(inDATNumber != 0);

	YDATCap* ydat;
	{
		RAutoReadSpinLock al(&Lock);
		DATCACHE::iterator i = this->Cache.find(inDATNumber);
		if ( i != this->Cache.end() )
		{
			ydat = (*i).second;
			ydat->Use();
			return ydat;
		}
	}
	{
		RAutoWriteSpinLock al(&Lock);

		try
		{
			ydat = new YDATCap( inDATNumber);
			ydat->Load(this->BoardName);
		}
		catch(RException e)
		{	//�ǂݍ��݂Ɏ��s.
			delete ydat;
			throw;
		}
		Cache[inDATNumber] = ydat;

		ydat->Use();

		this->UnHit ++;

		return ydat;
	}
}


YDAT* YDATCache::getNewDAT(DATNUMBER inDATNumber) throw (RAlreadyException)
{
	RAutoWriteSpinLock al(&Lock);

//	if ( Cache.end() != Cache.find(inDATNumber) ) throw RAlreadyException("���ł� DAT�͑��݂��Ă��܂�");

	YDATCap* ydat = new YDATCap(inDATNumber);
	ydat->Use();
	Cache[inDATNumber] = ydat;

	return ydat;
}

void YDATCache::Release(YDAT* inDAT)
{
	RAutoWriteSpinLock al(&Lock);
	//�댯�����ǂ܂����������Ȃ�?

	//�Q�ƃJ�E���g�̔j��.
	((YDATCap*)inDAT)->UnUse();
}

/**
 * Collecter:		�K�y�[�W�R���N�g
 *
 * @return void 
 */
void YDATCache::Collecter()
{
	RAutoWriteSpinLock al(&Lock);

	/*ChangeLog.txt:2003/01/23	�K�y�[�W�R���N�g���鎞�Ԃ̎w����Ԉ���Ă����̂ŏC�� 60 * 1 = 1�� �̂Ƃ���� 1000 * 1 = 1�� �Ƃ��Ă���(w
	*/
	//���S��?
	if ( Cache.size() < 20 )
	{
		/*ChangeLog.txt:2003/01/23	�N���b�V���ɂ��f�[�^�������̂��|���̂� 30���A�N�Z�X����Ă��Ȃ��t�@�C���͎����I�Ƀf�B�X�N�ɏ����߂��悤�ɏC��
		*/
		//�K�y�[�W�R���N�g����قǃf�[�^�͂��܂��Ă��܂���B
		//�������A�f�[�^�ی�̂���
		//30���A�N�Z�X���Ă��Ȃ��f�[�^�̓f�B�X�N�Ɏ����I�ɏ����߂��܂��B �L���b�V���̊J���͂��܂���B
		Saver( 60 * 30);
	}
	else if ( Cache.size() < 30 )
	{
		//30�����o�߂��Ă���f�[�^�̍폜
		DeleteWhere( 60*30);
	}
	else if ( Cache.size() < 40 )
	{
		//20�����o�߂��Ă���f�[�^�̍폜
		DeleteWhere(60*20);
	}
	else if ( Cache.size() < 50 )
	{
		//5�����o�߂��Ă���f�[�^�̍폜
		DeleteWhere( 60*5);
	}
	else
	{
		//0�����o�߂��Ă���f�[�^�̍폜
		DeleteWhere(0);
	}
}

/**
 * DeleteWhere:			�폜
 *
 * @param inPoolTime	�ێ��������
 * @return void 
 */
void YDATCache::DeleteWhere(int inPoolTime)
{
	DWORD now = time(NULL);
	for(DATCACHE::iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		if ( (!ydat->isUse()) && ydat->DifferenceTime(now) >= inPoolTime)
		{
			try
			{
				printf("YoujyoGC::(%s:%d)��GC����!\n",this->BoardName.c_str() , ydat->getDatNumber() );

				ydat->Save( this->BoardName );
				delete ydat;	//���ځ[��
				ydat = NULL;
				i = Cache.erase(i);
			}
			catch(RException e)
			{
				if (ydat)
				{
					printf("YoujyoGC::(%s:%d)��GC�������Ă���Œ��ɗ�O�����m:%s\n",
						this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
				}
				else
				{
					printf("YoujyoGC::(%s:�s��)��GC�������Ă���Œ��ɗ�O�����m:%s\n",
						this->BoardName.c_str() , e.getMessage() );
				}
			}
		}
	}
}

//�����ƃ�������ɂ������݂��Ă��� DAT �͊�Ȃ��̂Ńf�B�X�N�ɏ�������.
void YDATCache::Saver(int inPoolTime)
{
	DWORD now = time(NULL);
	for(DATCACHE::iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		if ( (!ydat->isUse()) && ydat->DifferenceOnMemoryTime(now) >= inPoolTime)
		{
			try
			{
				printf("YoujyoGC::(%s:%d)�͂��΂炭�A�N�Z�X����Ă��Ȃ��̂Ńt���b�V�����܂�!\n",this->BoardName.c_str() , ydat->getDatNumber() );
				ydat->Save(this->BoardName);
				ydat->WriteBack();
			}
			catch(RException e)
			{
				printf("(%s:%d)�̃L���b�V�����t���b�V�����Ă���Ƃ��ɗ�O�����m:%s\n",
					this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
			}
		}
	}
}


//���ׂẴL���b�V�����f�B�X�N�Ƀt���b�V������.
void YDATCache::Flush() const
{
	RAutoWriteSpinLock al(&Lock);

	for(DATCACHE::const_iterator i = Cache.begin() ; i != Cache.end() ; ++i)
	{
		YDATCap* ydat = (*i).second;
		try
		{
			ydat->Save(this->BoardName);
		}
		catch(RException e)
		{
			printf("(%s:%d)�̃L���b�V�����t���b�V�����Ă���Ƃ��ɗ�O�����m:%s\n",
				this->BoardName.c_str() , ydat->getDatNumber() , e.getMessage() );
		}
	}
}


//�g�p�����o��
void YDATCache::Dump() const
{
	RAutoWriteSpinLock al(&Lock);

	puts("========================== Cache Dump ============================");
	printf("�ێ��L���b�V�� %d\r\n" , Cache.size() );
	printf("��q�b�g�� %d\r\n" , this->UnHit );
	puts("========================== ========= ============================");
}

