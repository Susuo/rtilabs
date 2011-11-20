// RStreamUtil.cpp: RStreamUtil �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RStreamUtil.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


/**
 * WriteBuffer:
 *
 * @param	inBuffer		�������ރo�b�t�@
 * @return  ���ۂɏ������߂��T�C�Y
 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
 */
int RStreamUtil::WriteBuffer(string inBuffer) throw(RException)
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
int RStreamUtil::WriteBuffer(const char *inBuffer) throw(RException)
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
int RStreamUtil::WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException)
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
		sended = Stream->Write(inBuffer+sendedLen ,inBufferSize - sendedLen);
		if ( sended == 0 )
		{//�ǂݍ��ݏI��
			break;
		}
		else if (sended < 0)
		{//�ُ펖��
			throw RException(EXCEPTIONTRACE + "�������ݎ��ɃG���[���������܂���");
		}
	}
	return sended;
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
int RStreamUtil::ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException)
{
	/*ChangeLog.txt:2003/01/25	�ǂݍ��񂾍��v�T�C�Y�ł͂Ȃ��āA���ɓǂ߂��߂��T�C�Y��Ԃ��Ă����̂ŏC���B �悭�����Ă�����(w
	*/
	ASSERT(Stream != NULL);
	ASSERT(outBuffer != NULL);
	ASSERT(inStreamChecker != NULL);

	int		sended;			//���A���M�ł����T�C�Y.
	int		sendedLen;		//���݂܂łɑ��M�����T�C�Y.

	for(sendedLen = 0 ; sendedLen < inBufferSize ; sendedLen += sended)
	{
		//�f�[�^���M.
		sended = Stream->Read(outBuffer+sendedLen ,inBufferSize - sendedLen);
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
			return sendedLen + sended;
		}
		
	}
	return sendedLen;
}

