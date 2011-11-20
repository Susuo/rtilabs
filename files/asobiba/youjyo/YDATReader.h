// YDATReader.h: YDATReader �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDATREADER_H__13AAD101_48AA_4A67_A1F1_CB6E8F9491D3__INCLUDED_)
#define AFX_YDATREADER_H__13AAD101_48AA_4A67_A1F1_CB6E8F9491D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RPackSuper.h"

class YDATReader  
{
public:
	YDATReader();
	virtual ~YDATReader();

	static const char* YDATReader::makeHTML(RStringMap* outSmap ,int inNo ,const char* inData );
	static string Subject(const char* inDat,int len);

	static void test();

	//���� <> ��
	static const char* YDATReader::next(const char* str);
	//���� \n ��
	static const char* YDATReader::nextnr(const char* str,const char* end);
	//���� \n ��
	static const char* YDATReader::nextnr(const char* str);


	/**
	 * MaskKakikomi:			�������݂̃}�X�N
	 *
	 * @param inData			�ꏑ�����݂� dat
	 * @param inMaskSize		�}�X�N�̐�
	 * @param inMask			�}�X�N�f�[�^
	 * @return string			�}�X�N�������ꂽ dat �ɏ����߂���f�[�^
	 */
	static string YDATReader::MaskKakikomi(const char* inData , int inMaskSize   ,const string *inMask);

private:
	/**
	 * MaskOneRecord:			�ꍀ�ڂ��Ƃ̃}�X�N
	 *
	 * @param inData			�ꏑ�����ݍ���
	 * @param inMaskSize		�}�X�N�̐�
	 * @param inMask			�}�X�N�f�[�^
	 * @return string			�}�X�N�������ꂽ�f�[�^
	 */
	static string YDATReader::MaskOneRecord(const string& inData , int inMaskSize   ,const string *inMask);

	// html�炵�����̂��������ꍇ�����N�Ƃ��܂�
	static string YDATReader::makeLink(const string &inStr);

};

#endif // !defined(AFX_YDATREADER_H__13AAD101_48AA_4A67_A1F1_CB6E8F9491D3__INCLUDED_)
