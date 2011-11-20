// RGISSmallPathFinder.cpp: RGISSmallPathFinder �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallPathFinder.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
/*

RGISSmallPathFinder::RGISSmallPathFinder()
{
	this->Data = new RouteTestData[RouteTestAreaMax];
	memset(this->Data , 0 , sizeof(RouteTestData) * RouteTestAreaMax);
}

RGISSmallPathFinder::~RGISSmallPathFinder()
{
	delete	[]	this->Data;
}

void RGISSmallPathFinder::Find(const UPOINT * inStart , const UPOINT * inGoal , RGISSmallIntelligentCache* ioCache) throw(RException) 
{
	this->Cache.Create(ioCache);

	int startBlock = RGISSmallType::selectBlock(inPoint);
	int endBlock = RGISSmallType::selectBlock(inGoal);

	const RGISSmallParentDouroOnly* gis ;

	//�Ŋ�̃m�[�h������.
	this->Cache->Load( startBlock , gis);
	this->UseCount = 0;
	UPOINT nodeStart = FindNode(gis, inStart);

	//�Ŋ�̃m�[�h������.
	gis = this->Cache->Load( startBlock );
	UPOINT nodeGoal = FindNode(inGoal);

}


UPOINT RGISSmallPathFinder::FindNode(const RGISSmallParent* inGis , const UPOINT * inPoint)
{
	const RGISSmallDouro* douro = inGis->getDouro();


	int dnumber ;
	unsigned char cvnumber;
	UPOINT*	mostNear = NULL;
	int		distance = MAX_INT;

	for(int i = 0 ; i < douro->getCount() ; i++ )
	{
		const SmallDouroData* data = douro->getPoint(i);
		RGISSmallType* cv = data->CV;

		for(int n = 0; n < cv->getCount ; n++ )
		{
			const UPOINT * p = cv->getPoint(n);
			int diff = Distance(p->x > inStart->x ? p->x - inStart->x : inStart->x - p->x,p->y > inPoint->y ? p->y - inPoint->y : inPoint->y - p->y);

			if ( distance > diff )
			{
				distance = diff;
				mostNear = p;
				dnumber = i;
				cvnumber = n;
			}
		}
	}

	ASSERT(mostNear != NULL);
	ASSERT(UseCount == 0);

	{
		//��������n�܂�܂��B

		//�͂��܂��� cv �̐��̃m�[�h���L�^���܂��B
		const SmallDouroData* data = douro->getPoint(dnumber);
		RGISSmallType* cv = data->CV;
		RGISSmallNode* node = data->Node;

		this->UseCount = 0;

		int n;
		for(n = cvnumber ; n < cv->getCount() ; n++ )
		{
			if (node->Find(n) != node->getCount() ) break;
		} 
		if (n != node->getCount() )
		{
			RouteTestData* data = this->Data[UseCount++] ;
			data->DNumber = dnumber;
			data->xy = *(cv->getPoint(n));
			data->Active = true;
		}

		for(n = cvnumber - 1; n >= 0 ; n-- )
		{
			if (node->Find(n) != node->getCount() ) break;
		} 
		if (n < 0 )
		{
			RouteTestData* data = this->Data[UseCount++] ;
			data->DNumber = dnumber;
			data->xy = *(cv->getPoint(n));
			data->Active = true;
		}
	}
}

UPOINT RGISSmallPathFinder::FindRoute(const UPOINT * inGloalNodeXY , int GloalCount)
{
	RGISSmallParentDouroOnly*  gis;

	while(1)
	{
		//���H����....
		//�����A ��ǂ݉񐔂� LookAheadDependMax �ɂȂ邩�A
		//�r���Ń��[�N������������Ȃ��Ȃ��� RIORouteTestAreaMaxException ���ł�ΒT�����I���܂��B
		for(int i = 0; i < LookAheadDependMax ; i++)
		{
			if ( FindRouteUnit() != OK) break;
		}

		//���̐���̕]��.
		AppraiseNextGenesion():
	}
}

//���̐���̕]��.
void RGISSmallPathFinder::AppraiseNextGenesion()
{
	//�T���������ʂ�]�����āA
	//�S�[���܂� 4�Ԃ܂Ŏ擾���܂�.
	struct Ranking
	{
		int		no;					//�ꏊ
		int		distance;			//�S�[���܂ł̋���
	};
	const int SurvivalProspectCount = 4;
	Ranking rank[SurvivalProspectCount];
	for(int i = 0; ; i < SurvivalProspectCount ; i++)
	{
		rank[i].no = 0;
		rank[i].distance = INT_MAX;
	}

	int i;
	for(i = 0; i < this->UseCount ; i++)
	{
		RouteTestData* rdata = &Data[i];
		//�܏��H�ɂ܂�����͖v.
		if (! rdata->Active )	continue;

		//�X���u���b�N�̓��H�f�[�^�̓ǂݍ���.
		if ( ! this->Cache->Load( rdata->Block , gis) )
		{
			//�ǂݍ��ݎ��s.
			throw RException(EXCEPTIONTRACE + "�L���b�V���ɋL�^����Ă����f�[�^���ǂݍ��݂܂���!");
		}
		//���̃m�[�h�̓_�����߂܂�.
		RGISSmallDouroNode* douro = gis->getDouro();
		SmallDouroNodeData * ddata = douro->getPoint( rdata->DNumber );
		const RGISSmallNode* node = &ddata->Node;

		//����i�݂܂��B
		//�����_������΁A�����f�[�^�ɒǉ����Ă����܂��B
		//���̓��̏I�[�܂ōs������A�����܂��ł��B
		int nodePoint = rdata->nodePoint;
		//���̌����_�̈ʒu���擾���܂�.
		UPOINT * p = ddata->CV.getPoint( node->getPoint(nodePoint) );

		int diff = Distance(p->x > inGloalNodeXY->x ? p->x - inGloalNodeXY->x : inGloalNodeXY->x - p->x,p->y > inGloalNodeXY->y ? p->y - inGloalNodeXY->y : inGloalNodeXY->y - p->y);

		for(int n = 0; n < SurvivalProspectCount ; n++)
		{
			if ( rank[n].distance <= diff) continue;

			//�����������ɑ}������.
			memmove(&rank[n + 1] , &rank[n] , sizeof(Ranking) * ( SurvivalProspectCount - (n + 1)) );
			rank[n].no = i;
			rank[n].distance = diff;
		}
	}
	//�ŁA������������S�����ځ[��.

	//�܂��o�b�N�A�b�v�ɑޔ�����.
	RouteTestData backup[SurvivalProspectCount];
	for(i = 0 ; i < SurvivalProspectCount ; i++)
	{
		backup[i] = Data[ rank[i].no ];
	}
	//������.
	Clear();
	//�����擪�ɋl�߂ď�������.
	for(i = 0 ; i < SurvivalProspectCount ; i++)
	{
		Data[i] = backup[i];
		Data[i].Befor = -1;	//����̃f�[�^���N���A.
	}
	this->UseCount = SurvivalProspectCount;
}

//������.
void RGISSmallPathFinder::Clear()
{
	memset(this->Data , 0 , sizeof(RouteTestData) * RouteTestAreaMax);
}

SearchResult RGISSmallPathFinder::FindRouteUnit(const UPOINT * inGloalNodeXY , int GloalCount)
{
	RGISSmallParentDouroOnly*  gis;
	int count = this->UserCount;
	for(int i = 0; i < count ; i++)
	{
		RouteTestData* rdata = &Data[i]
		//�܏��H�ɂ܂�����͖v.
		if (! rdata->Active )	continue;

		//�X���u���b�N�̓��H�f�[�^�̓ǂݍ���.
		if ( ! this->Cache->Load( rdata->Block , gis) )
		{
			//�ǂݍ��ݎ��s.
			throw RException(EXCEPTIONTRACE + "�L���b�V���ɋL�^����Ă����f�[�^���ǂݍ��݂܂���!");
		}
		//���̃m�[�h�̓_�����߂܂�.
		RGISSmallDouroNode* douro = gis->getDouro();
		SmallDouroNodeData * ddata = douro->getPoint( rdata->DNumber );
		const RGISSmallNode* node = &ddata->Node;

		//����i�݂܂��B
		//�����_������΁A�����f�[�^�ɒǉ����Ă����܂��B
		//���̓��̏I�[�܂ōs������A�����܂��ł��B
		int nodePoint = rdata->nodePoint;
		//���̌����_�̈ʒu���擾���܂�.
		UPOINT * p = ddata->CV.getPoint( node->getPoint(nodePoint) );

		//���̓S�[���������肵�܂�?
		if ( isGoal(p , inGloalNodeXY , GloalCount) )
		{
			return Goal;
		}

		//���̌����_�ɏ������Ă���m�[�h��V���Ɍ����f�[�^�ɒǉ����܂��B
		//�܂�A���̌����_���Ȃ������ꍇ�͂ǂ��Ȃ邩����������킯�ł��B
		if ( ! OtherCross(douro , rdata�@, p ) )
		{
			return NotEnoughMemory;
		}

		//�J�E���^�[�A�b�v / �_�E��.
		if (rdata->Up)
		{
			nodePoint ++;
			//�I�[�܂ōs�����̂ł����܂�.
			if (nodePoint >= node->getCount() ) rdata->Active = false;
		}
		else
		{
			nodePoint --;
			//�I�[�܂ōs�����̂ł����܂�.
			if (nodePoint < 0 ) rdata->Active = false;
		}
	}
}

bool RGISSmallPathFinder::OtherCross(RGISSmallDouroNode* ioDouro ,const RouteTestData* inRdata,const UPOINT * inPoint)
{
	//���̓_�����L���Ă���m�[�h�������܂�.
	const RGISSmallType* cv;
	const RGISSmallNode* node;
	for(int n = 0 ; n < ioDouro->getCount() ; n++)
	{
		SmallDouroNodeData * ddata = ioDouro->getPoint(n);

		//���̓��H���g���Ă���l�����Ȃ����`�F�b�N.
		if ( ddata->AlreadyUse ) continue;

		//�g�p���t���O�𗧂Ă�.
		ddata->AlreadyUse = true;

		cv = &ddata->CV;
		node = &ddata->Node;
		for(int c = 0 ; c < cv->getCount() ; c ++)
		{
			UPOINT * p = cv->getPoint(c);
			if ( !(inPoint->x == p->x && inPoint->y == p->y) ) continue;

			//���L���Ă��邩������Ȃ��̂ŁA�m�[�h�ł��̃m�[�h���O���Ɍ��J���Ă��邩�ǂ������ׂ܂�.
			int nodePoint = node->Find(c)
			if ( ! nodePoint == node->getCount() ) continue;

			//�����_�ł�.
			//up = false		up = true
			//cv1   cv2  cv3  cv4
			// ------------------- �^�[�Q�b�g
			//           |
			//           |
			//           |���̓��H
			//           |

			//�I�[�ȊO�́A�E�ɑ�����
			if (c + 1 != cv->getCount())
			{
				RouteTestData* data = this->Data[UseCount++] ;
				data->Block = inRdata->Block;
				data->DNumber = inRdata->DNumber;
				data->nodePoint nodePoint;
				data->Active = true;
				data->Up = true;
				data->Befor = inRdata->Befor;
			}
			//�擪�ȊO�́A���Ɍ���
			if (c == 0)
			{
				RouteTestData* data = this->Data[UseCount++] ;
				data->Block = inRdata->Block;
				data->DNumber = inRdata->DNumber;
				data->nodePoint nodePoint;
				data->Active = true;
				data->Up = false;
				data->Befor = inRdata->Befor;
			}
			//�����p�̃����������ׂĎg���؂���?.
			if (UseCount >= RouteTestAreaMax)
			{
				return false;
			}
		}
	}
	return true;
}

//���������߂�.
int RGISSmallPathFinder::Distance(unsigned int inA ,unsigned  int inB) const
{
	return sqrt(inA * inA + inB * inB);
}
*/