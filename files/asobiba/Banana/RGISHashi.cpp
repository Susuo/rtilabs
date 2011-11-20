// RGISHashi.cpp: RGISHashi �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISHashi.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISHashi::RGISHashi()
{
	Count = 0;
	Points= NULL;

}

RGISHashi::~RGISHashi()
{
	Delete();
}


void RGISHashi::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISHashi::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "HA(ID{HA%d})");
	if (this->Count <= 0) return ;

	this->Points = new HashiData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string nameBuffer;

	string buffer;
	int i;
	//���g�p�t���O�𗧂Ă�.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "HA(ID{HA%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id >= Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		HashiData* p = &Points[id];

		//HA(ID{HA000001}){NM{}DH(IR{DK01101003763})}
		//HA(ID{HA000020}){NM{}TH(IR{TK01101000001})}
		rsize = sscanf(buffer.c_str() , "HA(ID{HA%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		try
		{
			nameBuffer = RGIS25000::GetName(buffer.c_str());
			p->NameIndex = ioNameDatabase->setName(nameBuffer.c_str() );
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^�̌`�����ӂł�! ���̃G���[�͂�������U������܂���:" + e.getMessage() );
		}
		p->Use = true;
	}
}

void RGISHashi::SmallPack(const string & inFilename,const RGISSlp * inSlp)  const throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "wb");

	FILE * fp = file.getHandle();

	//prefix
	//'HA'	HA�Ƃ������� 2byte
	//ver	�o�[�W���� unsigned char
	//count	�f�[�^�̐� ushort

	unsigned char uc;
	unsigned short us;
	unsigned long ul;

	fwrite( "HA" , sizeof(char) , 2 , fp);				//HA
	uc = 1;		fwrite( &uc , sizeof(uc) , 1 , fp);		//ver 1
	us = Count;	fwrite( &us , sizeof(us) , 1 , fp);		//�f�[�^�̐�

	//2bytes
	//name	ushort
	for(int n = 0 ; n < Count ; n++)
	{
		const HashiData* p = &Points[n];
		if (p->Use)	
		{
			us = (unsigned short) p->NameIndex;		fwrite( &us , sizeof(us) , 1 , fp);
		}
	}
}

void RGISHashi::test()
{
	try
	{
		RGISHashi dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.ha",&ndb);

		unsigned long r;
		const HashiData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 2);
		rr = dk.getPoint(0);
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
