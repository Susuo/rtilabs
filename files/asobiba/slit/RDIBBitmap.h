// RDIBBitmap.h: RDIBBitmap �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDIBBITMAP_H__67BD7A26_7F61_4C80_BA1D_BE22EC299BC5__INCLUDED_)
#define AFX_RDIBBITMAP_H__67BD7A26_7F61_4C80_BA1D_BE22EC299BC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RWin32Exception.h"

/**
 * class RDIBBitmap:	�f�o�C�X�Ɨ��r�b�g�}�b�v�쐬�x���Ƃ�
 *
 * @author rti
 */
class RDIBBitmap  
{
public:
	RDIBBitmap();
	virtual ~RDIBBitmap();

	/**
	 * Create:			�쐬���܂�
	 *
	 * @param inW		����
	 * @param inH		�c��
	 * @param inBit		�F��(2�ׂ̂���)
	 * @return void 
	 */
	void	Create(int inW , int inH , int inBit) throw(RWin32Exception);
	/**
	 * Release:			�j�����܂�
	 *
	 * @return void 
	 */
	void	Release();

	/**
	 * getImage:		�������̓��e�̎擾
	 *
	 * @return BYTE*	������(�Ԉ���Ă� delete ���Ȃ�����)
	 */
	BYTE*	getImage() 
	{
		return ImageMemory;
	}
	/**
	 * getImage:		�������̓��e�̎擾
	 *
	 * @return BYTE*	������(�Ԉ���Ă� delete ���Ȃ�����)
	 */
	BYTE*	getImage(int inX, int inY) 
	{
		return ImageMemory + (inX + inY * (Width )) * (Bit / 8);
	}
	/**
	 * getImage:		�������̓��e�̎擾
	 *
	 * @return const BYTE*	������(�Ԉ���Ă� delete ���Ȃ�����)
	 */
	const BYTE*	getConstImage() const
	{
		return ImageMemory;
	}
	/**
	 * getImage:		�������̓��e�̎擾
	 *
	 * @return BYTE*	������(�Ԉ���Ă� delete ���Ȃ�����)
	 */
	const BYTE*	getConstImage(int inX, int inY) const
	{
		return ImageMemory + (inX + inY * (Width )) * (Bit / 8);
	}
	/**
	 * getDC:		�f�o�C�X�R���e���L�X�g�̎擾
	 *
	 * @return HDC	�f�o�C�X�R���e�L�X�g(�Ԉ���Ă� DeleteObject ���Ȃ�����)
	 */
	HDC	getDC() 
	{
		return MemoryDC;
	}

	/**
	 * getDC:		�f�o�C�X�R���e���L�X�g�̎擾
	 *
	 * @return HDC	�f�o�C�X�R���e�L�X�g(�Ԉ���Ă� DeleteObject ���Ȃ�����)
	 */
	const HDC	getConstDC() const
	{
		return MemoryDC;
	}

	/**
	 * getWidth:	�����̎擾
	 *
	 * @return int ����
	 */
	int getWidth() const
	{
		return Width;
	}

	/**
	 * getHeight:	�c���̎擾
	 *
	 * @return int	�c��
	 */
	int getHeight() const
	{
		return Height;
	}
	/**
	 * getBit:		�F���̎擾
	 *
	 * @return int	�F��
	 */
	int getBit() const
	{
		return Bit;
	}
	//���e�̕ۑ�(��Ƀf�o�b�O�p
	void Save(const string & inFilename) const throw(RException);

private:
	BYTE*		ImageMemory;
	HDC			MemoryDC;
	HBITMAP		HBitmap;

	int			Width;
	int			Height;
	int			Bit;
};

#endif // !defined(AFX_RDIBBITMAP_H__67BD7A26_7F61_4C80_BA1D_BE22EC299BC5__INCLUDED_)
