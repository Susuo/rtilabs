
#include <windows.h>
#include <.\VCL\rti_vcl_base.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Base\rti_debug.h>

#include <.\VCL\rti_vcl_form.h>

/*******************************************************************/
//�R���X�g���N�^
/*******************************************************************/
TBaseObject::TBaseObject()
{
    //�C�x���g
    OnMouseDown     = NULL;
    OnMouseUp       = NULL;
    OnMouseMove     = NULL;
    OnKeyDown       = NULL;
    OnKeyUp         = NULL;
    OnEnterFocus    = NULL;
    OnExitFocus     = NULL;
    OnClose         = NULL;
#ifdef RGN_LIFE
    //���[�W����
    Rgn = NULL;
#endif
}

/*******************************************************************/
//�f�X�g���N�^
/*******************************************************************/
TBaseObject::~TBaseObject()
{
#ifdef RGN_LIFE
    //���[�W�����j��
    delete Rgn;
#endif
}

/*******************************************************************/
//�T�C�Y����
/*******************************************************************/
void TBaseObject::Align(int iFlg)
{
	HWND BosshWnd;

	if (!hMyOwner)       BosshWnd = GetDesktopWindow();
	else                 BosshWnd = (HWND)GethOwerWnd(); 

	RECT CliantRcect;
	RECT MyRcect;
	RECT CliantCRcect;
	RECT MyCRcect;

	if (!GetWindowRect(BosshWnd,&CliantRcect)) return;
	if (!GetClientRect(BosshWnd,&CliantCRcect)) return;
	if (!GetWindowRect(GethWnd(),&MyRcect)) return;
	if (!GetClientRect(GethWnd(),&MyCRcect)) return;

	switch(iFlg)
	{
	case ALIGN_TOP:	//�g�b�v�Ɉ����L�΂����킹
		MoveWindow(GethWnd(),0,0,
						CliantRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_DOWN:		//�_�E���Ɉ����L�΂����킹
		MoveWindow(GethWnd(),0,CliantCRcect.bottom-MyCRcect.bottom,
						CliantRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_LEFT:		//���t�g�Ɉ����L�΂����킹
		MoveWindow(GethWnd(),0,0,
						MyCRcect.right,CliantCRcect.bottom,TRUE);
		break;
	case ALIGN_RIGHT:		//���C�g�Ɉ����L�΂����킹
		MoveWindow(GethWnd(),CliantCRcect.right-MyCRcect.right,0,
						MyCRcect.right,CliantCRcect.bottom,TRUE);
		break;
    case ALIGN_CLIANT:      //�N���C�A���g�ɂ��킹��
		MoveWindow(GethWnd(),0,0,
						CliantCRcect.right,CliantCRcect.bottom,TRUE);
        break;
	case ALIGN_MOVE_TOP:	//�g�b�v�ɓ�����
		MoveWindow(GethWnd(),MyRcect.left,0,
						MyCRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_MOVE_DOWN:	//�_�E���ɓ�����
		MoveWindow(GethWnd(),MyRcect.left,CliantCRcect.bottom-MyCRcect.bottom,
						MyCRcect.right,MyRcect.bottom,TRUE);
		break;
	case ALIGN_MOVE_LEFT:	//���t�g�ɓ�����
		MoveWindow(GethWnd(),0,MyRcect.top,
						MyCRcect.right,MyCRcect.bottom,TRUE);
		break;
	case ALIGN_MOVE_RIGHT:	//���C�g�ɓ�����
		MoveWindow(GethWnd(),CliantCRcect.right-MyCRcect.right,MyRcect.top,
						MyCRcect.right,MyCRcect.bottom,TRUE);
		break;
    case ALIGN_MOVE_CENTER:	//�����Ɏ����Ă���
		MoveWindow(GethWnd(),(CliantCRcect.right-MyCRcect.right)/2,
                        (CliantCRcect.bottom-MyCRcect.bottom)/2,
						MyCRcect.right,MyCRcect.bottom,TRUE);
        break;
	}
}

/*******************************************************************/
//�w��̈ʒu�Ɉړ�
/*******************************************************************/
void TBaseObject::Move(int x,int y)
{
    RECT rt;
    GetWindowRect(GethWnd(),&rt);
    MoveWindow(GethWnd(),x,y,rt.right-rt.left,rt.bottom-rt.top,TRUE);
};

/*******************************************************************/
//��`
/*******************************************************************/
void TBaseObject::Jeneric(unsigned char kind)
{
    ObjectIT it;
    TBaseObject * TBO;
    for(it=GetComponentFarst();
        it != GetComponentEnd() ; it++)
        {
            TBO = ((TBaseObject*)(*it));
//            switch(kind)
//            {
//            }
            //�ċA�Ŏ����̎q���܂ł��ׂĉ���(�� ���܂�. �`���`���`���`��..
            TBO->Jeneric(kind);
            TBO->RefControl();
        }
};

/*******************************************************************/
//���[�W����
/*******************************************************************/
void TBaseObject::SetRgn(char *filename,char flg)
{
#ifdef RGN_LIFE
    Rgn = new TRgn(hWnd,filename,flg);
#endif
}

/*******************************************************************/
//�f�B�t�H���g �̂�[���񂾂҂�[��
/*******************************************************************/
LRESULT CALLBACK TBaseObject::StaticProc(HWND hwnd , UINT message,
						 WPARAM wParam,LPARAM lParam)
{

	TBaseObject *Me = ((TBaseObject*)GetWindowLong(hwnd, GWL_USERDATA));
	if (message==WM_NCDESTROY)
		return 0;
	if (Me)
		return Me->UserDisPacth(hwnd ,message, wParam,lParam);
	else
		return DefWindowProc(hwnd,message,wParam,lParam);
}

/*******************************************************************/
//��{�C�x���g �̂�[���񂾂҂�[��
/*******************************************************************/
LRESULT TBaseObject::UserDisPacth(HWND hwnd , UINT message,
					 WPARAM wParam,LPARAM lParam)
{
	TObject *Obj;
	switch(message)
	{
	case WM_COMMAND:
		//���b�Z�[�W�𑗂��Ă����I�u�W�F�N�g�N�𒲂ׂ�
		Obj = ((TObject*)
			GetWindowLong((HWND)lParam, GWL_USERDATA));
		//�R�}���h�C�x���g����
		if (!Obj) break;
		Obj->WmCommandCome(wParam);
		break;
    case WM_DESTROY:
        UserDestroy();
        return 0;
        break;
    case WM_KEYDOWN:    //�L�[�_�E��
        if (OnKeyDown) OnKeyDown(this,(int) wParam,(UINT)lParam);
        break;
    case WM_KEYUP:     //�L�[�A�b�v
        if (OnKeyUp) OnKeyUp(this,(int) wParam,(UINT)lParam);
        break;
    case WM_SETFOCUS:   //�t�H�[�J�X���󂯎����
        if (OnEnterFocus) OnEnterFocus(this);
        break;
    case WM_KILLFOCUS:  //�t�H�[�J�X���������
        if (OnExitFocus) OnExitFocus(this);
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN: //�{�^���_�E��
        //�����G���A�Ƀ}�E�X�_�E����`����
        if (OnMouseDown) OnMouseDown(this,(UINT)wParam,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP: //�{�^���A�b�v
        if (OnMouseUp) OnMouseUp(this,(UINT)wParam,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_MOUSEMOVE: //�}�E�X���[�u
        if (OnMouseMove) OnMouseMove(this,(UINT)wParam,LOWORD(lParam),HIWORD(lParam));
        break;
    case WM_CLOSE: //�N���[�Y
        if (OnClose) 
        {
            if (!OnClose(this))  return 0;
        }
 	default:
		if (Kind != OBJECT_KIND_FORM)
		    return (CallWindowProc( (WNDPROC)GetDefProc(), hwnd, message,wParam,lParam));
//		    return (CallWindowProc( (int (__stdcall *)(void))GetDefProc(), hwnd, message,wParam,lParam));
		else
	        return Application->DefProc(hwnd,message,wParam,lParam);
	}
	if (Kind != OBJECT_KIND_FORM)
	    return (CallWindowProc( (WNDPROC)GetDefProc(), hwnd, message,wParam,lParam));
//	    return (CallWindowProc( (int (__stdcall *)(void))GetDefProc(), hwnd, message,wParam,lParam));
	else
		return 0;
}
