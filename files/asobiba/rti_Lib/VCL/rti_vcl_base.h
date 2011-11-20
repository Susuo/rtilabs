#ifndef TBASEOBJECTH
#define TBASEOBJECTH

#include <.\VCL\rti_vcl_windowobject.h>
#include <.\Base\rti_rgn.h>
#include <windows.h>

#define ALIGN_TOP			0//�g�b�v�Ɉ����L�΂����킹
#define ALIGN_DOWN			1//�_�E���Ɉ����L�΂����킹
#define ALIGN_LEFT			2//���t�g�Ɉ����L�΂����킹
#define ALIGN_RIGHT			3//���C�g�Ɉ����L�΂����킹
#define ALIGN_CLIANT		4//�N���C�A���g�̑傫���ɂ��킹��
#define ALIGN_MOVE_TOP		6//�g�b�v�ɓ�����
#define ALIGN_MOVE_DOWN		7//�_�E���ɓ�����
#define ALIGN_MOVE_LEFT		8//���t�g�ɓ�����
#define ALIGN_MOVE_RIGHT	9//���C�g�ɓ�����
#define ALIGN_MOVE_CENTER	10//�����Ɏ����Ă���

#define JENERIC_FONT_CHANGE 0   //�t�H���g����`

class TBaseObject : public TWinObject 
{
#ifdef RGN_LIFE
    TRgn *Rgn;
#endif
public:
	//�f�B�t�H���g
	static LRESULT CALLBACK StaticProc
		(HWND hWnd , UINT message,WPARAM wParam,LPARAM lParam);
	//�f�B�X�p�b�`���[
	virtual LRESULT UserDisPacth
		(HWND hWnd , UINT message,WPARAM wParam,LPARAM lParam);
    //�A���z�u���K�v�ȃR���|�[�l���g�̃R���g���[���̍Ĕz�u�𑣂�
    virtual void RefControl() {};
 
    TBaseObject();
    ~TBaseObject();
    void SetRgn(char *filename,char flg);

	void Align(int iFlg);               //�ʒu���킹
    void Jeneric(unsigned char kind);   //��`
    HDC SpGetDC();                      //���܂ł̌o�߂����ׂďW�񂵂�DC �̍쐬

    //�}�E�X�������ꂽ
    MouseEvent  OnMouseDown;
    MouseEvent  OnMouseUp;
    MouseEvent  OnMouseMove;
    //�L�[�������ꂽ
    KeyEvent    OnKeyDown;
    KeyEvent    OnKeyUp;
    //�t�H�[�J�X
    SimpleEvent OnEnterFocus;
    SimpleEvent OnExitFocus;
    //�I�����Ă�����? true �I��  / false �܂�.
    BoolEvent OnClose;

	//���̃I�u�W�F�N�g���͂�
	void Destroy(){DestroyWindow((HWND)hWnd);};
	//���̃I�u�W�F�N�g��\������
	void Show() {ShowWindow((HWND)hWnd,SW_SHOW);};
	//���̃I�u�W�F�N�g���B��
	void Hide() {ShowWindow((HWND)hWnd,SW_HIDE);};
	//���̃I�u�W�F�N�g��L���ɂ���
	void Enabled() {EnableWindow((HWND)hWnd,TRUE);};
	//���̃I�u�W�F�N�g�𖳌��ɂ���
	void UnEnabled() {EnableWindow((HWND)hWnd,FALSE);};
	//���̃I�u�W�F�N�g���ړ�������
	void Move(int x,int y,int w,int h)
				{MoveWindow((HWND)hWnd,x,y,w,h,TRUE);};
	//���̃I�u�W�F�N�g���ړ�������
	void Move(int x,int y);

	//���̃I�u�W�F�N�g�̃L���v�V�����𓾂�
	void GetCaption(char *buf,int buf_size)
				{GetWindowText((HWND)hWnd, buf, buf_size);};
	//���̃I�u�W�F�N�g�̃L���v�V�������Z�b�g����
	void SetCaption(char *buf)
				{SetWindowText((HWND)hWnd, buf);};

};

#endif

