// FModPlay.h: FModPlay �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMODPLAY_H__42FA9301_CBCD_40DE_ACE7_5BD7BD397F39__INCLUDED_)
#define AFX_FMODPLAY_H__42FA9301_CBCD_40DE_ACE7_5BD7BD397F39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "fmod.h"

class FModPlay  
{
public:
	FModPlay()
	{
		Stream	=	NULL;
		Sptr	=	NULL;
		Channel	=	-1;

		FModCore = FMod::getInstance();
		FModCore->New();
	}
	virtual ~FModPlay()
	{
		if ( Stream != NULL )
		{
			Close();
		}

		FModCore->Delete();
	}


	//�t�@�C�����J��
	void Open(const string inFilename )	throw()
	{
		if (Stream != NULL)
		{
			Close();
		}

		Stream = FModCore->Open(inFilename );
	}
	//�Đ�����.
	void Play(FModCallable *inCallBack) throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"�Đ�����t�@�C�����J����Ă��܂���");
		}
		if (Channel >= 0)
		{
			throw RFModException(0,"�`���l�� %d �́A���łɍĐ����s���Ă��܂�",Channel);
		}

		Channel = FModCore->Play(Stream , inCallBack);
	}

	//��~����.
	void Stop() throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"�`���l��%d �͍Đ��������Ȃ��Ă��܂���",Channel);
		}

		FModCore->Stop(Channel);
		Channel = -1;
	}

	//�ꎞ��~ / ����
	void Pause(bool inON)	throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"�`���l��%d �͍Đ��������Ȃ��Ă��܂���",Channel);
		}

		FModCore->Pause(Channel , inON);
	}

	//�t�@�C���N���[�Y
	void Close()	throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"���̃X�g���[���ł͍Đ����s���Ă��܂���");
		}

		FModCore->Close(Stream);
	}

	//�{�����[��
	int getVolum() const throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"�`���l��%d �͍Đ��������Ȃ��Ă��܂���",Channel);
		}

		return FModCore->getVolum(Channel);
	}
	void setVolum(int inVolum) throw()
	{
		if (Channel < 0)
		{
			throw RFModException(0,"�`���l��%d �͍Đ��������Ȃ��Ă��܂���",Channel);
		}

		FModCore->setVolum(Channel,inVolum);
	}

	//�Đ��ʒu
	int  getPos() const throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"���̃X�g���[���ł͍Đ����s���Ă��܂���");
		}

		return FModCore->getPos(Stream);
	}
	void setPos(int inPos) throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"���̃X�g���[���ł͍Đ����s���Ă��܂���");
		}

		FModCore->setPos(Stream,inPos);
	}
	int  getLength() const throw()
	{
		if (Stream == NULL)
		{
			throw RFModException(0,"���̃X�g���[���ł͍Đ����s���Ă��܂���");
		}

		return FModCore->getLength(Stream);
	}

	//�Đ���?
	bool IsPlay() const
	{
		if (Channel < 0)
		{
			throw RFModException(0,"�`���l��%d �͍Đ��������Ȃ��Ă��܂���",Channel);
		}

		return FModCore->IsPlay(Channel);
	}
	//�ꎞ��~��?
	bool IsPause() const
	{
		if (Channel < 0)
		{
			throw RFModException(0,"�`���l��%d �͍Đ��������Ȃ��Ă��܂���",Channel);
		}

		return FModCore->IsPause(Channel);
	}

private:
	FMod * FModCore;

	FSOUND_STREAM	*Stream;
	FSOUND_SAMPLE	*Sptr;
	int				Channel;
};

#endif // !defined(AFX_FMODPLAY_H__42FA9301_CBCD_40DE_ACE7_5BD7BD397F39__INCLUDED_)
