// SusieCallable.h: SusieCallable �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIECALLABLE_H__F17020E0_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
#define AFX_SUSIECALLABLE_H__F17020E0_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Susie �v���O�C���̐i�����`�F�L������
class SusieCallable  
{
public:
	SusieCallable()
	{
	}
	virtual ~SusieCallable()
	{
	}
protected:
    //�܂� nNum==0 �ŃR�[������AnNum==nDenom �ɂȂ�܂�
    //����I�ɌĂ΂��B
    //�ߒl�� ��0 �̎��APlug-in�͏����𒆒f����B
	virtual int SusieaCallBack(int inNum,int inDenom) 
	{
		return 0;
	}
public:
	//Susie�v���O�C���̐i�����`�F�L������
	static  int __stdcall ProgressCallback(int inNum,int inDenom,long inData)
	{
		return ((SusieCallable*)inData)->SusieaCallBack(inNum,inDenom);
	}

};

#endif // !defined(AFX_SUSIECALLABLE_H__F17020E0_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
