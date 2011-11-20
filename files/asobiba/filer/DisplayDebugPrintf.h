// DisplayDebugPrintf.h: DisplayDebugPrintf �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAYDEBUGPRINTF_H__D44F33B3_CC04_49CA_B6BE_3AF1FEE6250D__INCLUDED_)
#define AFX_DISPLAYDEBUGPRINTF_H__D44F33B3_CC04_49CA_B6BE_3AF1FEE6250D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DisplayDebugPrintf;
#include "comm.h"
#include "Display.h"
#include "RfileLocalReport.h"
#include "RfileLocalBigIcon.h"
#include "RfileArcBigIcon.h"

//�f�o�b�O�\���p�̃t�@�C���ꗗ�\��.
class DisplayDebugPrintf : public Display
{
public:
	DisplayDebugPrintf(Window* inWindow = NULL ,__RFILETYPE inNavigateType = __RFILETYPE_LOCALFILE_REPORT);
	virtual ~DisplayDebugPrintf();

	//FixedColum���ύX���ꂽ�Ƃ��ɌĂ΂��.
	virtual void OnChangeFixedColum(){}
	//�A�N�e�B���ɂȂ�܂���
	virtual void OnActive() const{	}

	//��A�N�e�B���ɂȂ�܂���
	virtual void OnNoActive() const{	}

	string DebugBuffer;

	//�f�B�X�v���C�̎�ނ�Ԃ��܂��B
	virtual __DISPLAYTYPE getType() const 
	{
		return __DISPLAYTYPE_DEBUG;
	}

	//���݃J�[�\��������t�@�C�������擾���܂��B
	virtual string getActiveFilename() const 
	{
		return "";
	}

	//���݃}�[�N����Ă���t�@�C����n���܂��B
	virtual StringList getMarkFilenames() const 
	{
		return StringList();	//����Ȃ��̂͂Ȃ�
	}

	//�\�����ڂ̐ݒ�
	virtual void setRfileItems(int inSize,const Rfile** inRfileItems)
	{
		for (int i = 0; i < inSize ; i++)
		{
			const Rfile * p  = inRfileItems[i];
			DebugBuffer += p->getVars(__FILECOLUMN_TYPE_NAME) + " �� " + p->getVars(__FILECOLUMN_TYPE_EXT) + " ";
			TRACE("filename %s �� ext %s\n",
					p->getVars(__FILECOLUMN_TYPE_NAME).c_str() , p->getVars(__FILECOLUMN_TYPE_EXT).c_str() );
		}
	}
	//�f�B�X�v���C�E�B���h�E�̕��ƍ������擾
	virtual void GetWindowRect(RECT * outRC) const
	{
		ZeroMemory(outRC , sizeof(RECT) );
	}
	//�w�肵�����j���[���|�b�v�A�b�v����.
	virtual string PopupMenu(const string inMenuName) const throw(RWin32Exception,RNoComplateException,RNotFoundException) 
	{
		return "";	//���ƂŎ�������.
	}
	//�^�C�g�����擾����
	virtual string getTitle() const 
	{
		return "";
	}

	//�\��������̂̍쐬.
	virtual	void CreateShowObject(const PathInterface* inPath) 
	{
	}
	static void test();
};

#endif // !defined(AFX_DISPLAYDEBUGPRINTF_H__D44F33B3_CC04_49CA_B6BE_3AF1FEE6250D__INCLUDED_)
