#include "StartWithExeFile.h"
#ifdef APPLICATION_LIFE

#ifndef ___APPLICATIONH
#define ___APPLICATIONH

#include <windows.h>
#include <.\VCL\rti_vcl_windowobject.h>
#include <.\Music\rti_music.h>
#include <.\Music\rti_cd.h>
#include <.\Music\rti_wave_player.h>

#if defined(PROGRESS_LIFE) || defined(STATUS_LIFE) || defined(TABCOONTROL_LIFE) || defined(TOOLBAR_LIFE)
#define COMMCTRL_USING
#pragma comment(lib,"Comctl32.lib")
#endif

#ifdef COMMCTRL_USING
	#include <commctrl.h>
#endif

class TApplication : public TWinObject
{
private:
    int IdCounter;
    HINSTANCE hInstance;        //�C���X�g���N�^
    HINSTANCE hPrevInstance;
    LPSTR lpszArgs;
    int nWinMode;
    void Init();                //
    void End();
#ifdef MUSIC_LIFE
    TMusic *IamMusic;
#endif
#ifdef CD_LIFE
    TCDPlayer *IamCD;
#endif
#ifdef WAVE_SOUND_LIFE
    TWavePlayer *IamWave;
#endif
public:

#ifdef MUSIC_LIFE
    //���y���g���悧�[
    TMusic* MusicUse(){return (IamMusic ? NULL :IamMusic = new TMusic );};
    //���y�̃n���h���̎擾
    TMusic* GetMusic(){return IamMusic;}; 
#endif
#ifdef CD_LIFE
    //CD���g���悧�[
    TCDPlayer* CDUse(){return (IamCD ? NULL :IamCD = new TCDPlayer );};
    //CD�̃n���h���̎擾
    TCDPlayer* GetCD(){return IamCD;}; 
#endif
#ifdef WAVE_SOUND_LIFE
    //WAVE���g���悧�[
    TWavePlayer* WaveUse(){return (IamWave ? NULL :IamWave = new TWavePlayer );};
    //WAVE�̃n���h���̎擾
    TWavePlayer* GetWave(){return IamWave;}; 
#endif

    TApplication(HINSTANCE ihInstance , HINSTANCE ihPrevInstance,
				   LPSTR ilpszArgs , int inWinMode);
    ~TApplication();

    //�����Ă��� ID ���擾.
    int GetId(){return IdCounter++;};

    //�C���X�g���N�^�擾
    HINSTANCE GethInstance(){return hInstance;};
    //�E�B���h���[�h�̎擾
    int       GetnWinMode(){return nWinMode;};

    //���C���E�B���h�Ɉˑ����郁�b�Z�[�W�̏���
    LRESULT DefProc(HWND hwnd , UINT message,
           						 WPARAM wParam,LPARAM lParam);

    //�I���@���x��1
    void AppExit(){SendMessage((HWND)hWnd,WM_CLOSE,0L,0L);};
    //�I���@���x��2
    void AppDestroy(){SendMessage((HWND)hWnd,WM_DESTROY,0L,0L);};
    //�I���@���x��3
    void AppQuit(){SendMessage((HWND)hWnd,WM_QUIT,1L,0L);};
};

extern TApplication *Application;

#endif

#endif //LIFE END
