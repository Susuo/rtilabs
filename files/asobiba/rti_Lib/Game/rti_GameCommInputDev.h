
#include <.\Base\rti_joystick.h>
#include <.\Base\rti_key.h>

#define COMM_INPUT_MAX_DISCERNMENT 64	//���ʎq�̐�.
#define COMM_INPUT_MAX_REGISTRY 64		//���̓o�^�ꏊ�� ���ʂł���ő�̃L�[�̐�.

//���ʃf�o�C�X
enum DEVDISCERNMENT
{
	DEV_NONE,			//�����.
	DEV_KEYBORD,		//�L�[�{�[�h
	DEV_JOYSTICK1,		//�W���C�X�e�b�N1
	DEV_JOYSTICK2,		//�W���C�X�e�b�N2
	DEV_JOYSTICK3,		//�W���C�X�e�b�N3
	DEV_JOYSTICK4,		//�W���C�X�e�b�N4
	DEV_MOUSE,			//�}�E�X
};	
struct _CommKeyRegistryStruct
{
	DEVDISCERNMENT DevDiscernment;	//�f�o�C�X���ʎq
	DWORD Key;						//�L�[
};

/************************************************************************/
//�����C���^�[�t�F�[�X
/************************************************************************/
class TGameCommInput
{
	JSC JoyStickControl[4];
	_CommKeyRegistryStruct CommKeyRegistryStruct[COMM_INPUT_MAX_DISCERNMENT][COMM_INPUT_MAX_REGISTRY];
public:
	//	=================  ���\�b�h =====================
	TGameCommInput();
	~TGameCommInput();
	//�w�肵���L�[�� RegistryNum �Ԗڂ̃L�[�Ƃ��ēo�^.
	bool SetKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum);
	//�w�肵���L�[�� RegistryNum ���폜
	bool DestroyKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum);
	//�w�肵���L�[���X�V.
	bool UpdateKey(DEVDISCERNMENT DevDiscernment,DWORD OldKey,DWORD NewKey,int RegistryNum);
	//�w�肵���L�[�������Ă��邩�`�F�b�N
	bool Check(int RegistryNum);
	//�w�肵���L�[�������Ă��邩�`�F�b�N(�A�ł��Ă��Ȃ��Ƃ��߃o�[�W����)
	bool CheckOne(int RegistryNum);
};
