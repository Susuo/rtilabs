// RFileDB.cpp: RFileDB �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RFileDB.h"

const DWORD CELL_FLG_SIZE = sizeof(DWORD) * 2;
const int CELL_ALIVE_FLG	=	0x00000000;
const int CELL_DELETE_FLG	=	0x00000001;

#define IS_FLG_SET(flg,d)	(flg & d)
#define FLG_SET(flg,d)		(flg | d)
#define FLG_RESET(flg,d)	(flg & (~d))

/*
�f�[�^�\��:
	+0	+1	+2	+3	+4	+5	+6	+7	+8	+9	+10	+11
	(DWORD)			(DWORD)
	0	0	0	0	0	0	0	4	a	b	c	d
	|----flg----|	|--length---|	|---data----|

����:
	1.flg �� CELL_DELETE_FLG �̎��A���̃f�[�^�͍폜���ꂽ���̂Ƃ��Ĉ����B
	2.length �̐������f�[�^�͑����B

FAQ:
  Q:flg �� 4�o�C�g�Ȃ͖̂��ʂȂ̂ł�?
  A:�����ł��ˁB �܂��A�蔲���ł��B

*/	

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RFileDB::RFileDB()
{
	File = NULL;
	IndexFile = NULL;
	Index = NULL;
}

RFileDB::~RFileDB()
{
	Close();
}

/**
 * Close:			�f�[�^�x�[�X�̃N���[�Y
 *
 * @return void 
 */
void RFileDB::Close()
{
	if (File)
	{
		fclose(File);
		File = NULL;
	}
	if (IndexFile)
	{
		fclose(IndexFile);
		IndexFile = NULL;
	}
	delete [] Index;
	Index = NULL;
}

/**
 * Create:					�V�K�쐬
 *
 * @param inFilename		�t�@�C����	����Ώ㏑��
 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
 * @return void 
 */
void RFileDB::Create(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename , inHashTableSize , "w+b");
}

/**
 * Load:					�ǂݍ���
 *
 * @param inFilename		�t�@�C����
 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
 * @return void 
 */
void RFileDB::Load(const string& inFilename , int inHashTableSize) throw(RIOCannotOpenException)
{
	Open(inFilename , inHashTableSize , "r+b");

	//INDEX�̓ǂݎ��.
	DWORD buffer[3];
	while( !feof(IndexFile) )
	{
		fread( buffer , sizeof(DWORD) , 3 , IndexFile);
		InsertIndex(buffer[0] , buffer[1], buffer[2]  );
	}
}

/**
 * InsertIndex:		index �ɑ}��
 *
 * @param inKey		�����L�[
 * @param inPos		�}���ꏊ
 * @param inSize	�f�[�^�̃T�C�Y
 * @return void 
 */
void RFileDB::InsertIndex(DWORD inKey , DWORD inPos , DWORD inSize)
{
	const DWORD key = inKey % HashSize;
	(Index[key])[inKey] = OneParts(inPos, inSize);
}

/**
 * FindIndex:		index ��茟��
 *
 * @param inKey		�����L�[
 * @param outPos	�f�[�^�̏ꏊ
 * @param outSize	�f�[�^�̃T�C�Y
 * @return bool		���������ꍇ�@true 
 */
bool RFileDB::FindIndex(DWORD inKey , DWORD * outPos , DWORD * outSize) const
{
	const DWORD key = inKey % HashSize;
	SecondIndexDef * sd = & Index[key];
	SecondIndexDef::const_iterator i = sd->find(inKey);
	if ( i == sd->end() ) return false;

	*outPos = ((*i).second).first;
	*outSize = ((*i).second).second;
	return true;
}

/**
 * UpdateIndex:		index �̍X�V
 *
 * @param inKey		�����L�[
 * @param inPos		�f�[�^�̏ꏊ
 * @param inSize	�f�[�^�T�C�Y
 * @return void 
 */
void RFileDB::UpdateIndex(DWORD inKey , DWORD inPos, DWORD inSize)
{
	InsertIndex(inKey , inPos , inSize);
}

/**
 * DeleteIndex:		index �̍폜
 *
 * @param inKey		�����L�[
 * @return void 
 */
void RFileDB::DeleteIndex(DWORD inKey )
{
	const DWORD key = inKey % HashSize;
	SecondIndexDef * sd = & Index[key];
	sd->erase(inKey);
}

/**
 * Open:					�f�[�^�x�[�X�̃I�[�v��
 *
 * @param inFilename		�f�[�^�x�[�X��
 * @param inHashTableSize	�n�b�V���e�[�u���̑傫�� �f������
 * @param mode				fopen�̃��[�h
 * @return void 
 */
