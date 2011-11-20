// RGISFindHintEki.h: RGISFindHintEki �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISFINDHINTEKI_H__007F65E7_C019_467C_B65C_682DF2CB65C7__INCLUDED_)
#define AFX_RGISFINDHINTEKI_H__007F65E7_C019_467C_B65C_682DF2CB65C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISJoin.h"


const int NAME_SHORT_CUT_SIZE	= 8;

struct FindHintEkiData
{
	char			NameShortCut[NAME_SHORT_CUT_SIZE];	//�擪6���� ��������ꍇ�� \0 �Ńp�f�B���O.
	unsigned long	Block;			//�}�b�v�u���b�N
	unsigned short	NameIndex;		//���O index
};

class RGISFindHintEki
{
	typedef vector<FindHintEkiData*> UniqEki;
	UniqEki	Eki;
public:
	RGISFindHintEki();
	virtual ~RGISFindHintEki();

	void Create(unsigned long inBlock,const string & inName , unsigned short inNameIndex);
	void Write(const string & inDir);

	static JoinHeadFormatScript getPackInfo() ;

private:
	void Delete();
};

#endif // !defined(AFX_RGISFINDHINTEKI_H__007F65E7_C019_467C_B65C_682DF2CB65C7__INCLUDED_)
