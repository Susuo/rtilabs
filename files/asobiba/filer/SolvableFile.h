// SolvableFile.h: SolvableFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLVABLEFILE_H__4E5B2CB0_2CF6_47D6_82E4_F37AFF78F3A9__INCLUDED_)
#define AFX_SOLVABLEFILE_H__4E5B2CB0_2CF6_47D6_82E4_F37AFF78F3A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SolvableFile  ;

#include "comm.h"
#include "PathInterface.h"

//�t�@�C��������.
class SolvableFile  : public PathInterface
{
public:
	SolvableFile()
	{
	}
	virtual ~SolvableFile()
	{
	}

	//�������Ă�������
	SolvableFile* PleaseSolve(string inPath)
	{
		//�����łł���? �ł����牴���������@���Ă��ƂŎ������A���B
		if ( this->DoSolvable(inPath) ) return this;
		//�Ȃ�ł����A�����ł��Ȃ����B �������߂ہB
		return NULL;
	}

	//��f�B���N�g�����オ��
	virtual string UpDirectory() const ;

protected:
	//���̃p�X���������Ă݂�
	//�����ł����ꍇ�͉����������ʂ̃p�X�� �����o�� setPath �ŃZ�b�g���邱��.
	//www.yahoo.co.jp ---> http://www.yahoo.co.jp/ �ȂǂƂȂ�
	//�����ł����ꍇ�� true �����ł��Ȃ������ꍇ�� false
	virtual bool DoSolvable(string inPath)  = 0;

};

#endif // !defined(AFX_SOLVABLEFILE_H__4E5B2CB0_2CF6_47D6_82E4_F37AFF78F3A9__INCLUDED_)
