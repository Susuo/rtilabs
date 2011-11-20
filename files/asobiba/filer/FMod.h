// FMod.h: FMod �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMOD_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_)
#define AFX_FMOD_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "fmod_define.h"
#include "RAutoLibraryWithException.h"
#include "RDoNotInitaliseException.h"
#include "RIOCannotOpenException.h"
#include "RIOCannotReadException.h"
#include "RIOCannotCloseException.h"
#include "RArchiveException.h"
#include "REnum.h"
#include "RFModException.h"




#include "FModCallable.h"

class FMod  
{
private:
	FMod();		//signleton
public:
	virtual ~FMod();

	//���C���V�X�e���̎擾 singleton
	static FMod*	getInstance();

	//getInstance �������ƂŕK���Ăяo���Ă�������
	//�����A����������Ă��Ȃ������珉��������.
	void New() throw()
	{
		if ( RefCounter == 0) 
		{
			//���Ԃ̍쐬.
			Create();
			//������.
			setOutput(FSOUND_OUTPUT_DSOUND);
			setDrivers(1);
			Init(44100, 32, 0);
		}
		RefCounter++;
	}
	//New() �������N���X�� ���������Ƃ��ɌĂяo���Ă�������.
	void Delete()
	{
		RefCounter--;
		if (RefCounter <= 0)
		{
			Close();
			AutoLib.Destory();
		}
	}


	//�o�͐�ꗗ�擾
	RENUM getNumOutputs()
	{
		RENUM p;
		p.push_back( RENUMPAIR("Direct Sound",1) );
		p.push_back( RENUMPAIR("Windows Multimedia Waveout",2) );
		p.push_back( RENUMPAIR("ASIO",3) );
		return p;
	}
	//�o�͐�擾
	int getOutput()
	{
		return FSOUND_GetOutput();
	}
	//�o�͐挈��
	void setOutput(int inIndex) throw()
	{
		if (! FSOUND_SetOutput(inIndex) )
		{
			RaiseException();
		}
	}

	//�h���C�o�ꗗ�擾
	RENUM getNumDrivers()
	{
		RENUM p;
		for (int i=0; i < FSOUND_GetNumDrivers(); i++) 
		{
			p.push_back( RENUMPAIR( string( (char*)FSOUND_GetDriverName(i)),i+1) );
		}
		return p;
	}
	//�h���C�o�擾
	int getDrivers()
	{
		return FSOUND_GetDriver();
	}
	//�h���C�o����
	void setDrivers(int inIndex) throw()
	{
		if (! FSOUND_SetDriver(inIndex) )
		{
			RaiseException();
		}
	}

	//�������̏�����
	void Init(int mixrate, int maxsoftwarechannels, unsigned int flags) const throw()
	{
		if (!FSOUND_Init(mixrate, maxsoftwarechannels, flags	))
		{
			RaiseException();
		}
	}
	//�������̃N���[�Y
	void Close() const
	{
		FSOUND_Close();
	}




	//�t�@�C�����J��
	FSOUND_STREAM* Open(const string inFilename)	throw()
	{
		FSOUND_STREAM* stream = 
			FSOUND_Stream_OpenFile(inFilename.c_str() , FSOUND_NORMAL | FSOUND_LOOP_NORMAL | FSOUND_MPEGACCURATE, 0);
		if (stream == NULL)
		{
			RaiseException();	//�G���[�Ȃ̂ŗ�O
		}
		return stream;
	}

	//�Đ�����.
	int Play(FSOUND_STREAM* inStream , FModCallable *inCallBack) throw()
	{
		int channel = FSOUND_Stream_PlayEx(FSOUND_FREE, inStream, NULL, TRUE);
		FSOUND_SetPaused(channel, FALSE);

		return channel;
	}
	//��~����.
	void Stop(int inChannel) throw()
	{
		FSOUND_StopSound(inChannel);
	}

