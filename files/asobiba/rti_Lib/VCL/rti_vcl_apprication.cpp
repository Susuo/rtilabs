/**********************************************************/
// �A�v���P�[�V�����̓y��
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef APPLICATION_LIFE

#include <windows.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Base\rti_debug.h>

TApplication *Application;

/**********************************************************/
//�R���X�g���N�^
/**********************************************************/
TApplication::TApplication(HINSTANCE ihInstance , HINSTANCE ihPrevInstance,
				   LPSTR ilpszArgs , int inWinMode)
{
    //WinMain �̈������L�^���Ă���.
    hInstance     = ihInstance;
    hPrevInstance = ihPrevInstance;
    lpszArgs      = ilpszArgs;
    nWinMode      = inWinMode;
    //ID �R���N�^�[�̏����l
    IdCounter     = 100;
    //���̑��� = �{�X
    Kind = OBJECT_KIND_APPLICATION;
    //�E�B���h�n���h���͂Ƃ肠�����A NULL �ɂ��Ă���.
    hWnd			= NULL;
    //���̑��̏�����
    Init();
}

/**********************************************************/
//�f�X�g���N�^
/**********************************************************/
TApplication::~TApplication()
{
    //�I������
    End();
    //�����̎����Ă������ׂẴR���g���[�������
    OwerKill        = true;
    ChildDestroy();
}

/**********************************************************/
//������
/**********************************************************/
void TApplication::Init()
{
#ifdef COMMCTRL_USING
    InitCommonControls(); //�R�����R���g���[���̏�����
#endif
#ifdef MUSIC_LIFE
    IamMusic = NULL; //Midi �R���g���[���̏�����
#endif
#ifdef CD_LIFE
    IamCD = NULL;   //CD �R���g���[���̏�����
#endif
#ifdef WAVE_SOUND_LIFE
    IamWave = NULL; //Wave�R���g���[��
#endif
}

/**********************************************************/
//�I������
/**********************************************************/
void TApplication::End()
{
#ifdef CD_LIFE
    delete IamCD; //CD�j��
#endif
#ifdef MUSIC_LIFE
    delete IamMusic; //���y�j��
#endif
#ifdef WAVE_SOUND_LIFE
    delete IamWave; //Wave�j��
#endif
}


/**********************************************************/
//�{�X�ɕ���. DefProc
/**********************************************************/
LRESULT TApplication::DefProc(HWND hwnd , UINT message,
           						 WPARAM wParam,LPARAM lParam)
{
    switch(message)
    {
	case MM_MCINOTIFY:
		if (wParam==MCI_NOTIFY_SUCCESSFUL)
		{
#ifdef MUSIC_LIFE
            if (IamMusic) IamMusic->MciSuccessfulEnd();
#endif
#ifdef CD_LIFE
            if (IamCD) IamCD->MciSuccessfulEnd();
#endif
        }
		break;
#ifdef WAVE_SOUND_LIFE
    case MM_WOM_DONE:
        if (IamWave) IamWave->WaveSuccessfulEnd((WAVEHDR*)lParam);
        break;
#endif
 	default:
        //�{�X������グ�Ȃ̂� Windows �N�ɕ���.
		return DefWindowProc(hwnd,message,wParam,lParam);
    }
    return 0;
}


#endif //LIFE END
