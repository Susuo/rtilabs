// RGISSmallShareImage.cpp: RGISSmallShareImage �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallShareImage.h"
#include "RResorceImageLoad.h"
#include "BConf.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallShareImage::RGISSmallShareImage()
{

}

RGISSmallShareImage::~RGISSmallShareImage()
{

}

void RGISSmallShareImage::Create()
{
	//�n�}��Ŏg�p����C���[�W�̃��[�h.
	RResorceImageLoad ril;
	ril.LoadFile(&IconImage , BConf::getConf()->getCurrentDirectory() + "tool_bit.bmp");
}
