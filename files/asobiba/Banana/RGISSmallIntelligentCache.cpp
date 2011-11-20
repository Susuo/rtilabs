// RGISSmallIntelligentCache.cpp: RGISSmallIntelligentCache �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallIntelligentCache.h"
#include "RGISSmallGlobalToBlock.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallIntelligentCache::RGISSmallIntelligentCache()
{
	for(int i = 0 ; i < GIS_SMALL_CACHE_MAX ; i ++)
	{
		this->Cache[i] = NULL;
	}
}

RGISSmallIntelligentCache::~RGISSmallIntelligentCache()
{
	Destroy();
}

//�쐬
void RGISSmallIntelligentCache::Create(const string & inFilename , const string & inUserDataPath) throw(RException)
{
	File.Create(inFilename , inUserDataPath);
}

//�L���b�V���̔j��.
void RGISSmallIntelligentCache::Destroy()
{
	for(int i = 0 ; i < GIS_SMALL_CACHE_MAX ; i ++)
	{
		delete this->Cache[i];
		this->Cache[i] = NULL;
	}
}

void RGISSmallIntelligentCache::New(RDrawObject  * ioDraw  ,const RGISSmallDrawEffect* inEffect)
{
	File.KeepHanlde();

	RGISSmallDraw draw(ioDraw , inEffect);

	//�q�b�g�ʒu���v�Z���₷���悤�ɁA �f�B�X�v���C���W���u���b�N���W�ɕϊ�.
	LRECT hitBox = ConvertHitBox( inEffect->getGlobalDrawDisplay() );

	//�����Ă������L���b�V��.
	bool deletbleCache[GIS_SMALL_CACHE_MAX];

	//�L���b�V���𒲂ׂāA���ݕ\�����Ȃ���΂����Ȃ��G���A��������\�����A
	//�\�����Ȃ��Ă��悢�G���A��������A�L���b�V����������悤�Ƀt���O�𗧂Ă܂�.
	int i;
	for(i = 0 ; i < GIS_SMALL_CACHE_MAX ; i++)
	{
		if ( Cache[i] == NULL )
		{
			//NULL�Ȃ̂ŏ����Ă��悢.
			deletbleCache[i] = true;
		}
		else if ( isHit( &hitBox, Cache[i]->getBlock() )  )
		{
			//���ݕ\�����Ȃ���΂����Ȃ��G���A�Ȃ̂ŏ����Ă͂���.
			deletbleCache[i] = false;
			
			Cache[i]->Draw(&draw , BlockToDisplayRect(ioDraw , Cache[i]->getBlock(),  inEffect )  );

			TRACE("DrawCache:%d\n",Cache[i]->getBlock() );

		}
		else
		{
			//���ݕ\�����Ȃ��Ă������ꏊ�Ȃ̂ŏ����Ă悢.
			deletbleCache[i] = true;
		}
	}


	for(int y = hitBox.top; y < hitBox.bottom ; y++)
	{
		for(int x = hitBox.left; x < hitBox.right ; x++)
		{
			unsigned long key = y * NEXT_LINE_BLOCK + x;
			//�\�����Ȃ��Ⴞ�߂��ˁA�A�L���b�V���ɂ͂����Ă܂�?
			if (! findCache( key ) )
			{
				//�͂����Ă��Ȃ��̂��A���Ⴀ�A�ǉ�.
				RGISSmallParent * gis = new RGISSmallParent;

				bool ret = File.Load( key , gis);
				if (ret)
				{
					gis->Draw(&draw , BlockToDisplayRect(ioDraw , key,inEffect )  );

					TRACE("Draw!:%d\n",key );
				}
				else
				{
					delete gis;
					TRACE("���[�h�Ɏ��s���܂���:%d\n",key );

					continue;
				}
				//�ŁA�󂢂Ă���ꏊ�ɒǉ������.
				for(int n = 0 ; n < GIS_SMALL_CACHE_MAX ; n++ )
				{
					if ( deletbleCache[n] ) break;
				}
				if (n == GIS_SMALL_CACHE_MAX)
				{
					//�L���b�V�������t����B�B�B
					TRACE("�L���b�V���������ς��ɂȂ�܂���!!");
					//�d���Ȃ��̂ŁA�擪�̂Ƃ����D��.
					n = 0;
				}
				delete Cache[n];
				Cache[n] = gis;
			}
		}
	}
}

//�w�肵���u���b�N��ǂݍ���
const RGISSmallParent* RGISSmallIntelligentCache::Load(unsigned long inBlock)
{
	return LoadPrivate(inBlock);
}

