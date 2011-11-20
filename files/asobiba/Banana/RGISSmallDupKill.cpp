// RGISSmallDupKill.cpp: RGISSmallDupKill �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallDupKill.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSmallDupKill::RGISSmallDupKill()
{
	//�o�b�t�@�̏�����.
	memset(Points , sizeof(char*)  , DupKillBufferMax);
	//������̂Ƃ��ɂ킴�ƕԂ��A��̃o�b�t�@�[
	NullString[0] = '\0';

	Count = 0;
}

RGISSmallDupKill::~RGISSmallDupKill()
{

}

bool RGISSmallDupKill::Uniq(const char* inString)
{
	ASSERT(inString != NULL);

	if (inString[0] == '\0') return false;

	for(int i = 0 ; i < Count ; i ++)
	{
		if ( Points[i] == inString ) return false;
	}
	//�Ȃ��Ȃ�ǉ�.
	if (i != DupKillBufferMax)
	{
		Points[i] = inString;
		Count ++;
	}
	return true;
}

