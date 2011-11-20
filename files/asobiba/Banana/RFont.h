// RFont.h: RFont �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFONT_H__6E06688E_FF1B_4997_9914_6CE50E9D8BF4__INCLUDED_)
#define AFX_RFONT_H__6E06688E_FF1B_4997_9914_6CE50E9D8BF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RFont  
{
public:
	RFont();
	virtual ~RFont();

	void Create(int inFontSize ,const string & FontName ,  bool Bold , bool Italic , bool Underline , bool Strike ,int Charset);

	//�t�H���g�̎擾 (����ɔj������ׂ��炸)
	HFONT getFont() const
	{
		return NewFont;
	}
private:
	HFONT NewFont;
};

#endif // !defined(AFX_RFONT_H__6E06688E_FF1B_4997_9914_6CE50E9D8BF4__INCLUDED_)