//�w�肵���u���b�N��ǂݍ��� ��������
RGISSmallParent* RGISSmallIntelligentCache::LoadPrivate(unsigned long inBlock)
{
	//�����Ă������L���b�V��.
	bool deletbleCache[GIS_SMALL_CACHE_MAX];

	//�L���b�V���𒲂ׂāA���ݕ\�����Ȃ���΂����Ȃ��G���A��������\�����A
	//�\�����Ȃ��Ă��悢�G���A��������A�L���b�V����������悤�Ƀt���O�𗧂Ă܂�.
	int i;
	for(i = 0 ; i < GIS_SMALL_CACHE_MAX ; i++)
	{
		if ( Cache[i] == NULL )
		{
			//NULL�Ȃ̂ŏ����Ă��悢.
			deletbleCache[i] = true;
		}
		else if ( Cache[i]->getBlock() == inBlock )
		{
			//���ݕ\�����Ȃ���΂����Ȃ��G���A�Ȃ̂ŏ����Ă͂���.
			deletbleCache[i] = false;

			TRACE("DrawCache:%d\n",Cache[i]->getBlock() );
			return Cache[i];
		}
		else
		{
			//���ݕ\�����Ȃ��Ă������ꏊ�Ȃ̂ŏ����Ă悢.
			deletbleCache[i] = true;
		}
	}

	//�͂����Ă��Ȃ��̂��A���Ⴀ�A�ǉ�.
	RGISSmallParent * gis = new RGISSmallParent;

	bool ret = File.Load( inBlock , gis);
	if (ret)
	{
		TRACE("Draw!:%d\n",inBlock );
	}
	else
	{
		delete gis;
		TRACE("���[�h�Ɏ��s���܂���:%d\n",inBlock );

		return NULL;
	}
	//�ŁA�󂢂Ă���ꏊ�ɒǉ������.
	for(int n = 0 ; n < GIS_SMALL_CACHE_MAX ; n++ )
	{
		if ( deletbleCache[n] ) break;
	}
	if (n == GIS_SMALL_CACHE_MAX)
	{
		//�L���b�V�������t����B�B�B
		TRACE("�L���b�V���������ς��ɂȂ�܂���!!");
		//�d���Ȃ��̂ŁA�擪�̂Ƃ����D��.
		n = 0;
	}
	delete Cache[n];
	Cache[n] = gis;

	return gis;
}

//�w�肳�ꂽ�u���b�N���L���b�V���Ƃ͊֌W�Ȃ��ǂݍ���(�L���b�V�����Q�Ƃ��邱�Ƃ͂��Ȃ�)
bool RGISSmallIntelligentCache::LoadEx(unsigned long inBlock , RGISSmallParentImp* ioParentImp)
{
	bool ret = File.Load( inBlock , ioParentImp);
	if (ret)
	{
		TRACE("LoadEx!:%d\n",inBlock );
	}
	return ret;
}




//�L���b�V���Ɏw�肵���L�[�����邩�ǂ�������.
bool RGISSmallIntelligentCache::findCache(unsigned long inKey)
{
	for(int i = 0 ; i < GIS_SMALL_CACHE_MAX ; i++ )
	{
		if ( Cache[i] != NULL)
		{
			if ( Cache[i]->getBlock() == inKey )
			{
				return true;	//�݂���.
			}
		}
	}
	//�L���b�V���ɂ͂Ȃ��ł�.
	return false;
}



//���͈̔͂Ɋ܂܂�܂���?
bool RGISSmallIntelligentCache::isHit(const LRECT * inHitBox , unsigned long inBlock) const
{
	LPOINT xy;
	xy.y = inBlock / NEXT_LINE_BLOCK;
	xy.x = inBlock - (xy.y * NEXT_LINE_BLOCK);

	return (inHitBox->top		<= xy.y && inHitBox->bottom		> xy.y &&
			inHitBox->left		<= xy.x && inHitBox->right		> xy.x);
}

//Hit�����Ɏg���₷���悤�ɁA���炩���ߌv�Z���Ă����܂��B
LRECT RGISSmallIntelligentCache::ConvertHitBox(const LRECT * inDisplay) const
{
	RGISSmallGlobalToBlock g2b;
	g2b.Create(inDisplay->left	, inDisplay->top);
	unsigned long a	= g2b.getBlock();

	g2b.Create(inDisplay->right	, inDisplay->bottom);
	unsigned long b	= g2b.getBlock();

	LRECT ret;

	ret.top		= (a / NEXT_LINE_BLOCK);
	ret.left	= a - (ret.top * NEXT_LINE_BLOCK);
	ret.bottom	= (b / NEXT_LINE_BLOCK) ;
	ret.right	= b - (ret.bottom * NEXT_LINE_BLOCK);

	ret.bottom	++;
	ret.right	++;

	return ret;
}

