// YDAT.cpp: YDAT �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YDAT.h"
#include "YDATReader.h"
#include <sys/stat.h>
#include <sys/utime.h>
#include "imagehlp.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

/**
 * YDAT:	�R���X�g���N�^
 *
 * @param inBoardName		�̖��O
 * @param inDatNumber		DAT�̖��O
 * @return  
 */
YDAT::YDAT(DATNUMBER	inDatNumber)
{
	IndexMM.Create(MAX_NUMBER_FULL * sizeof(DWORD) );
	this->Index = (DWORD*) IndexMM.getMemory();

	DatMM.Create(MAX_DAT_SIZE * sizeof(char) );
	this->Dat = (char*) DatMM.getMemory();

	this->DatNumber = inDatNumber;

	this->Index[0] = '\0';
	this->Dat[0] = '\0';
	this->No = 0;
}

/**
 * ~YDAT:		�f�X�g���N�^
 *
 * @return  
 */
YDAT::~YDAT()
{
	DatMM.Release();
	IndexMM.Release();
}

/**
 * IndexLoad:		INDEX�t�@�C���̓ǂݍ���.
 *
 * @throw		RIOCannotOpenException	�t�@�C�����܂���.
 * @return void 
 */
void YDAT::IndexLoad(const string inBoardName)	
{
	string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\" + num2str(this->DatNumber) + ".idx";
	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "index�t�@�C�����J���܂���ł���" + filename);
	}

	int no = fread(this->Index,sizeof(DWORD),MAX_NUMBER_FULL,fp);

	this->No = no - 1;
	ASSERT(this->No >= 0) ;
	ASSERT(this->No <= MAX_NUMBER_FULL) ;

	fclose(fp);

}

/**
 * DATLoad:		DAT�t�@�C���̓ǂݍ���.
 *
 * @throw		RIOCannotOpenException	�t�@�C�����܂���.
 * @return void 
 */
void YDAT::DATLoad(const string inBoardName) throw(RIOCannotOpenException)
{
	ASSERT(this->Dat[0] == '\0');

	string filename =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\" + num2str(this->DatNumber) + ".dat";
	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "DAT�t�@�C�����J���܂���ł���" + filename);
	}

	const int readbyte = fread( this->Dat ,sizeof(char), Index[this->No] ,fp);
	ASSERT (readbyte == Index[this->No] );
	fclose(fp);

	//�ŏI�X�V���������߂�.
	struct _stat statbuf;
	if ( _stat( filename.c_str() ,  &statbuf) == -1)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "stat �̎擾�Ɏ��s" + filename);
	}
	this->LastModified = statbuf.st_mtime;

	//�U�u�W�F�N�Ƃ̃��[�h.
	if (this->No >= 1)
	{
		this->Subject = YDATReader::Subject( this->Dat + this->Index[0] , this->Index[1] -  this->Index[0] );
	}
}


/**
 * ReadAt:			inStartNo �` inEndNo �܂œǂݍ���
 *
 * @param outRPack	�ǂݍ��񂾓��e���ۑ������o�b�t�@.
 * @param inStartNo �J�n�n�_
 * @param inEndNo	�I���n�_
 * @return bool
 */
bool YDAT::ReadAt(RPack * outRPack,unsigned int inStartNo,unsigned int inEndNo)  const 
{
	ASSERT(this->Dat[0] != '\0');

	if (inStartNo < 0) inStartNo = 0;
	if (inEndNo >= No) inEndNo = No;
	
	if (inStartNo >= inEndNo) return false;

	RAutoReadSpinLock al(&Lock) ;
	outRPack->Write(this->Dat + this->Index[inStartNo] , this->Index[inEndNo] -  this->Index[inStartNo]);

	return true;
}

/**
 * ReadAtGoEND:		inStartNo �` ALL �܂œǂݍ���
 *
 * @param outRPack	�ǂݍ��񂾓��e���ۑ������o�b�t�@.
 * @param inStartNo �J�n�n�_
 * @return bool 
 */
