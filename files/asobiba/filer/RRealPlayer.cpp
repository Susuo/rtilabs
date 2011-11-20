// RRealPlayer.cpp: RRealPlayer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RRealPlayer.h"
#include "Rfiles.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RRealPlayer::RRealPlayer()
{

}

RRealPlayer::~RRealPlayer()
{

}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RRealPlayer::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RActiveXWindow::Create(inINstance, "rmocx.RealPlayer G2 Control.1" ,RealAudioObjects::IID_IRealAudio,(void**) &RealAudio,isParentWindow,0,0 ,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);

	puts("Control is K");
}

//�t�@�C�����J��
void RRealPlayer::Open(const string inFilename )	throw() 
{
	USES_CONVERSION;
	CComVariant varg[4];
	try
	{
		BSTR bstr;
		bstr = T2OLE( inFilename.c_str() );
		RealAudio->SetSource( bstr );

		//��������Ȃ��ƕ����ʂ� Audio���������łȂ�
		//�Q�l... http://www.jp.realnetworks.com/resources/sdk/vbplay.html
		bstr = T2OLE("one");
		RealAudio->put_Console(bstr);
		bstr = T2OLE("ImageWindow");
		RealAudio->put_Controls(bstr);

		RealAudio->SetOriginalSize();
	}
	catch(RException e)
	{
		printf("��O:\n %s " , e.getMessage() );
	}
}

//�Đ�����.
void RRealPlayer::Play() throw() 
{
	RealAudio->DoPlay();
}


//��~����.
void RRealPlayer::Stop() throw() 
{
	RealAudio->DoStop();
}


//�ꎞ��~ / ����
void RRealPlayer::Pause(bool inON)	throw() 
{
	RealAudio->DoPause();
}

//�t�@�C���N���[�Y
void RRealPlayer::Close()	throw() 
{
}


//�{�����[��
int RRealPlayer::getVolum() const throw() 
{
	short vol;
	RealAudio->GetVolume(&vol);
	return CHANGE_SCALE(vol ,100 , 255 );
}

void RRealPlayer::setVolum(int inVolum) throw() 
{
	RealAudio->SetVolume( REV_CHANGE_SCALE(inVolum ,100 , 255 ) );
}

//�Đ��ʒu
int  RRealPlayer::getPos() const throw() 
{
	long pos,len;
	RealAudio->GetPosition(&pos);
	RealAudio->GetLength(&len);

	if (len == 0) return 0;

	return CHANGE_SCALE(pos , len , 255);
}


void RRealPlayer::setPos(int inPos) throw() 
{
	long pos,len;
	RealAudio->GetLength(&len);
	
	pos = REV_CHANGE_SCALE(inPos , len , 255);

	RealAudio->SetPosition(pos);
}


//�Đ���?
bool RRealPlayer::IsPlay() const 
{
	return true;
}


//�ꎞ��~��?
bool RRealPlayer::IsPause() const 
{
	return true;
}




//�T�|�[�g���Ă܂�?
bool RRealPlayer::IsSupport(const string inFilename)
{
	//�Ƃ肠�����A�g���q����.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "RA" || ext == "RM" );
}

//�^�C�g�����擾����
string RRealPlayer::getTitle() const 
{
	BSTR title;
	RealAudio->GetTitle(&title);

	USES_CONVERSION;

	return OLE2CT(title);
}