void RFileDB::Open(const string& inFilename , int inHashTableSize , const char* mode) throw(RIOCannotOpenException)
{
	Close();

	File = fopen(inFilename.c_str() , mode);
	if (File == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + " �f�[�^�x�[�X" + inFilename + "���J���܂���");
	}
	const string index = inFilename + ".idx";
	IndexFile = fopen(index.c_str() , mode);
	if (IndexFile == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + " �f�[�^�x�[�X" + inFilename + "�̃C���f�b�N�X" + index + "���J���܂���");
	}

	Index = new SecondIndexDef[inHashTableSize];

	HashSize = inHashTableSize;
}
/**
 * Save:					�f�B�X�N�� index ���������݂܂��B index�� OnMemory �Ȃ̂ŃZ�[�u���Ȃ��Ɩ��Ӗ��ł�
 *
 * @return void 
 */
void RFileDB::Save() 
{
	DWORD writeBuffer[3];

	fseek( IndexFile , 0 , SEEK_SET ) ;
	for(int i = 0 ; i < HashSize ; i++)
	{
		SecondIndexDef* sid = &Index[i];

		SecondIndexDef::iterator end = sid->end() ;
		for(SecondIndexDef::const_iterator i = sid->begin() ; i != end ; ++i)
		{
			//�����L�[
			writeBuffer[0] = ((*i).first);
			//�f�[�^�̏ꏊ
			writeBuffer[1] = ((*i).second).first;
			//�f�[�^�̒���
			writeBuffer[2] = ((*i).second).second;

			fwrite(writeBuffer , sizeof(DWORD) , 3 , IndexFile);
		}
	}
}

/**
 * Flush:					�f�[�^�� flush ���܂��B
 *
 * @return void 
 */
void RFileDB::Flush()
{
	fflush(File);
	fflush(IndexFile);
}

/**
 * Select:					����
 *
 * @param inKey				�����L�[
 * @param outData			�f�[�^���������ރo�b�t�@
 * @return void 
 */
void RFileDB::Select(DWORD inKey , RBuffer * outData) const throw(RNotFoundException)
{
	DWORD	seekPos,size;
	if( !FindIndex(inKey, &seekPos , &size ) )	throw RNotFoundException("������܂���");

	if ( fseek(File , seekPos+CELL_FLG_SIZE , SEEK_SET ) != 0) throw RNotFoundException("�V�[�N�Ɏ��s���܂���");

	outData->reserve(size);

	//vector �͘A���I���������ł���͂��B �����M������
	//�؋�
	//	http://anubis.dkuug.dk/JTC1/SC22/WG21/docs/lwg-defects.html#69
	//	http://www.tietew.jp/cppll/archive/4822
	fread( &((*outData)[0]) , sizeof(char) , size , File);
}

/**
 * Insert:					�}��
 *
 * @param inKey				�����L�[
 * @param inData			�}������f�[�^
 * @return void 
 */
void RFileDB::Insert(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
{
	DWORD seekPos , size;
	if( FindIndex(inKey , &seekPos , &size) )throw RNotFoundException("���łɒl������܂�");

	if ( fseek(File , 0 , SEEK_END ) != 0) throw RNotFoundException("�V�[�N�Ɏ��s���܂���");

	InsertIndex( inKey , ftell(File) , inData.size());

	DWORD flg[2];	//control , size
	flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
	flg[1] = inData.size();
	fwrite( flg , sizeof(DWORD) , 2 , File);
	fwrite( &(inData[0]) , sizeof(char) , flg[1] , File);
}

/**
 * Update:					�ύX
 *
 * @param inKey				�����L�[
 * @param inData			�ύX����f�[�^
 * @return void 
 */
void RFileDB::Update(DWORD inKey ,const RBuffer& inData ) throw(RNotFoundException)
{
	DWORD	seekPos,size;
	if( !FindIndex(inKey, &seekPos , &size ) )	throw RNotFoundException("������܂���");

	if (inData.size() == (size) )
	{	//���Ƃ��Ƃ̑傫���Ƃ�������
		if ( fseek(File , seekPos+CELL_FLG_SIZE , SEEK_SET ) != 0) throw RNotFoundException("�V�[�N�Ɏ��s���܂���");
		fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);
	}
	else
	{
		DWORD flg[2];
		if ( fseek(File , seekPos , SEEK_SET ) != 0) throw RNotFoundException("�V�[�N�Ɏ��s���܂���");

		if ( (inData.size()+CELL_FLG_SIZE) < size )
		{	//���Ƃ��Ƃ̑傫����菬�����Ȃ� �Z���𕪊��ł���.
			flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
			flg[1] = inData.size();

			fwrite( flg , sizeof(DWORD) , 2 , File);
			fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);

			//�c��̃Z���͍폜���ꂽ�ƕ�.
			flg[0] = FLG_SET(0,CELL_DELETE_FLG);
			flg[1] = size - inData.size() - CELL_FLG_SIZE;
			ASSERT(flg[1] >= 0)
			fwrite( flg , sizeof(DWORD) , 2 , File);
		}
		else
		{	//���Ƃ��Ƃ̃T�C�Y���傫���Ȃ�.
			DWORD free = CheckFreeSize(seekPos + size) + size;
			if (free >= inData.size() + CELL_FLG_SIZE)
			{//�󂫗̈�Ɋi�[�ł���.
				flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
				flg[1] = inData.size();
				fwrite( flg , sizeof(DWORD) , 2 , File);
				fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);

				//�󂫗̈�͕��f����.
				flg[0] = FLG_SET(0,CELL_DELETE_FLG);
				flg[1] = free - inData.size();
				fwrite( flg , sizeof(DWORD) , 2 , File);
			}
			else
			{//�ǂ����悤�Ȃ�...
				//���݂̗̈�ɂ͎��܂肫��Ȃ��̂ŁA
				//���݂̗̈��j�����ăt�@�C���̍Ō�ɒǉ����܂�.
				flg[0] = FLG_SET(0,CELL_DELETE_FLG);
				flg[1] = size;
				fwrite( flg , sizeof(DWORD) , 2 , File);

				fseek(File , 0 , SEEK_END );	//�t�@�C���̍Ō��

				//�C���f�b�N�X�l�̍X�V.
				UpdateIndex(inKey , ftell(File) , inData.size());

				flg[0] = FLG_SET(0,CELL_ALIVE_FLG);
				flg[1] = inData.size();
				fwrite( flg , sizeof(DWORD) , 2 , File);
				fwrite( &(inData[0]) , sizeof(char) ,inData.size() , File);

			}
		}
	}
}

