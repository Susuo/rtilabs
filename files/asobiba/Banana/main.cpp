
#include "comm.h"
#include "WindowsMain.h"
#include "RConv.h"
#include "BAddressMatching.h"
#include "RGISSmallDraw.h"
#include "RGISKoudo.h"
#include "RToken.h"
#include "RGISNMEA0183.h"
#include "RMessageBox.h"
#include "RCreateFile.h"
#include "RGISMath.h"
#include "RNeoIFStreamUtil.h"
#include "RConfigFile.h"
#include "RStringRingBuffer.h"

struct FindNode;
typedef vector<FindNode*>   PtrFindNodes;

struct FindNode
{
	int		NodeNumber;
	int		Cost;
	bool	Stopper;

	FindNode*			Befor;
	PtrFindNodes		Child;
};



struct FindTestNode
{
	int		NodeNumber;

	int		TreeCount;
	struct Node
	{
		int		NodeNumber;
		int		Cost;
	};
	Node	Nodes[10];

};
typedef vector<FindTestNode*> FindTestNodes;


template <class T>	class FindNodeFindFunc
{
public:
	FindNodeFindFunc(int findNodeNumber)
	{
		this->NodeNumber = findNodeNumber;
	}
	bool operator ()(const T& u)
	{
		return u->NodeNumber == this->NodeNumber;
	}
private:
	int		NodeNumber;
};

class FindMotherNodes
{
public:
	FindMotherNodes()
	{
		Node = NULL;

		makeTestNode(0 , 2 , 1 , 20 , 3 , 5);
		makeTestNode(1 , 3 , 1 , 20 , 4 , 10 , 2 , 10);
		makeTestNode(2 , 2 , 1 , 10 , 5 , 10);
		makeTestNode(3 , 3 , 0 , 5  , 4 , 15 , 6 ,  8);
		makeTestNode(4 , 4 , 1 , 10 , 3 , 15 , 5 ,  20 , 7 ,  5);
		makeTestNode(5 , 3 , 2 , 10  ,4 , 20 , 8 ,  20);
		makeTestNode(6 , 2 , 3 , 8   ,7 , 20 );
		makeTestNode(7 , 3 , 6 , 20  ,4 , 5  , 8 ,  5);
		makeTestNode(8 , 2 , 7 , 5   ,5 , 20 );
	}
	~FindMotherNodes()
	{
		if (Node)
		{
			DeleteNode(Node);
		}
		for( FindTestNodes::iterator i = TestNodes.begin() ; i != TestNodes.end() ; i++)
		{
			delete *i;
		}
		
	}
	void makeTestNode(int nodeNumber , int treeCount , ...)
	{
		FindTestNode * p;
		p = new FindTestNode;
		p->NodeNumber = nodeNumber;
		p->TreeCount = treeCount;

		va_list marker;
		va_start(marker, treeCount );

		for(int i = 0 ; i < treeCount ; i++)
		{
			int number = va_arg( marker, int);
			int cost = va_arg( marker, int);

			p->Nodes[i].NodeNumber = number;
			p->Nodes[i].Cost = cost;
		}
		

		TestNodes.push_back(p);

		va_end( marker );
	}


void Create(int start , int goal)
{
	//�J�n�m�[�h��o�^.
	FindNode * node = new FindNode;
	node->NodeNumber = start;
	node->Cost = 0;
	node->Befor = NULL;
	node->Stopper = false;

	Node = node;
	BestNodes.push_back(node);

	PtrFindNodes::iterator bestI = BestNodes.begin() ;
	while( bestI != BestNodes.end() )
	{
		FindNode* activeNode = *bestI;
		if (activeNode->Stopper)
		{
			bestI ++ ;
			continue;
		}

		FindTestNodes::iterator routeNodeI = find_if( TestNodes.begin() , TestNodes.end() , 
			FindNodeFindFunc<FindTestNode*>(activeNode->NodeNumber) );
		if (routeNodeI == TestNodes.end())
		{
			throw RNotFoundException(EXCEPTIONTRACE + PrintFormat("�w�肳�ꂽ�m�[�h %d �͑��݂��܂���" , activeNode->NodeNumber) );
		}
		const FindTestNode * routeNode = *routeNodeI;

		//���̃m�[�h���番��ł���m�[�h���ׂĎ����Ă݂�.
		int treeCount = routeNode->TreeCount;
		for(int i = 0 ; i < treeCount ; i++)
		{
			//�O�̃m�[�h�ɖ߂�Ȃ炾��.
			if ( activeNode->Befor && activeNode->Befor->NodeNumber == routeNode->Nodes[i].NodeNumber )
			{
				continue;
			}
			int number = routeNode->Nodes[i].NodeNumber;
			int cost = activeNode->Cost + routeNode->Nodes[i].Cost;
			//���ɂ��̃m�[�h�Ɉړ�����Ƃ�����ǂꂭ�炢�������?
			//����͍ŒZ�Ȃ�?
			PtrFindNodes::iterator bestI2 = find_if( BestNodes.begin() , BestNodes.end() , 
				FindNodeFindFunc<FindNode*>(number) );
			if (bestI2 == BestNodes.end() )
			{
				//�܂��o�^����Ă��Ȃ��m�[�h
				FindNode * newNode = new FindNode;
				newNode->NodeNumber = number;
				newNode->Cost = cost;
				newNode->Befor = activeNode;
				newNode->Stopper = false;

				activeNode->Child.push_back(newNode);

				//�ŒZ�L�^���X�V.
				BestNodes.push_back(newNode);
			}
			else
			{
				if (cost > (*bestI2)->Cost )
				{
					//�ŒZ�ł͂Ȃ�.
					continue;
				}
				//�ŒZ�L�^���X�V.
				OveraideNode(*bestI2 ,number , cost , activeNode );
			}

		}
		activeNode->Stopper = true;
		bestI = BestNodes.begin();
	}

	PtrFindNodes::iterator bestGoalI = find_if( BestNodes.begin() , BestNodes.end() , 
		FindNodeFindFunc<FindNode*>( goal ) );
	if (bestGoalI == BestNodes.end())
	{
		ASSERT(0);
	}

	FindNode * bestGoal = *bestGoalI;

	return;
}

void OveraideNode(FindNode* oldBest ,int newNodeNumber , int newCost , FindNode* newParent)
{
	//�Â��m�[�h�̐e�ɁA�����͑r�����Ȃ��̎q���ł͂Ȃ��Ɛ≏�錾.
	if ( oldBest->Befor )
	{
		FindNode* parent = oldBest->Befor;
		PtrFindNodes::iterator my = find(parent->Child.begin() , parent->Child.end() , oldBest );
		if (my == parent->Child.end())
		{
			ASSERT(0);
			throw RException(EXCEPTIONTRACE + "�e�q�̊Ǘ������Ă��܂�");
		}
		parent->Child.erase(my);
	}

	int costDiff = oldBest->Cost - newCost;

	//�Â��m�[�h�����㏑��.
	ASSERT(oldBest->NodeNumber == newNodeNumber);
	oldBest->NodeNumber = newNodeNumber;
	oldBest->Cost = newCost;
	oldBest->Befor = newParent;
	oldBest->Stopper = false;

	//�V�����e�ɓo�^.
	newParent->Child.push_back(oldBest);

	//���̃m�[�h�̎q�����F�E��.
	for( PtrFindNodes::iterator i = oldBest->Child.begin() ; i != oldBest->Child.end() ; i++)
	{
		DeleteNode(*i);
	}
	oldBest->Child.clear();
}

void DeleteNode(FindNode* node)
{
	for( PtrFindNodes::iterator i = node->Child.begin() ; i != node->Child.end() ; i++)
	{
		DeleteNode(*i);
	}
	delete node;
}
	//�m�[�h�f�[�^
	FindNode*				Node;
	//����m�[�h�܂ł̍ŒZ�o�H
	PtrFindNodes			BestNodes;

