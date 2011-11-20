// RGISTetudo.cpp: RGISTetudo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISTetudo.h"
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


RGISTetudo::RGISTetudo()
{
	Count = 0;
	Points= NULL;

}

RGISTetudo::~RGISTetudo()
{
	Delete();
}


void RGISTetudo::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISTetudo::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

	this->Count = RGIS25000::GetMaxID(inFilename , "TK(ID{TK%d})");
	if (this->Count <= 0) return ;

	this->Points = new TetudoData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string nameBuffer;
	int cvID;
	char cvBuffer[10240];
	int sr;
	int dummy;

	string buffer;
	unsigned int i;
	//���g�p�t���O�𗧂Ă�.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		cvBuffer[0] = '\0';

		int id;

		int rsize = sscanf(buffer.c_str() , "TK(ID{TK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id >= (int)Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		TetudoData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "JT{");
		rsize = sscanf(toOption, "JT{%2s}SB{%2s}",
				 p->JT.ptr() ,  p->SB.ptr()  );
		if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		//�w���
		const char * erStart = strstr(buffer.c_str() , "ER(");
		if (erStart == NULL)
		{
			p->ER = 0xffff;
		}
		else
		{
			rsize = sscanf(erStart , "ER(IR{EK%5d%d})",	&dummy , &sr );
			if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

			p->ER = sr;
		}
		//�g���l�����
		const char * trStart = strstr(buffer.c_str() , "TR(");
		if (trStart == NULL)
		{
			p->TR = 0xffff;
		}
		else
		{
			rsize = sscanf(trStart , "TR(IR{TO%5d%d})", &dummy , &sr);
			if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

			p->TR = sr;
		}
		//�����
		const char * hrStart = strstr(buffer.c_str() , "HR(");
		if (hrStart == NULL)
		{
			p->HR = 0xffff;
		}
		else
		{
			rsize = sscanf(hrStart , "HR(IR{HA%5d%d})", &dummy , &sr);
			if (rsize != 2) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

			p->HR = sr;
		}

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



void RGISTetudo::Draw(const RGISDraw * inDraw) const
{
}



void RGISTetudo::SmallPack(const string & inDir,const RGISSlp * inSlp,
						  const RGISNameDatabase* inNameData,
						  const RGISEki * inEki, RGISFindHintEki* ioFindHintEki,
						  const RGISHashi * inHashi,const RGISTonneru * inTonneru)  const throw(RException)
{
	string dir = inDir + "TK/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCutTetudoData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//���O�f�[�^�x�[�X
		string Dir;								//���O�t�@�C�����쐬����ꏊ.

	public:

		unsigned short NameIndex;		//���̃}�b�v�̒���̖��O�ԍ�						2
		unsigned char SB;				//���												1
		unsigned short ER;				//�w�̖��O�@�w�ł͂Ȃ��ꍇ 0xffff .					2
		unsigned short HR;				//���̏��	���ł͂Ȃ��ꍇ 0xffff .					2
		unsigned short TR;				//�g���l���̏��	�g���l���ł͂Ȃ��ꍇ 0xffff .	2

		RGISCVCutTetudoData(const RGISNameDatabase* inNameData , const string inDir)
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
			unsigned short newEkiNameIndex = this->ER == 0xffff ? 0xffff :
				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->ER) );
			unsigned short newHashiNameIndex = this->HR == 0xffff ? 0xffff :
				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->HR) );
			unsigned short newTonneruNameIndex = this->TR == 0xffff ? 0xffff :
				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->TR) );

			//main
			ioFile->fwrite(&SB , sizeof(SB));									//1
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));				//2
			ioFile->fwrite(&newEkiNameIndex , sizeof(newEkiNameIndex));			//2
//			ioFile->fwrite(&newHashiNameIndex , sizeof(newHashiNameIndex));		//2
//			ioFile->fwrite(&newTonneruNameIndex , sizeof(newTonneruNameIndex));	//2
		}
	};

	RGISCVCutTetudoData	data(inNameData , namedir );

	RGISCvLight light;
	RGISCVCut cut;
	for(unsigned int n = 0 ; n < Count ; n++)
	{
		const TetudoData* p = &Points[n];
		if (p->Use)	
		{
			data.SB =  p->SB.OnePack();
			data.ER =  p->ER;
			data.ER = p->ER == 0xffff ? 0xffff : inEki->getPoint(p->ER - 1)->NameIndex;
			data.HR = p->HR == 0xffff ? 0xffff : inHashi->getPoint(p->HR - 1)->NameIndex;
			data.TR = p->TR == 0xffff ? 0xffff : inTonneru->getPoint(p->TR - 1)->NameIndex;
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

			//�w��������A�w�̌����q���g������.
			if ( p->ER != 0xffff )
			{
				UPOINT po = inSlp->getPoint( p->CV.getPoint(0) - 1 );
				unsigned long block = cut.selectBlock( &po ) ;
				unsigned short ekiNameIndex = inEki->getPoint(p->ER - 1)->NameIndex;
				string name = inNameData->getName(ekiNameIndex);

				RGISNameDatabaseDisk nameDataBase;
				unsigned short newEkiNameIndex = nameDataBase.Write(namedir + num2str(block)  , name );

				if ( !name.empty() )
				{
					ioFindHintEki->Create(block , name.c_str() , newEkiNameIndex );
				}
			}
		}
	}
}

JoinTempFormatScript RGISTetudo::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "TK";
	ret.BlockStartSimbol = 'T';
//	ret.DataSize = sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned char);	//2+2+2+2+1	=	9
	ret.DataSize = sizeof(unsigned char) + sizeof(unsigned short) + sizeof(unsigned short);	//SB + newNameIndex + newEkiNameIndex
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;
	ret.WithNode = false;

	return ret;
}

void RGISTetudo::test()
{
	try
	{
		//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
		//�e�X�g���[�`���͓������܂���
		/*
		RGISTetudo dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.tk",&ndb);

		unsigned long r;
		const TetudoData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 4);
		rr = dk.getPoint(0);

		ASSERT( (r = rr->CV.getPoint(0)) == 16984);
		ASSERT( (r = rr->CV.getPoint(1)) == 16981);
		ASSERT( (r = rr->CV.getPoint(2)) == 16980);
		ASSERT( (r = rr->CV.getPoint(3)) == 16978);
		ASSERT( (r = rr->CV.getPoint(4)) == 16977);
		ASSERT( (r = rr->CV.getPoint(5)) == 16975);
		ASSERT( (r = rr->CV.getPoint(6)) == 16974);
		ASSERT( (r = rr->CV.getPoint(7)) == 16973);
		ASSERT( (rrr = rr->SB) == "43");
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
