#include "StartWithExeFile.h"
#ifdef HIGH_SPEED_RESIDUAL_IMAGE_LIFE

#ifndef ___HIGH_SPEED_RESIDUAL_IMAGE_EFFECTH
#define ___HIGH_SPEED_RESIDUAL_IMAGE_EFFECTH
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X

//�����c��
class THighSpeedResidualImageEffect
{
private:
	// ========= �����o ===========
    BMD IOBmd;				 //���o��
	int IOBmdSizeDiv4;		 //���o�͉摜�̃T�C�Y�� / 4 ��������.(IO�� /4 �ł��܂肪�łȂ��摜��������Ȃ�.)
	int BufferUse;			 //���ݎg���ׂ��o�b�t�@
	BYTE **Buffer;			 //�o�b�t�@
	// ========= ���\�b�h =======
	void Init();			 //������
public:
	THighSpeedResidualImageEffect();
    ~THighSpeedResidualImageEffect();
    bool Set(BMD IOBmd);
	bool Draw();
};

#endif

#endif //LIFE END