	//�e�X�g�f�[�^
	FindTestNodes	TestNodes;


};



//VC�� consol �A�v���� windows�A�v���̈Ⴂ�́A linker �ɓn���I�v�V������
///subsystem:consol
//��
///subsystem:windows 
//
//�ŁA����B�ւ��[�ւ��[�ւ��[�ւ��[
// main �ɂ��邩 WinMain �ɂ��邩���߂Ă���悤��
//
//�ꉞ�A �v���v���Z�b�T�� _CONSOLE �������Ƃ����ق��������Ǝv�����ǁB
#if (! (_WIN32_WCE || VIEWONLY))
	int main()
#else
	int WINAPI WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance,  TCHAR* lpCmdLine, int nCmdShow)
#endif
{
	int ret = -1;

	FindMotherNodes ma;
	ma.Create(0 , 8);

#ifndef NOTEST
	RStringRingBuffer::test();
	RConfigFile::test();
	RGISKeiidoData::test();
	RGISMath::test();
	RGISNMEA0183::test();
	RGraphics::test();
	RStream::test();
	GISType::test();
	GISINTArray::test();
	RGISDouro::test();
	RGISTetudo::test();
	RGISKawa::test();
	RGISSuiikiKai::test();
	RGISEki::test();
	RGISGyousei::test();
	RGISChimei::test();
	RGISNode::test();
	RGISSlp::test();
	RGIS2500Head::test();
	RGISSlm::test();
	RGISCVCut::test();
	RGISNameDatabaseDisk::test();
	RGISNodeIntArray::test();
	BAddressParse::test();
	RFileUtil::test();
	//�e�X�g.
	RConv::test();
	RToken::test();
	RZlib::test();
	RGISSmallDraw::test();
	RNeoIFStreamUtil::test();
#endif

	try
	{
		//�N��...
		WindowsMain* winMain = WindowsMain::getMainSystem();
		ret = winMain->Run();
	}
	catch(RException e)
	{
		TRACE( e.getMessage() );
		RMessageBox::Message( e.getMessage() );
		ASSERT(0);

	}
	catch(std::bad_alloc)
	{
		TRACE( "BAD alloc!" );
		RMessageBox::Message( "BAD alloc!" );
		ASSERT(0);
	}
	catch(...)
	{
		TRACE( "���m�̃G���[" );
		RMessageBox::Message( "���m�̃G���[" );
		ASSERT(0);
	}

#ifndef _WIN32_WCE
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
#endif
	return ret;
}

