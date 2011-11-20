#include <StartWithExeFile.h>
#include <windows.h>

#ifdef JOY_STICK_LIFE

#ifndef ___JOY_STICKH
#define ___JOY_STICKH

#define JOY_POS_MIN		1000
#define JOY_POS_MAX		55000
//�������
#define JOY_POS_RIGHT	0x02 //�E
#define JOY_POS_LEFT	0x01 //��
#define JOY_POS_TOP		0x08 //��
#define JOY_POS_DOWN	0x04 //��
#define JOY_POS_NN      0x00 //�j���[�g����
//�{�^�����
#define JOY_BTN_A	0x01 //A�{�^��
#define JOY_BTN_B	0x02 //B�{�^��
#define JOY_BTN_C	0x04 //C�{�^��
#define JOY_BTN_D	0x08 //D�{�^��
//�����ƃ{�^���̃p�b�N.
//32  16 8 4 2 1	�����̏�ɃL�[������.
//B   A  U D R L
#define JOY_PACK_RIGHT	0x02 //�E
#define JOY_PACK_LEFT	0x01 //��
#define JOY_PACK_TOP	0x08 //��
#define JOY_PACK_DOWN	0x04 //��
#define JOY_PACK_NN     0x00 //�j���[�g����
#define JOY_PACK_A		0x10 //A�{�^��
#define JOY_PACK_B		0x20 //B�{�^��
#define JOY_PACK_C		0x40 //C�{�^��
#define JOY_PACK_D		0x80 //D�{�^��

#define JOY_PACK_BUTTOM_MASK	0xfffffff0
#define JOY_PACK_STICK_MASK		0x0f

class TJoyStick
{
private:
	unsigned int PadId;  //�p�b�hID
	JOYINFOEX JoyInfo;

	//�W���C�X�e�B�b�N�͎g����
	bool bPadAlive;
	//�����L�[�_�E���̃r�b�g���
	BYTE JoyKeyDown;
	//�O��̕����L�[�_�E���̃r�b�g���
	BYTE JoyKeyDownOld;

	//�O��̃{�^���L�[�_�E���̃r�b�g���
	DWORD JoyButtonDownOld;
public:

	TJoyStick(unsigned int UsePad,bool Sailent);
	MMRESULT JoyStickInfo(bool Sailent);
	void JoyStickRef();

	//�p�b�h���L�����ǂ������ׂ�
	bool PadAlive(){return bPadAlive;};
	//�{�^�����擾����
	DWORD ButtonGet(){return JoyInfo.dwButtons;}
	//�O��͉�����Ă��Ȃ��č��񏉂߂ĉ����ꂽ�{�^�����擾����
	DWORD ButtonOnesGet()
		{return (JoyInfo.dwButtons & (~JoyButtonDownOld) );};
	//�������擾����
	BYTE KeyGet(){return JoyKeyDown;};
	//�O��͉�����Ă��Ȃ��č��񏉂߂ĉ����ꂽ�����
	BYTE KeyOnesGet(){return (JoyKeyDown & (~JoyKeyDownOld) );};

	//�{�^���ƕ����� DWORD �ɂ߂ĕԂ��܂�.
	DWORD PackKeyGet(){return JoyKeyDown | (JoyInfo.dwButtons << 4);};
	//�{�^���ƕ����� DWORD �ɂ߂ĕԂ��܂�.(�O�񉟂���Ă��Ȃ�.)
	DWORD PackKeyGetOne(){
		return (JoyKeyDown & (~JoyKeyDownOld) ) | (JoyInfo.dwButtons & (~JoyButtonDownOld) );};
};

typedef TJoyStick* JSC;

#endif

#endif //LIFE END
