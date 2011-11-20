// RGISJoin.cpp: RGISJoin �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISJoin.h"
#include "RStdioFile.h"
#include "RConv.h"
#include "RStdioMemory.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISJoin::RGISJoin()
{

}

RGISJoin::~RGISJoin()
{

}

//��������
void RGISJoin::Join(const string & inDir , const string &inMapArcFilename ,
					const JoinTempFormatScript* inScript, int inSize,
					const JoinHeadFormatScript* inHead , int inHeadSize)
{
	//ZLIB����!
	Zlib.Create();

	//���ׂẴt�@�C�������X�g�̎擾.
	RStringVector List;
	GetAllFilenameList(&List , inDir,inScript,inSize);

	int n;

	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};

	unsigned long indexSize = List.size();
	IndexArea * index = new IndexArea[ indexSize ];

	{
		RStringVector::iterator i = List.begin();
		RStringVector::iterator e = List.end();
		for(n = 0 ; i != e ; i++ , n++)
		{
			index[n].Block = ::_atoi64( (*i).c_str() );
			index[n].Possition = 0;
		}
	}
	//�ł͏������݊J�n�ƁB
	RStdioFile master;
	master.Open(inMapArcFilename , "wb" );

	//�܂��A�V���{��
	string simbol = "KMAP";
	master.fwrite( simbol.c_str() , simbol.size() );
	//�o�[�W����
	unsigned char Version = 1;
	master.fwrite( &Version , sizeof(Version) );

	//index�̐� ���������݂܂��B
	master.fwrite( &indexSize , sizeof(indexSize) );

	unsigned long indexPos = master.ftell();
	//�Ƃ肠�����A�K���� index ���������݂܂��B (���ƂŐ������l�������܂�)
	master.fwrite( index , sizeof(IndexArea) * indexSize);

	//�w�b�_�̏�������.
	WriteHeader(&master ,inDir, inHead , inHeadSize );


	//inde�ɂ�����Ă���f�[�^�������Ă����܂��B
	for(n = 0 ; n <  List.size() ; n++)
	{
		//���̃u���b�N�̊J�n�ʒu�����߂�..
		index[n].Possition = master.ftell();

	
		//���k���邽�߂Ƀf�[�^������e���|����.
		RStdioFile zfile;
		string zPath = inDir + "/temp/zipping";
		zfile.Open(zPath , "wb");

		//�œK�����邽�߂ɗ��p����t�@�C��.
		RStdioFile optimizfile;
		string optimizPath = inDir + "/temp/optimiz";
		optimizfile.Open(optimizPath , "w+b");

		for(int k = 0 ; k < inSize ; k++)
		{
			const JoinTempFormatScript * script = &inScript[k];
			string path = inDir + "/temp/" + script->TempFileDirectoryName + "/" + num2str(index[n].Block) ;
			path = RConv::Replace(path , "/" , "\\");

			printf("�^�[�Q�b�g %s\n" , path.c_str() );
			
			try
			{
				RStdioFile file;
				file.Open(path , "rb");

				unsigned long dataSize = file.getSize();
				//�܂������𐔂��܂�
				unsigned long count = this->GetDataCount( &file , script , dataSize);

				//���L�f�[�^��O�ɏW�߂čœK������.
				dataSize = Optimize(&file , &optimizfile , script , count);

				//�V���{���̏�������
				zfile.fwrite( &script->BlockStartSimbol , sizeof(unsigned char) );
				//�T�C�Y�̏�������
				zfile.fwrite( &dataSize , sizeof(dataSize) );
				//�����̏�������
				zfile.fwrite( &count , sizeof(count) );
				//���g���ۂ��ƃR�s�[
				FileAppend( &zfile , &optimizfile , dataSize);
			}
			catch(RException e)
			{
				printf("�����������ɃG���[ : %s\n" , e.getMessage() );
				continue;
			}
		}
		zfile.Close();
		zfile.Open(zPath , "rb");

		//���k���āA����� master �ɒǉ����܂�.
		ZFileAppend(&master , &zfile);
	}

	//�ڎ����X�V���܂��B
	//�ڎ��ɖ߂��āA�㏑���Ȃ̂ł���[
	master.fseek(indexPos);
	master.fwrite( index , sizeof(IndexArea) * indexSize);

	delete [] index;
}


