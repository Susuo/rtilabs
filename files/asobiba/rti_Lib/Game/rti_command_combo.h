#include "StartWithExeFile.h"
#ifdef COMMAND_COMBO_MAP

#ifndef ___GAME_COMMAND_COMBOH
#define ___GAME_COMMAND_COMBOH

#include <.\Base\rti_ring_qeue.h>
#include <.\Base\rti_joystick.h>

struct _CommandKeyes
{
	DWORD PackKey;		//�L�[(�p�b�N)
	int	  Wait;			//��t���� �ŏI���R�[�h�� 0 �ł��鎖.
};

class TCommandCombo	//�i���̖��O�ł͂Ȃ�
{
	//�p�b�N �L�[�f�[�^���҂��.
    ringqueue256<DWORD> PackLogKey;
	//�W���C�X�e�B�b�N
	TJoyStick *JoyStick;
	//�L�[�T�[�`
	bool KeyCmp_SearchKey(_CommandKeyes *CKS);
public:
	TCommandCombo(){};
	//�W���C�X�e�B�b�N�o�^
	void SetJoyStick(TJoyStick *j){JoyStick = j;};
	//�L�[�o�^�̃{�X�I����.
	void Keyes();
	//�L�[��o�^
	void AddKeys(DWORD C){ PackLogKey.push_back(&C); };
	//�L�[���擾
	DWORD GetKeys(){ return (* PackLogKey. pop() );     };
	//�L�[����
	void KeyCmp();
};
#endif


#endif  //LIFE END

