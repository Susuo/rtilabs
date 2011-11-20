// RGISArc.cpp: RGISArc �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISArc.h"
#include "RGIS2500Head.h"
#include "RStdioFile.h"
#include "RBigArray2.h"
#include "RConv.h"



//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISArc::RGISArc()
{
	Count = 0;
	Points= NULL;
}

RGISArc::~RGISArc()
{
	Delete();
}

void RGISArc::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISArc::Create(const string & inFilename, int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	RStdioFile file;
	file.Open(inFilename , "rb");
	FILE * fp = file.getHandle();

	string buffer;
	buffer = file.ReadLine();

	//�w�b�_�[�̎擾.
	RGIS2500Head	head;
	head.Create( buffer.c_str() ,inScale);

	//�v�f�̑����̎擾.
	this->Count = head.getCount();
	this->Points = new ArcData[this->Count];



//	char LineCountArray[MAX_ARC_BUFFER][MAX_ARC_STRING_BUFFER];
	RBigArray2<char> bigarray(MAX_ARC_BUFFER , MAX_ARC_STRING_BUFFER);
	char **LineCountArray = bigarray.get();

	int line = 0;
	for(int i = 0  ; !file.IsEof(); i++ , line ++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		//�ǂݍ���.
		int lineCount;
		ArcData* p = &Points[i];
		int rsize = sscanf(buffer.c_str() , "L%d,%d,%d,%d" , &p->Type , &p->LineTag,&p->No,&lineCount);
		if (rsize != 4)
		{
			delete [] LineCountArray;
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %d �\�[�X[%s]" ,rsize , buffer.c_str() ) );
		}

		if (lineCount >= MAX_ARC_BUFFER)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(line) + "�s�ڂ̃f�[�^�� �A�[�N�̍ő�l" + num2str(MAX_ARC_BUFFER) + "�𒴂���" + num2str(lineCount) + "�ɂȂ�܂���");
		}

		//�A�[�N�̊e���_�f�[�^�̓ǂݍ���.
		for(int n = 0; n < lineCount ; n++ , line ++)
		{
			buffer = file.ReadLine();

			if (buffer.size() >= MAX_ARC_STRING_BUFFER)
			{
				throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(line) + "�s�ڂ̃f�[�^�� �A�[�N�̈�v�f�̒����̍ő�l" + num2str(MAX_ARC_STRING_BUFFER) + "�𒴂���" + num2str(buffer.size()) + "�ɂȂ�܂���");
			}

			strcpy(LineCountArray[n] , buffer.c_str() );
		}

		p->Array.Create(lineCount , (const char **) LineCountArray , &head );
	}
}


void RGISArc::Draw(const RGISDraw * inDraw,const RGIS2500IntArray* inArray , int inType) const
{
	if ( inArray->getCount() <= 0 ) return ;

	int arcCount = 0;
	int	targetArc = abs( inArray->getPoint(arcCount ));
	for( int i = 0; i < Count ; )
	{
		const ArcData* p = &Points[i];
		if ( targetArc == p->No )
		{
			inDraw->Polygon( &p->Array , 1, RGB(255,255,0) );

			arcCount ++;
			if (arcCount >= inArray->getCount()) return ;

			targetArc = abs(inArray->getPoint(arcCount));
			i = 0;
		}
		else	i++;
	}

	//�`��ł��Ȃ����[��������܂���.
	ASSERT(0);
}