bool YDAT::ReadAtGoEND(RPack * outRPack,unsigned int inStartNo)  const 
{
	ASSERT(this->Dat[0] != '\0');

	return this->ReadAt(outRPack,inStartNo,No);
}

/**
 * ReadByte:		inStartByte �` inEndByte �܂œǂݍ���
 *
 * @param outRPack		�ǂݍ��񂾓��e���ۑ������o�b�t�@.
 * @param inStartByte	�J�n�o�C�g
 * @param inEndByte		�I���o�C�g
 * @return bool 
 */
bool YDAT::ReadByte(RPack * outRPack,DWORD inStartByte,DWORD inEndByte)  const 
{
	ASSERT(this->Dat[0] != '\0');

	if (inEndByte >= this->Index[No]) inEndByte = this->Index[No];
	if (inStartByte >= inEndByte) return false;

	RAutoReadSpinLock al(&Lock) ;
	outRPack->Write(this->Dat + inStartByte,inEndByte - inStartByte);

	return true;
}

/**
 * ReadByteGoEND:		inStartByte �` ALL �܂œǂݍ���
 *
 * @param outRPack		�ǂݍ��񂾓��e���ۑ������o�b�t�@.
 * @param inStartByte	�J�n�o�C�g
 * @return bool 
 */
bool YDAT::ReadByteGoEND(RPack * outRPack,int inStartByte)  const 
{
	ASSERT(this->Dat[0] != '\0');

	return this->ReadByte(outRPack,inStartByte,this->Index[No]);
}

/**
 * Append:				��������
 *
 * @param	inRPack		�������ރf�[�^
 */
void YDAT::Append(const RPack * inRPack) throw(RIOCannotWriteException)
{
	RAutoWriteSpinLock al(&Lock) ;

	if (No >= MAX_NUMBER) throw RIOCannotWriteException(EXCEPTIONTRACE + "����ȏ㏑�����݂ł��܂���");
	if (this->Index[No]+inRPack->getSize() >= MAX_DAT_SIZE) throw RIOCannotWriteException(EXCEPTIONTRACE + "�ő�e�ʂ��I�[�o�[���Ă��܂�");

	DWORD start = this->Index[No];
	memcpy ( &this->Dat[start] , inRPack->getMemory() , inRPack->getSize() );
	this->Index[No+1] = 0 + start + inRPack->getSize();

	this->No++;

	this->LastModified = time(NULL);
}

/**
 * Append:				��������(Admin ���_���E�l�ʒu�܂�)
 *
 * @param	inRPack		�������ރf�[�^
 */
void YDAT::AppendAdmin(const RPack * inRPack) throw(RIOCannotWriteException)
{
	RAutoWriteSpinLock al(&Lock) ;

	if (No >= MAX_NUMBER_FULL-1) throw RIOCannotWriteException(EXCEPTIONTRACE + "����ȏ㏑�����݂ł��܂���");
	if (this->Index[No]+inRPack->getSize() >= MAX_DAT_SIZE) throw RIOCannotWriteException(EXCEPTIONTRACE + "�ő�e�ʂ��I�[�o�[���Ă��܂�");

	DWORD start = this->Index[No];
	memcpy ( &this->Dat[start] , inRPack->getMemory() , inRPack->getSize() );
	this->Index[No+1] = 0 + start + inRPack->getSize();

	this->No++;

	this->LastModified = time(NULL);
}


/**
 * Overwrite:				�I�[�o�[���C�h
 *
 * @param inDeleteNo 
 * @param inOverwrite 
 * @return void 
 */
