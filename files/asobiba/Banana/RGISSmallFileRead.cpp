// RGISSmallFileRead.cpp: RGISSmallFileRead �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallFileRead.h"
#include "RStdioMemory.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallFileRead::RGISSmallFileRead()
{
	this->Index = NULL;
}

RGISSmallFileRead::~RGISSmallFileRead()
{
	Delete();
}

void RGISSmallFileRead::Delete()
{
	if (Index != NULL)
	{
		delete [] this->Index;
		this->Index = NULL;
	}

	this->File.Close();
}

//WinCE �ȂǂŁA�T�X�y���h���ĕ��A����ƁA
//�t�@�C���n���h���������ɂȂ錻�ۂ�����B
//�����������邽�߂ɁA�����n���h���������ɂȂ��Ă�����A
//�w�肵���t�@�C�����J���Ȃ������ƂőΏ�����.
void RGISSmallFileRead::KeepHanlde() throw(RException)
{
	File.KeepHanlde(this->Filename , "rb");
	if ( this->FileSize != this->File.getSize() )
	{
		throw RException(EXCEPTIONTRACE + "���A�������t�@�C���̃T�C�Y���ȑO�ƈႢ�܂��B");
	}
}


void RGISSmallFileRead::Create(const string & inFilename , const string & inUserDataPath) throw(RException)
{
	Delete();

	//ZLIB����!
	Zlib.Create();
	//���[�U�f�[�^�Ǘ���������.
	UserDataFile.Create(inUserDataPath);

	//�t�@�C�����J����.
	this->File.Open(inFilename , "rb");
	this->FileSize = this->File.getSize();
	this->Filename = inFilename;

	char buffer[4];
	//�w�b�_�[�̓ǂݍ���
	this->File.fread( buffer , 4 );
	if (!(buffer[0] == 'K' && buffer[1] == 'M' && buffer[2] == 'A' && buffer[3] == 'P'))
	{
		throw RException(EXCEPTIONTRACE + "�擪��KMAP�̃v���t�B�b�N�X������܂���");
	}

	//�o�[�W�����̓ǂݍ���.
	unsigned char version;
	this->File.fread( &version , sizeof(version) );
	if (version != 1)
	{
		throw RException(EXCEPTIONTRACE + PrintFormat("���m�̃o�[�W�����ł� %d" , (int)version) );
	}

	//index���̓ǂݍ���
	unsigned long indexCount = 0;
	this->File.fread( &indexCount , sizeof(indexCount) );

	//index�̓ǂݍ���.
	Index = new IndexArea[indexCount];
	this->File.fread( Index , sizeof(IndexArea) * indexCount);

	//�ڎ��̒������L�^.
	this->IndexCount = indexCount;

	//�w�b�_�[�̒�����ǂ�
	unsigned long headerSize = 0;
	this->File.fread( &headerSize , sizeof(headerSize) );
	this->HeaderSize = headerSize;

	//�w�b�_�[�̈ʒu���L�^.
	this->HeaderPos = File.ftell();
}

