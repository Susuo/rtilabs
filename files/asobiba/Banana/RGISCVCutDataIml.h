// RGISCVCutDataIml.h: RGISCVCutDataIml �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISCVCUTDATAIML_H__4B32669C_4320_48EF_B436_F4BA9D4D15ED__INCLUDED_)
#define AFX_RGISCVCUTDATAIML_H__4B32669C_4320_48EF_B436_F4BA9D4D15ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RStdioFile.h"

class RGISCVCutDataIml  
{
public:
	RGISCVCutDataIml();
	virtual ~RGISCVCutDataIml();

	//�w�b�_�[���������܂Ȃ���ΐ���Ȃ��Ƃ��ɌĂ΂��.
	//���̒��ŁA ioFile �ɑ΂��ēK�؂ȃf�[�^�� fwrite ���Ă��������B
	//�Ԉ���Ă� ioFile�� close ������ seek�����肵�Ȃ�����!!
	virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount) = 0;
};


#endif // !defined(AFX_RGISCVCUTDATAIML_H__4B32669C_4320_48EF_B436_F4BA9D4D15ED__INCLUDED_)
