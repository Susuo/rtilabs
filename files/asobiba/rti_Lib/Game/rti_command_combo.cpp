#include "StartWithExeFile.h"
#ifdef COMMAND_COMBO_MAP

#include <.\Game\rti_command_combo.h>
#include <.\Base\rti_error_message.h>

/************************************************************************************/
//�L�[�o�^�̃{�X�I����.
/************************************************************************************/
void TCommandCombo::Keyes()
{
	//�W���C�X�e�B�b�N���擾.
	JoyStick->JoyStickRef();
	//�ǉ�
	AddKeys( JoyStick->PackKeyGet() );
}

/************************************************************************************/
//�R�}���h���͔���
/************************************************************************************/
void TCommandCombo::KeyCmp()
{
	struct _CommandKeyes Hadou[]={
						{	JOY_PACK_BUTTOM_MASK|	JOY_PACK_A		,10} ,
						{	JOY_PACK_BUTTOM_MASK|	JOY_POS_RIGHT	,10} ,
						{	JOY_PACK_BUTTOM_MASK|	JOY_POS_RIGHT | JOY_POS_DOWN,10} ,
						{	JOY_PACK_STICK_MASK	|	JOY_POS_DOWN	,0} ,
	}; 
	if ( KeyCmp_SearchKey(Hadou) )
	{
		MsgOr3("Ok.");
	}
}

/************************************************************************************/
//�L�[�T�[�`
/************************************************************************************/
bool TCommandCombo::KeyCmp_SearchKey(_CommandKeyes *CKS)
{
	int count_key;	//���݂��̃L�[���ǂ��܂Œ��ׂ���... �ƁA�����ԍ�
	int time;		//���ׂ鎞��
	int wait;		//�󂯕t���̂��莞��
	DWORD NowKey;		//�����O�L���[������o���� ���͂��ꂽ���.
	DWORD CommandKey;
	for(time = 0, wait = 1 , count_key = 0 ; wait ; time++ ,wait --)
	{
		NowKey = *(PackLogKey.back(time));
		CommandKey = CKS[count_key].PackKey;
		if ((NowKey&JOY_PACK_BUTTOM_MASK) == (CommandKey&JOY_PACK_BUTTOM_MASK) ||
			(NowKey&JOY_PACK_STICK_MASK)  == (CommandKey&JOY_PACK_STICK_MASK)	)
		{	//�T���Ă����L�[�ƈ�v����.
			wait = CKS[count_key].Wait;
			if (wait == 0) 
			{
				PackLogKey.clear();
				return true;	//����
			}
			count_key ++;
			time --; //�������A���ׂ�.(�{�^���̓���������OK)
		}
	}
	return false;
}



#endif  //LIFE END
