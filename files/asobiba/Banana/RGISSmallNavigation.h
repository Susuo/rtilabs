// RGISSmallNavigation.h: RGISSmallNavigation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISSMALLNAVIGATION_H__07FDBA6D_C409_4198_8112_7D6A9102707E__INCLUDED_)
#define AFX_RGISSMALLNAVIGATION_H__07FDBA6D_C409_4198_8112_7D6A9102707E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#include "comm.h"
//�i�r�Q�[�V�����Ŏg�p����������W���邽�߂̍\����.
//�N���X�ɂ��Ȃ��̂́A �Ȃ� CE �łł����N���X���ʂɍ쐬����ƃN���b�V�����Ă��܂�����.(�Ȃ�?
//VC++�ł͓��삷��񂾂��ǂȂ��B EVC �Ȃ񂩂��������̂��Ȃ�.
struct SmallPathfinderData
{
	struct SmallPathfinderDataOne
	{
		unsigned char			SB;
		unsigned char			FI;

		unsigned char			NodeCount;
		struct _Node
		{
			unsigned char	Pos;				//�m�[�h�͓��H�f�[�^�̉��Ԗڂɂ��邩?
			unsigned char	ConnectCount;		//�ڑ����铹�H�̐� (4���̏ꍇ�� 3 ���邱�ƂɂȂ�.)
			struct _Connect
			{
				unsigned char	AtherMap;		//�ʂ̃}�b�v�ɐڑ�?	0->�Ȃ�  �ȉ��e���L�[�����ɐڑ�.
				DWORD			Number;			//�ڑ�����铹�H�ԍ�
				unsigned char	Pos;			//�ڑ�����铹�H�̃m�[�h�̏ꏊ
			};
			_Connect *Connect;
		};
		_Node	*Node;

		unsigned char			CVCount;
		SPOINT				*	CV;
	};
	DWORD						DataCount;
	SmallPathfinderDataOne	*	Data;
};

struct SmallPathfinderOneData
{
	DWORD	MapNumber;
	DWORD	DouroNumber;
	unsigned char	DouroPos;
};

struct SmallPathfinderSave
{
	DWORD	Distance;
	newVector<SmallPathfinderOneData>	Data;
};

class RGISSmallNavigation  
{
public:
	RGISSmallNavigation();
	virtual ~RGISSmallNavigation();

	void Create(const RGISSmallIntelligentCache* inCache,int inDetourMax);
	void Find();

private:
	newVector<SmallPathfinderSave>	Save;
	int	DetourMax;
};
*/
#endif // !defined(AFX_RGISSMALLNAVIGATION_H__07FDBA6D_C409_4198_8112_7D6A9102707E__INCLUDED_)
