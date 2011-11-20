// RMedia.cpp: RMedia �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RMedia.h"

#include "RFModView.h"
#include "RMediaPlayer.h"
#include "RRealPlayer.h"
#include "RFlashPlayer.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RMedia::RMedia()
{
	Player = NULL;
}

RMedia::~RMedia()
{
	delete Player;
}

//�ǂݍ���
bool RMedia::Load(const string inFilename,HINSTANCE inINstance,HWND isParentWindow)
{
	//���łɎg�p���̏ꍇ�͔j������.
	if (Player)
	{
		Player->Stop();
		Player->Close();
		delete Player;
	}

	//RealPlayer �ł�����̂ł����[
	if ( RRealPlayer::IsSupport(inFilename) )
	{
		try
		{
			Player = new RRealPlayer;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "RRealPlayer �G���[:\n%s \n" , e.getMessage() );
		}
	}

	//MS-MediaPlayer �ł�����̂ł����[
	if ( RMediaPlayer::IsSupport(inFilename) )
	{
		try
		{
			Player = new RMediaPlayer;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "RMediaPlayer �G���[:\n%s \n" , e.getMessage() );
		}
	}
	//MicroMedia Flash�ł�����̂ł����[
	if ( RFlashPlayer::IsSupport(inFilename) )
	{
		try
		{
			Player = new RFlashPlayer;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "RFlashPlayer �G���[:\n%s \n" , e.getMessage() );
		}
	}
	

	//fmod.dll�ł�����̂ł����[
	if ( RFModView::IsSupport(inFilename) )
	{
		try
		{
			Player = new RFModView;
			Player->Create(inINstance,isParentWindow);
			Player->Open(inFilename);

			return true;
		}
		catch(RException e)
		{
			delete Player;
			Player = NULL;
			printf( "fmod.dll �G���[:\n%s \n" , e.getMessage() );
		}
	}
	return false;
}

