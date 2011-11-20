// YTransmitFile.cpp: YTransmitFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YTransmitFile.h"
#include <sys/stat.h>
#include <sys/utime.h>
#include "RDateConv.h"
#include "RSocketAsyncTransmitFile.h"
#include "YHttpdConf.h"
#include "RMimeType.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YTransmitFile::YTransmitFile()
{

}

YTransmitFile::~YTransmitFile()
{

}

//�t�@�C�������g���q�𕪗�
string YTransmitFile::Filename2Ext(const string inFilename) const
{
	const char* str = inFilename.c_str();
	for(int i = inFilename.length() - 1; str[i] != '.' && i >= 0 ; i--)
		;
	if (i < 0 )	return "";

	//�g���q���q������
	string ext = (char*)(str + i + 1);
	for(register int l = 0 ; ext[l] != '\0' ;l++) ext[l] = tolower(ext[l]);

	return ext;	// +1 �� '.' �̕��ł��B
}

void YTransmitFile::TransmitFile(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,RSocketForWinSock2* ioSocket)
{
	
	RSocketAsyncTransmitFile	transmitFile;
	transmitFile.Mount(ioSocket);

	string requestFilename = inRHTTPHeader->getPath();
	//�댯�ȃp�X�̔r��.
	if ( strstr( requestFilename.c_str() , ".." ) != NULL)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "���N�G�X�g�t�@�C�����Ɋ댯�ȕ������܂�ł���̂ŃL�����Z������܂���");
	}
	//index.html �̕ۊ�.
	if ( requestFilename[requestFilename.size() - 1] == '/')  requestFilename += "index.html";

	//�t�@�C�����������ɂ���.  
	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + requestFilename;

	//�ŏI�X�V���������߂�.
	struct _stat statbuf;
	if ( _stat( filename.c_str() ,  &statbuf) == -1)
	{
		throw RIOCannotOpenException(EXCEPTIONTRACE + "stat �̎擾�Ɏ��s" + filename);
	}

	const time_t lastModified = statbuf.st_mtime;
	const int filesize = statbuf.st_size;

	//If ModifiedSince �̏ꍇ�A�X�V����Ă��Ȃ����`�F�b�N.
	if ( inRHTTPHeader->IfModifiedSince.getValue().IsEnabled() &&
		 inRHTTPHeader->IfModifiedSince.getValue().getDate() >= lastModified	)
	{
		int len = sprintf( outShareBuffer ,	
									"HTTP/1.0 304 Not Modified\r\n"
									"Date: %s\r\n"
									"Server: Youjyo\r\n"
									"Connection: close\r\n"
									"Content-Length: 0\r\n"
									"\r\n"
								,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								);


		TRANSMIT_FILE_BUFFERS tfb = {0};
		tfb.Head = outShareBuffer;
		tfb.HeadLength = len;

		transmitFile.TransmitWithout( &tfb );
		transmitFile.WaitForComplate();
		return ;
	}
	//mime �̎擾
	const string mime = RMimeType::getInstance()->getMimeType( Filename2Ext( requestFilename ) );
	//Range���L���������獷���擾�����܂��B
	if ( inRHTTPHeader->Range.getValue().IsEnabled() )
	{
		/*ChangeLog.txt:2003/01/23	range ���s�����Ƃ��� Content-Range ���������������̂ŏC��.
			http://pc3.2ch.net/test/read.cgi/tech/1043275721/22
			Range�w���GET���鎞�ɕԂ����Content-Range����������ֶ�

			�{���Ȃ�
			Content-Length: 400001
			Content-Range: bytes 0-400000/400001�@�ƂȂ�Ƃ��낪

			Content-Length: 400001
			Content-Range: bytes 0-400001/400002�@�ɂȂ��Ă܂�			
		*/
		int rangeStart = inRHTTPHeader->Range.getValue().getStart();
		int rangeEnd = inRHTTPHeader->Range.getValue().getEnd();

		if (rangeEnd >= rangeStart && rangeEnd >= 0 && rangeStart >= 0)
		{
			//�t�@�C�����ł����]���͔F�߂Ȃ�.
			if (rangeEnd >= filesize) rangeEnd = filesize - 1;
			//���]���o�C�g
			int lenth = (rangeEnd+1) - rangeStart;

			int len = 
					sprintf( outShareBuffer ,	
								"HTTP/1.1 206 Partial Content\r\n"
								"Date: %s\r\n"
								"Server: Youjyo\r\n"
								"Last-Modified: %s\r\n"
								"Accept-Ranges: bytes\r\n"
								"Content-Length: %d\r\n"
								"Connection: close\r\n"
								"Content-Type: %s\r\n"
								"Content-Range: bytes %d-%d/%d\r\n"
								"\r\n"
							,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
							,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
							,	lenth
							,	mime.c_str()
							,	rangeStart , rangeEnd , filesize
							);
			TRANSMIT_FILE_BUFFERS tfb = {0};
			tfb.Head = outShareBuffer;
			tfb.HeadLength = len;

			transmitFile.Transmit(filename.c_str() ,rangeStart , rangeStart+lenth, &tfb );
			transmitFile.WaitForComplate();
			return ;
		}
	}
	//�ʏ�]��.
	{
		int len = sprintf( outShareBuffer ,	
									"HTTP/1.1 200 OK\r\n"
									"Date: %s\r\n"
									"Server: Youjyo\r\n"
									"Last-Modified: %s\r\n"
									"Accept-Ranges: bytes\r\n"
									"Content-Length: %d\r\n"
									"Connection: close\r\n"
									"Content-Type: %s\r\n"
									"\r\n"
								,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
								,	RDateConv::UnixTimeToHttpTime(lastModified).c_str()
								,	filesize
								,	mime.c_str()
								);

		TRANSMIT_FILE_BUFFERS tfb = {0};
		tfb.Head = outShareBuffer;
		tfb.HeadLength = len;

		transmitFile.Transmit(filename.c_str() ,0, filesize , &tfb );
		transmitFile.WaitForComplate();
	}
}
