// YReadCGITemplate.h: YReadCGITemplate �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YREADCGITEMPLATE_H__BEE10152_2FC9_4642_9CDA_4117D58D94AA__INCLUDED_)
#define AFX_YREADCGITEMPLATE_H__BEE10152_2FC9_4642_9CDA_4117D58D94AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHtmlTemplate.h"
#include "YDAT.h"
#include "YSettingTXT.h"

class YReadCGITemplate  
{
public:
	YReadCGITemplate();
	virtual ~YReadCGITemplate();

	void Load(const string inBoardName ,const YSettingTXT*	inSettingTXT);

	/**
	 * make:					�w�肳�ꂽ�͈͓��ŃX���b�h�̒��g�� html �ŕ\������.
	 *
	 * @param inDAT				�g�p���� DAT.
	 * @param inStart			�J�n�ʒu
	 * @param inEnd 
	 * @param is1san 
	 * @return const string 
	 */
	const string make(YDAT* inDAT ,unsigned int inStart ,unsigned int inEnd , bool is1san) const;

	static void test();

private:

	//�y�[�W�����[�v
	const RHtmlTemplate* PageJump;
	//�O100��
	const RHtmlTemplate* IfBefor100;
	//��100��
	const RHtmlTemplate* IfNext100;
	//�������ݕ\������. ���C���ł��ȁB
	const RHtmlTemplate* Kakiko;
	//���X����<!--?V:YResAlert-->�𒴂��Ă��܂��B<!--?V:YResOver-->�𒴂���ƕ\���ł��Ȃ��Ȃ��B
	const RHtmlTemplate* IfResWarning;
	//�T�C�Y��<!--?V:YSizeAlert-->KB�𒴂��Ă��܂��B<!--?V:YSizeOver-->KB�𒴂���ƕ\���ł��Ȃ��Ȃ��B
	const RHtmlTemplate* IfSizeWarning;
	//���X����<!--?V:YResOver-->�𒴂��Ă��܂��B�c�O�Ȃ���S���͕\�����܂���B
	const RHtmlTemplate* IfResOver;

	string	BoardName;
	
	RHtmlTemplate		Template;
	const YSettingTXT*	SettingTXT;

};

#endif // !defined(AFX_YREADCGITEMPLATE_H__BEE10152_2FC9_4642_9CDA_4117D58D94AA__INCLUDED_)
