#ifndef ___COMMH
#define ___COMMH
/**************************************************************/
//�����G�惋�[�`��
// GDI �� DirectDraw �̃C���^�[�t�F�[�X�̈Ⴂ���z������.
//�@����ɂ��A GDI �ł��A DirectDraw �ł��A���삷��v���O�������ł��܂�.
//  GDI �̑��x�͂Ƃ낢 ? Non Non ����k��. ����́A�ւ��ۂ��v���O���}�[��
//  ���΁A�Ƃ낢���[�`���ł����A�����ƍ��� DirectDraw �Ɣ�ׂĂ�
//  ���F�̂Ȃ����[�`�����ł��܂�. �܂�A�Ȋw�̏����ł�.
/**************************************************************/

#include <windows.h>
#include <.\rti_define.h>

//���݂̊G�惂�[�h
#define DRAW_MODE_MAX	2
enum UseDrawMode{	USE_GDI_MODE	,USE_DIRECTDRAW_MODE};				//�G�惂�[�h
enum ScreenSize {	SCREEN_SIZE_NONE,SCREEN_FULL_OK ,SCREEN_WINDOW_OK};	//��ʃT�C�Y

struct _NowScreenMode
{
	bool fFullScreen;		//�t���X�N�[������������ true
	unsigned int  iWidth,iHeight;	//�X�N���[���̕��ƍ���.
	unsigned int  iBpp;				//�F��
};

struct _CommDraw
{
	//������
	bool (*Init)(HWND hwnd,int w,int h,int bps);
    //�I������
	bool (*End)(void);
    //�w�肳�ꂽ�t�@�C������ BMP ��ǂݍ���
	bool (*GraphicsMake)(BMD bmd,const char* filename);
    //bmd->w bmd->h �̃T�C�Y��  �������m��
	bool (*GraphicsAlloc)(BMD bmd);
    //�m�ۂ������������
	bool (*GraphicsFree)(BMD bmd);
    //�O���t�B�b�N�X���b�N. ���������ƁA bmd->bm ����A�摜�f�[�^��ǂݏo����.
	bool (*GraphicsLock)(BMD bmd);
    //�A�����b�N
	bool (*GraphicsUnLock)(BMD bmd);
    //HDC �𓾂�.
	HDC (*GetDC)(BMD bmd);
    // HDC �폜
	bool (*DeleteDC)(BMD bmd,HDC hdc);
    // HDC �X�V
	bool (*UpdataDC)(BMD bmd,HDC hdc);
    //���݂̊G�惂�[�h�擾
	UseDrawMode  (*GetMode)(void);
    //����ʂ��A�G��
	void (*Ref)(HWND hwnd);
    //�t���X�N�[�����ɂ���
	bool (*DoFullScreen)(bool B = false);
    //�E�B���h�E���[�h�ɂ���.
	bool (*DoWindow)(bool B = false);
    //WM_MOVE �ɂ��A�E�B���h�E���ꂽ�E�B���h�̈ʒu
	void (*SetDrawPoint)(int x,int y);
};

extern struct _NowScreenMode NowScreenMode;
extern const struct _CommDraw* CommDraw;

#endif
