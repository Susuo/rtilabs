// MVStaticZLib.cpp: MVStaticZLib �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVStaticZLib.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////



MVStaticZLib* MVStaticZLib::getInstance()
{
	static MVStaticZLib a;
	return &a;
}

void MVStaticZLib::Create()
{
#ifndef _WIN32_WCE
	this->Zlib.Create("zlib.dll");
#else
	this->Zlib.Create("zlibce.dll");
#endif
	this->inflateEnd = (inflateEndDef) Zlib.GetProcAddress("inflateEnd");
	this->inflate = (inflateDef) Zlib.GetProcAddress("inflate");
	this->inflateInit2_ = (inflateInit2_Def) Zlib.GetProcAddress("inflateInit2_");
}

void MVStaticZLib::UnCompressed(unsigned char *outDest , int inDestSize , const unsigned char* inSrc , int inSrcSize) throw(RConferException)
{
	z_stream d_stream = {0}; /* decompression stream */

	d_stream.next_in  = (unsigned char*) inSrc;
	d_stream.next_out = (unsigned char*) outDest;
	d_stream.avail_in = inSrcSize;
	d_stream.avail_out = inDestSize;

	//����: inflateInit(&d_stream) ���ƁA inflate ���G���[�ɂȂ�܂��B
	//      �Ȃ���? inflateInit2 ����Y�񂾁B
	int err = this->inflateInit2_(&d_stream , -MAX_WBITS , ZLIB_VERSION, sizeof(z_stream));
	if (err != Z_OK )
	{
		ZLibThrow(err, "inflateInit");
	}

	err = this->inflate(&d_stream, Z_NO_FLUSH);
	if (err != Z_STREAM_END)
	{
		ZLibThrow(err, "inflate");
	}

	err = this->inflateEnd(&d_stream);
	if (err != Z_OK )
	{
		ZLibThrow(err, "inflateEnd");
	}
}

//�G���[�R�[�h���O�ɂ��Ĕ�΂�.
void MVStaticZLib::ZLibThrow(int inRet , const string & inMethodName) const throw(RConferException)
{
	switch(inRet)
	{
	case Z_OK:
		break;
	case Z_ERRNO:
		throw RConferException(inRet , inMethodName +"zlib::Z_ERRNO");
	case Z_STREAM_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::�s���Ȉ���");
	case Z_DATA_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::�f�[�^�����Ă���");
	case Z_MEM_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::������������Ȃ�");
	case Z_BUF_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::�o�̓o�b�t�@������Ȃ�");
	case Z_VERSION_ERROR:
		throw RConferException(inRet , inMethodName +"zlib::Z_VERSION_ERROR");
	default:
		throw RConferException(inRet , inMethodName +"zlib::���m�̃G���[");
	}
}