/**
 * Delete:					�폜
 *
 * @param inKey				�����L�[
 * @return void				
 */
void RFileDB::Delete(DWORD inKey ) throw(RNotFoundException)
{
	DWORD	seekPos,size;
	if( !FindIndex(inKey, &seekPos , &size ) )	throw RNotFoundException("������܂���");

	if ( fseek(File , seekPos , SEEK_SET ) != 0) throw RNotFoundException("�V�[�N�Ɏ��s���܂���");

	DeleteIndex( inKey  );

	DWORD flg;
	flg = FLG_SET(0,CELL_DELETE_FLG);
	fwrite( &flg , sizeof(DWORD) , 1 , File);
}

/**
 * IsHit:					���݃`�F�b�N
 *
 * @param inKey				�����L�[
 * @return bool 
 */
bool RFileDB::IsHit(DWORD inKey ) const
{
	DWORD	seekPos,size;
	return FindIndex(inKey, &seekPos , &size );
}


/**
 * Vacuum:					�s�v�ȃf�[�^�𐮗����܂�	(Windows�ł����� �f�t���O)
 *
 * @return void 
 */
void RFileDB::Vacuum( )
{
}


/**
 * CheckFreeSize:			���݂̏ꏊ���ǂꂾ���̃t���[�X�y�[�X���m�ۂł��邩���ׂ܂�
 *
 * @param inStart			�J�n����ꏊ
 * @return DWORD			�m�ۂł���T�C�Y
 */
DWORD RFileDB::CheckFreeSize(DWORD inStart)
{
	DWORD free = 0;
	DWORD nextPoint = inStart;

	DWORD flg[2];	//control , size

	while( !feof(File) )
	{
		fseek(File ,nextPoint , SEEK_SET );	//���̃Z���Ɉړ�
		if ( !fread( flg , sizeof(DWORD) , 2 , File) )		//���̃Z���̏����擾.
		{
			return free;
		}

		if ( !IS_FLG_SET(flg[0],CELL_DELETE_FLG) ) return free;

		free += flg[1];
		nextPoint += (flg[1] + CELL_FLG_SIZE);
	}
	return free;
}

