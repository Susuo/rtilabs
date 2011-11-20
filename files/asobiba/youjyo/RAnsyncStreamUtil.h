// RAnsyncStreamUtil.h: RAnsyncStreamUtil �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANSYNCSTREAMUTIL_H__F2A1A38C_F979_4623_8D2B_11585D3EF7A9__INCLUDED_)
#define AFX_RANSYNCSTREAMUTIL_H__F2A1A38C_F979_4623_8D2B_11585D3EF7A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RStreamAsync.h"
#include "RStreamUtil.h"

class RAnsyncStreamUtil  
{
public:
	RAnsyncStreamUtil();
	virtual ~RAnsyncStreamUtil();
	/**
	 * Mount:		�����ΏۂƂȂ�X�g���[�����}�E���g.
	 *
	 * @param ioStream	�����ΏۂƂȂ�X�g���[��
	 * @return void 
	 */
	void Mount(RStreamAsync *ioStream)
	{
		Stream = ioStream;
	}
	/**
	 * WriteBuffer:	�������ݗ\��
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @param	inBufferSize	�o�b�t�@�T�C�Y
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	void RAnsyncStreamUtil::WriteBufferReservation(const char *inBuffer ,int inBufferSize) throw(RException);

	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int RAnsyncStreamUtil::WriteBuffer(string inBuffer) throw(RException);

	/**
	 * WriteBuffer:
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer) throw(RException);

	/**
	 * WriteBuffer:	��������
	 *
	 * @param	inBuffer		�������ރo�b�t�@
	 * @param	inBufferSize	�o�b�t�@�T�C�Y
	 * @return  ���ۂɏ������߂��T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int RAnsyncStreamUtil::WriteBuffer(const char *inBuffer ,int inBufferSize) throw(RException);

	/**
	 * ReadBuffer:	�ǂݍ��ݗ\��
	 *
	 * @param	outBuffer		�ǂݍ��܂��o�b�t�@.
	 * @param	inBufferSize	�o�b�t�@�T�C�Y
	 * @return  �ǂݍ��񂾃T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	void RAnsyncStreamUtil::ReadBufferReservation(char *outBuffer ,int inBufferSize) throw(RException);


	/**
	 * ReadBuffer:	�ǂݍ���
	 *
	 * @param	outBuffer		�ǂݍ��܂��o�b�t�@.
	 * @param	inBufferSize	�o�b�t�@�T�C�Y
	 * @param	inStreamChecker	�ǂݍ��񂾃o�b�t�@�𒲍�����N���X.
	 * @return  �ǂݍ��񂾃T�C�Y
	 * @throws	RException	�������ݒ��ɃG���[�̏ꍇ.
	 */
	int RAnsyncStreamUtil::ReadBuffer(char *outBuffer ,int inBufferSize,StreamChecker* inStreamChecker) throw(RException);


private:
	RStreamAsync	* Stream;

};

#endif // !defined(AFX_RANSYNCSTREAMUTIL_H__F2A1A38C_F979_4623_8D2B_11585D3EF7A9__INCLUDED_)