	//�ꎞ��~ / ����
	void Pause(int inChannel,bool inON)	throw()
	{
		FSOUND_SetPaused(inChannel, inON );
	}

	//�t�@�C���N���[�Y
	void Close(FSOUND_STREAM* inStream )	throw()
	{
		FSOUND_Stream_Close(inStream);
	}

	//�{�����[��
	int getVolum(int inChannel) const throw()
	{
		return FSOUND_GetVolume(inChannel);
	}
	void setVolum(int inChannel,int inVolum) throw()
	{
		FSOUND_SetVolume(inChannel,	inVolum);
	}

	//�Đ��ʒu
	unsigned int  getPos(FSOUND_STREAM* inStream) const throw()
	{
		return FSOUND_Stream_GetPosition(inStream);
	}
	void setPos(FSOUND_STREAM* inStream,unsigned int inPos) throw ()
	{
		FSOUND_Stream_SetPosition(inStream,inPos);
	}
	unsigned int  getLength(FSOUND_STREAM* inStream) const throw()
	{
		return FSOUND_Stream_GetLength(inStream);
	}

	//�Đ���?
	bool IsPlay(int inChannel) const
	{
		return (FSOUND_IsPlaying(inChannel) != 0);
	}
	//�ꎞ��~��?
	bool IsPause(int inChannel) const
	{
		return (FSOUND_GetPaused(inChannel) != 0);
	}
private:

	void RaiseException() const throw();

//����ɂ����Ղ� DLL ����C���|�[�g����ɂ��
	typedef	void (WINAPI *FSOUND_CLOSE)			(VOID);
	FSOUND_CLOSE	FSOUND_Close;

	typedef	signed char (WINAPI *FSOUND_STREAM_CLOSE)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_CLOSE	FSOUND_Stream_Close;

	typedef	int (WINAPI *FSOUND_STREAM_GETLENGTHMS)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETLENGTHMS	FSOUND_Stream_GetLengthMs;

	typedef	int (WINAPI *FSOUND_STREAM_GETTIME)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETTIME	FSOUND_Stream_GetTime;

	typedef	int (WINAPI *FSOUND_STREAM_GETLENGTH)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETLENGTH	FSOUND_Stream_GetLength;

	typedef	unsigned int (WINAPI *FSOUND_STREAM_GETPOSITION)			(FSOUND_STREAM *stream);
	FSOUND_STREAM_GETPOSITION	FSOUND_Stream_GetPosition;

	typedef	signed char (WINAPI *FSOUND_STREAM_SETTIME)			(FSOUND_STREAM *stream, int ms);
	FSOUND_STREAM_SETTIME	FSOUND_Stream_SetTime;

	typedef	int (WINAPI *FSOUND_SETPAUSED)			(int channel, signed char paused);
	FSOUND_SETPAUSED	FSOUND_SetPaused;

	typedef	signed char (WINAPI *FSOUND_STREAM_PLAYEX)			(int channel, FSOUND_STREAM *stream, FSOUND_DSPUNIT *dsp, signed char startpaused);
	FSOUND_STREAM_PLAYEX	FSOUND_Stream_PlayEx;

	typedef	char* (WINAPI *FSOUND_SAMPLE_GETNAME)			(FSOUND_STREAM *stream);
	FSOUND_SAMPLE_GETNAME	FSOUND_Sample_GetName;

	typedef	signed char (WINAPI *FSOUND_SAMPLE_GETDEFAULTS)			(FSOUND_SAMPLE *sptr, int *deffreq, int *defvol, int *defpan, int *defpri);
	FSOUND_SAMPLE_GETDEFAULTS	FSOUND_Sample_GetDefaults;

	typedef	FSOUND_SAMPLE * (WINAPI *FSOUND_SAMPLE_GETSAMPLE)			(FSOUND_STREAM *stream);
	FSOUND_SAMPLE_GETSAMPLE	FSOUND_Stream_GetSample;