//�w�b�_�[���������݂܂��B
void RGISJoin::WriteHeader(RStdioFile * ioMaster , const string & inDir ,
					const JoinHeadFormatScript* inHead , int inHeadSize)
{

	//�w�b�_�̃T�C�Y��K���ɏ������݂܂��B (���ƂŐ������l�������܂�)
	unsigned long headerPos = ioMaster->ftell();
	unsigned long headerHeaderSize = 0;
	ioMaster->fwrite( &headerHeaderSize , sizeof(headerHeaderSize));

	//���Ƀw�b�_�����������݂܂��B
	for(int n = 0; n < inHeadSize ; n++)
	{
		const JoinHeadFormatScript * head = &inHead[n];

		string path = inDir + "/temp/" + head->TempFilename ;
		path = RConv::Replace(path , "/" , "\\");

		try
		{
			RStdioFile file;
			file.Open(path , "rb");

			unsigned long dataSize = file.getSize();
			unsigned long count = GetHeadDataCount( &file , head , dataSize);

			//�V���{���̏�������
			ioMaster->fwrite( &head->BlockStartSimbol , sizeof(unsigned char) );
			//�T�C�Y�̏�������
			ioMaster->fwrite( &dataSize , sizeof(dataSize) );
			//�����̏�������
			ioMaster->fwrite( &count , sizeof(count) );
			//master�Ɏ��g���ۂ��ƃR�s�[
			FileAppend( ioMaster , &file , dataSize);
		}
		catch(RException e)
		{
			printf("�����������ɃG���[ : %s\n" , e.getMessage() );
			continue;
		}
			
	}
	//�w�b�_�̐������l���������݂܂��B
	headerHeaderSize = ioMaster->ftell() - headerPos;

	ioMaster->fseek(headerPos);
	ioMaster->fwrite( &headerHeaderSize , sizeof(headerHeaderSize));

	//�I�[�ɖ߂��܂��B
	ioMaster->fseek(0,SEEK_END);
}


//master�Ɏ��g���ۂ��ƃR�s�[
void RGISJoin::FileAppend( RStdioFile * ioMaster , RStdioFile * ioFile , unsigned long inSize)
{
	//�擪�ɖ߂���
	ioFile->fseek(0);

	char * buffer = new char[inSize];

	ioFile->fread(buffer , inSize);
	ioMaster->fwrite(buffer , inSize);

	delete [] buffer;
}


//master�Ɉ��k�����������g���R�s�[
void RGISJoin::ZFileAppend( RStdioFile * ioMaster , RStdioFile * ioZFile)
{
	//���k���āA����� master �ɒǉ����܂�.
	unsigned long zsizeSrc = ioZFile->getSize();
	unsigned long zsizeDest = zsizeSrc * 1.5;	//�܂��A 1.5�{�ȏ�ɂ͂Ȃ�Ȃ��ł��傤��(w
	//���k�ɕK�v�ȃ��������m��.
	unsigned char * src = new unsigned char[zsizeSrc];			//�܂��A����Ȃɂł����͂Ȃ�Ȃ����� OnMemory�ő��v(w
	unsigned char * dest = new unsigned char[zsizeDest];		

	//���ׂēǂ�.
	ioZFile->fseek(0);
	ioZFile->fread(src , zsizeSrc);

	try
	{
		//���k!
		Zlib.Compress2(dest , &zsizeDest , src , zsizeSrc ,RZlib.BEST_COMPRESSION);

		printf("zip.. %d --> %d   (%d%%)\n" , zsizeSrc , zsizeDest , zsizeDest * 100 / zsizeSrc);

#ifdef _DEBUG
		//�f�o�b�O�̂Ƃ������L��...
		//�{���ɂ��܂������Ă���̂����ۂɉ𓀂��ă`�F�b�N���Ă݂�.
		try
		{
			//���ɖ߂��Ă݂�.
			unsigned long orignalSize = zsizeSrc;
			unsigned char * orignal = new unsigned char[orignalSize];
			Zlib.UnCompress(orignal , &orignalSize , dest , zsizeDest );

			//�T�C�Y�͂�������?
			ASSERT(orignalSize == zsizeSrc);
			ASSERT(memcmp(orignal , src , orignalSize) == 0 );

			delete [] orignal;
		}
		catch(RException e)
		{
			ASSERT(0);
		}
#endif

		//�}�X�^�[�ɃR�s�[.
		ioMaster->fwrite(&zsizeSrc , sizeof(zsizeSrc) );		//�܂����k�O�̗e��
		ioMaster->fwrite(dest , zsizeDest);						//���Ɏ���.
	}
	catch(RException e)
	{
		printf("���k���ɃG���[ : %s\n" , e.getMessage() );
		ASSERT(0);
	}

	//�������J��.
	delete [] dest;
	delete [] src;
}

