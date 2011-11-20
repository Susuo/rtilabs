#include "StartWithExeFile.h"
#ifdef BACKSURFACE_LIFE

#ifndef ___BACKSURFACEH
#define ___BACKSURFACEH
#include <.\Graphics\rti_graphics.h>
#include <.\Comm\rti_Comm.h>

extern BMD Sprite;

/*****************************************************************/
//����ʂ̌��ɂȂ���.	�K�i�ˑ�������.
/*****************************************************************/
class	TBacksurfaceBase
{
protected:
	virtual	void Make() = 0;					//�쐬
	virtual	void Destroy() = 0;					//�j��
	virtual	void Catch_WM_PAINT() = 0;			//WM_PAINT �̂Ƃ��̏���.
	virtual	void Catch_WM_MOVE(int x,int y) = 0;//WM_MOVE �̂Ƃ��̏���.
};

/*****************************************************************/
//����������ʂ̌��ɂȂ�	�������A�������͋��L���\�b�h
/*****************************************************************/
class	TBacksurfaceComm : public TBacksurfaceBase
{
protected:
	HWND	hWnd;
	void CommMake(HWND hwnd);
public:
	void CommDestroy();
    void Cls(){ RtiGraphics::Cls(Sprite,0);};	//��ʏ���.
	void Ref(){	CommDraw->Ref(hWnd);};	//���t���b�V��
	void OnPaint(){Catch_WM_PAINT();};	//WM_PAINT �̂Ƃ�.
	void OnMove (int lParam)
		{Catch_WM_MOVE( (int) LOWORD(lParam),(int) HIWORD(lParam) );};	//WM_MOVE  �̂Ƃ�.
};

/*****************************************************************/
//GDI ���x���̏���.
/*****************************************************************/
class	TGDIBackSurface : public TBacksurfaceComm
{
	void Make(){};			//�쐬
	void Destroy(){};			//�j��
	void Catch_WM_PAINT();				//WM_PAINT �̂Ƃ��̏���.
	void Catch_WM_MOVE(int x,int y);	//WM_MOVE �̂Ƃ��̏���.
public:
	TGDIBackSurface(HWND hwnd){CommMake(hwnd);};
};

/*****************************************************************/
//DirectDraw ���x���̏���.
/*****************************************************************/
class	TDirectDrawBackSurface : public TBacksurfaceComm
{
	void Make(){};			//�쐬
	void Destroy(){};			//�j��
	void Catch_WM_PAINT();				//WM_PAINT �̂Ƃ��̏���.
	void Catch_WM_MOVE(int x,int y);	//WM_MOVE �̂Ƃ��̏���.
public:
	TDirectDrawBackSurface(HWND hwnd){CommMake(hwnd);};
};

/*****************************************************************/
//����ʍ쐬���[�`��.
//�������A���[�U�͂��̃N���X�����𑀍삷�邾���ł悢.
//���̃N���X�͏�L�̃N���X���A�_�v�^�[���Ă����̂ł��邠�邠�邠�邠�邠�邠�邠�邠�邠��
//		(C)Copylight	2000 1/24	R.T.I 
/*****************************************************************/
class TBacksurface
{
	TBacksurfaceComm *BacksurfaceRtn;
public:
	TBacksurface(HWND hwnd)
	{
		switch(CommDraw->GetMode())
		{
		case USE_GDI_MODE:
			BacksurfaceRtn = new TGDIBackSurface(hwnd);
			break;
		case USE_DIRECTDRAW_MODE:
			BacksurfaceRtn = new TDirectDrawBackSurface(hwnd);
			break;
		}
	};
	~TBacksurface()
    {
        BacksurfaceRtn->CommDestroy();
        delete BacksurfaceRtn;
    };

	void Cls(){BacksurfaceRtn->Cls();};			//�N���A
	void Ref(){BacksurfaceRtn->Ref();};			//���t���b�V��

	void OnPaint(){BacksurfaceRtn->OnPaint();};	//�y�C���g�̂Ƃ��̏���.
	void OnMove(int lParam){BacksurfaceRtn->OnMove(lParam);};	//���[�u�̂Ƃ��̏���.
};


extern TBacksurface *Backsurface;

#endif

#endif //LIFE END
