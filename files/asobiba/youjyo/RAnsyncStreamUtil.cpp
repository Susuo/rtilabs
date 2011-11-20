// RAnsyncStreamUtil.cpp: RAnsyncStreamUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RAnsyncStreamUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RAnsyncStreamUtil::RAnsyncStreamUtil()
{
	Stream = NULL;
}

RAnsyncStreamUtil::~RAnsyncStreamUtil()
{

}

/**
 * WriteBuffer:	�������ݗ\��
 *
 * @param	inBuffer		�������ރo�b�t�@
 * @param	inBufferSize	�o�b�t�@�T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
void RAnsyncStreamUtil::WriteBufferReservation(const char *inBuffer ,int inBufferSize) throw(RException)
{
	if (inBufferSize == 0) return ;

	ASSERT(Stream != NULL);
	ASSERT(inBuffer != NULL);
	ASSERT(inBufferSize > 0);

	Stream->Write(inBuffer ,inBufferSize );
}

/**
 * WriteBuffer:
 *
 * @param	inBuffer		�������ރo�b�t�@
 * @return  ���ۂɏ������߂��T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
int RAnsyncStreamUtil::WriteBuffer(string inBuffer) throw(RException)
{
	return this->WriteBuffer(inBuffer.c_str() , inBuffer.size() );
}

/**
 * WriteBuffer:
 *
 * @param	inBuffer		�������ރo�b�t�@
 * @return  ���ۂɏ������߂��T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer) throw(RException)
{
	ASSERT(Stream != NULL);
	ASSERT(inBuffer != NULL);

	return this->WriteBuffer(inBuffer , strlen(inBuffer) );
}

/**
 * WriteBuffer:	��������
 *
 * @param	inBuffer		�������ރo�b�t�@
 * @param	inBufferSize	�o�b�t�@�T�C�Y
 * @return  ���ۂɏ������߂��T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException)
{
	if (inBufferSize == 0) return 0;

	ASSERT(Stream != NULL);
	ASSERT(inBuffer != NULL);
	ASSERT(inBufferSize > 0);

	int		sended;			//���A���M�ł����T�C�Y.
	int		sendedLen;		//���݂܂łɑ��M�����T�C�Y.

	for(sendedLen = 0 ; sendedLen < inBufferSize ; sendedLen += sended)
	{
		//�f�[�^���M.
		sended = Stream->WriteWaitForSignal();
		if ( sended == 0 )
		{//�ǂݍ��ݏI��
			break;
		}
		else if (sended < 0)
		{//�ُ펖��
			throw RException(EXCEPTIONTRACE + "�������ݎ��ɃG���[���������܂���");
		}
		//�f�[�^���M�\��.
		Stream->Write(inBuffer+sendedLen ,inBufferSize - sendedLen);
	}
	return sended;
}

/**
 * ReadBuffer:	�ǂݍ��ݗ\��
 *
 * @param	outBuffer		�ǂݍ��܂��o�b�t�@.
 * @param	inBufferSize	�o�b�t�@�T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
void RAnsyncStreamUtil::ReadBufferReservation(char *outBuffer ,int inBufferSize) throw(RException)
{
	ASSERT(Stream != NULL);
	Stream->Read(outBuffer ,inBufferSize );
}

/**
 * ReadBuffer:	�ǂݍ���
 *
 * @param	outBuffer		�ǂݍ��܂��o�b�t�@.
 * @param	inBufferSize	�o�b�t�@�T�C�Y
 * @param	inStreamChecker	�ǂݍ��񂾃o�b�t�@�𒲍�����N���X.
 * @return  �ǂݍ��񂾃T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
int RAnsyncStreamUtil::ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException)
{
	ASSERT(Stream != NULL);
	ASSERT(outBuffer != NULL);
	ASSERT(inStreamChecker != NULL);

	int		sended;			//���A���M�ł����T�C�Y.
	int		sendedLen;		//���݂܂łɑ��M�����T�C�Y.

	for(sendedLen = 0 ; sendedLen < inBufferSize ; sendedLen += sended)
	{
		//�f�[�^���M.
		sended = Stream->ReadWaitForSignal();
		if ( sended == 0 )
		{//�ǂݍ��ݏI��
			break;
		}
		else if (sended < 0)
		{//�ُ펖��
			throw RException(EXCEPTIONTRACE + "�ǂݍ��ݎ��ɃG���[���������܂���");
		}
		//���ǂݍ��񂾒��ɏI�����������邩�`�F�b�N.
		if ( ! inStreamChecker->Check(outBuffer , sendedLen , sendedLen+sended) )
		{	//�`�F�b�J�[�ɂ���~.
			return sended;
		}
		//�f�[�^�\��.
		Stream->Read(outBuffer+sendedLen ,inBufferSize - sendedLen);
	}
	return sended;
}

