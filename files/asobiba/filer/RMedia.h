// RMedia.h: RMedia �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIA_H__D918FCBA_5D18_46B9_8DFE_AF9D3F8A0F84__INCLUDED_)
#define AFX_RMEDIA_H__D918FCBA_5D18_46B9_8DFE_AF9D3F8A0F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMediaViewPlayer.h"
#include "RException.h"

class RMedia  
{
public:
	RMedia();
	virtual ~RMedia();

	//�ǂݍ���
	bool Load(const string inFilename,HINSTANCE inINstance,HWND isParentWindow);

	//�Đ�����.
	void Play()
	{
		try
		{
			Player->Play();	
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//��~����.
	void Stop() 
	{
		try
		{
			Player->Stop();
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//�ꎞ��~ / ����
	void Play(bool inON)	
	{
		try
		{
			Player->Stop();
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//�{�����[��
	int getVolum() const 
	{
		try
		{

			ASSERT(Player->getVolum () >= 0);
			ASSERT(Player->getVolum() <= 255);
			return Player->getVolum();
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
			return 0;
		}
	}

	void setVolum(int inVolum) 
	{
		if(inVolum < 0)	return;
		if(inVolum > 255)	return ;
		try
		{
			Player->setVolum(inVolum);
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//�Đ��ʒu
	int  getPos() const 
	{
		try
		{
			ASSERT(Player->getPos() >= 0);
			ASSERT(Player->getPos() <= 255);
			return Player->getPos();
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
			return 0;
		}
	}

	void setPos(int inPos) 
	{
		if(inPos <= 0) return ;
		if(inPos > 255) return;
		try
		{
			Player->setPos(inPos);
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//�Đ���?
	bool IsPlay() const
	{
		try
		{
			Player->IsPlay();
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//�ꎞ��~��?
	bool IsPause() const
	{
		try
		{
			Player->IsPause();
		}
		catch(RException e)
		{
			printf( "�G���[:\n%s \n" , e.getMessage() );
		}
	}

	//���f�B�A�E�B���h�̎擾.
	const RWindow* getWindow()
	{
		return Player->getWindow();
	}

	//�^�C�g���̎擾
	string getTitle() const
	{
		return Player->getTitle();
	}

private:
	RMediaViewPlayer*	Player;
};

#endif // !defined(AFX_RMEDIA_H__D918FCBA_5D18_46B9_8DFE_AF9D3F8A0F84__INCLUDED_)