//������t�H�[�}z�b�g�ŋL�^����Ă���t�@�C����
//�����𐔂��܂�
unsigned long RGISJoin::StringCount(RStdioFile * ioFile , unsigned long inSize)
{
	//�擪�ɖ߂���
	ioFile->fseek(0);

	unsigned long count;

	//�Ƃ肠�����A�S���ǂ�
	char* buffer = new char[inSize + 1];
	ioFile->fread(buffer , inSize);

	int addpoint = 0;
	for(count = 0 ;  ; count++)
	{
		const char * name = buffer + addpoint ;
		//���̕�����.
		addpoint += strlen(name) + 1;	//+1�� \0�̕�.
		if (addpoint >= inSize)
		{
			count ++;
			break;	//����ȏ�Ȃ��̂ł��B
		}
	}
	delete [] buffer;

	return count;
}


//������t�H�[�}z�b�g�ŋL�^����Ă���t�@�C����
//�����𐔂��܂�
unsigned long RGISJoin::FixedAndStringCount(RStdioFile * ioFile , unsigned long inSize , int inFixedSize)
{
	//�擪�ɖ߂���
	ioFile->fseek(0);

	unsigned long count;

	//�Ƃ肠�����A�S���ǂ�
	char* buffer = new char[inSize + 1];
	ioFile->fread(buffer , inSize);

	int addpoint = inFixedSize;
	for(count = 0 ;  ; count++)
	{
		const char * name = buffer + addpoint ;
		//���̕�����.
		addpoint += strlen(name) + 1 + inFixedSize;	//+1�� \0�̕�.
		if (addpoint >= inSize)
		{
			count ++;
			break;	//����ȏ�Ȃ��̂ł��B
		}
	}
	delete [] buffer;

	return count;
}

//�����𐔂��܂�
unsigned long RGISJoin::GetHeadDataCount(RStdioFile * ioFile , const JoinHeadFormatScript* inScript , unsigned long inSize) 
{
	unsigned long count;

	//�擪�ɖ߂���
	ioFile->fseek(0);

	if (inScript->HeadType == HeadTypeFixedAndString)
	{//������
		count = FixedAndStringCount(ioFile , inSize , inScript->DataSize);
	}
	else if (inScript->HeadType == HeadTypeFixed)
	{//�Œ�f�[�^ 
		count = inSize / inScript->DataSize;
	}
	else
	{
		ASSERT(0);
	}
	return count;
}

//�����𐔂��܂�
unsigned long RGISJoin::GetDataCount(RStdioFile * ioFile , const JoinTempFormatScript* inScript , unsigned long inSize) 
{
	unsigned long count;

	//�擪�ɖ߂���
	ioFile->fseek(0);

	if (inScript->CVType == CVTypeString)
	{//������
		count = StringCount(ioFile , inSize);
	}
	else
	{//�ʏ�f�[�^

//		unsigned char cvSize;
//		unsigned char ndSize;
		for(count = 0;  ioFile->ftell() < inSize ; count ++)
		{
			DataSkip(ioFile ,inScript );
		}
	}
	return count;
}

