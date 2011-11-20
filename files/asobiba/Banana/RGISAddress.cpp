// RGISAddress.cpp: RGISAddress �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISAddress.h"
#include "RStdioFile.h"
#include "Sec2Utm.h"
#include "RFileUtil.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISAddress::RGISAddress()
{
	Count = 0;
	Points= NULL;
}

RGISAddress::~RGISAddress()
{
	Delete();
}

void RGISAddress::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISAddress::Create(const string & inFilename,int inCode , RGISNameDatabase * ioNameDatabase , int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	if (! RDiskUtil::Exist( inFilename ) )
	{
		TRACE(inFilename + "�͂Ȃ��̂ŏȗ����܂�");
		return ;
	}

	RStdioFile file;
	file.Open(inFilename , "rb");

	string buffer;
	buffer = file.ReadLine();

	this->Count = RFileUtil::LineCount(inFilename) - 2;
	if (this->Count <= 0) return ;

	//�v�f�̑����̎擾.
	this->Points = new AddressData[this->Count];

	char TodoufukenBuffer[1024];
	char CityBuffer[1024];
	char MachiBuffer[1024];
	double banshi;	//�Ԓn�� �����_�œ����Ă���������.. 12402JGD---> ��t���̎R���S��Ԕ������̐����K�J�� (�n�}�f�[�^�̃R�s�y�̓C�J���̂ō폜)
	int dummy;
	double dummyF;
	double x,y;

	unsigned int i;
	//���g�p�t���O�𗧂Ă�.
	for( i = 0  ;i < this->Count ; i++)	this->Points[i].Use = false;

	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (buffer == "")
		{
			break;
		}

		//�ǂݍ���.
		AddressData* p = &Points[i];
		int rsize = sscanf(buffer.c_str() , 
			"%1024[^,],%1024[^,],%1024[^,],%lf,%d,%lf,%lf,%lf,%lf,%d,%d" , 
			TodoufukenBuffer,CityBuffer,MachiBuffer,&banshi,&dummy,&dummyF,&dummyF,&y,&x,&dummy,&dummy);
		p->Banchi = (int) banshi;

		if (rsize != 11)
		{
			if (rsize >= 1)
			{
				if ( noQoute(TodoufukenBuffer) == "�s���{����" )
				{
					continue;
				}
			}
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str() ) );
		}

//		p->Todoufuken = noQoute(TodoufukenBuffer);
//		p->City = noQoute(CityBuffer);
		p->Machi = noQoute(MachiBuffer);
		
		try
		{
			p->NameIndex = ioNameDatabase->setName(p->Machi.c_str() );
		}
		catch(RException e)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("setName�����s���܂��� %s %d�s:\n" ,inFilename.c_str() , i) + e.getMessage() );
		}
		p->Code = inCode;

		RGISKeiidoData xDeg,yDeg;
		xDeg.setDouble(x);
		yDeg.setDouble(y);


		RGISMathZantei zantei;
		zantei.Create(inScale);

		UPOINT xy = zantei.RevKukan25000ToKeiido( &xDeg , &yDeg );
		p->xy =     zantei.ConvertScale(xy.x , xy.y);

		p->Use = true;
//		TRACE("%d Address XY , %I64u %I64u \n" , i, p->xy.x , p->xy.y);

	}
}

// " " �ł������Ă���P��̃N�N�����͂����܂��B
const string RGISAddress::noQoute(const string inStr)
{
	int size = inStr.size() - 2;
	if (size <= 0) return "";

	return inStr.substr(1 , size );
}

//�X�P�[����ύX���A�f�B�X�v���C���W�ɂ��܂��B
void RGISAddress::ConvertDisplayScal(int inShift) throw(RException)
{
	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{
		this->Points[i].xy.x = RGISMathZantei::ConvertDisplayScalX( (unsigned long) (this->Points[i].xy.x) );
		this->Points[i].xy.y = RGISMathZantei::ConvertDisplayScalY( (unsigned long) (this->Points[i].xy.y) );
	}
}


void RGISAddress::Draw(const RGISDraw * inDraw ,const RGISNameDatabase * ioNameDatabase) const
{
	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{
		if (this->Points[i].Use)
			inDraw->TexOut( &this->Points[i].xy , ioNameDatabase->getName( this->Points[i].NameIndex ) , 1, RGB(255,0,0) );
	}
}

void RGISAddress::SmallPack(const string & inDir, const RGISNameDatabase* inNameData, RGISFindHintAddress * ioFindHintAddress)  const throw(RException)
{
	string dir = inDir + "AD/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NA/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCutAddressData  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//���O�f�[�^�x�[�X
		string Dir;								//���O�t�@�C�����쐬����ꏊ.

	public:

		unsigned short Code;			//01010 �̂悤�Ȓ��R�[�h.				2
		unsigned short NameIndex;		//���̃}�b�v�̒���̖��O�ԍ�			2
		unsigned short Banchi;			//�Ԓn									1

		RGISCVCutAddressData(const RGISNameDatabase* inNameData , const string inDir)
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
			ioFile->fwrite(&Code , sizeof(Code));
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));
			//sub
			ioFile->fwrite(&Banchi , sizeof(Banchi));
		}
	};

	RGISCVCutAddressData	data(inNameData , namedir);

	RGISCVCut cut;
	for(unsigned int n = 0 ; n < Count ; n++)
	{
		const AddressData* p = &Points[n];
		if (p->Use)
		{
			data.Code =  p->Code;
			data.NameIndex = (unsigned short)p->NameIndex;
			data.Banchi = p->Banchi;
			cut.Write(dir , &p->xy , &data );

			//�Z���̌����q���g���쐬
			{
				unsigned long block = cut.selectBlock( &p->xy ) ;
				ioFindHintAddress->Create(block , p->Code);
			}

		}
	}
}

JoinTempFormatScript RGISAddress::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "AD";
	ret.BlockStartSimbol = 'A';
	ret.DataSize =  sizeof(unsigned short) + sizeof(unsigned short);	//Code +  newNameIndex
	ret.DataSizeSub =  sizeof(unsigned short);	// Banchi
	ret.CVType = CVTypeOnePoint;
	ret.WithNode = false;

	return ret;
}