void RFileDB::test()
{
	{
		RFileDB		db;
		db.Create("testdata/rfdbtest1.dat" , 13);

		//�Ƃ肠���� �ǉ��ł��邩?
		{
			RBuffer buffer;
			buffer = "test10";
			db.Insert(10,buffer);
			buffer = "test20";
			db.Insert(20,buffer);
			buffer = "test30";
			db.Insert(30,buffer);
			buffer = "test40";
			db.Insert(40,buffer);
			buffer = "test50";
			db.Insert(50,buffer);

			db.Select(10,&buffer);
			ASSERT( buffer == "test10" ); 
			db.Select(20,&buffer);
			ASSERT( buffer == "test20" ); 
			db.Select(30,&buffer);
			ASSERT( buffer == "test30" ); 
			db.Select(40,&buffer);
			ASSERT( buffer == "test40" ); 
			db.Select(50,&buffer);
			ASSERT( buffer == "test50" ); 
		}
		//�폜�ł��邩?
		{
			RBuffer buffer;
			db.Delete(10);
			db.Delete(30);
			db.Delete(50);

			ASSERT( db.IsHit(10) == false);
			ASSERT( db.IsHit(20) == true);
			ASSERT( db.IsHit(30) == false);
			ASSERT( db.IsHit(40) == true);
			ASSERT( db.IsHit(50) == false);

		}
		//�ύX�ł��邩?
		{
			RBuffer buffer;

			//�����傫��
			buffer = "test20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "test20");

			//������
			buffer = "t20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "t20");

			//�������傫��
			buffer = "test!!20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "test!!20");

			//�ƂĂ��傫��
			buffer = "teeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeest20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "teeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeest20");

			//������
			buffer = "t20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "t20");
		
			//�����傫��
			buffer = "test20";
			db.Update(20 , buffer);
			db.Select(20 , &buffer);
			ASSERT( buffer == "test20");
		}
	}
	{
		RFileDB		db;
		RBuffer buffer;
		RBuffer t;
		db.Create("testdata/rfdbtest2.dat" , 13);

		buffer = "���ɂ���";
		db.Insert(3 , buffer);
		buffer = "���ɂ�";
		db.Insert(2 , buffer);
		buffer = "�ɂ�����";
		db.Insert(0 , buffer);
		buffer = "���ɂ������";
		db.Insert(1 , buffer);

		buffer = "���ɂ����";
		db.Update(1 , buffer);
		buffer = "�ɂ�����";
		db.Update(0 , buffer);
		buffer = "���ɂ�";
		db.Update(2 , buffer);
		buffer = "���ɂ�������";
		db.Update(3 , buffer);

		t = "�ɂ�����";
		db.Select(0 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);

		t = "���ɂ����";
		db.Select(1 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);

		t = "���ɂ�";
		db.Select(2 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);

		t = "���ɂ�������";
		db.Select(3 , &buffer);
		ASSERT( memcmp(buffer.c_str() , t.c_str() , t.size() ) == 0);
	}
	{
		RFileDB		db;
		RBuffer buffer;
		RBuffer t;
		db.Create("testdata/rfdbtest2.dat" , 13);

		buffer = "���ɂ����";
		db.Insert(9 , buffer);
		buffer = "���ɂ��݂���";
		db.Insert(0 , buffer);
		buffer = "���ɂ����";
		db.Insert(1 , buffer);
		buffer = "���ɂ�";
		db.Insert(6 , buffer);
		buffer = "���ɂ�����";
		db.Insert(2 , buffer);
		buffer = "�ɂ���";
		db.Insert(3 , buffer);
		buffer = "���ɂ����";
		db.Insert(7 , buffer);
		buffer = "���ɂ������";
		db.Insert(4 , buffer);
		buffer = "���ɂ����";
		db.Insert(5 , buffer);
		buffer = "���ɂ������";
		db.Insert(8 , buffer);

		AssertFinder(&db ,10,  "���ɂ��݂���" , "���ɂ����" , "���ɂ�����", "�ɂ���", "���ɂ������", 
							   "���ɂ����", "���ɂ�", "���ɂ����", "���ɂ������", "���ɂ����");
		buffer = "���ɂ������";
		db.Update(6 , buffer);
		AssertFinder(&db ,10,  "���ɂ��݂���" , "���ɂ����" , "���ɂ�����", "�ɂ���", "���ɂ������", 
							   "���ɂ����", "���ɂ������", "���ɂ����", "���ɂ������", "���ɂ����");

		buffer = "���ɂ���";
		db.Update(4 , buffer);
		AssertFinder(&db ,10,  "���ɂ��݂���" , "���ɂ����" , "���ɂ�����", "�ɂ���", "���ɂ���", 
							   "���ɂ����", "���ɂ������", "���ɂ����", "���ɂ������", "���ɂ����");

		buffer = "�ɂ�����";
		db.Update(0 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ����" , "���ɂ�����", "�ɂ���", "���ɂ���", 
							   "���ɂ����", "���ɂ������", "���ɂ����", "���ɂ������", "���ɂ����");

		buffer = "���ɂ�";
		db.Update(1 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ�����", "�ɂ���", "���ɂ���", 
							   "���ɂ����", "���ɂ������", "���ɂ����", "���ɂ������", "���ɂ����");

		buffer = "���ɂ������";
		db.Update(7 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ�����", "�ɂ���", "���ɂ���", 
							   "���ɂ����", "���ɂ������", "���ɂ������", "���ɂ������", "���ɂ����");

		buffer = "���ɂ������";
		db.Update(2 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ������", "�ɂ���", "���ɂ���", 
							   "���ɂ����", "���ɂ������", "���ɂ������", "���ɂ������", "���ɂ����");

		buffer = "���ɂ�";
		db.Update(3 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ������", "���ɂ�", "���ɂ���", 
							   "���ɂ����", "���ɂ������", "���ɂ������", "���ɂ������", "���ɂ����");

		buffer = "���ɂ�";
		db.Update(5 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ������", "���ɂ�", "���ɂ���", 
							   "���ɂ�", "���ɂ������", "���ɂ������", "���ɂ������", "���ɂ����");

		buffer = "���ɂ�";
		db.Update(8 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ������", "���ɂ�", "���ɂ���", 
							   "���ɂ�", "���ɂ������", "���ɂ������", "���ɂ�", "���ɂ����");

		buffer = "���ɂ�";
		db.Update(9 , buffer);
		AssertFinder(&db ,10,  "�ɂ�����" , "���ɂ�" , "���ɂ������", "���ɂ�", "���ɂ���", 
							   "���ɂ�", "���ɂ������", "���ɂ������", "���ɂ�", "���ɂ�");
	}
	//�X�g���X
	{
		printf("�X�g���X�e�X�g������ \r\n");
		RFileDB		db;
		db.Create("testdata/rfdbtest10.dat" , 13);

		
		RBuffer buffer;

//		const int testCount = 1000000;
		const int testCount = 100;

		//�ǉ�����f�[�^
		string callme[12];
		callme[0] = "���ɂ������";
		callme[1] = "���ɂ�����";
		callme[2] = "�ɂ�����";
		callme[3] = "���ɂ�";
		callme[4] = "���ɂ������";
		callme[5] = "���ɂ�";
		callme[6] = "���ɂ�����";
		callme[7] = "���ɂ�������";
		callme[8] = "���ɂ���";
		callme[9] = "���ɂ��݂���";
		callme[10] = "���ɂ����";
		callme[11] = "�ɂ���";
/*

		//�ǉ�����f�[�^
		string callme[12];
		callme[0] = "�� �|�����| ���Z�����A��D�����̑�D���Ȃ��Z�����A�����C�ł����H���͍ŋ߂��Z�����ɉ�Ȃ����������Ă��āA�Ƃ��Ă��Ƃ��Ă��҂����C�����ł����ς��ł��B���A�{���͂������Ă��Z�����ƈꏏ�ɂ������̂ɁA�������Ƃ��Z�����ɉ�Ȃ��ł��邩��c�c�B������A�s�A�m�̂��m�Âł��A�����߂����Ȃ΂���e���Ă��܂��́B�����͂��Z�����Ƒ������悤�ɁA�}���A�l�ɂ��F�肵�ĐQ�܂��B�����ƍ���͂��Z�����̖�������邩�ȁc�c�B���Z�����A��D���o���������������a���� �X���Q�R�������� ���������g�� �P�S�Wcm�R�R���|�C���g�������͂Ƃɂ������Z�����̂��Ƃ���D���I�@�Ƃ��Ă����Z�����ɓ���Ă��āA���ł������̂��΂ɂ��Ă��ꂽ�炢���̂ɂƎv���Ă�A������҂�Â���V�Ō��C�ȏ��̎q�ł��B";
		callme[1] = "�ԕ� �|���ف| �ԕ�͂������Ă��Z����܂��������܂��I���Z����܁A�����C�ł����H�ԕ�̓`�A�̗��K�𖈓��ꐶ��������΂��Ă܂��I�@�������Z����܂̉��������������āA�����҂�����Ȃ����炢!!�@���Z����܂Ɂu�ԕ�̂������ł���΂ꂽ��v���Č����Ă��炤�̂��A�ԕ�̂������Ⴂ������̖��Ȃ񂾂��c�c�@�ł��A����ɂ͂���ς薈���̗��K������΂�Ȃ�������ˁB���Z����܁A�ԕ�̉����y���݂ɂ��ĂĂˁI�o���������������a���� �P���V�������� �R�r�����g�� �P�S�Rcm�R�R���|�C���g�ԕ䂿���͂��Z�����̂��Ƃ��������邽�߂Ƀ`�A���[�f�B���O������Ă���A�Ƃ��Ă����Z�����z���̖��B�Ԃ���Ă�̂��D���ŁA�ӊO�ɂ�����ƃh�W���ۂ��Ƃ��������݂����ł��B";
		callme[2] = "�q �|�܂���| �{�N�ƈꏏ�ɗV�ڂ���B�n�@�C�A���ɂ��I�@�c�c�₟���ƁA�N�����Ȃ��`�H�@�����A�����Ƒ҂��Ă��񂾂��I�@���ɂ��́A�܂������˂ڂ����Ȃ񂾂���Ȃ��c�c�B�c�c���ւցA���܂œ������Ă΂����肾�������ǁA���������͂��܂�������ˁI�@��΂ɂ܂�܂�P���A�{�N�ɕt�������Ă��炤�񂾂�����@�c�c���ɂ������A�����Ƃӂ��񂩂炢���ς��X�|�[�c���������C�C�Ǝv����B�̂𓮂����̂��Ă����Ƃɂ����y�������A����Ɂc�c��������A�����ȃX�|�[�c���Q�l�ꏏ�ɂł��邶��Ȃ��H�o���������������a���� �P�O���P�W�������� �V�������g�� �P�T�Ocm�R�R���|�C���g�q�����̓X�|�[�c��D���̌��C�Ȗ��B���������ȃX�|�[�c�����Z�����ƈꏏ�ɂ�肽���ăE�Y�E�Y���Ă��܂��B�Ȃ��ł����ӂȂ̂̓{�[�h�n�ŁA�t�@�b�V���������ꕗ�������݂����B";
		callme[3] = "��� �|������| ���Z�l�͐�΂Ɏ��̃��m�Ȃ񂾂���B���Z�l�ցB���̊Ԃ��Z�l���͂��Ă��u�[�c�A�X�S�N�������悩��������A�����낢�̔������Ǝv���Ă��X�ɍs������c�c��������؂�Ă��́B�Ȃ񂾂��^�������Ƃ��Z�l�������������Ƃ��Ă�݂����Ɏv���āA������Ƌ����Ă��܂�����c�c�B���A�ŋߊ����I�ɂȂ��Ă���̂�����H�@����Ⴀ�A���͖������ǁc�c�����炢���Z�l�̂��Ƃ������Ă鏗�̎q�́A���̐���1�l�����Ȃ��Ǝv���́I�@���Ƃ��Z�������āA���Z�l�Ǝ��͐�΂Ɍ��΂��^���ɂ���c�c���ˁH�o���������������a���� �P�Q���Q�O�������� �ˎ�����g�� �P�T�Xcm�R�R���|�C���g��낿���͂����������Ŏ��M�����Ղ�I�@�ޏ��̃��b�Z�[�W�ɂ́A�Z��Ƃ����֌W�𒴂��āA�P�l�̏����Ƃ��Ă��Z�����Ɍ��Ă��炢�����Ƃ����C���������ӂ�Ă��܂��B";
		callme[4] = "���q �|�ЂȂ��| �q�i�͂��ɂ����܂����Ȃ��Ɓ@�T�r�V�C�T�r�V�C�a�ɂȂ����Ⴄ��I���ɂ����܂ցB���ɂ����܂��A��������āA�q�i�̓T�r�V�C�T�r�V�C�a�ɂ��������Ⴂ�܂����B�������ɂ����܂����Ȃ��[�A�Ǝv������A������҂�܂��o���Ⴄ�񂾂�B���A�ł��A�X�O����������������A�}�}�ɂ͌�����Ȃ�������B�q�i�A���炢�H�@�c�c������A�����܂��q�i�̃g�R���ɉ�ɗ��Ă��[�������B���������A���������҂��Ă܂��I�@�q�i���B�o���������������a���� �W���P�T�������� ���q�����g�� �P�R�Qcm�R�R���|�C���g���q�����͂P�Q�l�̖��B�̒��łP�Ԃ̂��`�r�����B�҂������ŁA�������Z����񂪈ꏏ�ɂ��ėV��ł���邱�Ƃ�]��ł��܂��B�q�������Ɉ���\���̎d�����_�C���N�g�ȂƂ�����c�c�B";
		callme[5] = "�f�G �|�܂肦�| �����Z��l�Ƃ���������c�c�B�e���Ȃ�Z��l�c�c�B���΂炭���ڂɂ�����Ă��܂��񂯂�ǁA�����C�ł�������Ⴂ�܂����H�@�����ŋ߂͏������q�������݂����ł��B���A�a�@�ŌZ��l�ɃZ�[�^�[��҂�ł݂܂����B�Z�[�^�[��҂ނ̂͏��߂Ă���������A���܂��肶��Ȃ��݂����Ȃ�ł�����ǁc�c�B�Z��l�����Ă�������Ƃ��ꂵ���ł��B�c�c���̑z�����Z�[�^�[�̕҂ݖڂ�ʂ��ē`���܂��悤�Ɂc�c�o���������������a���� �S���S�������� ���r�����g�� �P�S�Wcm�R�R���|�C���g�f�G�����͕a��ŁA�����Ƃ₩�ȉ����[�����̎q�B���Z�����̂��Ƃ���D���Ȃ̂ɁA�̂��キ�Ă��Z�����Ǝv���悤�ɉ�Ȃ��̂������C�ɕa��ł���Ƃ��낪�����炵���̂ł��B";
		callme[6] = "���� �|����䂫�| �ɂ����܁A�����Ղ菢���オ��I�n�[�C�I�@�ɂ����܁B�ŋ߂͂��͂񂿂��ƐH�ׂĂ܂��́H�P�̍������������Ȃ��Ɓc�c�A���Č����Ă�������̂̓E���V�C���ǁA�ɂ����܂������Ă��܂�����A�P���߂�����B������A�z���g�Ȃ炢�����P�����΂ɂ��Ă��������Ă�����ꂽ�炢����ł��̂ɁB�c�c���ӂӁ@�������P���ɂ����܂̂��ł���ɂ�����ǂ�������H�@�c�c������o���������������a���� �Q���P�P�������� ���r�����g�� �P�S�Ocm�R�R���|�C���g���Ⴟ���͂���������D���ŁA���Z�����ɂ��͂������Ă�����̂����������ł��B�ޏ��̗����͑����ɑn���I�ŋ�������邱�Ƃ��������ǁA�������܂��ޏ��̖��͂������肷���ł��ˁB";
		callme[7] = "��z �|�����| �A�j�L�̂��������[�������A�҂��Ă��n�C�A�A�j�L�I�@���낻��܂��A�p�\�R���̃j���[���f�����o���낤�V�[�Y�������ǁA�����A�j�L�̃^�[�Q�b�g�͌��܂����H�@���͂˂��c�c����ς�A���^�J�����t���̃��o�C���m�[�g���c�c���Ă����̂̓E�\�ŁA���܂̓��J��z�̉����^���Œ�������A�����V�����}�V���͂���Ȃ���@�ӂӂ��c�c�A�j�L������A���S�����H�@���x�̃O���[�h�A�b�v������������A���悢�惁�J��z���ŏI�`�ԁI�@�c�c�ł�����������A���̑���ɃA�j�L�̐��b�������悤���Ȃo���������������a���� �V���X�������� �I�����g�� �P�T�Qcm�R�R���|�C���g��z�����̓��J�j�b�N�Ȏ�����d�]�n�̏��̎q�B��ɂ����������肷���邽�߁A�������Z�����ɋ~���̎�����߂Ă��܂��B�ŋ߂P�Ԃ̑��͎����\�b�N���̃��J��z�i���ݐ��쒆�j�B";
		callme[8] = "��e �|�������| �c�c�c�c�₠�A�Z����c�c�c�c�B�c�c�c�c�₠�A�Z����B�c�c�c�c�c�c���C�������ˁB�c�c�c�c���͂��̊ԁA�X�e�L�ȔL�̃~�C������ɓ��ꂽ�񂾂��A�悩������Z��������ɂ��Ȃ����H�@�c�c�c�c�ƂĂ��悭�������Ă��āc�c�c�c��Ԃ����Ɋ����Ȃ񂾁B�܂��������΂炵�����̂���c�c�c�c�B�c�c�c�c�����͎����A�Z����̃~�C��������Ă݂����ȁc�c�c�c�B�c�c�c�c��������̂̎p���i���ɕۑ����Ă��������C�����́c�c�c�c�Ñ��������ς�肪�Ȃ��񂾁c�c�c�c�ˁA�Z����c�c�c�c�B�o���������������a���� �R���U�������� �������g�� �P�T�Vcm�R�R���|�C���g�N�[���Ń~�X�e���A�X�ȃ��[�h�̐�e�����́A�����ڂǂ���J���g�▂�p���D���ȏ��̎q�B�����l���Ă���̂��悭�킩��Ȃ����Ƃ��������ǁA����ł����Z�����̂��Ƃ͍D���݂����i�H�j";
		callme[9] = "�t�� �|�͂邩�| ���������ς��Z�N���܂�����肢�����܂�����悤�₭�A����ł��܂����c�c���^�N�V�̌Z�N���܁@���^�N�V�A�Ƃ��Ă����ꂵ���ł��I�@�����ԗ��ꗣ��ɂȂ��Ă��܂�������ǁA����Ƃ����΂ł��d���ł���������܂�������B�Z�N���܂ɂ���������������ǍȌ���ɂȂ��悤�A���������؂��S���x����������肨�m�Â��ė��܂����́B�����A����ƕ��������ӂł��c�c���ꂩ��͉��������Ă��@���^�N�V���Z�N���܂�����肢�����܂�����I�o���������������a���� �T���P�U�������� ���������g�� �P�T�Ucm�R�R���|�C���g�t�̂����̓X�e�L�ȑ�a���q�ɂȂ��Ă��Z�����ɂ��d���������Ǝv���Ă鏗�̎q�B�����p�����̊�]�̂�����݂����Ȃ񂾂��ǁA���ɖ��������Ȑ��i�݂����Ŗ\���M�~�ȓ_���`�����H";
		callme[10] = "�l�t �|��΁| �Z�`���}�̔閧�́@�l�t���݁[��ȃ`�F�L�����Ⴄ��f�X�I����`�A�Z�`���}�A���`�������I�@���ӂӂ��A���ɌZ�`���}������������@�l�t�͂ˁA�Z�`���}�ɉ���߂ɂ͂�΂�C�M���X�������ė����̂�I�@���T��̎l�t�ɂ���������Z�`���}���C�`�R���ˁB�Z�`���}�A�l�t����������ɂ͂��ꂩ��B�����Ȃ񂩂��Ă���������񃀃_��@�Z�`���}�̔閧�́A�l�t���݁`��Ȗ\�����Ⴄ�I�l�t�̃J���y�L�閧�����ŌZ�`���}�̃n�[�g���`�F�L�`�F�L�`�F�L���!!�o���������������a���� �U���Q�P�������� �o�q�����g�� �P�S�Xcm�R�R���|�C���g���T������̂���l�t�����́A���Z�����}�j�A�̏��̎q�B�͂邩�C�M���X����A���Z�����̔閧�𔭌����邽�߂ɂ���ė��܂����B���Ȃ�g���ł鐫�i�̗l�q�Ő悪�v������܂��c�c�B";
		callme[11] = "������ �|���肠�| �A�C�X���Ƃ���������́c�c������B�������́c�c�t�����X���痈�܂����B���́c�c�}�}�����c�c�ˁA�����������{�ɋA������X�e�L�ȌZ�₪���Ę������ɂ����Ȃ��Ƃ������Ă����c�c���Č����Ă��́B�Z��c�c����{���H�@�}�}���͂�����̂��Ƃ����������Ă������ǁA������͂Ƃ��Ă��|���Ę������̂��Ɓc�c�����ƃL���C�������́B�c�c������B�c�c�ł��A�Z��͂ƂĂ��D�������ˁ@�Z��́c�c�������̂��ƁA���������肵�Ȃ��H�o���������������a���� �P�P���Q�������� 嶍����g�� �P�R�Xcm�R�R���|�C���g�����������̓t�����X����C��n���Ă���ė������B���a�Ȑ��i�ŁA���������ƂɂȂ�ƃX�O�ɋ����o���Ă��܂��炵���ł����A�D�������Z�����̂��Ƃ����͑�D���݂����ł��B";
*/
		StTestData *InsertMap = new StTestData[testCount];
		StTestData *UpdateMap = new StTestData[testCount];
		//�X�g���X�e�X�g�p�̃f�[�^���쐬.
		StressTestMakeRandomData( InsertMap ,testCount , 12 );
		StressTestMakeRandomData( UpdateMap ,testCount , 12 );

		printf("�X�g���X�e�X�g�J�n \r\n");
		time_t startTime = time(NULL);
		time_t stepStartTime = time(NULL);
		int i;

		for(i = 0; i < testCount ; i++)
		{
			db.Insert(InsertMap[i].pos , callme[ InsertMap[i].str ] );
		}

		printf("Step�o�ߎ��� : %d �b\r\n", time(NULL) - stepStartTime );
		stepStartTime = time(NULL);
		//����ɒǉ����ꂽ�̂��m�F.
		for(i = 0; i < testCount ; i++)
		{
			db.Select(InsertMap[i].pos , &buffer);
			ASSERT( memcmp(buffer.c_str() , callme[ InsertMap[i].str ].c_str() , callme[ InsertMap[i].str ].size() ) == 0);
		}

		printf("Step�o�ߎ��� : %d �b\r\n", time(NULL) - stepStartTime );
		stepStartTime = time(NULL);

		for(i = 0; i < testCount ; i++)
		{
			db.Update(UpdateMap[i].pos , callme[ UpdateMap[i].str ] );
		}

		printf("Step�o�ߎ��� : %d �b\r\n", time(NULL) - stepStartTime );
		stepStartTime = time(NULL);
		//����ɒǉ����ꂽ�̂��m�F.
		for(i = 0; i < testCount ; i++)
		{
			db.Select(UpdateMap[i].pos , &buffer);
			ASSERT( memcmp(buffer.c_str() , callme[ UpdateMap[i].str ].c_str() , callme[ UpdateMap[i].str ].size() ) == 0);
		}
		printf("Step�o�ߎ��� : %d �b\r\n", time(NULL) - stepStartTime );
		printf("�o�ߎ��� : %d �b\r\n", time(NULL) - startTime );

		delete [] UpdateMap;
		delete [] InsertMap;
	}
}