/*

//�œK��
DWORD RGISJoin::Optimize(RStdioFile *  ioReadFile , RStdioFile * ioWriteFile , const JoinTempFormatScript* inScript , int inCount) 
{
	//�擪�ɖ߂���
	ioReadFile->fseek(0);
	ioWriteFile->fseek(0);

	//���̂Ƃ��� CVTypeMultiPoint �`���ȊO�́A���̈��k�������
	//�t�ɂł����Ȃ��Ă����܂����߉����s��Ȃ�.
	if (inScript->CVType == CVTypeNone || inScript->CVType == CVTypeString || inScript->CVType == CVTypeByteData)
	{
		FileAppend(ioWriteFile , ioReadFile , ioReadFile->getSize() );
		return ioReadFile->getSize();
	}


	char * mainData = new char[inScript->DataSize];
	char * mainDataWork = new char[inScript->DataSize];

	char * copyWorkBuffer = new char[65535];

	//����������ł��邩�ǂ������ׂ邽�߂̃e�[�u��.
	bool * checkTable =  new bool[inCount];
	memset(checkTable , 0 , sizeof(bool) * inCount);

	for(int ori = 0 ; ori < inCount ; ori ++)
	{
		DWORD starttell = ioReadFile->ftell();

		DWORD skip = DataSkip(ioReadFile ,inScript , mainData);

		ASSERT(ioReadFile->ftell() - skip == starttell);

		DWORD firstBlockEnd = ioReadFile->ftell() ;

		unsigned char identityDataCount = 0;

		//���ɏ�����.
		if ( checkTable[ori] ) continue;

		//���L�w�b�_����
		ioWriteFile->fwrite(mainData , inScript->DataSize  );
		DWORD identityDataCountTell = ioWriteFile->ftell();
		//�� 0���Ǝ����̂�
		ioWriteFile->fwrite(&identityDataCount , sizeof(identityDataCount)  );
		//�����̃f�[�^�����̏�������
		ioReadFile->fseek(starttell + inScript->DataSize);
		ioReadFile->fread(copyWorkBuffer , skip - inScript->DataSize);
		ioWriteFile->fwrite(copyWorkBuffer , skip - inScript->DataSize  );

		ASSERT(ioReadFile->ftell() == firstBlockEnd );

		checkTable[ori] = true;

		//���̃f�[�^�Ɠ����f�[�^�͂���܂���?
		for(int des = ori + 1 ; des < inCount ; des ++ )
		{
			ASSERT(! ioReadFile->IsEof() );

			DWORD starttell2 = ioReadFile->ftell();
			DWORD skip2 = DataSkip(ioReadFile ,inScript , mainDataWork);
			ASSERT(ioReadFile->ftell() - skip2 == starttell2);
			DWORD firstBlockEnd2 = ioReadFile->ftell() ;

			//���ɏ�����.
			if ( checkTable[des] ) continue;

			//�����f�[�^�ł���?
			if (memcmp(mainData ,mainDataWork , inScript->DataSize ) != 0)	continue;

			//�ł́A���L���܂��傤.
			identityDataCount ++ ;

			//�����̃f�[�^�����̏�������
			ioReadFile->fseek(starttell2 + inScript->DataSize);
			ioReadFile->fread(copyWorkBuffer , skip2 - inScript->DataSize);
			ioWriteFile->fwrite(copyWorkBuffer , skip2 - inScript->DataSize  );

			ASSERT(ioReadFile->ftell() == firstBlockEnd2 );

			checkTable[des] = true;

			//�܂Ƃ߂��鐔�̏���𒴂�����I���
			if (identityDataCount >= 255) break;
		}
		//identityDataCount �ɋ��L�����l�������Ă���͂��Ȃ̂ŁA
		//������������݂܂�.
		DWORD endPos = ioWriteFile->ftell();
		ioWriteFile->fseek(identityDataCountTell);
		ioWriteFile->fwrite(&identityDataCount , sizeof(identityDataCount)  );
		//�Ō���Ɉړ�.
		ioWriteFile->fseek(endPos);

		//��� des �̃��[�v�ňړ��������Ƃ��Ȃ��������Ƃɂ��܂�.
		ioReadFile->fseek(firstBlockEnd);
	}

	DWORD starttell2 = ioReadFile->ftell();

	delete [] checkTable;
	delete [] copyWorkBuffer;
	delete [] mainDataWork;
	delete [] mainData;

	printf("optimize --> %s %d -> %d\r\n" ,inScript->TempFileDirectoryName.c_str() , 
		ioReadFile->getSize() , ioWriteFile->ftell() );
	return ioWriteFile->ftell();	//�T�C�Y��Ԃ�.

}
*/

