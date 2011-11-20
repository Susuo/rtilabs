// MVReaderThread.h: MVReaderThread �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVREADERTHREAD_H__4966B572_8A84_4521_A660_0BA5AA3A7113__INCLUDED_)
#define AFX_MVREADERTHREAD_H__4966B572_8A84_4521_A660_0BA5AA3A7113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RThread.h"
#include "MVPageData.h"
#include "MVImagePreConvert.h"

/**
 * class MVReaderThread:		��ǂ݃X���b�h
 *
 * @author 
 */
class MVReaderThread : public RThread  
{
public:
	MVReaderThread();
	virtual ~MVReaderThread();

	/**
	 * Setting:			��ƂɕK�v�ȍޗ����X���b�h�ɓn��
	 *					���̎��X���b�h�͂܂����삵�Ă��Ȃ��B
	 *
	 * @param poolData				���k����Ă���f�[�^ (�X���b�h����ƏI����A�ӔC�������ĊJ�����܂�)
	 * @param inSize				poolData �̃T�C�Y
	 * @param inUncompressedSize	poolData ���𓀂����ꍇ�̃T�C�Y
	 * @param compressionMethod		���k�Ɏg�p����Ă���A���S���Y��
	 * @param inImageConvert		�ϊ��Ɏg�p����p�����[�^
	 * @return void 
	 */
	void Setting(char* poolData , int inSize , int inUncompressedSize  , int compressionMethod ,
				 const MVImagePreConvert * inImageConvertParam);

	/**
	 * TakeoverPageData:���ʕ������������܂��B
	 *					���̎��X���b�h�͓�����I�����Ă��Ȃ���΂Ȃ�܂���.
	 *
	 *					��x�ĂԂƃX���b�h�� ���ʕ� ���A�Ă񂾓z�ɂ킽�������̂��Ǝv���f�[�^�̏��L����j������̂Œ���
	 *
	 * @return MVPageData*	���ʕ�	���̐��ʕ��́A�󂯎�����z���ӔC�������ĊJ������K�v������܂�.
	 */
	MVPageData* TakeoverPageData() throw(RException);

private:
	//�X���b�h����
	virtual unsigned int Run() ;
	/**
	 * UnComp:				�X���b�h�����Ŏ��ۂ̓W�J������S������.
	 *
	 * @return void 
	 */
	void MVReaderThread::UnComp()  throw(RException);


	char* PoolData ;
	int Size ;
	int UncompressedSize;
	int CompressionMethod ;

	MVImagePreConvert ImageConvertParam;
	string	Error;

	MVPageData* PoolPageData;
};

#endif // !defined(AFX_MVREADERTHREAD_H__4966B572_8A84_4521_A660_0BA5AA3A7113__INCLUDED_)
