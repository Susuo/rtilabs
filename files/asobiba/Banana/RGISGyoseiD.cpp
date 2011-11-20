// RGISGyoseiD.cpp: RGISGyoseiD �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISGyoseiD.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISGyoseiD::RGISGyoseiD()
{
	Count = 0;
	Points= NULL;
	Code = 0;

}

RGISGyoseiD::~RGISGyoseiD()
{
	Delete();
}


void RGISGyoseiD::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISGyoseiD::Create(const string & inFilename , unsigned long inCode) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "GD(ID{GD%d})");
	if (this->Count <= 0) return ;

	this->Code = inCode;

	this->Points = new GyoseiDData[this->Count];
	memset(this->Points , 0 , sizeof(ChimeiData) * this->Count);

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string nameBuffer;
	char cvBuffer[10240];

	string buffer;
	int i;
	//���g�p�t���O�𗧂Ă�.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		cvBuffer[0] = '\0';

		int id,ptID,pt;
		const char * cmMain = strstr(buffer.c_str() , "GD(");
		if (cmMain == NULL)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("�ǂݍ��݂Ɏ��s���܂��� %s �\�[�X[%s]" ,inFilename.c_str() ,  buffer.c_str()  ) );
		}
		int rsize = sscanf(cmMain , "GD(ID{GD%d})",	&id );
		if (rsize != 1)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );
		}

		id = id -1;	//1����n�܂邽��.
		if (id >= Count || id	 < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		GyoseiDData* p = &Points[id];

		const char * ptStart = strstr(buffer.c_str() , "PT(");
		if (ptStart == NULL)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^��CV�`�����ӂł�!");
		}
		rsize = sscanf(ptStart , "PT(ID{PT%d}){%d}",	&ptID , &pt);
		if (rsize != 2) continue;
		p->PT = pt;

		try
		{
			nameBuffer = RGIS25000::GetName(buffer.c_str());
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^�̌`�����ӂł�! ���̃G���[�͂�������U������܂���:" + e.getMessage() );
		}

		p->Name = nameBuffer;
		p->Use = true;
	}
}


void RGISGyoseiD::SmallPack(const string & inPath , RGISFindCityToNumberConvert * ioFindCityToNumberConvert ) const throw(RException)
{
	for(int n = 0 ; n < Count ; n++)
	{
		const GyoseiDData* p = &Points[n];
		if (p->Use)	
		{
			ioFindCityToNumberConvert->Create(p->Name , this->Code);
		}
	}
}