bool RGISSmallFileRead::Load(unsigned long inBlock , RGISSmallParentImp * ioParent)
{
	//�w�肳�ꂽ�u���b�N�̌���.
	int i = 0;
	for( ; i < this->IndexCount ; i++)
	{
		if ( this->Index[i].Block == inBlock )
			break;
	}
	//������Ȃ�����?
	if ( i == this->IndexCount )
	{
		return false;
	}

	//�u���b�N���̌v�Z.		���̃u���b�N�̊J�n�ʒu�������Ό����̂ł��B
	unsigned long blockStart = this->Index[i].Possition;
	unsigned long blockSize ;
	if (i+1 == this->IndexCount)
	{
		blockSize =  this->FileSize - blockStart;	//�Ō�̃u���b�N�̓t�@�C���I�[�܂ő����Ă���.
	}
	else
	{
		blockSize =  this->Index[i + 1].Possition - blockStart;
	}

	//���[�U�[��`�f�[�^��ǂݍ��ރo�b�t�@.
	char * userData = NULL;

	unsigned char simbol;				// D �Ȃǂ̃f�[�^�V���{��
	unsigned long dataSize;				// ���̃f�[�^�̒���(bytes) �w�b�_������.
	unsigned long dataCount;			// �f�[�^�̂悤���̐�

	//�ǂݍ��݊J�n
	this->File.fseek( blockStart );

	//�܂��A���k�f�[�^���𓀂��܂�.
	unsigned long fullSize;				//unzip����Ɛ��鐳���ȑ傫��.
	this->File.fread( &fullSize , sizeof(fullSize) );

	unsigned char * dest = new unsigned char[fullSize];
	unsigned char * src = new unsigned char[blockSize];
	//�u���b�N�T�C�Y�́A���k�O�T�C�Y���L�^���Ă��镔�����܂ނ��� �����Z���܂�
 	this->File.fread( src , blockSize - sizeof(fullSize));

	try
	{
		//���̃u���b�N�̃��[�U�f�[�^��ǂݍ��݂܂�.
		userData = UserDataFile.Load(inBlock , &Zlib);

		//�𓀂̎��s.
		unsigned long realSize = fullSize;
		Zlib.UnCompress(dest , &realSize , src , blockSize);
		ASSERT(realSize == fullSize);

		//�܂��A�����g��Ȃ��\�[�X�̈�̔j��.
		delete [] src;	src  = NULL;

		//�𓀂����f�[�^�̉��.
		RStdioMemory mem;
		mem.Open((char*)dest);
		while( mem.ftell() < realSize )
		{
			simbol = mem.readGetUC();
			dataSize = mem.readGetUL();
			dataCount = mem.readGetUL();

			TRACE("�V���{�� %c �f�[�^�� %ld �f�[�^�̐� %d \n" , simbol , dataSize , (int)dataCount);

			ASSERT( isalpha(simbol) );

			try
			{
				//���̃V���{���̃f�[�^����������!
				ioParent->Parse(simbol , dataSize , dataCount , mem.ptr() );
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
				throw ;	//���try��
			}

			mem.fseek(dataSize , SEEK_CUR);
		}

		//���[�U��`�f�[�^������ꍇ�̏���.
		if (userData)
		{
			mem.Open((char*)userData);
			simbol = mem.readGetUC();
			dataSize = mem.readGetUL();
			dataCount = mem.readGetUL();

			try
			{
				//���̃V���{���̃f�[�^����������!
				ioParent->Parse(simbol , dataSize , dataCount , mem.ptr() );
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
				throw ;	//���try��
			}
		}

		//����g�p���郁������ RGISSmallParent �̊Ǌ����ɂ���(�d�v!!!!!)
		//�܂�A���̃������� delete �͂��ׂ� RGISSmallParent ���s��.
		//�Y��Ȃ��悤��
		ioParent->setData(inBlock ,(char*) dest , userData);

	}
	catch(RException e)
	{
		delete [] src;
		delete [] dest;
		delete [] userData;

		return false;
	}

	//�𓀂Ɏg�����������̊J��.
	delete [] src;
//	delete [] dest;

	//�L�^����Ă���u���b�N���Ǝ��ۂ̒l���A�҂���Ƃ��������Ȃ���΂����Ȃ�
	ASSERT((this->File.ftell() - blockStart) == blockSize);
	return true;
}


bool RGISSmallFileRead::LoadHeader(RGISSmallHeaderImp * ioHedaer)
{
	//�w�b�_�[�̈ʒu�Ɉړ�
	this->File.fseek( HeaderPos );

	
	unsigned char simbol;				// D �Ȃǂ̃f�[�^�V���{��
	unsigned long dataSize;				// ���̃f�[�^�̒���(bytes) �w�b�_������.
	unsigned long dataCount;			// �f�[�^�̂悤���̐�

	unsigned long pos = this->File.ftell();	
	for(  ;  pos < this->HeaderSize + HeaderPos;  pos = this->File.ftell() )
	{
		this->File.fread( &simbol , sizeof(simbol) );
		this->File.fread( &dataSize , sizeof(dataSize) );
		this->File.fread( &dataCount , sizeof(dataCount) );

		if (ioHedaer->getSimbol() == simbol)
		{	//�����܂���!

			//�ǂݍ���.
			char * buffer = new char[dataSize + 1];
			this->File.fread(buffer , dataSize );

			try
			{
				//���̃V���{���̃f�[�^����������!
				ioHedaer->Parse(simbol , dataSize , dataCount , buffer);
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
				delete [] buffer;
				return false;
			}

			delete [] buffer;
			return true;
		}
		//������Ȃ��̂œǂݔ�΂�.
		this->File.fseek(dataSize , SEEK_CUR);
	}

	return false;
}

//���[�U�[�f�[�^�̕ύX.
bool RGISSmallFileRead::UserDataModify(unsigned long inBlock, RGISSmallParentImp * ioParent,
									 const int * inDeleteNumber , int inDeleteCount ,
									 const char * inData , int inSize,int inCount) throw(RException)
{
	//�f�[�^�̕ύX.
	UserDataFile.Modify(inBlock, inDeleteNumber ,inDeleteCount ,  inData , inSize , inCount , &Zlib);

	//���̃u���b�N�̃��[�U�f�[�^��ǂݍ��݂܂�.
	char * userData = UserDataFile.Load(inBlock , &Zlib);
	//���[�U��`�f�[�^������ꍇ�̏���.
	if (userData)
	{
		unsigned char simbol;				// D �Ȃǂ̃f�[�^�V���{��
		unsigned long dataSize;				// ���̃f�[�^�̒���(bytes) �w�b�_������.
		unsigned long dataCount;			// �f�[�^�̂悤���̐�

		RStdioMemory mem;
		mem.Open((char*)userData);
		simbol = mem.readGetUC();
		dataSize = mem.readGetUL();
		dataCount = mem.readGetUL();

		try
		{
			//���̃V���{���̃f�[�^����������!
			ioParent->Parse(simbol , dataSize , dataCount , mem.ptr() );
		}
		catch(RException e)
		{
			TRACE( e.getMessage() );

			delete [] userData;

			return false;
		}

		ioParent->UserDataReload(userData);
	}
	return true;
}
