// BBananaConfig.h: BBananaConfig �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BBANANACONFIG_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_)
#define AFX_BBANANACONFIG_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

const int COLOR_MAX	=	255;

enum
{
	COLOR_MOJI			=	10,		//����
	COLOR_CAPTION		=	11,		//caption
	COLOR_PEN1			=	20,		//�y��
	COLOR_PEN2			=	21,		//�y��
	COLOR_PEN3			=	22,		//�y��
	COLOR_PEN4			=	23,		//�y��
	COLOR_PEN5			=	24,		//�y��
	COLOR_PEN6			=	25,		//�y��
	COLOR_TRACKLOG		=	26,		//�g���b�N���O	�y��7�Ԃ��g���b�N���O�Ƃ��ė��p����.
	COLOR_TATEMORO		=	30,		//�����g
	COLOR_JYOUTI		=	30,		//���傤���g
	COLOR_TEIENRO		=	40,		//�뉀�H
	COLOR_ISHIDAN		=	41,		//�Βi
	COLOR_DOURO			=	42,		//��ʓ�
	COLOR_KOUSOKU_DOURO	=	43,		//�������H
	COLOR_EKI			=	50,		//�w
	COLOR_TETUDO_JR		=	51,		//���ʓS��JR
	COLOR_TETUDO		=	52,		//���ʓS��
	COLOR_ROMENDENSYA	=	53,		//�H�ʓd��
	COLOR_TIKATETU		=	54,		//�n�����S��
	COLOR_TETUDOU_SONOTA=	55,		//���̑�
	COLOR_MIZU			=	100,	//��
	COLOR_KOUDO200		=	101,	//���x200����
	COLOR_KOUDO400		=	102,	//���x400����
	COLOR_KOUDO600		=	103,	//���x600����
	COLOR_KOUDO800		=	104,	//���x800����
	COLOR_KOUDO1000		=	105,	//���x1000����
	COLOR_KOUDO1500		=	106,	//���x1500����
	COLOR_KOUDO2000		=	107,	//���x2000����
	COLOR_KOUDO2500		=	108,	//���x2500����
	COLOR_KOUDO3000		=	109,	//���x3000����
	COLOR_KOUDO4000		=	110,	//���x4000����
	COLOR_KOUDO5000		=	111,	//���x5000����
	COLOR_KOUDOOVER		=	112,	//���xover����
};


class BBananaConfig  
{
private:
	BBananaConfig();
public:
	virtual ~BBananaConfig();


	static BBananaConfig* getInstance();

	void Load();
	void Save();

	void UpMapDirectory(const string & inMapFilename , const string & inUserDataDirectory);
	void UpNowPoint(const LPOINT & inNowXY , int inScale , int inDeg);

	const string& getUserDataDirectory() const
	{
		return this->UserDataDirectory;
	}
	const string& getMapFilename() const
	{
		return this->MapFilename;
	}
	const LPOINT& getPoint() const
	{
		return this->Point;
	}
	const int getScale() const
	{
		return this->Scale;
	}
	const int getDeg() const
	{
		return this->Deg;
	}
	const bool getRotateFlg() const
	{
		return this->RotateFlg;
	}
	void setRotateFlg(bool inFlg) 
	{
		this->RotateFlg = inFlg;
	}

	unsigned short getColor(unsigned short inColor) const
	{
		return this->Color[inColor];
	}
	const unsigned short* getColors() const
	{
		return this->Color;
	}

private:
	string UserDataDirectory;
	string MapFilename;
	LPOINT Point;
	int Scale;
	int Deg;
	bool RotateFlg;

	unsigned short Color[COLOR_MAX];
};

#endif // !defined(AFX_BBANANACONFIG_H__9D103848_AECE_432C_A31F_25244D94FCE3__INCLUDED_)
