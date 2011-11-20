// YTextFile.h: YTextFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YTEXTFILE_H__98F21711_6629_41AF_9E2E_B56A7457084A__INCLUDED_)
#define AFX_YTEXTFILE_H__98F21711_6629_41AF_9E2E_B56A7457084A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RPackSuper.h"

class YTextFile  
{
public:
	YTextFile()
	{
		Update();
	}
	virtual ~YTextFile()
	{
	}

	/**
	 * Show:				�o�͂��܂�.
	 *
	 * @param outPack		�o�͐�.
	 * @return virtual void 
	 */
	virtual void Show(RPack * outPack) const = 0;


	/**
	 * getLastModified:		�ŏI�X�V����Ԃ��܂��B
	 *
	 * @return time_t		�ŏI�X�V��.
	 */
	time_t getLastModified() const
	{
		return this->LastModified;
	}

protected:
	/**
	 * Update:			���e���X�V�����Ƃ��ǂ�ł�������.
	 *
	 * @return void 
	 */
	void Update()
	{
		this->LastModified = time(NULL);
	}

private:
	//�ŏI�X�V����
	time_t		LastModified;

};

#endif // !defined(AFX_YTEXTFILE_H__98F21711_6629_41AF_9E2E_B56A7457084A__INCLUDED_)