//�����̏��Ƀf�[�^�[�x�[�X�Ɋi�[����Ă��邩�ǂ����e�X�g����   �e�X�g�p
void RFileDB::AssertFinder(const RFileDB * inDB , int inNumber ,...)
{
	va_list ap;
	va_start(ap, inNumber);

	RBuffer buffer;
	va_start(ap, inNumber);

	string str;
	for(int i = 0; i < inNumber ; i++ )
	{
		str = va_arg( ap, const char*);
		inDB->Select( i , &buffer);

		ASSERT( memcmp(buffer.c_str() , str.c_str() , str.size() ) == 0);
	}
	va_end(ap);
}


//�X�g���X�e�X�g�p�̃f�[�^���쐬.
void RFileDB::StressTestMakeRandomData(StTestData * outData , const int inTestCount , const int inTestDataCount)
{
	srand((unsigned)time(NULL));

	//���j�[�N�ɂȂ�悤��.
	char * flg = new char[inTestCount];
	memset(flg , 0 , sizeof(char) * inTestCount);

	int lastUniq = 0;
	//�����_���ɒǉ�
	for(int i = 0; i < inTestCount ; i++)
	{
		int r = rand() % inTestCount;

		if ( flg[r] != 0)
		{	//�����Ă��Ȃ��Ȃ�󂫂�T��
			for(int l = lastUniq ; l < inTestCount ; l ++)
			{
				if ( flg[l] == 0)
				{
					r = l;
					lastUniq = l;
					break;
				}
			}
		}
		flg[r] = 1;
		outData[i].pos = r;
		outData[i].str = rand() % inTestDataCount ;
	}

	delete [] flg;
}
