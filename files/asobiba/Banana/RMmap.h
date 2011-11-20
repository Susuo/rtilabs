// RMmap.h: RMmap �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMMAP_H__569625AA_1ADE_40D5_83F0_8CF362C49E4A__INCLUDED_)
#define AFX_RMMAP_H__569625AA_1ADE_40D5_83F0_8CF362C49E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RWin32Exception.h"

//�������}�b�v
class RMmap  
{
public:
	RMmap();
	virtual ~RMmap();

	/**
	 * Create:					���������������蓖�Ă�
	 *
	 * @param inMappingSize		�ő劄�蓖�ăT�C�Y
	 * @return void 
	 */
	void Create(DWORD inMappingSize) throw(RWin32Exception);
	/**
	 * Create:					�t�@�C�����������Ɋ��蓖�Ă�
	 *
	 * @param inFilename		�t�@�C����
	 * @param inMappingSize		�ő劄�蓖�ăT�C�Y
	 * @return void 
	 */
	void Create(const string inFilename ,DWORD inMappingSize, DWORD inOpenFlg = OPEN_EXISTING) throw(RWin32Exception);

	/**
	 * Release:					�������̔j��
	 *
	 * @return void 
	 */
	void Release();

	/**
	 * getMemory:				�������̎擾
	 *
	 * @return void*			������
	 */
	void* getMemory() const
	{
		return MapMemory;
	}

	/**
	 * Flush:					�}�b�v�̃t���b�V��
	 *
	 * @param inFlushSize		�t���b�V������T�C�Y   0���w�肷��ƑS���������݂܂��B
	 *							�Ȃ񂩁A�T�C�Y�w�肵�Ă������ɂȂ�Ȃ��?
	 * @return void 
	 */
	void Flush(DWORD inFlushSize);

	static void test();

private:
	HANDLE		MapFile;			//�t�@�C���n���h��
	HANDLE		MapObject;			//�}�b�v�I�u�W�F�N�g
	void*		MapMemory;			//�}�b�v����������.
};

#endif // !defined(AFX_RMMAP_H__569625AA_1ADE_40D5_83F0_8CF362C49E4A__INCLUDED_)
