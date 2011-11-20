#include "StartWithExeFile.h"
#ifdef MUSIC_LIFE
/*******************************************************************/
/*
    Midi �Đ����[�`�� for MCI

    Thank you For EL (Eazy Link Library)    Botchy

    �č\�z RTI

*/
/*******************************************************************/

#include <.\Music\rti_music.h>
#include <.\VCL\rti_vcl_apprication.h>

/*********************************************************************/
//������
/*********************************************************************/
bool TMusic::Init()
{
	PlayMusic = false;
	LoopMusic = false;
	LoopMode  = true;

	return true;
}

/*********************************************************************/
//�I������
/*********************************************************************/
void TMusic::End()
{
	mciSendString("stop midi",NULL,0,NULL);
	mciSendString("close midi",NULL,0,NULL);
}

/*********************************************************************/
//�Đ�
/*********************************************************************/
void TMusic::Play(char* FileName)
{
	char Buffer[256];

	sprintf(Buffer,"open \"%s\" type sequencer alias midi",FileName);

	if (PlayMusic) Stop();

	mciSendString(Buffer,NULL,0,NULL);
	mciSendString("seek midi to start",NULL,0,NULL);
	mciSendString("play midi notify",NULL,0,Application->GethWnd() );

	PlayMusic =true;
	LoopMusic =false;
	LoopMode  =true;
}

/*********************************************************************/
//���[�v�Đ�
/*********************************************************************/
void TMusic::Replay()
{
	if (LoopMode)
	{
		mciSendString("seek midi to start",NULL,0,NULL);
		mciSendString("play midi notify",NULL,0,Application->GethWnd() );
	}
	else
    { //�X�g�b�v
		Stop();
	}

	LoopMusic= true;
}

/*********************************************************************/
//�X�g�b�v
/*********************************************************************/
void TMusic::Stop()
{
	if (PlayMusic)
	{
		mciSendString("stop midi",NULL,0,NULL);
		mciSendString("close midi",NULL,0,NULL);

		PlayMusic= false;
	}
}

/*********************************************************************/
//���[�v�̖�����
/*********************************************************************/
void TMusic::NoLoop(void)
{
	LoopMode= false;
}

/*********************************************************************/
//�Đ����I�����܂�������.
/*********************************************************************/
void TMusic::MciSuccessfulEnd()
{
    // MCI������o�b�t�@
    char MciBuffer[32];

    if (PlayMusic )
	{
	    mciSendString("status midi mode",MciBuffer,32,NULL);
    	if (MciBuffer[0]=='s' && MciBuffer[1]=='t')
		{
			Replay();
		}
	}
}


#endif  //LIFE END