void YDAT::Overwrite(int inDeleteNo , const RPack * inOverwrite) throw(RIOCannotWriteException)
{
	RAutoWriteSpinLock al(&Lock) ;

	if(inDeleteNo < 0) throw RIOCannotWriteException(EXCEPTIONTRACE + "�㏑������͈͂��ςł�");
	if (this->No < inDeleteNo)throw RIOCannotWriteException(EXCEPTIONTRACE + "�㏑������͈͂��ςł�");

	//�I���W�i���̕�����̒���
	DWORD start = this->Index[inDeleteNo];
	DWORD next = this->Index[inDeleteNo+1];
	DWORD orinalLen = next - start;

	//�㏑��������̒���
	DWORD OverwriteLen = inOverwrite->getSize();

	//���炵�܂��B
	memmove( this->Dat + start + OverwriteLen , this->Dat + next , this->Index[this->No] - next );
	//�㏑�����܂��B
	memcpy(this->Dat + start , inOverwrite->getMemory() , OverwriteLen);

	//index���v�Z���Ȃ���.
	DWORD sa;
	if (OverwriteLen > orinalLen)
	{
		//���Ƃ�葝����̂ō������v���X���܂��B
		sa = OverwriteLen - orinalLen;
		//���͗e�ʃI�[�o�[�Ƃ����Ă܂�����?
		if (this->Index[this->No] + sa >= MAX_DAT_SIZE)
		{
			throw RIOCannotWriteException(EXCEPTIONTRACE + "DAT�̗e�ʂ��t���Ȃ̂ŁA�㏑���ł��܂���ł���");
		}

		for(int i = inDeleteNo+1 ; i <= this->No ; i ++)
		{
			this->Index[i] += sa;
		}
	}
	else
	{
		//���Ƃ�茸��̂ō������}�C�i�X���܂��B
		sa = orinalLen - OverwriteLen;
		for(int i = inDeleteNo+1 ; i <= this->No ; i ++)
		{
			this->Index[i] -= sa;
		}
	}
	
}

/**
 * IndexLoad:		INDEX�t�@�C���̏�������
 *
 * @throw		RIOCannotOpenException	�t�@�C�����܂���.
 * @return void 
 */
void YDAT::IndexSave(const string inBoardName) const  throw(RIOCannotOpenException) 
{
	if (this->No <= 0)  return;

	string directory =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\";
	MakeSureDirectoryPathExists(directory.c_str());
	string filename = directory + num2str(this->DatNumber) + ".idx";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "index�t�@�C�����J���܂���ł���" + filename);
	}

	fwrite(this->Index,sizeof(DWORD), this->No + 1,fp);

	fclose(fp);

}

/**
 * DATLoad:		DAT�t�@�C���̏�������
 *
 * @throw		RIOCannotOpenException	�t�@�C�����܂���.
 * @return void 
 */
void YDAT::DATSave(const string inBoardName) const throw(RIOCannotOpenException)  
{
	ASSERT(this->Dat[0] != '\0');
	if (this->No <= 0)  return;

	string directory =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\dat\\";
	MakeSureDirectoryPathExists(directory.c_str());
	string filename = directory + num2str(this->DatNumber) + ".dat";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "DAT�t�@�C�����J���܂���ł���" + filename);
	}

	const int writebyte = fwrite( this->Dat ,sizeof(char), Index[this->No] ,fp);
	ASSERT (writebyte == Index[this->No] );
	fclose(fp);

	struct _utimbuf utimebuf;
	utimebuf.actime  = this->LastModified ;
	utimebuf.modtime = this->LastModified  ;
	_utime( filename.c_str() , &utimebuf );


}


