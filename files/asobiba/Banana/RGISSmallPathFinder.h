// RGISSmallPathFinder.h: RGISSmallPathFinder �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLPATHFINDER_H__20EB24EB_2DDE_4ED4_806F_5BC79E38E1A5__INCLUDED_)
#define AFX_RGISSMALLPATHFINDER_H__20EB24EB_2DDE_4ED4_806F_5BC79E38E1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISSmallIntelligentCache.h"
/*
const int LookAheadDependMax	=	50;
const int RouteTestAreaMax		=	80000;		//960000Bytes -> �� 937.5k

class RGISSmallPathFinder  
{
	enum SearchResult
	{
		OK ,
		NotEnoughMemory ,
		Goal ,
	};

	struct RouteTestData
	{
		unsigned long	Block;						//�u���b�N�ԍ�
		unsigned long	DNumber;					//���H�ԍ�
		bool	Active;								//�L���Ȍ����|�C���g�ł��邩?
		bool	Up;
		unsigned char		nodePoint;				//�m�[�h�̏ꏊ
		int		Befor;								//��O�� RouteTestData �̏ꏊ
	};

	typedef vector<UPOINT>	RootRode;

	//��s�錾.
	class RGISSmallAccumulationCahe;

	//�o�H�͓��H�ȊO�쐬���Ȃ��̂œ��H�����p�[�X�Ő؂�΂悢.
	class RGISSmallParentDouroOnly : public RGISSmallParentImp
	{
	private:
		//���̃N���X�̍쐬�A�j���́A RGISSmallAccumulationCahe �ȊO�͍s���Ă͐���Ȃ�!
		friend class RGISSmallAccumulationCahe;
		RGISSmallParentDouroOnly()
		{
			UseCount = 1;
		}
		~RGISSmallParentDouroOnly()
		{
		}

		//�Q�Ɖ񐔂�ύX���閽�߂� RGISSmallAccumulationCahe �ȊO�͍s���Ă͐���Ȃ�!
		void Use()
		{
			UseCount ++;
		}
		bool UnUse()
		{
			UseCount --;
			return (UseCount <= 0) ;
		}
	public:
		//�V���{���̉���
		virtual void Parse(unsigned long inBlock ,unsigned char inSimbol ,unsigned long inDataSize ,unsigned long inDataCount ,char* ioBuffer) throw(RException)
		{
			ASSERT(Block == 0 || Block == inBlock);
			Block = inBlock;

			if (inSimbol == 'D')
			{
				Douro.Create(inBlock , inDataSize ,inDataCount ,ioBuffer);
			}
		}

		RGISSmallDouroNode* getDouro() 
		{
			return &Douro;
		}

		//��������������O���[�v�����߂�.
		unsigned long getBlock() const
		{
			return Block;
		}

	private:
		int					UseCount;
		RGISSmallDouroNode	Douro;
		unsigned long		Block;
	};

	//�����炪�����Ƃ����܂ŏ����Ȃ����Ƃ�ۏ؂���ςݗ��Ď��̃L���b�V��.
	class RGISSmallAccumulationCahe
	{
	public:
		typedef vector<RGISSmallParentDouroOnly*>	DouroCache;

		RGISSmallAccumulationCahe()
		{
		}
		virtual ~RGISSmallAccumulationCahe()
		{
			DouroCache::iterator i = Cache.begin();
			DouroCache::iterator e = Cache.end();

			for( ; i != e ; i++)
			{
				delete (*i);
			}
			Cache.clear();
		}

		//�t�@�C���̓ǂݍ��݂� RGISSmallIntelligentCache �Ɉ�C���Ă��邽��.
		void Create(RGISSmallIntelligentCache* ioCache)
		{
			ParentCache = ioCache;
		}

		//�ǂݍ���
		RGISSmallParentDouroOnly* Load(unsigned long inBlock)
		{
			//�L���b�V���ɂ���?
			DouroCache::iterator i = Cache.begin();
			DouroCache::iterator e = Cache.end();
			for( ; i != e ; i++)
			{
				if ( (*i)->getBlock() == inBlock )
				{
					(*i)->Use();
					return *i;
				}
			}

			//�Ȃ�����쐬.
			RGISSmallParentDouroOnly* ret = new RGISSmallParentDouroOnly;
			ParentCache->LoadEx(inBlock , ret);

			Cache.push_back(ret);
			return ret;
		}

		//Load�Ŋm�ۂ������H���A�s�v�ɐ�������ĂԂ���
		void UnLoad(RGISSmallParentDouroOnly* ioDouro)
		{
			if ( ioDouro->UnUse() )
			{
				DouroCache::iterator i = find(Cache.begin() , Cache.end() , ioDouro);
				if (i == Cache.end())
				{
					ASSERT(0);
					return ;
				}
				Cache.erase(i);
				delete *i;
			}
		}
	private:
		RGISSmallIntelligentCache*	ParentCache;

		DouroCache					Cache;
	};


public:
	RGISSmallPathFinder();
	virtual ~RGISSmallPathFinder();

	void Find(const UPOINT * inStart , const UPOINT * inGoal , RGISSmallIntelligentCache* ioCache) throw(RException) ;
private:
	void FindNode(const UPOINT * inPoint);
	UPOINT RGISSmallPathFinder::FindRoute(const UPOINT * inGloalNodeXY);

private:
	RGISSmallAccumulationCahe	Cache;

	int					UseCount;
	RouteTestData	*	Data;	//���H�T���p�̃��[�N�G���A	
								//�X�^�b�N��Ɋm�ۂ���ƃ�����������Ȃ��Ȃ�Ƃ����Ȃ����� new �Ŋm�ۂ��܂�.
};
*/
#endif // !defined(AFX_RGISSMALLPATHFINDER_H__20EB24EB_2DDE_4ED4_806F_5BC79E38E1A5__INCLUDED_)
