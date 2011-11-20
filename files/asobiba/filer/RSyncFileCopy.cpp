// RSyncFileCopy.cpp: RSyncFileCopy �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RSyncFileCopy.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSyncFileCopy::RSyncFileCopy()
{
	for(int i = 0 ; i < BUFFER_COUNT ; i++)
	{
		Buffer[i] = new char[BUFFER_SIZE];
	}
}

RSyncFileCopy::~RSyncFileCopy()
{
	for(int i = 0 ; i < BUFFER_COUNT ; i++)
	{
		delete[] Buffer[i];
	}
}

void RSyncFileCopy::Run(string inReadPath , Rfile* ioRead,
						string inWritePath , Rfile* ioWrite) throw()
{
	return;

	struct POINT_STRUCT
	{
		int				BufferCount;
		unsigned long	ModBufferSize;
	};
	POINT_STRUCT	read = {0};
	POINT_STRUCT	write = {0};
	bool			read_end = false;
	unsigned long	read_mod_buffer = 0;
	string			errro;

	try
	{
		ioRead->Open ( inReadPath , __RFILEOPENTYPE_READ );
		ioWrite->Open ( inWritePath , __RFILEOPENTYPE_WRITE );

		ioRead->Read(Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
		//���[�h�o�b�t�@�𖞃^���ɂ��܂��B
		while(read.ModBufferSize < BUFFER_SIZE) 
		{
			ioRead->WaitFor();
			read.ModBufferSize += ioRead->GetReport();
			if (read.ModBufferSize >= BUFFER_SIZE) break;

			bool ret = ioRead->Read(Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
			if (!ret)
			{
				//�ǂݍ��݂� EOF �܂œǂݍ��݂܂����B
//				printf("-------Eof-----1\n");
				read_end = true;
				read_mod_buffer = read.ModBufferSize;
				//���Ƃ��ꂾ���������񂾂�I���ł��B
				write.ModBufferSize = BUFFER_SIZE - read_mod_buffer;
				break;
			}
		}
		//�ǂݍ��݂��܂������Ȃ�A���̓ǂݍ��݃o�b�t�@�ɃZ�b�g.
		if (!read_end)
		{
			read.ModBufferSize = read_mod_buffer;
			read.BufferCount = (read.BufferCount+1) % BUFFER_COUNT;
		}

		//���S�񓯊�IO �v���O����������.
		while(1)
		{
			ioWrite->Write( Buffer[write.BufferCount] , BUFFER_SIZE - write.ModBufferSize);

			//�ǂݍ��ݏI���܂���?
			if ( ioRead->Poll() )
			{
				//�����A�o�b�t�@�X�V�A�ƁB
				read.ModBufferSize += ioRead->GetReport();

				//�o�b�t�@�����ׂď������܂���?
				if (read.ModBufferSize >= BUFFER_SIZE)
				{
					//���̃o�b�t�@��
					read.BufferCount = (read.BufferCount+1) % BUFFER_COUNT;
					read.ModBufferSize = 0;
					//���̃o�b�t�@�͎��͎g���Ă��肵��
					if (read.BufferCount == write.BufferCount) 
					{	//�g���Ă�������A�ǂ��B
						while(write.ModBufferSize < BUFFER_SIZE) 
						{
							ioWrite->WaitFor();
							write.ModBufferSize += ioWrite->GetReport();
							if (write.ModBufferSize >= BUFFER_SIZE) break;

							ioWrite->Write(Buffer[write.BufferCount] , BUFFER_SIZE - write.ModBufferSize);
						}
						write.BufferCount = (write.BufferCount+1) % BUFFER_COUNT;
						write.ModBufferSize = 0;
					}
				}
			}
			//�ǂݍ��݊J�n.
			if (!read_end)
			{
				bool ret = ioRead->Read( Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
				if (!ret)
				{
					//�ǂݍ��݂� EOF �܂œǂݍ��݂܂����B
//					printf("-------Eof-----2\n");
					read_end = true;
					read_mod_buffer = read.ModBufferSize;
				}
			}

			//�������ݏI���܂���?
			if ( ioWrite->Poll() )
			{
				//�����A�o�b�t�@�X�V�A�ƁB
				write.ModBufferSize += ioWrite->GetReport();

				//�o�b�t�@�����ׂď������܂���?
				if (write.ModBufferSize >= BUFFER_SIZE)
				{
					if (!read_end)
					{//�����͂܂������Ă���
						//���̃o�b�t�@��
						write.BufferCount = (write.BufferCount+1) % BUFFER_COUNT;
						write.ModBufferSize = 0;
						//���̃o�b�t�@�͎��͎g���Ă��肵��
						if (write.BufferCount == read.BufferCount) 
						{
							//�g���Ă�������A�ǂ��B
							while(read.ModBufferSize < BUFFER_SIZE) 
							{
								ioRead->WaitFor();
								read.ModBufferSize += ioRead->GetReport();

								if (read.ModBufferSize >= BUFFER_SIZE) break;

								bool ret = ioRead->Read(Buffer[read.BufferCount] , BUFFER_SIZE - read.ModBufferSize);
								if (!ret)
								{
									//�ǂݍ��݂� EOF �܂œǂݍ��݂܂����B
//									printf("-------Eof-----3\n");
									read_end = true;
									read_mod_buffer = read.ModBufferSize;
									//���Ƃ��ꂾ���������񂾂�I���ł��B
									write.ModBufferSize = BUFFER_SIZE - read_mod_buffer;
									break;
								}
							}
							//�ǂݍ��݂��܂������Ȃ�A���̓ǂݍ��݃o�b�t�@�ɃZ�b�g.
							if (!read_end)
							{
								read.BufferCount = (read.BufferCount+1) % BUFFER_COUNT;
								read.ModBufferSize = 0;
							}
						}
					}
					else
					{
						//���̃o�b�t�@�͍Ō�̃o�b�t�@?
						if (write.BufferCount == read.BufferCount) 
						{//�S���I�����! while(1) ���ʂ��邼�[!!
							break;
						}
						//���̃o�b�t�@��
						write.BufferCount = (write.BufferCount+1) % BUFFER_COUNT;
						write.ModBufferSize = 0;
						//�V�����o�b�t�@�͍Ō�̃o�b�t�@�ł���?
						if (write.BufferCount == read.BufferCount) 
						{	//�[������������.
							write.ModBufferSize = BUFFER_SIZE - read_mod_buffer;
						}
					}
				}
			}
		}
		ioRead->Close ( );
		ioWrite->Close ( );
	}
	catch(RException e)
	{
		ioRead->Close ( );
		ioWrite->Close ( );
		throw RIOException( "�R�s�[�������ɃG���[���������܂���.\n %s" , e.getMessage() );
	}
}

#include "RfileLocalReport.h"
#include "RfilesLocal.h"

void RSyncFileCopy::Test()
{
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send1.jpg" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv1.jpg" , 0 , 0 ,0 );
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		FileCheck("c:\\test2\\copy_test_send1.jpg","c:\\test2\\copy_test_resv1.jpg");
	}
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send3.jpg" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv3.jpg" , 0 , 0 ,0 );
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		FileCheck("c:\\test2\\copy_test_send3.jpg","c:\\test2\\copy_test_resv3.jpg");
	}
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send4.mp3" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv4.mp3" , 0 , 0 ,0 );
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		FileCheck("c:\\test2\\copy_test_send4.mp3","c:\\test2\\copy_test_resv4.mp3");
	}
	/*
	puts("-----------------------------------------------");
	{
		RfileLocalReport read;
		RfileLocalReport write;
		RSyncFileCopy rsfc;
		RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test2\\");

		read.Attach( &p , "copy_test_send2.lzh" , 0 , 0 ,0 );
		write.Attach( &p , "copy_test_resv2.lzh" , 0 , 0 ,0 );
		DWORD timer = ::timeGetTime();
		try
		{
			rsfc.Run("c:\\test2\\" , &read , "c:\\test2\\" , &write );
		}
		catch(RIOException e)
		{
			printf("%s",e.getMessage() );
			ASSERT(0);
		}
		printf( "������������ %ld ms \n" , ::timeGetTime() - timer);
		FileCheck("c:\\test2\\copy_test_send2.lzh","c:\\test2\\copy_test_resv2.lzh");
	}
	*/

}

void RSyncFileCopy::FileCheck(const char* inSrc,const char* inDest)
{
	FILE* src = fopen(inSrc , "rb" );
	ASSERT(src);
	FILE* dest = fopen(inDest , "rb" );
	ASSERT(dest);

	string error;

	char* srcBuffer = new char[BUFFER_SIZE];
	char* destBuffer = new char[BUFFER_SIZE];
	while(1)
	{
		int srclen = fread( srcBuffer , 1 , BUFFER_SIZE , src);
		int destlen = fread( destBuffer , 1 , BUFFER_SIZE , dest);

		if (srclen == 0 && destlen == 0) break;

		if (srclen != destlen)
		{
			fclose(src);			fclose(dest);
			//�T�C�Y�s��v
			ASSERT(0);
			break;
		}
		if ( memcmp(srcBuffer ,  destBuffer ,srclen) != 0)
		{
			fclose(src);			fclose(dest);
			//���e�s��v
			ASSERT(0);
			break;
		}
	}
	fclose(src);			fclose(dest);
	delete []  srcBuffer ;
	delete [] destBuffer ;
}
