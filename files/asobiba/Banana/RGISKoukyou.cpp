// RGISKoukyou.cpp: RGISKoukyou �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISKoukyou.h"
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

RGISKoukyou::RGISKoukyou()
{
	Count = 0;
	Points= NULL;

}

RGISKoukyou::~RGISKoukyou()
{
	Delete();
}


void RGISKoukyou::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISKoukyou::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "KO(ID{KO%d})");
	if (this->Count <= 0) return ;

	this->Points = new KoukyouData[this->Count];
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
		int rsize = sscanf(buffer.c_str() , "KO(ID{KO%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id >= Count || id	 < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		KoukyouData* p = &Points[id];

		//KO(ID{KO000001}SR{84}){NM{���X�x�@���v�ؖ쒓�ݏ�}JS{�͉A485}PT(ID{PT000093}){000093}}
		const char * toOption = strstr(buffer.c_str() , "SR{");
		rsize = sscanf(toOption , "SR{%2s}",
				p->SR.ptr() );
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

		const char * ptStart = strstr(buffer.c_str() , "PT(");
		if (ptStart == NULL)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^��CV�`�����ӂł�!");
		}
		rsize = sscanf(ptStart , "PT(ID{PT%d}){%d}",	&ptID , &pt);
		if (rsize != 2) continue;	//PT���Ȃ��f�[�^�͖���

		p->PT = pt;
		p->Use = true;
	}
}

void RGISKoukyou::SmallPack(const string & inDir,const RGISSlp * inSlp, const RGISNameDatabase* inNameData)  const throw(RException)
{
	string dir = inDir + "KO/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCutKoukyouData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//���O�f�[�^�x�[�X
		string Dir;								//���O�t�@�C�����쐬����ꏊ.

	public:

		unsigned char SR;				//���								1
		unsigned short NameIndex;		//���̃}�b�v�̒���̖��O�ԍ�		2

		RGISCVCutKoukyouData(const RGISNameDatabase* inNameData , const string inDir)
		{
			this->NameData = inNameData;
			this->Dir = inDir;
		}

		//���ꂪ�Ăяo�����.
		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			//����̏ꏊ�ɖ��O���X���C�h.
			RGISNameDatabaseDisk nameDataBase;
			unsigned short newNameIndex = 
				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->NameIndex) );

			//
			ioFile->fwrite(&SR , sizeof(SR));
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));
		}
	};

	RGISCVCutKoukyouData	data(inNameData , namedir);

	RGISCVCut cut;
	UPOINT po;
	for(int n = 0 ; n < Count ; n++)
	{
		const KoukyouData* p = &Points[n];
		if (p->Use)	
		{
			data.SR =  p->SR.OnePack();
			data.NameIndex = (unsigned short)p->NameIndex;
			po = inSlp->getPoint(p->PT - 1);

			cut.Write(dir , &po , &data );
		}
	}
}
JoinTempFormatScript RGISKoukyou::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "KO";
	ret.BlockStartSimbol = 'U';
	ret.DataSize = sizeof(unsigned char);			//SR
	ret.DataSizeSub = sizeof(unsigned short);		//NameIndex
	ret.CVType = CVTypeOnePoint;
	ret.WithNode = false;

	return ret;
}



void RGISKoukyou::test()
{
	try
	{
		RGISChimei dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.ko",&ndb);

		unsigned long r;
		const ChimeiData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 2);
		rr = dk.getPoint(0);

		ASSERT((r = rr->PT) == 000001);
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
