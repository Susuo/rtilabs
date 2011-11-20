// RGISNode.cpp: RGISNode �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISNode.h"
#include "RGIS25000.h"
#include "RDiskUtil.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


RGISNodeIntArray::RGISNodeIntArray()
{
	this->Count = 0;
	this->Points = NULL;
}

RGISNodeIntArray::~RGISNodeIntArray()
{
	Delete();
}

void RGISNodeIntArray::Create(const char* inStr) throw (RException)
{
	ASSERT(this->Points == NULL);

	string findStr = "BD(IR{ND";

	//��������̂������܂��B
	int i;
	int count;
	const char * p = inStr;

	for(count = 0 ; p != NULL; count++)
	{
		p = strstr(p , findStr.c_str() );
		if (p == NULL) break;

		p += findStr.size();
	}
	if (count <= 0) return;

	//���킩�����̂Ń������m��.
	this->Count = count;
	this->Points = new unsigned long[count];


	//�ϊ����Ȃ���ǉ�.
	int dummy;
	int nd;
	p = inStr;
	for(i = 0 ; i < count; i++)
	{
		p = strstr(p , findStr.c_str() );
		ASSERT(p != NULL);

		int rsize = sscanf(p , "BD(IR{ND%5d%d})", &dummy,	&nd );
		if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %d �\�[�X[%s]" ,rsize , inStr  ) );

		this->Points[i] = nd;

		p += findStr.size();
	}

}

void RGISNodeIntArray::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISNodeIntArray::test()
{
	//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
	//�e�X�g���[�`���͓������܂���
	/*
	unsigned long r;
	/// @test BD(IR{ND ��F�����Đ������p�[�X�ł��邩?
	try
	{
		string src = "���y�n���@�̗������Ȃ��̂Ńf�[�^�͍폜���܂���";

		RGISNodeIntArray ndArray;
		ndArray.Create( src.c_str() );

		ASSERT( (r = ndArray.getCount() ) == 2);
		ASSERT( (r = ndArray.getPoint(0) ) == 2407);
		ASSERT( (r = ndArray.getPoint(1) ) == 2461);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
	/// @test BD(IR{ND ���܂������Ȃ����n���Ă���������?
	try
	{
		string src = "���y�n���@�̗������Ȃ��̂Ńf�[�^�͍폜���܂���";

		RGISNodeIntArray ndArray;
		ndArray.Create( src.c_str() );

		ASSERT( (r = ndArray.getCount() ) == 0);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
	*/
}


RGISNode::RGISNode()
{
	this->Count = 0;
	this->Points = NULL;
}

RGISNode::~RGISNode()
{
	Delete();
}

void RGISNode::Create(const string & inFilename) throw (RException)
{
	ASSERT(Points == NULL);

//	TRACE("douro %s\n" , inFilename.c_str() );

	this->Count = RGIS25000::GetMaxID(inFilename , "DS(ID{DS%d})");
	if (this->Count <= 0) return ;

	this->Points = new NodeData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string buffer;
	int i;
	int pt;
	//���g�p�t���O�𗧂Ă�.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "DS(ID{DS%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id >= Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		NodeData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "PT(");
		rsize = sscanf(toOption , "PT(ID{PT%d}",
				&pt );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		p->PT = pt;

		p->Use = true;
	}
}

void RGISNode::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

