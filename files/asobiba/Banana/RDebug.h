// RDebug.h: RDebug �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDEBUG_H__4A6D6880_2257_4DB5_A88E_198C4502ACE5__INCLUDED_)
#define AFX_RDEBUG_H__4A6D6880_2257_4DB5_A88E_198C4502ACE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RDebug  
{
public:
	RDebug();
	virtual ~RDebug();

	/**
	 * MemoryToFile:		���������t�@�C����
	 *
	 * @param inFilename	�t�@�C����
	 * @param inMemory		������
	 * @param inSize		�������T�C�Y
	 * @return void 
	 */
	static void RDebug::MemoryToFile(const string inFilename ,const void* inMemory , int inSize);

	/**
	 * MemoryToFile:		���������t�@�C����
	 *
	 * @param inFilename	�t�@�C����
	 * @param inMemory		������
	 * @return void 
	 */
	static void RDebug::MemoryToFile(const string inFilename ,const char* inMemory );

	/**
	 * MemoryToFile:		���������t�@�C����
	 *
	 * @param inFilename	�t�@�C����
	 * @param inMemory		������
	 * @return void 
	 */
	static void RDebug::MemoryToFile(const string inFilename ,string inMemory );

	/**
	 * FileCheck:			��̃t�@�C�����������ǂ����`�F�b�N.
	 *
	 * @param inSrc			�t�@�C����1
	 * @param inDest		�t�@�C����2
	 * @return void 
	 */
	static void RDebug::FileVerify(const string inSrc,const string inDest);

};

#endif // !defined(AFX_RDEBUG_H__4A6D6880_2257_4DB5_A88E_198C4502ACE5__INCLUDED_)
