// RGISPgn.cpp: RGISPgn �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISPgn.h"
#include "RStdioFile.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RGISNameDatabaseDisk.h"
#include "RBigArray2.h"
#include "RConv.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISPgn::RGISPgn()
{
	Count = 0;
	Points= NULL;
}

RGISPgn::~RGISPgn()
{
	Delete();
}

void RGISPgn::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}

void RGISPgn::Create(const string & inFilename, int inScale) throw(RException)
{
	ASSERT(Points == NULL);

	this->Filename = inFilename;

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
	this->Points = new PgnData[this->Count];

//	char LineCountArray[MAX_PRG_BUFFER][MAX_PRG_STRING_BUFFER];
	RBigArray2<char> bigarray(MAX_PRG_BUFFER , MAX_PRG_STRING_BUFFER);
	char **LineCountArray = bigarray.get();

	int line = 0;
	for(int i = 0  ; !file.IsEof(); i++ , line ++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		//�ǂݍ���.		A1105,261,12,750.0,999.9
		int lineCount;
		double x,y;
		PgnData* p = &Points[i];
		int rsize = sscanf(buffer.c_str() , "A%d,%d,%d,%lf,%lf" , &p->Type , &p->No,&lineCount , &x , &y);
		if (rsize != 5)
		{
			throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );
		}

		if (lineCount >= MAX_PRG_BUFFER)
		{
			throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(line) + "�s�ڂ̃f�[�^�� �|���S���̍ő�l" + num2str(MAX_PRG_BUFFER) + "�𒴂���" + num2str(lineCount) + "�ɂȂ�܂���");
		}

		//�|���S���̊e���_�f�[�^�̓ǂݍ���.
		for(int n = 0; n < lineCount ; n++ , line ++)
		{
			buffer = file.ReadLine();

			if (buffer.size() >= MAX_PRG_STRING_BUFFER)
			{
				throw RException(EXCEPTIONTRACE + inFilename + "�� " + num2str(line) + "�s�ڂ̃f�[�^�� �|���S���̈�v�f�̒����̍ő�l" + num2str(MAX_PRG_STRING_BUFFER) + "�𒴂���" + num2str(buffer.size()) + "�ɂȂ�܂���");
			}

			strcpy(LineCountArray[n] , buffer.c_str() );
		}
		p->Array.Create(lineCount , (const char **)LineCountArray );
		p->Representative = head.convertXY(x,y);
	}
}

void RGISPgn::Draw(const RGISDraw * inDraw , const RGISArc * inArc) const
{
	for(int n = 0 ; n < this->Count ; n++)
	{
		PgnData* p = &Points[n];

		inArc->Draw(inDraw , & p->Array , p->Type);
	}
}

void RGISPgn::SmallPack(const RGISArc * inArc , const RGISAtr * inAtr , 
						const string & inPath  ,const string & inMark,RGISNameDatabase* ioNameDatabase,
						RGIS2500InfoVector* gis2500infoVector)
{
	string dir = inPath + inMark + "/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inPath + "NX/";
	RDiskUtil::MakeDirectory(namedir);


	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCut2500Data  : public RGISCVCutDataIml
	{
	private:
		const RGISNameDatabase* NameData;		//���O�f�[�^�x�[�X
		string Dir;								//���O�t�@�C�����쐬����ꏊ.

	public:
		string filename;

		unsigned short NameIndex;		//���̃}�b�v�̒���̖��O�ԍ�						2

		RGISCVCut2500Data(const RGISNameDatabase* inNameData , const string inDir )
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
			ioFile->fwrite(&newNameIndex , sizeof(newNameIndex));

		}
	};

	RGISCVCut2500Data	data(ioNameDatabase , namedir);

	data.filename = this->Filename;

	RGISCVCut cut;
	for(int n = 0 ; n < this->Count ; n++)
	{
		const PgnData* p = &Points[n];

		const AtrData* atr = NULL;
		///���̃|���S���ɊY������ atr ��T��
		{
			int atrCountMax = inAtr->getCount();
			for(int m = 0 ; m < atrCountMax ; m++ )
			{
				const AtrData* temp = inAtr->getPoint(m);
				if ( temp->No == p->No )
				{
					atr = temp;
					break;
				}
			}
		}

		string name ;
		if (atr != NULL && !atr->attrib[1].empty() )
		{
			try
			{
				data.NameIndex = ioNameDatabase->setName( atr->attrib[1].c_str() );
			}
			catch(RException e)
			{
				throw RException(EXCEPTIONTRACE + PrintFormat("setName�����s���܂��� %s [%s]:\n" , this->Filename.c_str() , atr->attrib[1].c_str() ) + e.getMessage() );
			}
			name = atr->attrib[1];
		}
		else
		{
			data.NameIndex = 0;
			name = "";
		}

		const RGIS2500IntArray*	pgnArray = &p->Array;
		int						pgnType = p->Type;

		//�|���S����񂪂Ȃ��ꍇ�͖v.
		if (pgnArray->getCount() <= 0)
		{
			continue;
		}

		//���ݏ��������A�[�N��
		int arcCountMax = inArc->getCount();
		int arcCount = 0;
		int	targetArc = abs( pgnArray->getPoint(arcCount ));
		for( int i = 0; i < arcCountMax ; )
		{
			const ArcData* arc = inArc->getPoint(i);
			if ( targetArc == arc->No )
			{
				cut.Write(dir , &arc->Array , &data );

				gis2500infoVector->push_back( new RGIS2500Info(&arc->Array , name) );

				//���̃A�[�N�ōŌ�?
				arcCount ++;
				if (arcCount >= pgnArray->getCount()) break ;

				targetArc = abs(pgnArray->getPoint(arcCount));
				i = 0;
			}
			else	i++;
		}
		
	}
}


void RGISPgn::SmallPackNoWrite(const RGISArc * inArc , const RGISAtr * inAtr , RGIS2500InfoVector* gis2500infoVector)
{
	for(int n = 0 ; n < this->Count ; n++)
	{
		const PgnData* p = &Points[n];

		const AtrData* atr = NULL;
		///���̃|���S���ɊY������ atr ��T��
		{
			int atrCountMax = inAtr->getCount();
			for(int m = 0 ; m < atrCountMax ; m++ )
			{
				const AtrData* temp = inAtr->getPoint(m);
				if ( temp->No == p->No )
				{
					atr = temp;
					break;
				}
			}
		}

		string name ;
		if (atr != NULL && !atr->attrib[1].empty() )
		{
			name = atr->attrib[1];
		}
		else
		{
			name = "";
		}

		const RGIS2500IntArray*	pgnArray = &p->Array;
		int						pgnType = p->Type;

		//�|���S����񂪂Ȃ��ꍇ�͖v.
		if (pgnArray->getCount() <= 0)
		{
			continue;
		}

		//���ݏ��������A�[�N��
		int arcCountMax = inArc->getCount();
		int arcCount = 0;
		int	targetArc = abs( pgnArray->getPoint(arcCount ));
		for( int i = 0; i < arcCountMax ; )
		{
			const ArcData* arc = inArc->getPoint(i);
			if ( targetArc == arc->No )
			{
				gis2500infoVector->push_back( new RGIS2500Info(&arc->Array , name) );

				//���̃A�[�N�ōŌ�?
				arcCount ++;
				if (arcCount >= pgnArray->getCount()) break ;

				targetArc = abs(pgnArray->getPoint(arcCount));
				i = 0;
			}
			else	i++;
		}
		
	}
}