//�t�@�C���Ƀm�[�h������������.
void RGISNode::WriteToFile(RStdioFile * ioFile ,const RGISNode * inNode , const RGISNodeIntArray* inNodeArray ,
						   const UPOINT * writePointArray ,int inPointCount , const RGISSlp* inSlp,
						   const RGISGyousei* inGyousei) throw(RException)
{
	//�m�[�h���ǂ� CV �ɓ��Ă͂܂�̂������z��.
	//256 �̗��R�́A CV�� 256�ȏ�s���ƕ�������悤�ɂ��Ă��邽��.
	unsigned char NodeCV[256];


	int count = 0;
	for(int i = 0; i < inPointCount ; i++)
	{
		const UPOINT * srcPoint = &writePointArray[i];

		//�m�[�h���� srcPoint ���L�ڂ���Ă���� �m�[�h�Ƃ��ď������܂��B
		int nodeArrayCount = inNodeArray->getCount();
		for(int n = 0 ; n < nodeArrayCount ; n++)
		{
			int nodeAS = inNodeArray->getPoint(n) ;
			int pt = inNode->getPoint( nodeAS - 1)->PT;
			UPOINT destPoint = inSlp->getPoint(pt  - 1 );

			//�m�[�h�ɋL�ڂ���Ă����n�_?
			if (srcPoint->x == destPoint.x && srcPoint->y == destPoint.y)
			{
				ASSERT(count < 256);
				ASSERT(i < inPointCount);

				//�܂�A���̃|�C���g�ɑ����Ă��� cv�̗v�f�� �m�[�h�ɓ�����킯�ł��B
				NodeCV[count] = i;

				count++;
			}
		}
	}
	/*
	if (inPointCount > 1)
	{
		//�s���P�ʂŃ}�b�v�͍���Ă��邽�߁A���̒��ɂȂ����Ă��铹�H�Ȃǂ͏����ł��܂���B
		//�����ŁA�s���̕����肬��ɂ����Ă��铹�H�̓m�[�h�Ƃ��Ă��邵�𒅂��܂��B
		//�܂�A�m�[�h�Ƃ��Ă��邵�͂��Ă��邯�ǁA���̓m�[�h�ł͂Ȃ����H���ł�\��������܂��B
		//�����̓f�R�[�_�[������΂��Ă�������(w
		int gc = inGyousei->getCount();
		for(i = 0 ; i < gc ; i ++)
		{
			const GISINTArray * gcv = &((inGyousei->getPoint(i))->CV);
			int gcvc = gcv->getCount();
			for(int n = 0 ; n < gcvc ; n++)
			{
				int pt = gcv->getPoint(n);
				UPOINT destPoint = inSlp->getPoint(pt  - 1 );

				const UPOINT * startPoint = &writePointArray[0];
				const UPOINT * endPoint = &writePointArray[ inPointCount - 1 ];

				//�m�[�h�ɋL�ڂ���Ă����n�_?
				if (startPoint->x == destPoint.x && startPoint->y == destPoint.y)
				{
					ASSERT(count < 256);
					NodeCV[count] = 0;	//�ŏ��̓_
					count++;
				}
				else if (endPoint->x == destPoint.x && endPoint->y == destPoint.y)
				{
					ASSERT(count < 256);
					NodeCV[count] = inPointCount - 1;	//�Ō�̓_
					count++;
				}
			}
		}
	}
	*/

	ASSERT(count < 256);

	//���ʂ��t�@�C���ɏ�������.

	//�܂��A�����������݂܂��B
	unsigned char size = count;
	ioFile->fwrite( &size , sizeof(size) );

	//���ʂ� 0��������A�f�[�^���������܂��ɏI���܂��B
	if (size == 0)	return ;

	//���ʂ̏�������.
	ioFile->fwrite( NodeCV , sizeof(unsigned char) , size);


}



void RGISNode::test()
{
		//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
		//�e�X�g���[�`���͓������܂���
/*
	unsigned long r;
	/// @test DS ��F�����Đ������p�[�X�ł��邩?
	try
	{
		RGISNode node;
		node.Create( "test/test.ds" );

		ASSERT( (r = node.getCount() ) == 2);
		ASSERT( (r = node.getPoint(0)->PT ) == 350);
		ASSERT( (r = node.getPoint(1)->PT ) == 357);
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
	/// @test�m�[�h�𐳂����ۑ��ł��邩?
	try
	{
		RGISSlm		slm;
		RGISSlp		slp;
		RGISDouro	douro;
		RGISNode	node;
		RGISHashi	hashi;
		RGISTonneru	tonneru;
		RGISNameDatabase ndb;
		RGISGyousei gyousei;

		slp.Create("test/node/test.slp" , &slm);

		douro.Create("test/node/test.dk" , &ndb);
		node.Create("test/node/test.ds");

		RDiskUtil::Delete("test/node_pack");
		RDiskUtil::MakeDirectory("test/node_pack");
		douro.SmallPack("test/node_pack/", &slp , &ndb , &gyousei , &node , &hashi , &tonneru );

		RStdioFile file;
		file.Open("test/node_pack/DK/0","rb");

		{
			//�f�[�^������ǂݔ�΂�.
			file.fseek( douro.getPackInfo().DataSize , SEEK_CUR);
			file.fseek( douro.getPackInfo().DataSizeSub , SEEK_CUR);

			//�ŏ��� node �̕���.
			unsigned char ndCound;
			unsigned char nnn[256];
			file.fread(&ndCound , sizeof(ndCound) );
			ASSERT(ndCound == 1);
			file.fread(nnn , sizeof(unsigned char) , ndCound);
			ASSERT(nnn[0] == 1);

			//CV������ǂݔ�΂�.
			unsigned char cvCound;
			file.fread(&cvCound , sizeof(cvCound) );
			file.fseek( sizeof(unsigned short) * 2, SEEK_CUR);
			file.fseek( (cvCound - 1)* sizeof(unsigned char) * 2, SEEK_CUR);
		}
		{
			//�f�[�^������ǂݔ�΂�.
			file.fseek( douro.getPackInfo().DataSize , SEEK_CUR);
			file.fseek( douro.getPackInfo().DataSizeSub , SEEK_CUR);

			//���� node �̕���.
			unsigned char ndCound;
			unsigned char nnn[256];
			file.fread(&ndCound , sizeof(ndCound) );
			ASSERT(ndCound == 1);
			file.fread(nnn , sizeof(unsigned char) , ndCound);
			ASSERT(nnn[0] == 1);

			//CV������ǂݔ�΂�.
			unsigned char cvCound;
			file.fread(&cvCound , sizeof(cvCound) );
			file.fseek( cvCound  * sizeof(unsigned short) * 2, SEEK_CUR);
		}
	}
	catch(RException e)
	{
		TRACE(e.getMessage() );
		ASSERT(0);
	}
*/
}

