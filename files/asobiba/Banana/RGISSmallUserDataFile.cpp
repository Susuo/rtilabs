// RGISSmallUserDataFile.cpp: RGISSmallUserDataFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallUserDataFile.h"
#include "RStdioFile.h"
#include "RStdioMemory.h"
#include "RStdioMemoryWrite.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////



RGISSmallUserDataFile::RGISSmallUserDataFile()
{

}

RGISSmallUserDataFile::~RGISSmallUserDataFile()
{

}

void RGISSmallUserDataFile::Create(const string & inPath)
{
	this->Path = inPath;
}

//�w�肳�ꂽ�u���b�N�̃��[�U��`�f�[�^��ǂݍ��݂܂�
//�߂�l�́A������ new ���ꂽ�������ł��B
//���̃N���X�͂��̃��������Ǘ����Ȃ��̂ŁA�K�؂ȏꏊ�ŊJ������K�v������܂��B
//�\��ʂ�ł���΁A RGISSmallParent �� UserData �Ƃ��������o�ŊǗ�����A���̕�炷���͋L���ꂽ�����ɏ��ł��܂�.
char* RGISSmallUserDataFile::Load(unsigned long inBlock ,const RZlib * inZlib) const
{
	return FileLoad(inBlock , inZlib , 0 );
}

//�w�肳�ꂽ�u���b�N�̃��[�U��`�f�[�^��ǂݍ��݂܂�
//�߂�l�́A������ new ���ꂽ�������ł��B
//���̃N���X�͂��̃��������Ǘ����Ȃ��̂ŁA�K�؂ȏꏊ�ŊJ������K�v������܂��B
char* RGISSmallUserDataFile::FileLoad(unsigned long inBlock ,const RZlib * inZlib,int inOverplusAlooc ) const
{
	unsigned char * src = NULL;
	unsigned char * dest = NULL;
	try
	{
		RStdioFile file;
		file.Open(Path + num2str(inBlock) ,"rb");

		//�t�@�C���̐擪�ɉ𓀂���Ɖ��o�C�g�ɂȂ邩�Ƃ�����񂪂���܂�.
		unsigned long fullSize;				//unzip����Ɛ��鐳���ȑ傫��.
		file.fread( &fullSize , sizeof(fullSize) );
		//�擪�ȊO�̃f�[�^�͂��ׂĈ��k����Ă��܂�.
		int zipSize = file.getSize() - sizeof(fullSize);

		//�t�@�C����ǂݍ��ނ��߂̃��������m�ۂ��܂��B
		src = new unsigned char[zipSize];
		dest = new unsigned char[fullSize + inOverplusAlooc];

		//zip���ꂽ�f�[�^��ǂݍ��݂܂�.
		file.fread(src , zipSize);

		//�𓀂̎��s.
		unsigned long realSize = fullSize;
		inZlib->UnCompress(dest , &realSize , src , zipSize);
		ASSERT(realSize == fullSize);

		//�܂��A�����g��Ȃ��\�[�X�̈�̔j��.
		delete [] src;	src  = NULL;
	}
	catch(RException e)
	{
		delete [] src;
		delete [] dest;
		
		return NULL;
	}

	//�𓀂Ɏg�����������̊J��.
	delete [] src;

	return (char*)dest;
}

//�t�@�C���ɃZ�[�u����.
void RGISSmallUserDataFile::FileSave(unsigned long inBlock ,const char * inData , unsigned long inSize,const RZlib * inZlib) const throw(RException)
{
	unsigned char * dest = NULL;
	try
	{
		RStdioFile file;
		file.Open(Path + num2str(inBlock) ,"wb");

		//���k������̃f�[�^�������߂�̈�. (������Ƒ傫�߂Ɋm�ۂ��܂�)
		unsigned long zipSize = inSize + 1024;
		dest = new unsigned char[zipSize];
		inZlib->Compress2(dest , &zipSize , (unsigned char*)inData , inSize);

		//�������݊J�n.

		file.fwrite( &inSize , sizeof(inSize) );
		file.fwrite( dest , zipSize );

	}
	catch(RException e)
	{
		delete [] dest;
		
		throw;
	}

	delete [] dest;

	ASSERT( FileTest(inBlock , inZlib) );
}

