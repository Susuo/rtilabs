// RGISKawa.cpp: RGISKawa �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISKawa.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RGISNameDatabaseDisk.h"
#include "RGISCvLight.h"
#include "RConv.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISKawa::RGISKawa()
{
	Count = 0;
	Points= NULL;

}

RGISKawa::~RGISKawa()
{
	Delete();
}


void RGISKawa::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISKawa::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "KK(ID{KK%d})");
	if (this->Count <= 0) return ;

	this->Points = new KawaData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string nameBuffer;
	int cvID;
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

		int id;
		int rsize = sscanf(buffer.c_str() , "KK(ID{KK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id >= Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		KawaData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "SB{");
		rsize = sscanf(toOption  , "SB{%2s}",
				 p->SB.ptr()  );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );


		const char * cvStart = strstr(buffer.c_str() , "CV(");
		if (cvStart == NULL)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^��CV�`�����ӂł�!");
		}
		rsize = sscanf(cvStart , "CV(ID{CV%d}){%10240[^}]}",				&cvID , cvBuffer);
		if (rsize != 2) continue;	//CV���Ȃ��f�[�^�͖���.

		try
		{
			nameBuffer = RGIS25000::GetName(buffer.c_str());
			p->NameIndex = ioNameDatabase->setName(nameBuffer.c_str() );
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^�̌`�����ӂł�! ���̃G���[�͂�������U������܂���:" + e.getMessage() );
		}

		p->CV.Create(cvBuffer);
		p->Use = true;
	}
}


void RGISKawa::Draw(const RGISDraw * inDraw) const
{
}

void RGISKawa::SmallPack(const string & inDir,const RGISSlp * inSlp,
						  const RGISNameDatabase* inNameData)  const throw(RException)
{
	string dir = inDir + "KK/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCutKawaData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//���O�f�[�^�x�[�X
		string Dir;								//���O�t�@�C�����쐬����ꏊ.

	public:

		unsigned short NameIndex;		//���̃}�b�v�̒���̖��O�ԍ�			2
		unsigned char SB;				//���									1

		RGISCVCutKawaData(const RGISNameDatabase* inNameData , const string inDir)
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

			//main
			ioFile->fwrite(&SB , sizeof(SB));
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));
		}
	};

	RGISCVCutKawaData	data(inNameData , namedir);

	RGISCvLight light;
	RGISCVCut cut;
	for(int n = 0 ; n < Count ; n++)
	{
		const KawaData* p = &Points[n];
		if (p->Use)	
		{
			data.SB =  p->SB.OnePack();
			data.NameIndex = (unsigned short)p->NameIndex;

			GISINTArray lightCV;
			if ( light.DoLight( inSlp , &p->CV , &lightCV) )
			{
				cut.Write(dir , inSlp , &lightCV , &data );
			}
			else
			{
				cut.Write(dir , inSlp , &p->CV , &data );
			}
		}
	}
}

JoinTempFormatScript RGISKawa::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "KK";
	ret.BlockStartSimbol = 'K';
	ret.DataSize = sizeof(unsigned char) + sizeof(unsigned short);	//SB + NameIndex
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;
	ret.WithNode = false;

	return ret;
}


void RGISKawa::test()
{
	try
	{
		//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
		//�e�X�g���[�`���͓������܂���
		/*
		RGISKawa dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.kk",&ndb);

		unsigned long r;
		const KawaData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 4);
		rr = dk.getPoint(0);

		ASSERT( (r = rr->CV.getPoint(0)) == 18340);
		ASSERT( (r = rr->CV.getPoint(1)) == 18341);
		ASSERT( (r = rr->CV.getPoint(2)) == 18342);
		ASSERT( (rrr = rr->SB) == "55");
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
