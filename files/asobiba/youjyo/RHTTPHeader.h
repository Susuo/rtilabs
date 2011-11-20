// RHTTPHeader.h: RHTTPHeader �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTTPHEADER_H__2A8B4619_18FE_4BAB_8EB5_015DB8836076__INCLUDED_)
#define AFX_RHTTPHEADER_H__2A8B4619_18FE_4BAB_8EB5_015DB8836076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RHTTPHeaderAttribute.h"
#include "RHTTPHeaderAttributeRange.h"
#include "RHTTPHeaderAttributeDate.h"
#include "RRemoteHost.h"

class RHTTPHeader  
{
public:
	RHTTPHeader();
	virtual ~RHTTPHeader();

	/**
	 * ReadBuffer:				�o�b�t�@����ǂݍ��݃w�b�_�Ƃ��ĉ�͂��܂��B
	 *
	 * @param inHeaderBuffer	��͂���o�b�t�@
	 * @param inHeaderSize		�o�b�t�@�T�C�Y
	 * @return void 
	 */
	void ReadBuffer(char * ioHeaderBuffer , int inHeaderSize);

	void setRemoteHost(in_addr inAddr)
	{
		RemoteHost.setIP(inAddr);
	}

	//���\�b�h�̃Q�b�g
	const string getMethod()const	{		return Method;	}
	//�p�X�̃Q�b�g.
	const string getPath()	const	{		return Path;	}

	//�R���e���g�����O�X
	RHTTPHeaderAttribute<int>	ContentLength;
	//Range
	RHTTPHeaderAttribute<RHTTPHeaderAttributeRange>	Range;
	//If-Modified-Since
	RHTTPHeaderAttribute<RHTTPHeaderAttributeDate>	IfModifiedSince;

	//�����[�g�z�X�g	�w�b�_�Ɗ֌W�Ȃ����ǂƂ肠����(w
	RRemoteHost					RemoteHost;

	static void test();

private:
/**
 * FirstScan:				�ŏ��̈�s���X�L����
 *
 * @param isFirst			��ԍŏ��̃w�b�_?
 * @param inLine			��s
 * @return void 
 */
void RHTTPHeader::FirstScan(const char* inLine);
/**
 * CommandScan:				�ŏ��̈�s�ȊO���X�L����
 *
 * @param inLine			��s
 * @return void 
 */
void RHTTPHeader::CommandScan(const char* inLine);

private:
	//���\�b�h��
	string	Method;
	//���N�G�X�g�p�X
	string	Path;
};

#endif // !defined(AFX_RHTTPHEADER_H__2A8B4619_18FE_4BAB_8EB5_015DB8836076__INCLUDED_)