//�œK��
DWORD RGISJoin::Optimize(RStdioFile *  ioReadFile , RStdioFile * ioWriteFile , const JoinTempFormatScript* inScript , int inCount) 
{
	//�擪�ɖ߂���
	ioReadFile->fseek(0);
	ioWriteFile->fseek(0);

	//���̂Ƃ��� CVTypeMultiPoint �`���ȊO�́A���̈��k�������
	//�t�ɂł����Ȃ��Ă����܂����߉����s��Ȃ�.
	if (inScript->CVType == CVTypeNone || inScript->CVType == CVTypeString || inScript->CVType == CVTypeByteData)
	{
		FileAppend(ioWriteFile , ioReadFile , ioReadFile->getSize() );
		return ioReadFile->getSize();
	}

	//���L�f�[�^������ꏊ.
	char		mainData[16];
	memset(mainData , 0 , 16);

	//���L����Ȃ��ʂ̃f�[�^��ǂݎ���ƃo�b�t�@.
	RBuffer unShareWriteBufferVector;
	unShareWriteBufferVector.reserve(65535);
	char * unShareWriteBuffer = &unShareWriteBufferVector[0];

	//���ʂɂł������ȂƂ�����s�b�N�A�b�v.
	typedef newVector<OptimizeTemp> OptimizeDatabase;
	OptimizeDatabase db;

	for(int ori = 0 ; ori < inCount ; ori ++)
	{
		DWORD starttell = ioReadFile->ftell();

		DWORD skip = DataSkip(ioReadFile ,inScript , mainData);

		ASSERT(ioReadFile->ftell() - skip == starttell);

		OptimizeDatabase::iterator i = db.begin();
		OptimizeDatabase::iterator e = db.end();
		for( ; i != e ;  ++i)
		{
			if ( memcmp( (*i)->Share , mainData , 16) == 0)
			{
				//����̂Ŋ����̂ɒǉ�.
				(*i)->Pos.push_back(starttell);
				break;
			}
		}
		if (i == e)
		{
			//�Ȃ��̂ŐV�K�쐬.
			OptimizeTemp* p = new OptimizeTemp;
			memcpy( (void*) (p->Share) , mainData , 16);
			p->Pos.push_back(starttell);

			db.push_back(p);
		}
	}
	for ( OptimizeDatabase::iterator opti = db.begin() ;  opti != db.end() ; ++opti)
	{
		const vector<DWORD> * sharePos = &((*opti)->Pos);
		int shareCount = sharePos->size();
		vector<DWORD>::const_iterator bi = sharePos->begin();

		ASSERT(shareCount >= 1);

		//���L������ 255 �Â��������Ȃ��̂ŁA
		//����ȏ㋤�L���Ă��镔���́A���� �u���b�N�ɏ�������ł����܂�.
		do
		{
			//���L����.
			ioWriteFile->fwrite( (*opti)->Share , inScript->DataSize  );
			//���L��
			const int temp = (shareCount < 257 ? shareCount : 256) - 1;
			ASSERT(temp >= 0 && temp <= 255);
			const unsigned char writeShareCount = temp;
			ioWriteFile->fwrite( &writeShareCount , sizeof(writeShareCount)  );

			for(int i = 0 ; i <= writeShareCount  ; ++bi , ++i)
			{
				DWORD starttell = *bi;
				//���L����Ȃ��ʂ̃f�[�^�����߂܂�.
				ioReadFile->fseek(starttell);
				DWORD skip = DataSkip(ioReadFile ,inScript , NULL);
				ioReadFile->fseek(starttell + inScript->DataSize);

				//���L����Ȃ��ʂ̃f�[�^��ǂݎ��A�t�@�C���ɏ�������.
				ioReadFile->fread(unShareWriteBuffer , skip - inScript->DataSize);
				ioWriteFile->fwrite(unShareWriteBuffer , skip - inScript->DataSize);
			}

			shareCount = shareCount - writeShareCount - 1;
			ASSERT(shareCount >= 0);
		}
		while(shareCount > 0);
	}

	printf("optimize --> %s %d -> %d\r\n" ,inScript->TempFileDirectoryName.c_str() , 
		ioReadFile->getSize() , ioWriteFile->ftell() );
	return ioWriteFile->ftell();	//�T�C�Y��Ԃ�.

}

DWORD RGISJoin::DataSkip(RStdioFile * ioFile , const JoinTempFormatScript* inScript , char * outMainData )
{
	unsigned char cvSize;
	unsigned char ndSize;

	DWORD tell = ioFile->ftell();

	if (outMainData == NULL)
	{
		//���C���f�[�^�̓ǂݔ�΂�
		ioFile->fseek(inScript->DataSize , SEEK_CUR);
	}
	else
	{
		ioFile->fread(outMainData , inScript->DataSize);
	}

	//�T�u�f�[�^ (�ϓ��f�[�^)�̓ǂݔ�΂�.
	ioFile->fseek(inScript->DataSizeSub , SEEK_CUR);

	//�m�[�h������ꍇ�̓m�[�h�f�[�^�̓ǂݔ�΂�.
	if (inScript->WithNode)
	{
		//�m�[�h��ǂݔ�΂�.
		ioFile->fread(&ndSize , sizeof(ndSize));
		//���ꂾ���̓ǂݔ�΂�.
		ioFile->fseek(sizeof(unsigned char) * ndSize,SEEK_CUR);
	}

	if (inScript->CVType == CVTypeOnePoint)
	{
		// x , y �̃f�[�^�̕��̓ǂݔ�΂�.
		ioFile->fseek(sizeof(unsigned short) * 2,SEEK_CUR);
	}
	else if (inScript->CVType == CVTypeMultiPoint)
	{
		//CV���̓ǂݍ���
		ioFile->fread(&cvSize , sizeof(cvSize));

		ASSERT(cvSize > 0);

		//���ꂾ���̓ǂݔ�΂�.
		//�܂� unsigned short * 2 �̐擪����.
		ioFile->fseek(sizeof(unsigned short) * 2 ,SEEK_CUR);
		//�c��� char *2 �̍����f�[�^�ł�.
		ioFile->fseek(sizeof(char) * 2 * (cvSize-1),SEEK_CUR);
	}
	else if (inScript->CVType == CVTypeByteData)
	{
		// 1byte �̃f�[�^�̕��̓ǂݔ�΂�.
		ioFile->fseek(sizeof(unsigned char),SEEK_CUR);
	}

	return ioFile->ftell() - tell;
}

