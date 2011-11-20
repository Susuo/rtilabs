// YBBS.h: YBBS �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YBBS_H__18D276CD_B0BD_452C_A839_248197EFCD5C__INCLUDED_)
#define AFX_YBBS_H__18D276CD_B0BD_452C_A839_248197EFCD5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "YDAT.h"
#include "YDatCache.h"
#include "YDATReader.h"
#include "YDATWritter.h"
#include "RCollect.h"
#include "YRequestPath.h"
#include "RStreamUtil.h"
#include "ROutOfRangeException.h"
#include "RHTTPPOSTParser.h"
#include "RSpinLock.h"
#include "YSubject.h"
#include "YDATCache1.h"
#include "RHTTPHeader.h"
#include "YIndexHTML.h"
#include "YReadCGITemplate.h"
#include "YSubBackHTML.h"
#include "YBuildThread.h"
#include "YTextFile.h"
#include "YSettingTXT.h"
#include "YWriteHTML.h"
#include "YDeleteWait.h"
#include "YLocalRule.h"


#include "RDateConv.h"
class YBBS  
{
public:
	YBBS(string inName);
	virtual ~YBBS();

	//�ǂݍ���.
	void Read(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,  const YRequestPath* inRequestPath, RStreamUtil * outBuffer ) throw(RIOCannotOpenException);

	//��������.
	void Write(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) ;

	//�V�K.
	void New(char* outShareBuffer, const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) ;

	//�V�K�X��.
	void NewThread(char* outShareBuffer, POST& ioPost, RStreamUtil * outBuffer);

	//���ځ[��
	void Aborn(DATNUMBER inDeleteTarget ,int inDeleteSize ,const int * inDelete) throw(RIOCannotOpenException,RIOCannotWriteException);

	//�폜�˗��̒�o�⏕
	void YBBS::NewSakujyoIrai(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , POST& ioPost, RStreamUtil * outBuffer);


	/**
	 * SystemWrite:				�V�X�e������̃��b�Z�[�W
	 *
	 * @param outShareBuffer	TLS�o�b�t�@.
	 * @param ioPost			�������ޓ��e
	 * @return void 
	 */
	void SystemWrite(char* outShareBuffer,POST& ioPost);

	//����DAT��Subject�̎擾.
	const string getSubject(DATNUMBER inDatNumber) throw(RNotFoundException,RIOCannotOpenException);

	//�ًc����!
	void Igiari(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) ;

	//�폜�˗����ꂽ�������݂̂�΂��P����}�X�N����.
	void MaskReWrite(DATNUMBER inDatNumber ,const string & password ,
		 int inDeleteSize ,const int * inDelete , int inMaskSize   ,const string *inMask ) 
		 throw(RIOCannotOpenException,RIOCannotWriteException,RNotFoundException);

	/////����p�X���b�h�ɂ����s
	//�폜�l�^�X�N.
	void KillerTask(char* outShareBuffer);
	//DAT�K�y�[�W�R���N�^
	void GCTask();

	/////////////////�Ǘ��җp����
	//�����o��.
	void Dump();
	///���ׂĂ̏����f�B�X�N�Ƀt���b�V������.
	void Flush();

private:
	//DAT�̎擾.
	YDAT* YBBS::getDAT(DATNUMBER inDATNumber) throw(RIOCannotOpenException);
	//�V�K����
	YDAT* YBBS::getNewDAT(DATNUMBER inDATNumber);

	//read.cgi �R���p�`�ȕ\�����s���܂��B DAT�̉���͂��̒��ōs���܂�(�������̂���).
	void RequestTypeCGI(char* outShareBuffer,YDAT * inDat , int inStart, int inEnd , bool is1san , RStreamUtil * outBuffer)	throw(RIOCannotOpenException);
	//�t�@�C���]��.
	void TextFileTransmit(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,YTextFile* inUseFile,  RStreamUtil * outBuffer);

	//REQUESTTYPE_DAT
	void YBBS::RequestTypeDat(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader , const YRequestPath* inRequestPath, RStreamUtil * outBuffer)	throw(RIOCannotOpenException);
	//�������ݓ��e�̃`�F�b�N.
	void YBBS::WriteCheck(POST& ioPost) throw(RException);
	//�폜�������.
	const string YBBS::makeSakuJyoMessage(POST& ioPost) throw(RException);
	//�폜�˗�
	void YBBS::NewDelete(char* outShareBuffer,const RHTTPHeader* inRHTTPHeader ,POST& ioPost, RStreamUtil * outBuffer ) throw(RException);
	//�ًc����!
	void YBBS::Igiari(char* outShareBuffer,POST& ioPost, RStreamUtil * outBuffer ) throw(RException);
	//IP��������AID�\��������A mail�A�h���X�̉E�ɂ��邢�낢��ȏ������܂��B
	const string YBBS::MakeAppendData(const RHTTPHeader* inRHTTPHeader ,const POST & iopPost) const;
	//���O�`�F�b�N
	const string YBBS::CheckName(const RHTTPHeader* inRHTTPHeader ,const string & inName) const;
	//�A�N�փ`�F�b�N.
	void AccessCheck(const RHTTPHeader* inRHTTPHeader) throw(RException);

	//���b�Z�[�W�𕁒ʂɓ]������.
	void YBBS::TranceString(char* outShareBuffer,const string &inMessage , RStreamUtil * outBuffer)	throw(RException)
	{
		/*ChangeLog.txt:2003/01/23	Connection: close �̍Ō�� \r\n��Y��Ă��� �l�X�P �Ō����Ȃ������̂��C��
			http://pc3.2ch.net/test/read.cgi/tech/1043275721/10
		*/
		int len = sprintf( outShareBuffer ,	"HTTP/1.1 200 OK\r\n"
											"Date: %s\r\n"
											"Server: Youjyo\r\n"
											"Accept-Ranges: bytes\r\n"
											"Connection: close\r\n"
											"Content-Type: text/html\r\n"
											"Content-Length: %d\r\n"
											"\r\n"
										,	RDateConv::UnixTimeToHttpTime(time(NULL)).c_str()
										,	inMessage.size()
										);

		outBuffer->WriteBuffer( outShareBuffer , len ); 
		outBuffer->WriteBuffer( inMessage.c_str() , inMessage.size() ); 
	}
	//���̔̃����N.
	const string ThisBoard() const
	{
		return "http://" + YHttpdConf::getInstance()->getServerName() + "/test/read.cgi/" + this->BoardName + "/";
	}
	//����DAT�ւ̃����N.
	const string ThisBoardShortCut(DATNUMBER inDatNumber) const
	{
		return "../test/read.cgi/" + this->BoardName + "/" + num2str(inDatNumber) + "/";
	}

	
private:
	//��
	string		BoardName;

	//DAT�L���b�V��
	YDATCache	Cache;

	//subject.txt
	YSubject	Subject;

	//index.html
	YIndexHTML	IndexHTML;

	//read.cgi �̉��
	YReadCGITemplate	ReadCGI;

	//subback.html
	YSubBackHTML	SubBackHTML;

	//�V�K�X���b�h�쐬
	YBuildThread	BuildThread;

	//�ݒ�t�@�C��
	YSettingTXT		SettingTXT;

	//�������݂܂��� html
	YWriteHTML		WriteHTML;

	//�폜�҂�
	YDeleteWait		DeleteWait;

	//���[�J�����[��
	YLocalRule		LocalRule;
};

#endif // !defined(AFX_YBBS_H__18D276CD_B0BD_452C_A839_248197EFCD5C__INCLUDED_)
