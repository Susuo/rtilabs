// VirtualPath.h: VirtualPath �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALPATH_H__F59605AD_E21A_4FE0_8695_29B41BFFEAB2__INCLUDED_)
#define AFX_VIRTUALPATH_H__F59605AD_E21A_4FE0_8695_29B41BFFEAB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


//���z�p�X���l�X�g�ł��鐔.
const int MAX_NEXT_VIRTUAL_PATH	(128);

class VirtualPath  
{
private:
	VirtualPath()	{	}					//�Ăׂ܂���� �j����
public:
	VirtualPath(string inPath);
	virtual ~VirtualPath();

	//�t���p�X�̎擾.
	string getFullPath()			const	{		return Path;	}
	//�p�X�̎擾.
	string getSubPath(int inIndex)	const
	{
		ASSERT(inIndex >= 0);
		ASSERT(inIndex <= DilimiteresMax );

		if (inIndex == 0)
		{	//�擪
			return Path.substr( 0 , PathDilimiteres[0] );
		}
		else
		{	//����ȊO�̉���
			const int startPos = PathDilimiteres[inIndex-1]+2;
			return Path.substr( startPos , PathDilimiteres[inIndex] - startPos );
		}
	}
	//�p�X�̐[���̎擾.
	int getPathDilimiterCount()	const	{	return DilimiteresMax; }

//�T�[�r�X(�V���Y�̂�������)
	//���z�ł͂Ȃ��p�X�̎擾.
	string getNativePath()	const	{		return getSubPath(0);	}
	//�Ō�̃p�X�̎擾.
	string getLastPath()	const	{		return getSubPath(DilimiteresMax);	}
	//�Ōォ���O�̃p�X
	string getAdjacentPath()	const	
	{
		if (DilimiteresMax == 0)
		{
			return getNativePath();
		}
		else
		{
			return getSubPath(DilimiteresMax-1);
		}
	}

//�e�X�g
	static void test();

private:
	//���z�p�X�̃t���p�X.
	string	Path;	
	//�p�X�̋�؂� +1�͔ԕ�.
	int		PathDilimiteres[MAX_NEXT_VIRTUAL_PATH+1];
	//�����݂̍ő�l
	int		DilimiteresMax;	//
};

#endif // !defined(AFX_VIRTUALPATH_H__F59605AD_E21A_4FE0_8695_29B41BFFEAB2__INCLUDED_)
