#ifndef ___RING_QUEUEH
#define ___RING_QUEUEH

/**************************************************************************************/
//�����O�L���[ STL �R���p�`�ɂȂ�������?
//			(C) R.T.I
/**************************************************************************************/
#include <stdio.h>

template<class _T>

class ringqueue256
{
	//�����O�L���[�o�b�t�@
	_T RingQueueBuffer[256];
	//255 �` 0 �܂ł��������[�v ( Oo-y~~ )
	unsigned char Top ;

public:
	ringqueue256(){Top = 0;};
	//�v�f��ǉ�.
	void push_back(_T* p){RingQueueBuffer[Top++] = *p;} ;
	//�v�f���폜.
	void pop_back(){Top--; } ;
	//�擪�̗v�f��Ԃ�
//	_T begin(){return &RingQueueBuffer[0];};
	//�Ō�̗v�f��Ԃ�.
//	_T end(){return &RingQueueBuffer[Top];};
	//�T�C�Y��Ԃ�.
	int size(){return Top;};
	//����ۂ��ǂ����Ԃ�.
	bool empty(){ return (size == 0); };
	//�ŏ��̒l��Ԃ�.
	_T* front(){return &RingQueueBuffer[0]; } ;
	//�Ō�̒l���擾���āA�Ō�̒l --
	_T* pop(){return &RingQueueBuffer[--Top]; } ;
	//�Ōォ��w�肳�ꂽ��Ԗڂ̃f�[�^
	_T* back(int a = 0){return &RingQueueBuffer[ unsigned char(Top-1-a)]; } ;
	//�N���A
	void clear(){Top = 0;	memset(RingQueueBuffer,0,256);};
	//���݂̏I�[�̎擾.
	unsigned char getTop(){return Top;}
	//�v�f���擾.
	void getes(_T* p,unsigned char len , unsigned char pos )
	{	for( unsigned char i = 0 ; i < len ; i++ , p++ ) *p = RingQueueBuffer[ (unsigned char)(i + pos) ];	}

};


#endif
