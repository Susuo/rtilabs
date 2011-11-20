#include "StartWithExeFile.h"
#ifdef ROTATE_MAP

#ifndef ___MAP_ROTATEH
#define ___MAP_ROTATEH
/**************************************************************************************/
//�~���X�N���[���}�b�v
/**************************************************************************************/

#include <.\Game\rti_map.h>

class TMapRotate	: public TMap
{
protected:
public:
	//�R���X�g���N�^
	TMapRotate(){};
	//�}�b�v���G�悵�܂�(�~���X�N���[��)
	void DrawMain(BMD bmd,int w,int h);

	virtual int GetScreenX(int x)
		{
				int X = x - Nowx;
				if (X > MAP_FULLX) X -= MAP_FULLX;	//�␳
				if (X < 0) X += MAP_FULLX;			//�␳
				return X;
		};
	virtual int GetScreenY(int y)
		{
				int Y = y - Nowy;
				if (Y > MAP_FULLY) Y -= MAP_FULLY;	//�␳
				if (Y < 0) Y += MAP_FULLY;			//�␳
				return Y;
		};

	//�}�b�v���G�悵�܂�
	virtual void Draw(BMD bmd){TMapRotate::DrawMain(bmd,SCREENXSIZE,SCREENYSIZE);};	
	//��l���ړ��Ȃǂɂ��}�b�v�X�N���[��(�~��)
	virtual void MapMasterMove(short nx,short ny,int speed);
	//�w�肵���|�C���g����ʂ̒��S�ɂ���.
	virtual void SetCeneter(short nx,short ny);
	//���̃|�C���g��ʉ߂ł��邩�`�F�b�N
	virtual bool If(short nx,short ny);			
    //�~���X�N���[���̏ꍇ�̃L�����N�^�ʒu�̕␳
    void MapHosei(int *x,int *y);
	//�~���X�N���[���̏ꍇ�̃L�����N�^�ʒu�̕␳(�I�[�i�[���[�h���҂��)
	void MapHosei(short *x,short *y);
};


#endif

#endif //LIFE END