void RGISJoin::GetAllFilenameList(RStringVector * inList , const string & inDir ,const JoinTempFormatScript* inScript, int inSize)
{
	for(int i = 0 ; i < inSize ; i++)
	{
		string path = inDir + "/temp/" + inScript[i].TempFileDirectoryName + "/*.*";
		path = RConv::Replace(path , "/" , "\\");
		
		WIN32_FIND_DATA data;
		HANDLE handle = ::FindFirstFile(path.c_str() , &data );
		if (handle == INVALID_HANDLE_VALUE )
		{
			continue ;
		}
		do
		{
			if ( !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				//�܂��o�^����Ă��Ȃ����O��������o�^����
				RStringVector::iterator i = find(inList->begin() , inList->end() , data.cFileName );
				if (i == inList->end())		inList->push_back(data.cFileName);

			}
		}
		while( ::FindNextFile(handle , &data) );

		::FindClose(handle);
	}
	sort(inList->begin() , inList->end() );
}

//�n�}�t�@�C�������Ă��Ȃ����������̃`�F�b�N.
void RGISJoin::Check(const string & inMasterFilename) throw (RException)
{
	//ZLIB����!
	Zlib.Create();

	RStdioFile file;
	file.Open(inMasterFilename , "rb" );

	char buffer[MAX_PATH];
	int i;

	unsigned long fileSize = file.getSize();

	//�w�b�_�[�̓ǂݍ���
	file.fread( buffer , 4 );
	if (!(buffer[0] == 'K' && buffer[1] == 'M' && buffer[2] == 'A' && buffer[3] == 'P'))
	{
		throw RException(EXCEPTIONTRACE + "�������`�F�b�N�G���[:�擪��KMAP�̃v���t�B�b�N�X������܂���");
	}

	//�o�[�W�����̓ǂݍ���.
	unsigned char version;
	file.fread( &version , sizeof(version) );
	if (version != 1)
	{
		throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:���m�̃o�[�W�����ł� %d" , (int)version) );
	}

	//index���̓ǂݍ���
	unsigned long indexCount = 0;
	file.fread( &indexCount , sizeof(indexCount) );

	//index�̓ǂݍ���.
	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};
	IndexArea* index = new IndexArea[indexCount];
	file.fread( index , sizeof(IndexArea) * indexCount);

	try
	{
		//�e�u���b�N�̍\���`�F�b�N.
		unsigned char simbol;				// D �Ȃǂ̃f�[�^�V���{��
		unsigned long dataSize;				// ���̃f�[�^�̒���(bytes) �w�b�_������.
		unsigned long dataCount;			// �f�[�^�̂悤���̐�
		for(i = 0 ; i < indexCount ; i++)
		{
			printf("�ԍ� %d �u���b�N�ԍ� %ld �u���b�N�� %ld \n" , i , index[i].Block , index[i].Possition);

			unsigned long blockStart = index[i].Possition;
			unsigned long blockSize ;
			if (i+1 == indexCount)
			{
				blockSize =  fileSize - blockStart;	//�Ō�̃u���b�N�̓t�@�C���I�[�܂ő����Ă���.
			}
			else
			{
				blockSize =  index[i + 1].Possition - blockStart;
			}
			file.fseek( blockStart );

			//�܂��A���k�f�[�^���𓀂��܂�.
			unsigned long fullSize;				//unzip����Ɛ��鐳���ȑ傫��.
			file.fread( &fullSize , sizeof(fullSize) );

			unsigned char * dest = new unsigned char[fullSize];
			unsigned char * src = new unsigned char[blockSize];

			//�u���b�N�T�C�Y�́A���k�O�T�C�Y���L�^���Ă��镔�����܂ނ��� �����Z���܂�
 			file.fread( src , blockSize  - sizeof(fullSize) );	
			try
			{
				//�𓀂̎��s.
				unsigned long realSize = fullSize;
				Zlib.UnCompress(dest , &realSize , src , blockSize);
				ASSERT(realSize == fullSize);

				//�܂��A�����g��Ȃ��\�[�X�̈�̔j��.
				delete [] src;	src  = NULL;

				//�𓀂����f�[�^�̉��.
				RStdioMemory mem;
				mem.Open((char*)dest );
				while( mem.ftell() < realSize )
				{
					mem.fread( &simbol , sizeof(simbol) );
					mem.fread( &dataSize , sizeof(dataSize) );
					mem.fread( &dataCount , sizeof(dataCount) );

					printf("�V���{�� %c �f�[�^�� %ld �f�[�^�̐� %d \n" , simbol , dataSize , (int)dataCount);

					if ( ! ::isalpha(simbol) )
					{
						throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:�A���t�@�x�b�g�ȊO�̃V���{���𔭌� %c  �ʒu:%ld" , simbol , file.ftell() ) );
					}
					//�ǂݔ�΂�.
					mem.fseek( dataSize , SEEK_CUR);
				}
			}
			catch(RException e)
			{
				delete [] src;
				delete [] dest;

				ASSERT(0);
				throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:�𓀒��ɃG���[���������܂��� �u���b�N�ԍ� %ld �u���b�N�J�n�ʒu:%ld : %s" ,
						index[i].Block , blockStart , e.getMessage() ) );
			}
			delete [] src;
			delete [] dest;

			//�L�^����Ă���u���b�N���Ǝ��ۂ̒l���A�҂���Ƃ��������Ȃ���΂����Ȃ�
			if((file.ftell() - blockStart) != blockSize)
			{
				throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:�ڎ��������Ă���u���b�N��[%ld] �� ���ۂ̃u���b�N��[%ld]�������܂���. �u���b�N�ԍ� %ld �u���b�N�J�n�ʒu:%ld" ,
					blockSize ,  (file.ftell() - blockStart)  , index[i].Block , blockStart) );
			}
		}
		if ( file.ftell() != fileSize )
		{
			throw RException(EXCEPTIONTRACE + "�������`�F�b�N�G���[:�S���̃u���b�N���������Ă��ǂ蒅�����ꏊ���t�@�C���̏I�[�ł͂���܂���");
		}
	}
	catch(RException e)
	{
		delete [] index;
		throw;
	}
	delete [] index;
}


