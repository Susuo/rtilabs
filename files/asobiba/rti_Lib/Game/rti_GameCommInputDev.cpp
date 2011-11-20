#include <.\Game\rti_GameCommInputDev.h>
#include <.\Base\rti_debug.h>

TGameCommInput::TGameCommInput()
{
	ZeroMemory(JoyStickControl,sizeof(JSC) * 4);
}
TGameCommInput::~TGameCommInput()
{
	//�W���C�X�e�b�N�R���g���[���̔j��.
	for(int i = 0; i < 4 ; i++)
	{
		delete JoyStickControl[i];
	}
}



//�w�肵���L�[�� RegistryNum �Ԗڂ̃L�[�Ƃ��ēo�^.
bool TGameCommInput::SetKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum)
{
	//�o�^����ꏊ ���������Ȃ����`�F�b�N.
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;

	switch(DevDiscernment)
	{
	case DEV_JOYSTICK1:	//�W���C�X�e�b�N
	case DEV_JOYSTICK2:	//�W���C�X�e�b�N
	case DEV_JOYSTICK3:	//�W���C�X�e�b�N
	case DEV_JOYSTICK4:	//�W���C�X�e�b�N
		{
			int joy_control_number = DevDiscernment - DEV_JOYSTICK1;
			if ( !JoyStickControl[ joy_control_number ] )
			{	//���̃W���C�X�e�B�b�N���錾����Ă��Ȃ�������錾����.
				JoyStickControl[ joy_control_number ]
						= new TJoyStick(JOYSTICKID1+joy_control_number,true);
			}
		}
		break;
	case DEV_KEYBORD:	//�L�[�{�[�h������.
	case DEV_MOUSE:		//�}�E�X
		break;
	default:
		return false;
	}
	//���̃f�[�^��o�^����.
	int i;
	_CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		if (lpCommKeyRegistryStruct->DevDiscernment == DEV_NONE)
		{	//����ۂ̏ꏊ����!! �o�^���J�n����.
			lpCommKeyRegistryStruct->DevDiscernment = DevDiscernment;
			lpCommKeyRegistryStruct->Key            = Key;
			break;
		}
		lpCommKeyRegistryStruct ++;	//���̃f�[�^��.
	}
	if (i == COMM_INPUT_MAX_REGISTRY ) return false; //�L�[�͖��^���ł�.
	return true;
}

//�w�肵���L�[�� RegistryNum ���폜
bool TGameCommInput::DestroyKey(DEVDISCERNMENT DevDiscernment,DWORD Key,int RegistryNum)
{
	//�o�^����ꏊ ���������Ȃ����`�F�b�N.
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;
	//���̃f�[�^���������č폜����.
	int i;
	_CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		if (lpCommKeyRegistryStruct->DevDiscernment == DevDiscernment &&
												lpCommKeyRegistryStruct->Key == Key)
		{	//�^�[�Q�b�g����.
			lpCommKeyRegistryStruct->DevDiscernment = DEV_NONE;
			lpCommKeyRegistryStruct->Key            = 0;
			break;
		}
		lpCommKeyRegistryStruct ++;	//���̃f�[�^��.
	}
	if (i == COMM_INPUT_MAX_REGISTRY ) return false; //����ȃL�[����܂���.
	return true;
}

//�w�肵���L�[���X�V.
bool TGameCommInput::UpdateKey(DEVDISCERNMENT DevDiscernment,DWORD OldKey,DWORD NewKey,int RegistryNum)
{
	//�o�^����ꏊ ���������Ȃ����`�F�b�N.
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;
	//���̃f�[�^���������čX�V����.
	int i;
	_CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		if (lpCommKeyRegistryStruct->DevDiscernment == DevDiscernment &&
												lpCommKeyRegistryStruct->Key == OldKey)
		{	//�^�[�Q�b�g����. 
			lpCommKeyRegistryStruct->Key            = NewKey;
			break;
		}
		lpCommKeyRegistryStruct ++;	//���̃f�[�^��.
	}
	if (i == COMM_INPUT_MAX_REGISTRY ) return false; //����ȃL�[����܂���.
	return true;
}

//�w�肵���L�[�������Ă��邩�`�F�b�N
bool TGameCommInput::Check(int RegistryNum)
{
	//�ꏊ ���������Ȃ����`�F�b�N. (���ꂭ�炢���Ă��������.)
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;

	register int i;
	register _CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		switch(lpCommKeyRegistryStruct->DevDiscernment)
		{
		case DEV_KEYBORD:
			//�L�[�{�[�h�`�F�b�N
			if ( KeyPush( (unsigned char)lpCommKeyRegistryStruct->Key) ) return true;
			break;
		case DEV_JOYSTICK1:
		case DEV_JOYSTICK2:
		case DEV_JOYSTICK3:
		case DEV_JOYSTICK4:
			{	//�W���C�X�e�B�b�N�`�F�b�N
				JSC jsc = JoyStickControl[lpCommKeyRegistryStruct->DevDiscernment - DEV_JOYSTICK1];
				if ( jsc->PadAlive())
				{
					jsc->JoyStickRef();
					if ( jsc->PackKeyGet() & lpCommKeyRegistryStruct->Key)	return true;
				}
			}
			break;
		default:
			return false;
		}
		lpCommKeyRegistryStruct ++;	//���̃f�[�^��.
	}
	return false;
}

//�w�肵���L�[�������Ă��邩�`�F�b�N(�A�ł��Ă��Ȃ��Ƃ��߃o�[�W����)
bool TGameCommInput::CheckOne(int RegistryNum)
{
	//�ꏊ ���������Ȃ����`�F�b�N. (���ꂭ�炢���Ă��������.)
	if (RegistryNum < 0 || RegistryNum >= COMM_INPUT_MAX_DISCERNMENT) return false;

	register int i;
	register _CommKeyRegistryStruct * lpCommKeyRegistryStruct = &CommKeyRegistryStruct[RegistryNum][0];
	for(i = 0 ; i < COMM_INPUT_MAX_REGISTRY ; i++)
	{
		switch(lpCommKeyRegistryStruct->DevDiscernment)
		{
		case DEV_KEYBORD:
			//�L�[�{�[�h�`�F�b�N
			if ( KeyPushOne( (unsigned char)lpCommKeyRegistryStruct->Key) ) return true;
			break;
		case DEV_JOYSTICK1:
		case DEV_JOYSTICK2:
		case DEV_JOYSTICK3:
		case DEV_JOYSTICK4:
            /*
			{	//�W���C�X�e�B�b�N�`�F�b�N
				JSC jsc = JoyStickControl[lpCommKeyRegistryStruct->DevDiscernment - DEV_JOYSTICK1];
				if (jsc->PadAlive())
				{
					jsc->JoyStickRef();
					if ( jsc->PackKeyGetOne() & lpCommKeyRegistryStruct->Key)	return true;
				}
			}
            */
			break;
		default:
			return false;
		}
		lpCommKeyRegistryStruct ++;	//���̃f�[�^��.
	}
	return false;
}

