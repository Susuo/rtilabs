// RGISDouro.cpp: RGISDouro �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISDouro.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"
#include "RGISCvLight.h"
#include "RConv.h"



//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISDouro::RGISDouro()
{
	Count = 0;
	Points= NULL;

}

RGISDouro::~RGISDouro()
{
	Delete();
}


void RGISDouro::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISDouro::Create(const string & inFilename,RGISNameDatabase * ioNameDatabase) throw(RException)
{
	ASSERT(Points == NULL);

//	TRACE("douro %s\n" , inFilename.c_str() );

	this->Count = RGIS25000::GetMaxID(inFilename , "DK(ID{DK%d})");
	if (this->Count <= 0) return ;

	this->Points = new DouroData[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	string 	nameBuffer;
	int cvID;
	char cvBuffer[10240];
	int sr;
	int dummy;

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
		int rsize = sscanf(buffer.c_str() , "DK(ID{DK%d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id >= Count || id < 0)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		DouroData* p = &Points[id];

		const char * toOption = strstr(buffer.c_str() , "JT{");
		rsize = sscanf(toOption , "JT{%2s}YU{%2s}SB{%2s}FI{%2s}",
				p->JT.ptr() , p->YU.ptr() , p->SB.ptr() , p->FI.ptr() );
		if (rsize != 4) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		//�g���l�����
		const char * trStart = strstr(buffer.c_str() , "TR(");
		if (trStart == NULL)
		{
			p->TR = 0xffff;
		}
		else
		{
			int rsize = sscanf(trStart , "TR(IR{TO%5d%d})",	&dummy , &sr );
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
			rsize = sscanf(hrStart , "HR(IR{HA%5d%d})", &dummy,	&sr );
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

		const char * ndBuffer = strstr(buffer.c_str() , "BD(IR{ND");

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
		p->Node.Create(ndBuffer);
		p->Use = true;
	}
}

void RGISDouro::Draw(const RGISDraw * inDraw) const
{
}

void RGISDouro::SmallPack(const string & inDir,const RGISSlp * inSlp,
						  const RGISNameDatabase* inNameData,
						  const RGISGyousei * inGyousei , const RGISNode * inNode , 
						  const RGISHashi * inHashi,const RGISTonneru * inTonneru)  const throw(RException)
{
	string dir = inDir + "DK/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCutDouroData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//���O�f�[�^�x�[�X
		string Dir;								//���O�t�@�C�����쐬����ꏊ.
		const RGISSlp*			Slp;		//���_���
		const RGISNode *		Node;		//�m�[�h���
		const RGISGyousei *		Gyousei;	//�s���g

	public:

		unsigned short NameIndex;		//���̃}�b�v�̒���̖��O�ԍ�						2
		unsigned char SB;				//���												1
		unsigned char FI;				//����												1
		unsigned short HR;				//���̏��	���ł͂Ȃ��ꍇ 0xffff .					2
		unsigned short TR;				//�g���l���̏��	�g���l���ł͂Ȃ��ꍇ 0xffff .	2

		const RGISNodeIntArray*	NodeArray;	//�m�[�h

		RGISCVCutDouroData(const RGISNameDatabase* inNameData , const string inDir ,
			const RGISSlp * inSlp , const RGISNode * inNode, const RGISGyousei * inGyousei)
		{
			this->NameData = inNameData;
			this->Dir = inDir;
			this->Slp = inSlp;
			this->Node = inNode;
			this->Gyousei = inGyousei;
		}

		//���ꂪ�Ăяo�����.
		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			//����̏ꏊ�ɖ��O���X���C�h.
			RGISNameDatabaseDisk nameDataBase;
			unsigned short newNameIndex = 
				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->NameIndex) );
//			unsigned short newHashiNameIndex = this->HR == 0xffff ? 0xffff :
//				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->HR) );
//			unsigned short newTonneruNameIndex = this->TR == 0xffff ? 0xffff :
//				nameDataBase.Write(this->Dir + num2str(inBlock)  , NameData->getName(this->TR) );
			//main
			ioFile->fwrite(&SB , sizeof(SB));
			ioFile->fwrite(&FI , sizeof(FI));
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));
//			ioFile->fwrite(&newHashiNameIndex , sizeof(newHashiNameIndex));
//			ioFile->fwrite(&newTonneruNameIndex , sizeof(newTonneruNameIndex));

			//����̓m�[�h�͏����Ȃ�.
			//�܂��A���ē��ł��Ă��Ȃ�����
//			RGISNode::WriteToFile(ioFile ,this->Node ,  NodeArray , writePointArray , inPointCount , Slp , this->Gyousei);
		}
	};

	RGISCVCutDouroData	data(inNameData , namedir , inSlp , inNode , inGyousei);

	RGISCvLight light;
	RGISCVCut cut;
	for(int n = 0 ; n < Count ; n++)
	{
		const DouroData* p = &Points[n];

		if (p->Use)	
		{
			data.SB =  p->SB.OnePack();
			data.FI =  p->FI.OnePack();
			data.NameIndex = (unsigned short)p->NameIndex;

//			if (data.NameIndex > 0)
//			{
//				string aaa = inNameData->getName(data.NameIndex);
//				puts("a");
//			}

//			data.HR = p->HR == 0xffff ? 0xffff : inHashi->getPoint(p->HR - 1)->NameIndex;
//			data.TR = p->TR == 0xffff ? 0xffff : inTonneru->getPoint(p->TR - 1)->NameIndex;
			data.NodeArray = &p->Node;

			GISINTArray lightCV;
			if ( light.DoLightAndNode(inNode , &p->Node , inSlp , &p->CV , &lightCV) )
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

JoinTempFormatScript RGISDouro::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "DK";
	ret.BlockStartSimbol = 'D';
//	ret.DataSize = sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned short) + sizeof(unsigned char);	//2+2+2+1	=	7
	ret.DataSize = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned short);	//SB + FI + NameIndex
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;

	//����̓m�[�h�͏����Ȃ�.
	//�܂��A���ē��ł��Ă��Ȃ�����
//	ret.WithNode = true;
	ret.WithNode = false;

	return ret;
}

void RGISDouro::test()
{
	try
	{
		//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
		//�e�X�g���[�`���͓������܂���
		/*
		RGISDouro dk;
		RGISNameDatabase ndb;
		dk.Create("test/test.dk",&ndb);

		unsigned long r;
		const DouroData*  rr;
		string rrr;
		ASSERT((r = dk.getCount() ) == 6);
		rr = dk.getPoint(0);

		ASSERT( (r = rr->CV.getPoint(0)) == 1091);
		ASSERT( (r = rr->CV.getPoint(1)) == 1112);
		ASSERT( (r = rr->CV.getPoint(2)) == 1117);
		ASSERT( (rrr = rr->SB) == "13");

		ASSERT( (r = rr->Node.getCount()) == 2);
		ASSERT( (r = rr->Node.getPoint(0)) == 35);
		ASSERT( (r = rr->Node.getPoint(1)) == 42);
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}
