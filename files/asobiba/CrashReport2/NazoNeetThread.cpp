
#include "stdafx.h"
#include "NazoNeetThread.h"


unsigned int NazoNeetThread::Run()
{
	//�q�[�v�̎擾�f������肽���̂œK���ɁB
	vector<char> v;
	v.reserve(1024);
	memset(&v[0] , 1024 ,72);	//���@�̐��� 72. ���[�����������߂�ȂƉ��x������(ry

	int a = 0;
	//�j�[�g�̈��
	//�j�[�g�̒��͑����A�����Ă܂��Q��.
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//�ǂ����Ă�������񂽂������Ȃ��́H
	a = 1;
	//�j�[�g�̖ڊo��
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//2ch�̃`�F�b�N���ă_�E�\���Ă��Ȃʂ��Ă����瓭�����ԂȂ񂩂Ȃ��ˁH ���̗����͂��������B
	a = 2;
	//�j�[�g�̒�
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//���Ԃ̖ڂ�������
	a = 3;
	//�j�[�g�̂����
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//�������畉�����ȂƎv�����B
	a = 4;
	//�j�[�g�̗[��
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//�e�u�����I�v �u���邹�[�o�o�@�v ���Ƒ��̒c�R �C�C�n�i�V�_�[ (;_;
	a = 5;
	//�j�[�g�̖�
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//�ł��A�l�͓����̂��������B�������A�ڂ��͓����Ȃ������񂾁B�j�[�g�ł��Ă������񂾁B���߂łƂ��B
	a = 6;
	//�j�[�g�̐[��
	this->Sleep(1000000);
	if (this->IsHoldUp()) return 0;	//���ɂ��肪�Ƃ��B�ꂳ�悤�Ȃ�B�����Ă��ׂẴj�[�g�ɂ��߂łƂ��B
	a = 7;
	//�j�[�g�̑���
	this->Sleep(1000000);
	a = 8;
	
	//�������������΂�����
	return 0;
}