class RGISSmallUserDataFileHeader
{
public:
	void Create(const char* inData)
	{
		RStdioMemory mem;
		mem.Open(inData);

		Simbol = mem.readGetUC();
		DataSize = mem.readGetUL();
		DataCount = mem.readGetUL();

		DataStart = mem.ptr();
	}

	//�f�[�^�T�C�Y�ƌ����̕ύX
	void Change(unsigned char inSimbol , unsigned long inDataSize , unsigned long inDataCount)
	{
		Simbol = inSimbol;
		DataSize = inDataSize;
		DataCount = inDataCount;
	}

	int getHeaderSize() const
	{
		return sizeof(Simbol) + sizeof(DataSize) + sizeof(DataCount) ;
	}

	void Write(char * outData) const
	{
		RStdioMemoryWrite mem;
		mem.Open(outData);

		mem.writeGetUC(Simbol);
		mem.writeGetUL(DataSize);
		mem.writeGetUL(DataCount);
	}

	// D �Ȃǂ̃f�[�^�V���{��
	unsigned char getSimbol() const
	{
		return Simbol;
	}
	// ���̃f�[�^�̒���(bytes) �w�b�_������.
	unsigned long getDataSize() const
	{
		return DataSize;
	}
	// �f�[�^�̗v�f�̐�
	unsigned long getDataCount() const
	{
		return DataCount;
	}
	//���f�[�^
	const char* getDataStart() const
	{
		return DataStart;
	}
	//���̃f�[�^�̃w�b�_�[������
	const char* getNextBlock() const
	{
		return DataStart + DataSize;
	}


private:
	unsigned char Simbol;				// D �Ȃǂ̃f�[�^�V���{��
	unsigned long DataSize;				// ���̃f�[�^�̒���(bytes) �w�b�_������.
	unsigned long DataCount;			// �f�[�^�̗v�f�̐�

	const char* DataStart;				// ���f�[�^
};



//�ǉ�.
void RGISSmallUserDataFile::Append(unsigned long inBlock ,const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException)
{
	Modify(inBlock , NULL , 0 , inData , inSize , inCount , inZlib);
}


//�폜
void RGISSmallUserDataFile::Delete(unsigned long inBlock ,const int * inDeleteNumber , int inSize ,const RZlib * inZlib) const throw(RException)
{
	Modify(inBlock , inDeleteNumber , inSize , "" , 0 , 0 , inZlib);
}

//�ǉ�.
void RGISSmallUserDataFile::Modify(unsigned long inBlock ,const int * inDeleteNumber , int inDeleteCount ,
								   const char * inData , int inSize,int inCount , const RZlib * inZlib) const throw(RException)
{
	RGISSmallUserDataFileHeader header;

	//�ǉ������f�[�^�̎��ȃe�X�g.
	ASSERT(MySelfTest(inData,inSize,inCount) );

	//�t�@�C����ǂݍ���.
	char * src = FileLoad(inBlock , inZlib , inSize );
	if (src == NULL)
	{
		//�폜���Ȃ��ꍇ�́A�����̒ǉ��Ȃ킯�Ńt�@�C�����Ȃ��Ă��G���[�ɂ��Ȃ�.
		if (inDeleteCount != 0)
		{
			throw RException(EXCEPTIONTRACE + "���[�U��`�t�@�C�����J���܂���");
		}
		//�t�@�C����V�K�쐬���Ēǉ�.
		int realSize = header.getHeaderSize() + inSize;
		char * data = new char[realSize];
		header.Change( 'Z' , inSize , inCount);
		header.Write(data);
		memcpy(data + header.getHeaderSize() , inData , inSize);

		//�������ނȂ��悤�e�X�g
		ASSERT(MySelfTest(data) );

		FileSave(inBlock , data , realSize  , inZlib);

		delete [] data;

		return ;
	}
	char * nowSrc = src ;

	//���t�@�C���̃e�X�g.
	ASSERT(MySelfTest(src) );
	ASSERT(MySelfTest(src) );

	header.Create(src);
	int realSize = header.getDataSize();

	char * dest = new char[ header.getHeaderSize() + header.getDataSize() + inSize ];
	char * nowDest = dest ;

	int writeCount = 0;

	//�w�b�_�[�����̃R�s�[.
	memcpy(dest , src , header.getHeaderSize() );
	nowDest += header.getHeaderSize();
	nowSrc += header.getHeaderSize();

	if (inDeleteCount == 0)
	{
		//�폜�͍s��Ȃ�.  ���ׂẴf�[�^���R�s�[����.
		int size = header.getDataSize() ;
		memcpy(nowDest , nowSrc , size );
		nowDest += size;

		writeCount = header.getDataCount();

		//�P���ɃR�s�[�������ʂ��e�X�g
		ASSERT(MySelfTest(dest) );
	}
	else
	{
		//�f�[�^���X�L�������č폜���Ȃ���΂Ȃ�Ȃ��f�[�^�ȊO�� dest �ɏ����ʂ�.
		for(unsigned int i = 0 ; i < header.getDataCount() ; i ++)
		{
			//���̃f�[�^�ֈړ�.
			char * p = NextData(nowSrc);

			//���̃f�[�^�͍폜�Ώۂł���?
			for(int skip = 0 ; skip < inDeleteCount ; skip ++ )
			{
				if ( i == inDeleteNumber[skip] ) break;
			}
			if (skip == inDeleteCount)
			{
				//�폜���Ȃ������炵���̂ŏ�������.
				memcpy(	nowDest , nowSrc , p - nowSrc);
				nowDest += p - nowSrc;
				writeCount++;
			}

			nowSrc = p;
		}
	}
	//�ǉ�����f�[�^.
	memcpy(nowDest, inData , inSize);
	nowDest+= inSize;
	writeCount += inCount;

	int writeSize = nowDest - dest ;

	//�w�b�_���X�V.
	header.Change( header.getSimbol() ,  writeSize - header.getHeaderSize(), writeCount);
	header.Write(dest);


	//�������ނȂ��悤�e�X�g
	ASSERT(MySelfTest(dest) );

	//�f�[�^�̏�������.
	FileSave(inBlock , dest , writeSize , inZlib);

	delete [] dest;
	delete [] src;
}