void YDAT::test()
{
	//��񂾂��e�X�g
	//�������݂Ă���.
	{
		YDAT * dat = new YDAT(1234567);
		{
			RPack pack;
			pack.Write("���ꂪ������!!",sizeof("���ꂪ������!!") );
			dat->Append(&pack);
			ASSERT(dat->No == 1);
			ASSERT(dat->Index[dat->No] == sizeof("���ꂪ������!!") );
		}
		dat->Save("test");
		delete dat;
	}
	//��݂��݂Ă���
	{
		YDAT * dat = new YDAT(1234567);
		dat->IndexLoad("test");
		ASSERT( dat->No == 1 );
		ASSERT(dat->Index[dat->No] == sizeof("���ꂪ������!!") );
		dat->DATLoad("test");
		ASSERT( dat->No == 1 );

		{
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "���ꂪ������!!" , pack.getSize() ) == 0 );
		}
		delete dat;
	}

	//3��e�X�g
	//�������݂Ă���.
	{
		YDAT * dat = new YDAT(1234567);
		{
			RPack pack;
			pack.Write("���ꂪ������!!",sizeof("���ꂪ������!!") );
			dat->Append(&pack);
			ASSERT(dat->Index[dat->No] == sizeof("���ꂪ������!!") );

			pack.Write("�N������������!!",sizeof("�N������������!!") );
			dat->Append(&pack);
			ASSERT(dat->Index[dat->No] - dat->Index[dat->No - 1] == sizeof("�N������������!!") );

			pack.Write("�ʕ񂵂܂���",sizeof("�ʕ񂵂܂���") );
			dat->Append(&pack);
			ASSERT(dat->Index[dat->No] - dat->Index[dat->No - 1] == sizeof("�ʕ񂵂܂���") );
			ASSERT(dat->No == 3);
		}
		dat->IndexSave("test");
		dat->DATSave("test");
		delete dat;
	}
	//��݂��݂Ă���
	{
		YDAT * dat = new YDAT(1234567);
		dat->IndexLoad("test");
		ASSERT( dat->No == 3 );
		ASSERT(dat->Index[1] == sizeof("���ꂪ������!!") );
		ASSERT(dat->Index[2] - dat->Index[1] == sizeof("�N������������!!") );
		ASSERT(dat->Index[3] - dat->Index[2] == sizeof("�ʕ񂵂܂���") );

		dat->DATLoad("test");
		ASSERT( dat->No == 3 );

		{
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "���ꂪ������!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "�N������������!!" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "���ꂪ������!!\0�N������������!!\0�ʕ񂵂܂���" , pack.getSize() ) == 0 );
			
		}
		delete dat;
	}

	
	//�㏑���e�X�g.
	{
		YDAT * dat = new YDAT(1234567);
		dat->IndexLoad("test");
		ASSERT( dat->No == 3 );
		ASSERT(dat->Index[1] == sizeof("���ꂪ������!!") );
		ASSERT(dat->Index[2] - dat->Index[1] == sizeof("�N������������!!") );
		ASSERT(dat->Index[3] - dat->Index[2] == sizeof("�ʕ񂵂܂���") );

		dat->DATLoad("test");
		ASSERT( dat->No == 3 );

		{
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "���ꂪ������!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "�N������������!!" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( (const char*)pack.getMemory()  , "���ꂪ������!!\0�N������������!!\0�ʕ񂵂܂���" , pack.getSize() ) == 0 );
		}
		//�㏑�����s 2�Ԗڂ��㏑��. ���Ƃ�菬�����㏑��
		{
			RPack overwrite;
			overwrite.Write("���ہ[" , sizeof("���ہ["));
			dat->Overwrite( 1 , &overwrite);

			const char* r;
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "���ꂪ������!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "���ہ[" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "���ꂪ������!!\0���ہ[\0�ʕ񂵂܂���" , pack.getSize() ) == 0 );

		}
		//�㏑�����s 2�Ԗڂ�����ɏ㏑��. ���Ƃ��ł����㏑��
		{
			RPack overwrite;
			overwrite.Write("�E�G�[�n�b�n�b" , sizeof("�E�G�[�n�b�n�b"));
			dat->Overwrite( 1 , &overwrite);

			const char* r;
			RPack pack;
			dat->ReadAt( &pack , 0 , 0+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "���ꂪ������!!" , pack.getSize() ) == 0) ;
			dat->ReadAt( &pack , 1 , 1+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "�E�G�[�n�b�n�b" , pack.getSize() ) == 0 );
			dat->ReadAt( &pack , 2 , 2+1 );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );


			dat->ReadAtGoEND( &pack , 2  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "�ʕ񂵂܂���" , pack.getSize() ) == 0 );
			dat->ReadAtGoEND( &pack , 0  );
			ASSERT( memcmp( r = (const char*)pack.getMemory()  , "���ꂪ������!!\0�E�G�[�n�b�n�b\0�ʕ񂵂܂���" , pack.getSize() ) == 0 );

		}
		delete dat;
	}
	puts("YDAT::test() OK");
}

