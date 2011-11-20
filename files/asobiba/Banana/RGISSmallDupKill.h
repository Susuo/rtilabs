// RGISSmallDupKill.h: RGISSmallDupKill �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLDUPKILL_H__D713A1EA_1B9C_424C_B21E_EA46E52BBCB8__INCLUDED_)
#define AFX_RGISSMALLDUPKILL_H__D713A1EA_1B9C_424C_B21E_EA46E52BBCB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

const int DupKillBufferMax = 256;

class RGISSmallDupKill  
{
public:
	RGISSmallDupKill();
	virtual ~RGISSmallDupKill();

	bool Uniq(const char* inString);
private:
	unsigned int Count;
	const char* Points[DupKillBufferMax];
	//������̂Ƃ��ɂ킴�ƕԂ��A��̃o�b�t�@�[
	char NullString[1];
};

#endif // !defined(AFX_RGISSMALLDUPKILL_H__D713A1EA_1B9C_424C_B21E_EA46E52BBCB8__INCLUDED_)
