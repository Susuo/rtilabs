#include "StartWithExeFile.h"
#ifdef HIGH_SPEED_RESIDUAL_IMAGE_LIFE
/***************************************************************/
//�����c������  (C) Copylight R.T.I  since 2000...
//�������Ȏc���������s���܂��ł�.
/***************************************************************/


#include <.\Base\rti_debug.h>
#include <.\Base\rti_error_message.h>
#include <.\Graphics\Effect\rti_High_Speed_Residual_Image.h>
#include <.\Comm\rti_Comm.h>

/***************************************************************/
//�R���X�g���N�^
/***************************************************************/
THighSpeedResidualImageEffect::THighSpeedResidualImageEffect()
{
	//�O�̂��ߏ��������Ă����܂�.
	BufferUse = 0;
	Buffer = NULL;
}

/***************************************************************/
//�f�X�g���N�^
/***************************************************************/
THighSpeedResidualImageEffect::~THighSpeedResidualImageEffect()
{
	Init();
}

/***************************************************************/
//�o�b�t�@�̃N���A.
/***************************************************************/
void THighSpeedResidualImageEffect::Init()
{
	if (!Buffer) return ;
	int i;
	for(i = 0 ; i < 2 ; i++)
	{
		delete [] Buffer[i];
	}
	delete [] Buffer;
}

/***************************************************************/
//�Z�b�g
/***************************************************************/
bool THighSpeedResidualImageEffect::Set(BMD IOBmd)
{
	int AllSize;
	AllSize = (IOBmd->w * IOBmd->h * 3);
	if ( (AllSize%4) != 0)
	{
		FriendlyError("THighSpeedResidualImageEffect::Set",
			"�w��摜�o�^","�T�C�Y��4�̔{���ł͂Ȃ�","W*H*3 �� 4 �̔{���ɂȂ�悤�ɂ���!");
		return false;
	}
	//�O�̂��ߏ�����
	Init();
	//�o�^ 
	IOBmdSizeDiv4 =	AllSize / 4;
	THighSpeedResidualImageEffect::IOBmd = IOBmd;
	BufferUse = 0;
	//�o�b�t�@�̍쐬.
	Buffer = new BYTE*[2];
	Buffer[0] = new BYTE[AllSize];
	Buffer[1] = new BYTE[AllSize];

	//���b�N
	if ( !CommDraw->GraphicsLock(IOBmd) )	return false;
	memcpy(Buffer[0],IOBmd->bm,AllSize);
	memcpy(Buffer[1],IOBmd->bm,AllSize);
	//�A�����b�N
	CommDraw->GraphicsUnLock(IOBmd);
	return true;
}

/***************************************************************/
//�G��.
/***************************************************************/
bool THighSpeedResidualImageEffect::Draw()
{
	//���b�N
	if ( !CommDraw->GraphicsLock(IOBmd) )	return false;

	//�N���X�̃����o�A�N�Z�X�� ���X�e�b�v���̖��ʂ��������邽�߂�(�j?) ���[�J���ɕύX.
	BYTE*  Src = IOBmd->bm;
	BYTE** Dest= Buffer;
	int IoDiv4 = IOBmdSizeDiv4;
	//�����A���̗͂��v���m�点�Ă��!!
	_asm
	{
		mov esi,Src	//esi �Ɍ��摜�������Ă܂�.
		mov eax,Dest
		mov edi,[eax]		//�ŏ��̃o�b�t�@
		mov ecx,[eax+4]		//���̃o�b�t�@
		mov edx,IoDiv4	//�ő�]����(4�o�C�g�ÂA�N�Z�X����4�o�C�g�Âv�Z�i�[���܂�.)
						//����ɂ��A�]�����A 133% �̌������ł�͂�(���Ԃ�).
						//���z MMX ���Ă���.
		//��]�� edx �̓��[�v�J�E���^�[�Ɏg���Ă܂�.
	TRANCE_LOOP:
			//���Ȃ݂ɁA eax ebx �������Ă܂�.
			//�܂��ASrc��肢�������܂�.
			mov  eax,[esi]
			mov  ebx,[edi]
			push eax		//�I���W�i���̕ۑ�.
			shr  ebx,1		// /2
			shr  eax,1		// /2
			and  ebx,0x7f7f7f7f	//�N�b���s���O.
			and  eax,0x7f7f7f7f	//�N�b���s���O.
			
			//0�ԃo�b�t�@����������.
			add	 eax,ebx	//Src �� 0�Ԃ̔��������o��.

			pop  ebx		//�I���W�i���f�[�^���A.
			mov  [edi],ebx	//�I���W�i���Ɋi�[.

			//���� ebx ���A�����Ă܂�.  eax �ɂ́A Src��0�Ԃ̔��������͂����Ă܂�.
			shr  eax,1		    // /2
			mov  ebx,[ecx]
			shr  ebx,1		    // /2
			and  eax,0x7f7f7f7f	//�N���b�v
			and  ebx,0x7f7f7f7f	//�N���b�v

			add  eax,ebx		//Src��0�Ԃ̔����� �ɁA 1�Ԃ̕��ς�����āA����!
			mov  [esi],eax
		/*
			//�܂��A 0 �ԃo�b�t�@�Ƀ\�[�X�̓��e���R�s�[���܂�.
			//�ł��A 0 �ԃo�b�t�@�̓��e���\�[�X�ɔ������ō������Ȃ��Ă͂Ȃ�܂���.
			mov eax , [edi]			//0��
			shr eax,1				// /2
			and eax,0x7f7f7f7f		//�s�v�r�b�g�̐؂�̂�

			mov ebx , [esi]
			push ebx		//�I���W�i���̕ۑ�
			shr ebx,1		// /2
			and ebx,0x7f7f7f7f//�s�v�r�b�g�̐؂�̂�
			add eax,ebx		//0 �� Src �������܂�.(���ϒl)
			pop ebx			//�I���W�i�����A
			mov [edi],ebx	//0�ԂɃI���W�i���摜���R�s�[

			//���āA���� 1�ԃo�b�t�@�̏��������܂�.
			//�����ł́A eax �ɑO��( (Src+0��)/2) ������܂�.
			shr eax,1		// /2
			and eax,0x7f7f7f7f//�s�v�r�b�g�̐؂�̂�
			mov ebx,[ecx]
			shr ebx,1		// /2
			and ebx,0x7f7f7f7f//�s�v�r�b�g�̐؂�̂�
			add eax,ebx

			mov [esi],eax	//����𑗂�Ԃ��܂�.
		*/
			add ecx ,4
			add edi ,4
			add esi ,4
		dec edx
        jnz  TRANCE_LOOP
	}
	//�A�����b�N
	CommDraw->GraphicsUnLock(IOBmd);

	BYTE *T;
	T = (Buffer[0]);
	(Buffer[0]) = (Buffer[1]);
	(Buffer[1]) = T;

	return true;
}

#endif	//LIFE END
