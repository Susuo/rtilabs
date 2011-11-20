// RStreamUtil.h: RStreamUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSTREAMUTIL_H__A0BEA0FA_B53F_483F_ACB5_6B20219513DC__INCLUDED_)
#define AFX_RSTREAMUTIL_H__A0BEA0FA_B53F_483F_ACB5_6B20219513DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStream.h"
#include "RException.h"


class StreamChecker
{
public:
	StreamChecker(){}
	~StreamChecker(){}
	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead) = 0;
};


//�A��������s�R�[�h�܂�
class StreamCheckerForSecutiveReturnCode : public StreamChecker
{
public:
	StreamCheckerForSecutiveReturnCode()
	{
		Separater = 0;
		ConectStart = 0;
	}
	~StreamCheckerForSecutiveReturnCode()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		if (inAllReadyRead -4  < 0) inAllReadyRead = 0;

		for (int i = inAllReadyRead; i < (inNowRead-3) ; i ++) 
		{
			if ( (inBuffer[i] == '\r' && inBuffer[i+1] == '\n' && inBuffer[i+2] == '\r' && inBuffer[i+3] == '\n') )
			{
				ConectStart = i + 4;
				Separater = i;
				return false;
			}
			else if (	(inBuffer[i] == '\r' && inBuffer[i+1] == '\r') ||
						(inBuffer[i] == '\r' && inBuffer[i+1] == '\r')  )
			{
				ConectStart = i + 2;
				Separater = i;
				return false;
			}
		}
		return true;
	}

	int getSeparater()
	{
		return Separater;
	}
	int getConectStart()
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
};

//�w�蒷���܂�
class StreamCheckerForLength : public StreamChecker
{
public:
	StreamCheckerForLength(int inLength)
	{
		this->Length = inLength;
	}
	~StreamCheckerForLength()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		return  (inNowRead < this->Length);
	}
private:
	//�ǂݍ��ޒ���
	int	Length;
};


class RStreamUtil	
{
public:
	RStreamUtil()
	{
		Stream = NULL;
	}
	~RStreamUtil()
	{
	}

	/**
	 * Mount:		�����ΏۂƂȂ�X�g���[�����}�E���g.
	 *
	 * @param ioStream	�����ΏۂƂȂ�X�g���[��
	 * @return void 
	 */
	void Mount(RStream *ioStream)
	{
		Stream = ioStream;
	}

	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @param	inBufferSize	�o�b�t�@�T�C�Y
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException);
	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int WriteBuffer(const char *inBuffer) throw(RException);
	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int WriteBuffer(string inBuffer) throw(RException);

	/**
	 * ReadBuffer:
	 *
	 * @param	outBuffer		�ǂݍ��܂��o�b�t�@.
	 * @param	inBufferSize	�o�b�t�@�T�C�Y
	 * @param	inStreamChecker	�ǂݍ��񂾃o�b�t�@�𒲍�����N���X.
	 * @return  �ǂݍ��񂾃T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException);
private:
	RStream	* Stream;
};

#endif // !defined(AFX_RSTREAMUTIL_H__A0BEA0FA_B53F_483F_ACB5_6B20219513DC__INCLUDED_)
