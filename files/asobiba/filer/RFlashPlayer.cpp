// RFlashPlayer.cpp: RFlashPlayer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RFlashPlayer.h"
#include "Rfiles.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RFlashPlayer::RFlashPlayer()
{

}

RFlashPlayer::~RFlashPlayer()
{

}


//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RFlashPlayer::Create(HINSTANCE inINstance,HWND isParentWindow) throw()
{
	RActiveXWindow::Create(inINstance,"ShockwaveFlash.ShockwaveFlash.1" , ShockwaveFlashObjects::IID_IShockwaveFlash, (void**)&ShockwaveFlash,isParentWindow,0, 0,WS_EX_STATICEDGE | WS_EX_ACCEPTFILES);
	puts("Control is OK");
}

//�t�@�C�����J��
void RFlashPlayer::Open(const string inFilename )	throw() 
{
	USES_CONVERSION;
	BSTR filename = T2OLE( inFilename.c_str() );
	ShockwaveFlash->LoadMovie( 0 , filename );
}

//�Đ�����.
void RFlashPlayer::Play() throw() 
{
	ShockwaveFlash->Play();
}


//��~����.
void RFlashPlayer::Stop() throw() 
{
	ShockwaveFlash->Stop();
}


//�ꎞ��~ / ����
void RFlashPlayer::Pause(bool inON)	throw() 
{
	if (inON)	ShockwaveFlash->Play();
	else		ShockwaveFlash->Stop();
}

//�t�@�C���N���[�Y
void RFlashPlayer::Close()	throw() 
{

}


//�{�����[��
int RFlashPlayer::getVolum() const throw() 
{
	return 0;
}

void RFlashPlayer::setVolum(int inVolum) throw() 
{
}

//�Đ��ʒu
int  RFlashPlayer::getPos() const throw() 
{
	long pos,all;
	ShockwaveFlash->get_TotalFrames(&all);
	ShockwaveFlash->CurrentFrame(&pos);

	printf("%f / %f \n", pos , all);

	return CHANGE_SCALE( pos , all , 255) ;
}


void RFlashPlayer::setPos(int inPos) throw() 
{
	long pos,all;
	ShockwaveFlash->get_TotalFrames(&all);
	pos = REV_CHANGE_SCALE( inPos , all , 255) ;

	ShockwaveFlash->GotoFrame(pos);
}


//�Đ���?
bool RFlashPlayer::IsPlay() const 
{
	VARIANT_BOOL	b;
	ShockwaveFlash->get_Playing(&b);
	return b == VARIANT_TRUE;
}


//�ꎞ��~��?
bool RFlashPlayer::IsPause() const 
{
	return true;
}


//�T�|�[�g���Ă܂�?
bool RFlashPlayer::IsSupport(const string inFilename)
{
	//�Ƃ肠�����A�g���q����.
	string ext = Rfiles::Filename2Ext(inFilename);
	return  (ext == "SWF");
}

//�^�C�g���̎擾
string RFlashPlayer::getTitle() const 
{
	return "";
}