//���̃f�[�^�Ɉړ�.
char* RGISSmallUserDataFile::NextData(const char* inData) const
{
	int header = 
			sizeof(unsigned short)	+		//x
			sizeof(unsigned short)	;		//y
	RStdioMemory Mem;
	Mem.Open(inData + header);

	unsigned char type;
	unsigned char size;
	while(1)
	{
		type = Mem.readGetUC();	//type
		size = Mem.readGetUC();	//size
		if (type == 0xff && size == 0) break;
		Mem.fseek(size , SEEK_CUR);
	}
	return (char*) Mem.ptr();
}

//���ȃ`�F�b�N.
bool RGISSmallUserDataFile::MySelfTest(const char* inData) const
{
	RGISSmallUserDataFileHeader header;
	header.Create(inData);
	if (  header.getSimbol() != 'Z')
	{
		ASSERT(0);
		return false;
	}

	//���ׂẴf�[�^���X�L�������Ă݂�.
	char * data = (char*) header.getDataStart();
	const char * dataOver = data + header.getDataSize();
	for(unsigned int i = 0 ; i < header.getDataCount() ; i ++)
	{
		//���̃f�[�^�ֈړ�.
		data = NextData(data);

		if (dataOver < data)
		{
			ASSERT(0);
			return false;
		}
	}

	//�f�[�^�̃T�C�Y���L�q���Ă���̂œ������Ȃ�͂�.
	if (dataOver != data)
	{
		ASSERT(0);
		return false;
	}

	return true;
}

//���ȃ`�F�b�N �w�b�_���Ȃ��f�[�^�̂�.
bool RGISSmallUserDataFile::MySelfTest(const char* inData , int inSize , int inCount) const
{
	//���ׂẴf�[�^���X�L�������Ă݂�.
	char * data = (char*) inData;
	const char * dataOver = data + inSize;
	for(int i = 0 ; i < inCount ; i ++)
	{
		//���̃f�[�^�ֈړ�.
		data = NextData(data);

		if (dataOver < data)
		{
			ASSERT(0);
			return false;
		}
	}

	//�f�[�^�̃T�C�Y���L�q���Ă���̂œ������Ȃ�͂�.
	if (dataOver != data)
	{
		ASSERT(0);
		return false;
	}

	return true;

}

//�t�@�C���e�X�g.
bool RGISSmallUserDataFile::FileTest(unsigned long inBlock ,const RZlib * inZlib) const
{
	char * p = FileLoad(inBlock , inZlib , 0);
	if ( MySelfTest(p) == false)
	{
		delete [] p ;

		ASSERT(0);
		return false;
	}

	delete [] p ;

	return true;

}
