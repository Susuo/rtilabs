#include "StartWithExeFile.h"
#ifdef CD_LIFE
/*******************************************************************/
/*
    Audio CD �Đ����[�`�� for MCI

    Thank you For EL (Eazy Link Library)    Botchy

    �č\�z RTI

*/
/*******************************************************************/

#include <.\Music\rti_cd.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <windows.h>

/*******************************************************************/
//������
/*******************************************************************/
void TCDPlayer::Init()
{
    //�ȕʏ��\���̏�����
    ZeroMemory(CDInfo,sizeof(struct _CDInfo)*CD_MAX);

    //��~���Ԃ̏�����
	PauseM=-1;
	PauseS=-1;
	PauseF=-1;
    //�Ȑ�
	SongMax=0;

    //�ŏ��̋ȂƍŌ�̋�
	StartSongNo=0;
	EndSongNo=0;

    //��Ԃ̏�����
	PlayCD  =false;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
//�I������
/*******************************************************************/
void TCDPlayer::End()
{
    Stop();
}


/*******************************************************************/
//CD���̎擾
/*******************************************************************/
void TCDPlayer::GetInfo()
{
	int i;
	char Buffer[64];
    // MCI������o�b�t�@
    char MciBuffer[32];

	// CD�̎g�p�J�n
	mciSendString("open cdaudio",NULL,0,NULL);

	// �Ȑ��̎擾
	mciSendString("status cdaudio number of tracks",MciBuffer,32,NULL);

	SongMax=atoi(MciBuffer);

	// �S�̂̎��Ԃ̎擾
	mciSendString("status cdaudio length",MciBuffer,32,NULL);

	Buffer[0]=MciBuffer[0];
	Buffer[1]=MciBuffer[1];
	Buffer[2]=NULL;

	CDInfo[0].Time.m=atoi(Buffer);

	Buffer[0]=MciBuffer[3];
	Buffer[1]=MciBuffer[4];
	Buffer[2]=NULL;

	CDInfo[0].Time.s=atoi(Buffer);

	Buffer[0]=MciBuffer[6];
	Buffer[1]=MciBuffer[7];
	Buffer[2]=NULL;

	CDInfo[0].Time.f=atoi(Buffer);

	for (i=1;i<=SongMax;i++)
	{
		// �Ȃ̊J�n���Ԃ̎擾
		wsprintf(Buffer,"status cdaudio position track %d",i);

		mciSendString(Buffer,MciBuffer,32,NULL);

		Buffer[0]=MciBuffer[0];
		Buffer[1]=MciBuffer[1];
		Buffer[2]=NULL;

		CDInfo[i].Start.m=atoi(Buffer);

		Buffer[0]=MciBuffer[3];
		Buffer[1]=MciBuffer[4];
		Buffer[2]=NULL;

		CDInfo[i].Start.s=atoi(Buffer);

		Buffer[0]=MciBuffer[6];
		Buffer[1]=MciBuffer[7];
		Buffer[2]=NULL;

		CDInfo[i].Start.f=atoi(Buffer);

		// �Ȃ̎��Ԃ̎擾
		sprintf(Buffer,"status cdaudio length track %d",i);

		mciSendString(Buffer,MciBuffer,32,NULL);

		Buffer[0]=MciBuffer[0];
		Buffer[1]=MciBuffer[1];
		Buffer[2]=NULL;

		CDInfo[i].Time.m=atoi(Buffer);

		Buffer[0]=MciBuffer[3];
		Buffer[1]=MciBuffer[4];
		Buffer[2]=NULL;

		CDInfo[i].Time.s=atoi(Buffer);

		Buffer[0]=MciBuffer[6];
		Buffer[1]=MciBuffer[7];
		Buffer[2]=NULL;

		CDInfo[i].Time.f=atoi(Buffer);
	}

	// CD�̎g�p�I��
	mciSendString("close cdaudio",NULL,0,NULL);
}

/*******************************************************************/
//�Ȃ̍Đ� ( �S�� )
/*******************************************************************/
void TCDPlayer::AllPlay()
{
    //�Đ�����������ق点��.
	if (PlayCD) Stop();

	// CD�̎g�p�J�n
	mciSendString("open cdaudio",NULL,0,NULL);

	// �Ȃ̍Đ�
	mciSendString("play cdaudio notify",NULL,0,Application->GethWnd() );

	StartSongNo=1;
	EndSongNo=SongMax;

	PlayCD  =true;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
//  �Ȃ̍Đ� ( �P�Ȏw�� )
/*******************************************************************/
void TCDPlayer::Play(int SongNo)
{
	char Buffer[64];

    //�Đ�����������ق点��.
	if (PlayCD) Stop();

	// CD�̎g�p�J�n
	mciSendString("open cdaudio",NULL,0,NULL);

	if (SongNo<SongMax)
	{
		wsprintf(Buffer,
				"play cdaudio from %02d:%02d:%02d to %02d:%02d:%02d notify",
				CDInfo[SongNo].Start.m,CDInfo[SongNo].Start.s,
				CDInfo[SongNo].Start.f,CDInfo[SongNo+1].Start.m,
				CDInfo[SongNo+1].Start.s,CDInfo[SongNo+1].Start.f);
	}
	else
	{
		wsprintf(Buffer,"play cdaudio from %02d:%02d:%02d notify",
				CDInfo[SongNo].Start.m,CDInfo[SongNo].Start.s,
				CDInfo[SongNo].Start.f);
	}

	// �Ȃ̍Đ�
	mciSendString(Buffer,NULL,0,Application->GethWnd() );

	StartSongNo=SongNo;
	EndSongNo=SongNo;

	PlayCD  =true;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
//�Ȃ̍Đ� ( �͈͎w�� )
/*******************************************************************/
void TCDPlayer::Play(int SongNo1,int SongNo2)
{
	char Buffer[64];

    //�Đ�����������ق点��.
	if (PlayCD) Stop();

	// CD�̎g�p�J�n
	mciSendString("open cdaudio",NULL,0,NULL);

	if (SongNo2<SongMax)
	{
		wsprintf(Buffer,
				"play cdaudio from %02d:%02d:%02d to %02d:%02d:%02d notify",
				CDInfo[SongNo1].Start.m,CDInfo[SongNo1].Start.s,
				CDInfo[SongNo1].Start.f,CDInfo[SongNo2+1].Start.m,
				CDInfo[SongNo2+1].Start.s,CDInfo[SongNo2+1].Start.f);
	}
	else
	{
		wsprintf(Buffer,"play cdaudio from %02d:%02d:%02d notify",
				CDInfo[SongNo1].Start.m,CDInfo[SongNo1].Start.s,
				CDInfo[SongNo1].Start.f);
	}

	// �Ȃ̍Đ�
	mciSendString(Buffer,NULL,0,Application->GethWnd() );

	StartSongNo=SongNo1;
	EndSongNo=SongNo2;

	PlayCD  =true;
	LoopCD  =false;
	LoopMode=true;
}

/*******************************************************************/
/*-  �Ȃ̍Đ� ( ���Ԏw�� )                                      �� �����Ŏg�p  -*/
/*-                                                                            -*/
/*-    int StartM : �J�n���� ( �� )                                            -*/
/*-    int StartS : �J�n���� ( �b )                                            -*/
/*-    int StartF : �J�n���� ( �t���[�� )                                      -*/
/*-    int EndM   : �I������ ( �� )                                            -*/
/*-    int EndS   : �I������ ( �b )                                            -*/
/*-    int EndF   : �I������ ( �t���[�� )                                      -*/
/*******************************************************************/
void TCDPlayer::Play(int StartM,int StartS,int StartF,int EndM,int EndS,int EndF)
{
	char Buffer[64];

    //�Đ�����������ق点��.
	if (PlayCD) Stop();

	// CD�̎g�p�J�n
	mciSendString("open cdaudio",NULL,0,NULL);

	wsprintf(Buffer,"play cdaudio from %02d:%02d:%02d to %02d:%02d:%02d notify",
			StartM,StartS,StartF,EndM,EndS,EndF);

	// �Ȃ̍Đ�
	mciSendString(Buffer,NULL,0,Application->GethWnd() );
	PlayCD  =true;
	LoopCD  =false;
}

/*******************************************************************/
//  �Ȃ̃��[�v�Đ�
/*******************************************************************/
void TCDPlayer::Replay()
{
	if (LoopMode)
	{
		Play(StartSongNo,EndSongNo);
	}
	else
	{
		Stop();
	}

	LoopCD=TRUE;
}

/*******************************************************************/
// �Ȃ̒�~
/*******************************************************************/
void TCDPlayer::Stop()
{
	if (PlayCD)
	{
		mciSendString("stop cdaudio",NULL,0,NULL);
		mciSendString("close cdaudio",NULL,0,NULL);

		PlayCD  =false;
	}
}

/*******************************************************************/
/*-  �Ȃ̈ꎞ��~                                                              -*/
/*-                                                                            -*/
/*-    BOOL Flag : true  = �ꎞ��~                                            -*/
/*-                false = �ꎞ��~����                                        -*/
/*******************************************************************/

void TCDPlayer::Pause(bool Flag)
{
	char Buffer[64];
    // MCI������o�b�t�@
    char MciBuffer[32];

	if (Flag)
	{
        //�ꎞ��~
		if (PlayCD)
		{   
			// ���݂̎��Ԃ��擾
			mciSendString("status cdaudio position",MciBuffer,32,NULL);

			Buffer[0]=MciBuffer[0];
			Buffer[1]=MciBuffer[1];
			Buffer[2]=NULL;

			PauseM=atoi(Buffer);

			Buffer[0]=MciBuffer[3];
			Buffer[1]=MciBuffer[4];
			Buffer[2]=NULL;

			PauseS=atoi(Buffer);

			Buffer[0]=MciBuffer[6];
			Buffer[1]=MciBuffer[7];
			Buffer[2]=NULL;

			PauseF=atoi(Buffer);

			// ��~
			Stop();
		}
	}
	else
	{   
        //���A
		if (PauseM!=-1 && PauseS!=-1 && PauseF!=-1)
		{
			if (PauseM>CDInfo[0].Time.m ||
				PauseM==CDInfo[0].Time.m && PauseS>CDInfo[0].Time.s ||
				PauseM==CDInfo[0].Time.m && PauseS==CDInfo[0].Time.s &&
				PauseF>CDInfo[0].Time.m)
			{
				Play(StartSongNo,EndSongNo);
			}
			else
			{
				// ���Ԏw��ōĐ�
				if (EndSongNo==SongMax)
				{
					Play(PauseM,PauseS,PauseF,
							   CDInfo[0].Time.m,CDInfo[0].Time.s,
							   CDInfo[0].Time.f);
				}
				else
				{
					Play(PauseM,PauseS,PauseF,
							   CDInfo[EndSongNo+1].Start.m,
							   CDInfo[EndSongNo+1].Start.s,
							   CDInfo[EndSongNo+1].Start.f);
				}
			}

			PauseM=-1;
			PauseS=-1;
			PauseF=-1;
		}
	}
}

/*******************************************************************/
//  �Ȃ̃X�L�b�v
//    int Count : �X�L�b�v�� ( 1:���̋� / -1:�O�̋� )
/*******************************************************************/
void TCDPlayer::Skip(int Count)
{
	int Now;

	Now= NowSong()+Count;

	if (Now<StartSongNo)
	{
		Now=EndSongNo;
	}

	Pause(true);

	PauseM=CDInfo[Now].Start.m;
	PauseS=CDInfo[Now].Start.s;
	PauseF=CDInfo[Now].Start.f;

	Pause(false);
}

/*******************************************************************/
//�Ȃ̑�����
//    int Time : �����莞�� ( �b )
/*******************************************************************/
void TCDPlayer::Forward(int Time)
{
    //�ꎞ��~��Ԃ����
	Pause(true);

	PauseS+=Time;

	if (PauseS>=60)
	{
		PauseS-=60;

		if (++PauseM>CDInfo[0].Time.m ||
			PauseM==CDInfo[0].Time.m && PauseS>CDInfo[0].Time.s ||
			PauseM==CDInfo[0].Time.m && PauseS==CDInfo[0].Time.s &&
			PauseF>CDInfo[0].Time.f)
		{
			PauseM=CDInfo[0].Time.m;
			PauseS=CDInfo[0].Time.s;
			PauseF=CDInfo[0].Time.f;
		}
	}
    //��~����  �����肵���Ƃ��납�烊�X�^�[�g
	Pause(false);
}

/*******************************************************************/
/*-  �Ȃ̊��߂�                                                                -*/
/*-                                                                            -*/
/*-    int Time : ���߂����� ( �b )                                            -*/
/*******************************************************************/
void TCDPlayer::Backward(int Time)
{
    //�ꎞ��~��Ԃ����
	Pause(true);

	PauseS-=Time;

	if (PauseS<0)
	{
		PauseS+=60;

		if (--PauseM<0)
		{
			PauseM=0;
			PauseS=0;
			PauseF=0;
		}
	}
    //��~����  �����肵���Ƃ��납�烊�X�^�[�g
	Pause(false);
}

/*******************************************************************/
//CD�}����Ԃ̎擾
//    �߂�l : TRUE  = CD����
//             FALSE = CD�Ȃ�
/*******************************************************************/
bool TCDPlayer::Ready()
{
    // MCI������o�b�t�@
    char MciBuffer[32];

	mciSendString("status cdaudio media present",MciBuffer,32,NULL);

	if (MciBuffer[0]=='t')
	{
		mciSendString("status cdaudio ready",MciBuffer,32,NULL);

		if (MciBuffer[0]=='t')
		{   
            //CD ������܂���
			return true;
		}
	}
    //CD ���Ȃ��ł���
	return false;
}

/*******************************************************************/
//  CD�h���C�u�̃I�[�v��
/*******************************************************************/
void TCDPlayer::Open()
{
	mciSendString("set cdaudio door open",NULL,0,NULL);
}

/*******************************************************************/
//  CD�h���C�u�̃N���[�Y
/*******************************************************************/
void TCDPlayer::Close()
{
	mciSendString("set cdaudio door closed",NULL,0,NULL);
}

/*******************************************************************/
//���[�v�Đ��̖�����  ( Play�֐��̌�Ɏg�p )
/*******************************************************************/
void TCDPlayer::NoLoop()
{
	LoopMode=FALSE;
}

/*******************************************************************/
// CD�̋Ȑ��̎擾
/*******************************************************************/
int TCDPlayer::MaxSong()
{
	return SongMax;
}

/*******************************************************************/
//  �Đ����̋Ȕԍ��̎擾
/*******************************************************************/
int TCDPlayer::NowSong()
{
    // MCI������o�b�t�@
    char MciBuffer[32];

	mciSendString("status cdaudio current track",MciBuffer,32,NULL);

	if (PlayCD)
	{
		return atoi(MciBuffer);
	}
	else
	{
		return 0;
	}
}

/*********************************************************************/
//�Đ����I�����܂�������.
/*********************************************************************/
void TCDPlayer::MciSuccessfulEnd()
{
    // MCI������o�b�t�@
    char MciBuffer[32];

	if (PlayCD)
	{
		mciSendString("status cdaudio mode",MciBuffer,32,NULL);

        if (MciBuffer[0]=='s' && MciBuffer[1]=='t')
        {
			Replay();   //���s�[�g
		}
	}
}


#endif
