// RFunctionScript.h: RFunctionScript �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFUNCTIONSCRIPT_H__049A8A30_B2CA_413B_870D_A6D73C1C3EE8__INCLUDED_)
#define AFX_RFUNCTIONSCRIPT_H__049A8A30_B2CA_413B_870D_A6D73C1C3EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

#include "RConv.h"
#include "Rfiles.h"
#include "StringList.h"
#include "RParseException.h"

//��s���
//�\����͂̃\�[�X�͉����Ȃ�ˁ[ ������ĉ����ւ��ۂ������� �����B 
class Sentence
{
public:
	string Parse(string inSentence) throw(RParseException);
private:
	string Run(string inRunnable,string inStdOut) throw(RParseException);

};





//�X�N���v�g���
//�}�N���́u���Ӂv���p�N�邱�ƂɌ���(w
class RFunctionScript  
{
public:
	RFunctionScript();
	virtual ~RFunctionScript();

	string Interpreter(const string inScript) const throw (RParseException);

private:

	//�}�N���W�J
	string MacroReplace(const string inScript) const throw(RParseException);

	//�V���[�g�p�X�ɕϊ�.
	string ConvertSortFilename(const string inLong) const
	{
		char buf[MAX_PATH];
		GetShortPathName(inLong.c_str() , buf , MAX_PATH);
		return buf;
	}
	//aaaa.txt -> aaaa
	string getFilenameOnly(const string inFilename) const
	{
		const string ext = Rfiles::Filename2Ext( inFilename );
		return inFilename.substr(0,inFilename.size() - ext.size() - 1);
	}
	//aaaa.txt -> txt
	string getExtOly(const string inFilename) const
	{
		return Rfiles::Filename2Ext( inFilename );
	}

};

#endif // !defined(AFX_RFUNCTIONSCRIPT_H__049A8A30_B2CA_413B_870D_A6D73C1C3EE8__INCLUDED_)