void RGISSmallIntelligentCache::Find(RGISSmallHeaderImp* ioFindHeader)
{
	File.LoadHeader(ioFindHeader);
}

//�u���b�N����f�B�X�v���C���W�ɕϊ�.
RECT RGISSmallIntelligentCache::BlockToDisplayRect(const RDrawObject *inDraw ,unsigned long inBlock , const RGISSmallDrawEffect* inEffect) const
{
	//�u���b�N�����΍��W��.
	LPOINT xy;
	xy.y = inBlock / NEXT_LINE_BLOCK;
	xy.x = inBlock - (xy.y * NEXT_LINE_BLOCK);

	LRECT rc;
	rc.top = xy.y * 65536;
	rc.left = xy.x * 65536;
	rc.bottom = rc.top + 65536;
	rc.right = rc.left + 65536;

	//�\������_�̍��W�ɕϊ� (�X�P�[���ϊ�)
	RECT ret = inEffect->ConvertShowDisplay(&rc);

	if ( ret.top < 0 ) ret.top = 0;
	if ( ret.left < 0 ) ret.left = 0;
	if ( ret.bottom > inDraw->getHeight() ) ret.bottom = inDraw->getHeight();
	if ( ret.right > inDraw->getWidth() ) ret.right = inDraw->getWidth();

	return ret;
}


//���[�U�[�f�[�^�̕ҏW.
void RGISSmallIntelligentCache::UserDataModify(const LPOINT * inPoint , const int * inDeleteNumber , int inDeleteCount ,
											   const char * inData , int inSize,int inCount ) throw(RException)
{
	//�u���b�N�̓���
	RGISSmallGlobalToBlock g2b;
	g2b.Create(inPoint->x , inPoint->y);
	unsigned long block = g2b.getBlock();
	//�u���b�N�̃��[�h.
	RGISSmallParent * parent = LoadPrivate( block );
	if (parent == NULL) throw RNotFoundException("�u���b�N %d �����[�h�ł��܂���" , block);
	//��������
	File.UserDataModify(block, parent , inDeleteNumber , inDeleteCount , inData , inSize , inCount );
}


//���[�U�[�f�[�^�̌���.
bool RGISSmallIntelligentCache::UserDataFind(const LRECT * inSearchArea , RGISSmallUserDataRead * outUserData ) throw(RException)
{
	//�u���b�N�̓���
	RGISSmallGlobalToBlock g2b;
	g2b.Create(inSearchArea->left , inSearchArea->top);
	unsigned long block = g2b.getBlock();
	//�u���b�N�̃��[�h.
	const RGISSmallParent * parent = Load( block );
	if (parent == NULL) throw RNotFoundException("�u���b�N %d �����[�h�ł��܂���" , block);
	//����
	int distance = parent->FindUserData(outUserData , INT_MAX , inSearchArea);

	//�u���b�N�̓���
	g2b.Create(inSearchArea->left , inSearchArea->bottom);
	unsigned long block2 = g2b.getBlock();
	if (block != block2)
	{
		//�u���b�N�̃��[�h.
		parent = Load( block );
		if (parent == NULL) throw RNotFoundException("�u���b�N %d �����[�h�ł��܂���" , block);
		//����
		distance = parent->FindUserData(outUserData, distance , inSearchArea);
	}

	//�u���b�N�̓���
	g2b.Create(inSearchArea->right , inSearchArea->top);
	block2 = g2b.getBlock();
	if (block != block2)
	{
		//�u���b�N�̃��[�h.
		parent = Load( block );
		if (parent == NULL) throw RNotFoundException("�u���b�N %d �����[�h�ł��܂���" , block);
		//����
		distance = parent->FindUserData(outUserData, distance , inSearchArea);
	}

	//�u���b�N�̓���
	g2b.Create(inSearchArea->right , inSearchArea->bottom);
	block2 = g2b.getBlock();
	if (block != block2)
	{
		//�u���b�N�̃��[�h.
		parent = Load( block );
		if (parent == NULL) throw RNotFoundException("�u���b�N %d �����[�h�ł��܂���" , block);
		//����
		distance = parent->FindUserData(outUserData, distance , inSearchArea);
	}

	return (distance != INT_MAX);
}

