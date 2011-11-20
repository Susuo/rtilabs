// YWriteHTML.h: YWriteHTML �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YWRITEHTML_H__0DD00913_11E0_4625_9D11_7FC6583AEAD5__INCLUDED_)
#define AFX_YWRITEHTML_H__0DD00913_11E0_4625_9D11_7FC6583AEAD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHtmlTemplate.h"
#include "YSettingTXT.h"

class YWriteHTML  
{
public:
	YWriteHTML();
	virtual ~YWriteHTML();

	void Load(const string inBoardName,const YSettingTXT*	inSettingTXT);

	//�G���[�쐬.
	const string makeError(const string & inErrorMessage , const string & inIP , 
								   const string & inFROM , const string & inMail , const string & inMESSAGE) const;
	//�������݂܂����쐬.
	const string makeKakikoSimasuta() const;
	//�������݊m�F�쐬.
	const string makeKakikoKakunin(const string & inIP , const string & inFROM , const string & inMail , const string & inMESSAGE) const;
private:

	//�G���[
	RHtmlTemplate	Error;
	//�������݂܂���
	RHtmlTemplate	KakikoSimasuta;
	//�������݊m�F.
	RHtmlTemplate	KakikoKakunin;

	string BoardName;

	const YSettingTXT*	SettingTXT;

};

#endif // !defined(AFX_YWRITEHTML_H__0DD00913_11E0_4625_9D11_7FC6583AEAD5__INCLUDED_)
