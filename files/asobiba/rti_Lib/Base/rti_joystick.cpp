/*****************************************************************/
/*****************************************************************/
// �V�Ԗ_(^^;;  �W���C�X�e�B�b�N
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef JOY_STICK_LIFE
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_joystick.h>

/*****************************************************************/
//�R���X�g���N�^
/*****************************************************************/
TJoyStick::TJoyStick(unsigned int UsePad,bool Sailent)
{
	//�p�b�hID��ۑ�
	PadId = UsePad;

	//�W���C�X�e�B�b�N�̖߂�l���󂯎���̏����ݒ�
	JoyInfo.dwSize   = sizeof(JOYINFOEX);
	JoyInfo.dwFlags	 = JOY_RETURNX | JOY_RETURNY | JOY_RETURNBUTTONS;
	JoyKeyDown = 0;
	JoyInfo.dwButtons = 0;

	//�W���C�X�e�B�b�N���g���邩�ǂ������ׂĂ���
	if (JoyStickInfo(Sailent)==JOYERR_NOERROR)
	{
		bPadAlive = true;
	}
    else
    {
		bPadAlive = false;
    }

}



/*****************************************************************/
//�W���C�X�e�B�b�N�̏��  Sailent��false �̂Ƃ��͕��傽����ق��Ă���܂�
/*****************************************************************/
MMRESULT TJoyStick::JoyStickInfo(bool Sailent)
{
	
	MMRESULT MMret = joyGetPosEx(PadId,&JoyInfo);

	if (!Sailent)
	{
		switch(MMret)
		{
		case MMSYSERR_NODRIVER:
			FriendlyError("TJoyStick::JoyStickInfo","�W���C�X�e�B�b�N�F�����s",
				"�W���C�X�e�B�b�N �h���C�o�����݂��܂���","�h���C�o�����Ă�������");
			break;
		case JOYERR_PARMS:
			FriendlyError("TJoyStick::JoyStickInfo","�W���C�X�e�B�b�N�F�����s",
				"�w�肳�ꂽ�W���C�X�e�B�b�NID �������ł�","�W���C�X�e�B�b�N����Ă܂���?");
			break;
		case JOYERR_UNPLUGGED:
			FriendlyError("TJoyStick::JoyStickInfo","�W���C�X�e�B�b�N�F�����s",
				"�w�肳�ꂽ�W���C�X�e�B�b�N�̓V�X�e���ɐڑ�����Ă��܂���","�����Ȃ���.");
			break;
		}
	}

	return MMret;
}


/*****************************************************************/
//�W���C�X�e�B�b�N�����X�V
/*****************************************************************/
void TJoyStick::JoyStickRef()
{
	int x,y;

	//�{�^���ƕ����̑O��̒l��ۑ�
	JoyButtonDownOld = JoyInfo.dwButtons; //�{�^��
	JoyKeyDownOld = JoyKeyDown;           //����

	MMRESULT MMret = joyGetPosEx(PadId,&JoyInfo);
	x = JoyInfo.dwXpos;
	y = JoyInfo.dwYpos;

	//����(private�����o)
	JoyKeyDown = 0;      
	if (x < JOY_POS_MIN) JoyKeyDown |= JOY_POS_LEFT; //�E
	if (x > JOY_POS_MAX) JoyKeyDown |= JOY_POS_RIGHT; //��
	if (y < JOY_POS_MIN) JoyKeyDown |= JOY_POS_TOP; //��
	if (y > JOY_POS_MAX) JoyKeyDown |= JOY_POS_DOWN; //��

}

#endif
