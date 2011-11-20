// YSubject.h: YSubject �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSUBJECT_H__23F6F6C7_5F76_4273_B80B_962F000481DB__INCLUDED_)
#define AFX_YSUBJECT_H__23F6F6C7_5F76_4273_B80B_962F000481DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YDAT.h"
#include "RSpinLock.h"
#include "RStreamUtil.h"
#include "RHtmlTemplate.h"
#include "YTextFile.h"
#include "YHttpdConf.h"
/*
readme.html:Step4
<B>subject.txt�ɉ����܂œo�^�ł��܂���?</B>
	300���܂œo�^�ł��܂��B
	����ȍ~�̂��̂́A�ߋ����O�q�ɂɑ����܂��B

	���������������ɂ́A YSubject.h �����������Ă��������B
*/

const int MAX_SUBJECT	=	300;
const int MAX_SUBJECT_LINE	=	1024;

class YNiseDAT
{
public:
	YNiseDAT(DATNUMBER inNumber , string inSubject )
	{
		ASSERT(inNumber != 0);
		this->Number = inNumber;
		this->Subject = inSubject;
	}
	YNiseDAT()
	{
	}

	void SubjectChange(string inSubject)
	{
		this->Subject = inSubject;
	}

	string CompateLine() const
	{
		ASSERT(this->Number != 0);
		ASSERT(num2str(this->Number) != "");

		return num2str(this->Number) + ".dat<>" + this->Subject + "\n";
	}

	string getSubject() const 
	{
		return this->Subject + "\n";
	}

	DATNUMBER getDatNumber() const
	{
		return this->Number;
	}
private:
	DATNUMBER	Number;
	string		Subject;
};


class YSubject  : public YTextFile
{
public:

	YSubject();
	virtual ~YSubject();

	void Load(const string inBoardName);
	void Save(const string inBoardName);
	void Age(const YDAT* inDAT);
	void Sage(const YDAT* inDAT);

	/**
	 * Show:				�\��			 YTextFile���p��
	 *
	 * @param outRPack		����Ɋi�[
	 * @return void 
	 */
	virtual void Show(RPack * outRPack) const;

	int getNumbers(DATNUMBER * outNumbers , int inSize) const;

	int WhatPosition(DATNUMBER inDATNumber) const;
	/**
	 * DrawThreadIndex:			�X���b�h�ꗗ�̕`��
	 *
	 * @param inTemplate		�`��Ɏg���e���v���[�g
	 * @param inReadCGIPath		read.cgi�̃p�X
	 * @param inCount			���`�悷�邩?
	 * @return string			�`�悵������	
	 */
	const string DrawThreadIndex(const RHtmlTemplate* inTemplate , const string inReadCGIPath ,  int inCount) const;

	//�e�X�g
	static void test();

private:
	//DAT
	YNiseDAT*	DATs[MAX_SUBJECT];

	//���ݕێ����Ă��� DAT ��.
	int			No;

	mutable RSpinLock	Lock;
};

#endif // !defined(AFX_YSUBJECT_H__23F6F6C7_5F76_4273_B80B_962F000481DB__INCLUDED_)
