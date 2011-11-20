// YIndexHTML.h: YIndexHTML �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINDEXHTML_H__38A1CEFB_2347_4F39_8335_28F4DF5C86E0__INCLUDED_)
#define AFX_YINDEXHTML_H__38A1CEFB_2347_4F39_8335_28F4DF5C86E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RNotFoundException.h"
#include "YDATCache1.h"
#include "YSubject.h"
#include "RSpinLock.h"
#include "YTextFile.h"
#include "YSettingTXT.h"
#include "YLocalRule.h"
/*
readme.html:Step3
<B>��s�ɏ������߂钷����?</B>
	��s�ɏ������߂钷���́A 100�o�C�g�ł��B
	�܂��A�������߂�s���� 40 �s�܂łł��B
	�Ȃ��Aindex.html�ł́A 20 �s�𒴂�����̂͏ȗ�����܂��B
	���������������ɂ́A YIndexHTML.h �����������Ă��������B

<B>index.html �ŉ����܂ŕ\������܂���?</B>
	index.html �ł� 10���܂ŕ\������\������ł��܂��B
	���������������ɂ́A YIndexHTML.h �����������Ă��������B

<B>index.html �� menu �ɕ\�������X���b�h�̐���?</B>
	index.html �ł� 40���܂ł̃X���b�h�� menu �ɕ\�����܂��B
	���������������ɂ́A YIndexHTML.h �����������Ă��������B
*/

//�������݂̈�s�̒���
const int ONE_LINE_MAX	=	100;
//�������݂̍s��
const int ALL_LINE_MAX	=	40;
//index.html �ŏȗ������T�C�Y.
const int ABBREVIATION_LINE	=	20;
//�܂�A�ʒu�������݂̍ő�T�C�Y��..
const int ONE_RES_MAX	=	(ONE_LINE_MAX * ALL_LINE_MAX);
//DAT �̌�납��\�����鐔
const int ONE_DAT_TAIL	=	10;
//�\������ DAT �̐�.
const int TOP_DAT		=	10;
//�܂�A��y�[�W�ɕ\�����邽�߂Ƀv�[�����Ă����Ȃ���΂����Ȃ��������݂̐�
const int ALL_PAGE_RES	=	(ONE_DAT_TAIL * TOP_DAT);
//index.html �ɕ\������ �X���b�h�ꗗ.
const int SHOW_INDEX_MAX	=	40;


class YIndexHTMLAlloc
{
public:
	YIndexHTMLAlloc()
	{
		for(int i = 0 ; i < TOP_DAT ; i++)
		{
			Cache[i] = "";	//size() == 0 �ɂ���.
			CacheIndex[i] = &Cache[i];
		}
	}
	void setCache(int inNo,const string & inStr)
	{
		ASSERT(inNo >= 0 && inNo < TOP_DAT);
		(*CacheIndex[inNo]) = inStr;
	}
	string getCache(int inNo)
	{
		ASSERT(inNo >= 0 && inNo < TOP_DAT);
		return (*CacheIndex[inNo]);
	}
	inline void NewAge(const string & inStr)
	{
		string* newpoint = CacheIndex[TOP_DAT-1];
		//���� DAT ����ԏ�ɂ��邽�߂ɁA 1 -> 2 , 2 -> 3 , 3 -> 4 �ƃV�t�g���܂��B
		{
			for(int i = TOP_DAT-1 ; i > 0 ; i--)
			{
				CacheIndex[i] = CacheIndex[i-1];
			}
		}
		//age��ꂽ�f�[�^.
		CacheIndex[0] = newpoint;
		*newpoint =		inStr;
	}
	inline void Age(const string & inStr,int oldpos)
	{
		ASSERT(oldpos >= 0 && oldpos < TOP_DAT);
		//befor
		//0
		//1
		//2 <- ���ꂪ age ����
		//3
		//4

		//after
		//2 <- �����Ȃ�
		//0
		//1
		//3
		//4

		//2�̗̈��ی�.
		string* newpoint = CacheIndex[oldpos];

		//2 ���グ��X�y�[�X����邽�� 0 , 1 ���X���C�h�����܂��B
		{
			for(int i = oldpos ; i > 0 ; i--)
			{
				CacheIndex[i] = CacheIndex[i-1];
			}
		}
		//age��ꂽ�f�[�^.
		CacheIndex[0] = newpoint;
		*newpoint =		inStr;
	}
	inline void Sage(const string & inStr,int oldpos)
	{
		ASSERT(oldpos >= 0 && oldpos < TOP_DAT);
		(*CacheIndex[oldpos]) = inStr;
	}

	void Dump();
private:
	//�֋X��� index.
	string*	CacheIndex[TOP_DAT];
	//����.
	string	Cache[TOP_DAT];
};

class YIndexHTML  : public YTextFile
{
public:
	YIndexHTML();
	virtual ~YIndexHTML();

	//�����ݒ�.
	void Setting(const string inBoardName  ,const YSettingTXT*	inSettingTXT);

	//subject.txt ��� top ��ǂݍ��� index.html ���쐬. �L���b�V�����g��Ȃ�.
	void Load(YDATCache* ioCache , YSubject* inSubject,YLocalRule* inLocalRule)throw(RIOCannotOpenException);

	//age / sage �̏�������
	void YIndexHTML::Write(YDAT* inDAT ,YSubject* inSubject,YLocalRule* inLocalRule,int  oldPos , bool isAge );


	/**
	 * Show:				�\��			 YTextFile���p��
	 *
	 * @param outRPack		����Ɋi�[
	 * @return void 
	 */
	virtual void Show(RPack * outRPack) const;

	static void YIndexHTML::test();

private:
	//�L���b�V�����烍�[�h���� index.html �̍쐬������.
	void YIndexHTML::LoadWithCache(YSubject* inSubject,YLocalRule* inLocalRule);

	//index.html �̒��g ���ł���.
	string						IndexHTML;
	//BBS�̖��O
	string						BoardName;
	//�ݒ�֌W
	const YSettingTXT*			SettingTXT;

	//�S��
	RHtmlTemplate				AllTemplate ;
	//�X���b�h�ꗗ
	const RHtmlTemplate*		ThreadIndex ;
	//�X���b�h�P��
	const RHtmlTemplate*		ThreadsTemplate ;
	//��������}�邽�߂ɃX���b�h�ŌŒ�̂Ƃ���̕�����؂�o�������c
	const RHtmlTemplate*		SubThreadTemplate;
	//�ꏑ������
	const RHtmlTemplate*		KakikoTemplate ;

	//�L���b�V��.
	YIndexHTMLAlloc				Cache;

	mutable RSpinLock	Lock;

};

#endif // !defined(AFX_YINDEXHTML_H__38A1CEFB_2347_4F39_8335_28F4DF5C86E0__INCLUDED_)
