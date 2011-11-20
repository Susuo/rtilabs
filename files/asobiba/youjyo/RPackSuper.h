// RPackSuper.h: RPackSuper �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RPACKSUPER_H__4A989A1D_111A_4862_9574_52417852088C__INCLUDED_)
#define AFX_RPACKSUPER_H__4A989A1D_111A_4862_9574_52417852088C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


class RPackSuper
{
public:
	/**
	 * RPack:		�R���X�g���N�^
	 *
	 */
	RPackSuper()
	{
		this->Memory = NULL;
		this->Len = 0;
		this->ForcesBuffer = NULL;
		this->ForcesBufferLen = 0;
	}
	/**
	 * RPack:			�����l���R���X�g���N�^
	 *
	 * @param inMem		�i�[���郁����
	 * @param inLen		�������̒���
	 */
	RPackSuper(const void* inMem,int inLen)
	{
		this->Memory = NULL;
		this->Len = 0;
		this->ForcesBuffer = NULL;
		this->ForcesBufferLen = 0;

		Write(inMem , inLen);
	}
	~RPackSuper()
	{
		delete [] this->Memory;
	}
	/**
	 * Write:			�V�K�Ɋi�[����(�����̃������͉������܂�)
	 *
	 * @param inStr		������
	 */
	void Write(const char* inStr)
	{
		this->Write( (const void*)inStr , strlen(inStr) );
	}

	/**
	 * Write:			�V�K�Ɋi�[����(�����̃������͉������܂�)
	 *
	 * @param inMem		�i�[���郁����
	 * @param inLen		�������̒���
	 */
	void Write(const void* inMem,int inLen)
	{
		ASSERT(inMem != NULL);
		ASSERT(inLen >= 0);

		delete [] Memory;
		if ( this->ForcesBufferLen < inLen)
		{
			this->Memory = new char[inLen];
			memcpy(this->Memory,inMem , inLen);
		}
		else
		{
			Memory = NULL;
//			memcpy(this->ForcesBuffer,inMem , inLen);
			char * p = this->ForcesBuffer;
			_asm
			{
				cld
				mov ecx,inLen
				mov esi,inMem
				add ecx,3
				mov edi,p
				shr ecx,2
				rep movsd;
			}
		}
		this->Len = inLen;
	}

	/**
	 * Read:			�V�K�Ɋi�[����(�����̃������͉������܂�)
	 *
	 * @param outMem	�ǂݏo��������
	 * @param inLen		�������̒���
	 * @return int		�ǂݏo�����T�C�Y.
	 */
	int Read(char* outMem,int inLen) const
	{
		ASSERT(outMem != NULL);
		ASSERT(inLen >= 0);

		//�␳.
		if (inLen > this->Len) inLen = this->Len ;

		memcpy(outMem , this->Memory , inLen);
		return inLen;
	}

	/**
	 * getMemory:	�i�[���Ă��郁�����֒��Q��
	 *
	 * @return const void*	�ǂݏo��Only�ȃ�����.
	 */
	const void*	getMemory() const
	{
		return this->Memory != NULL ? this->Memory : this->ForcesBuffer;
	}

	/**
	 * getSize:		�i�[���Ă��郁�����T�C�Y
	 *
	 * @return const int	�T�C�Y.
	 */
	int	getSize() const
	{
		return this->Len;
	}

	void Mount(char* inForcesBuffer , int inBufferLen)
	{
		ForcesBuffer = inForcesBuffer;
		ForcesBufferLen = inBufferLen;
	}

private:
	//�O�ł͑���Ȃ��Ȃ����Ƃ��̂��߂̎��O�q�[�v
	char*	Memory;
	int		Len;

	//�D�悵�Ďg���O�̃o�b�t�@
	char*	ForcesBuffer;
	int		ForcesBufferLen;
};

typedef RPackSuper	RPack;

#endif // !defined(AFX_RPACKSUPER_H__4A989A1D_111A_4862_9574_52417852088C__INCLUDED_)