	typedef	signed char (WINAPI *FSOUND_SAMPLE_SETSYNCHCALLBACK)			(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
	FSOUND_SAMPLE_SETSYNCHCALLBACK	FSOUND_Stream_SetSynchCallback;

	typedef	signed char (WINAPI *FSOUND_SAMPLE_SETENDCALLBACK)			(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
	FSOUND_SAMPLE_SETENDCALLBACK	FSOUND_Stream_SetEndCallback;

	typedef	int (WINAPI *FSOUND_GETERROR)			(VOID);
	FSOUND_GETERROR	FSOUND_GetError;

	typedef	int (WINAPI *FSOUND_GETDRIVER)			(VOID);
	FSOUND_GETDRIVER FSOUND_GetDriver;

	typedef	signed char (WINAPI *FSOUND_SETDRIVER)			(int driver);
	FSOUND_SETDRIVER FSOUND_SetDriver;

	typedef	signed char * (WINAPI *FSOUND_SETDRIVERNAME)			(int id);
	FSOUND_SETDRIVERNAME FSOUND_GetDriverName;

	typedef int (WINAPI *FSOUND_GETNUMDRIVERS)			(VOID);
	FSOUND_GETNUMDRIVERS FSOUND_GetNumDrivers;

	typedef int (WINAPI *FSOUND_GETOUTPUT)			(VOID);
	FSOUND_GETOUTPUT FSOUND_GetOutput;

	typedef signed char (WINAPI *FSOUND_SETOUTPUT)			(int outputtype);
	FSOUND_SETOUTPUT	FSOUND_SetOutput;

	typedef float (WINAPI *FSOUND_GETVERSION)			(VOID);
	FSOUND_GETVERSION	FSOUND_GetVersion;

	typedef float (WINAPI *FSOUND_INIT)			(int mixrate, int maxsoftwarechannels, unsigned int flags);
	FSOUND_INIT	FSOUND_Init;

	typedef FSOUND_STREAM* (WINAPI *FSOUND_STREAM_OPENFILE)			(const char *filename, unsigned int mode, int memlength);
	FSOUND_STREAM_OPENFILE	FSOUND_Stream_OpenFile;

	typedef signed char (WINAPI *FSOUND_STOPSOUND)			(int channel);
	FSOUND_STOPSOUND	FSOUND_StopSound;

	typedef int (WINAPI *FSOUND_GETPAN)			(int channel);
	FSOUND_GETPAN	FSOUND_GetPan;

	typedef int (WINAPI *FSOUND_GETVOLUME)			(int channel);
	FSOUND_GETVOLUME	FSOUND_GetVolume;

	typedef signed char (WINAPI *FSOUND_SETVOLUME)			(int channel,int vol);
	FSOUND_SETVOLUME	FSOUND_SetVolume;

	typedef signed char (WINAPI *FSOUND_SETPOS)			(int channel,int pan);
	FSOUND_SETPOS		FSOUND_SetPan;

	typedef	signed char (WINAPI *FSOUND_STREAM_SETPOSITION)			(FSOUND_STREAM *stream, unsigned int position);
	FSOUND_STREAM_SETPOSITION	FSOUND_Stream_SetPosition;

	typedef int (WINAPI *FSOUND_ISPLAYING)			(int channel);
	FSOUND_ISPLAYING	FSOUND_IsPlaying;

	typedef signed char (WINAPI *FSOUND_GETPAUSED)			(int channel);
	FSOUND_GETPAUSED	FSOUND_GetPaused;
private:
	//!�������ł��Ȃ������� RDoNotInitaliseException
	//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
	void Create() throw() ;
private:
	RAutoLibraryWithException	AutoLib;
	int					RefCounter;

};

#endif // !defined(AFX_FMOD_H__83F886AC_09F7_4DCA_A298_F82DE1089607__INCLUDED_)