//�n�}�̏ڂ������𓾂�.
void RGISJoin::Info(const string & inMasterFilename) throw (RException)
{
	//ZLIB����!
	Zlib.Create();

	RStdioFile file;
	file.Open(inMasterFilename , "rb" );

	char buffer[MAX_PATH];
	int i;

	unsigned long fileSize = file.getSize();

	//�w�b�_�[�̓ǂݍ���
	file.fread( buffer , 4 );
	if (!(buffer[0] == 'K' && buffer[1] == 'M' && buffer[2] == 'A' && buffer[3] == 'P'))
	{
		throw RException(EXCEPTIONTRACE + "�������`�F�b�N�G���[:�擪��KMAP�̃v���t�B�b�N�X������܂���");
	}

	//�o�[�W�����̓ǂݍ���.
	unsigned char version;
	file.fread( &version , sizeof(version) );
	if (version != 1)
	{
		throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:���m�̃o�[�W�����ł� %d" , (int)version) );
	}

	//index���̓ǂݍ���
	unsigned long indexCount = 0;
	file.fread( &indexCount , sizeof(indexCount) );

	//index�̓ǂݍ���.
	struct IndexArea
	{
		unsigned long Block;
		unsigned long Possition;
	};
	IndexArea* index = new IndexArea[indexCount];
	file.fread( index , sizeof(IndexArea) * indexCount);

	RStdioFile outlog;
	outlog.Open("mapinfo.csv" , "w+b");

	string l = PrintFormat("�u���b�N�ԍ�,�u���b�N�S�̒���(byte),�u���b�N�ɐ�߂銄��,���k�ς݃u���b�N�S�̒���(byte),���k�ς݃u���b�N�ɐ�߂銄��,�V���{��,�f�[�^��(byte),�f�[�^�̐�,1�f�[�^�̕��σT�C�Y\n");
	outlog.fwrite(l.c_str() , l.size() );

	try
	{
		//�e�u���b�N�̍\���`�F�b�N.
		unsigned char simbol;				// D �Ȃǂ̃f�[�^�V���{��
		unsigned long dataSize;				// ���̃f�[�^�̒���(bytes) �w�b�_������.
		unsigned long dataCount;			// �f�[�^�̂悤���̐�
		unsigned long zipdataSize;			// ���̃u���b�N�����k�����Ƃ��̃T�C�Y.
		for(i = 0 ; i < indexCount ; i++)
		{
//			TRACE("�ԍ� %d �u���b�N�ԍ� %ld �u���b�N�� %ld \n" , i , index[i].Block , index[i].Possition);

			unsigned long blockStart = index[i].Possition;
			unsigned long blockSize ;
			if (i+1 == indexCount)
			{
				blockSize =  fileSize - blockStart;	//�Ō�̃u���b�N�̓t�@�C���I�[�܂ő����Ă���.
			}
			else
			{
				blockSize =  index[i + 1].Possition - blockStart;
			}
			file.fseek( blockStart );

			//�܂��A���k�f�[�^���𓀂��܂�.
			unsigned long fullSize;				//unzip����Ɛ��鐳���ȑ傫��.
			file.fread( &fullSize , sizeof(fullSize) );

			unsigned char * dest = new unsigned char[fullSize];
			unsigned char * src = new unsigned char[blockSize];

			//�u���b�N�T�C�Y�́A���k�O�T�C�Y���L�^���Ă��镔�����܂ނ��� �����Z���܂�
 			file.fread( src , blockSize  - sizeof(fullSize) );	
			try
			{
				//�𓀂̎��s.
				unsigned long realSize = fullSize;
				Zlib.UnCompress(dest , &realSize , src , blockSize);
				ASSERT(realSize == fullSize);

				//�܂��A�����g��Ȃ��\�[�X�̈�̔j��.
				delete [] src;	src  = NULL;

				//�𓀂����f�[�^�̉��.
				RStdioMemory mem;
				mem.Open((char*)dest );
				while( mem.ftell() < realSize )
				{
					mem.fread( &simbol , sizeof(simbol) );
					mem.fread( &dataSize , sizeof(dataSize) );
					mem.fread( &dataCount , sizeof(dataCount) );
					{
						const char* nama = mem.ptr();
						zipdataSize = dataSize * 10;
						char * zipdata = new char[zipdataSize];
						Zlib.Compress2(  (unsigned char*)zipdata , &zipdataSize ,
							(const unsigned char*)nama , dataSize , RZlib.BEST_COMPRESSION);
						delete [] zipdata;
					}

					l = PrintFormat("%ld,%ld,%d%%,%ld,%d%%,%c,%ld,%ld,%d%%\n",
						index[i].Block,									//�u���b�N�ԍ�
						fullSize,										//�u���b�N�S�̒���(byte)
						(int)(((double)dataSize)/fullSize * 100),		//�u���b�N�ɐ�߂銄��
						blockSize,										//���k�ς݃u���b�N�S�̒���(byte)
						(int)(((double)zipdataSize)/blockSize * 100),	//���k�ς݃u���b�N�ɐ�߂銄��
						simbol,											//�V���{��
						dataSize,										//�f�[�^��(byte)
						dataCount,										//�f�[�^�̐�
						(int)((double)dataCount/dataSize * 100)			//1�f�[�^�̕��σT�C�Y
						);
					outlog.fwrite(l.c_str() , l.size() );
//					TRACE("�V���{�� %c �f�[�^�� %ld �f�[�^�̐� %d \n" , simbol , dataSize , (int)dataCount);

					if ( ! ::isalpha(simbol) )
					{
						throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:�A���t�@�x�b�g�ȊO�̃V���{���𔭌� %c  �ʒu:%ld" , simbol , file.ftell() ) );
					}
					//�ǂݔ�΂�.
					mem.fseek( dataSize , SEEK_CUR);
				}
			}
			catch(RException e)
			{
				delete [] src;
				delete [] dest;

				ASSERT(0);
				throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:�𓀒��ɃG���[���������܂��� �u���b�N�ԍ� %ld �u���b�N�J�n�ʒu:%ld : %s" ,
						index[i].Block , blockStart , e.getMessage() ) );
			}
			delete [] src;
			delete [] dest;

			//�L�^����Ă���u���b�N���Ǝ��ۂ̒l���A�҂���Ƃ��������Ȃ���΂����Ȃ�
			if((file.ftell() - blockStart) != blockSize)
			{
				throw RException(EXCEPTIONTRACE + PrintFormat("�������`�F�b�N�G���[:�ڎ��������Ă���u���b�N��[%ld] �� ���ۂ̃u���b�N��[%ld]�������܂���. �u���b�N�ԍ� %ld �u���b�N�J�n�ʒu:%ld" ,
					blockSize ,  (file.ftell() - blockStart)  , index[i].Block , blockStart) );
			}
		}
		if ( file.ftell() != fileSize )
		{
			throw RException(EXCEPTIONTRACE + "�������`�F�b�N�G���[:�S���̃u���b�N���������Ă��ǂ蒅�����ꏊ���t�@�C���̏I�[�ł͂���܂���");
		}
	}
	catch(RException e)
	{
		delete [] index;
		throw;
	}
	delete [] index;
}

