/**********************************************************/
// �G�f�B�g�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef EDIT_LIFE

#ifndef ___TEDITEVENTH
#define ___TEDITEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>

//�G�f�B�g�̃C�x���g�N���X
//�G�f�B�g������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TEditEvent	: public TBaseObject
{
public:
	//�ŏ��̍s�̓��e���o�b�t�@�ɃR�s�[ (1�s���͐�p�ɍ���Ă݂܂���)
	void Get(char *buf) 
	{SendMessage(hWnd,EM_GETLINE,0L,(LPARAM)buf);};
    
    //���͂���Ă��郄�c�� int �^�ɕϊ����ĕԂ��܂�
    int GetInt();

	//�w��s�̓��e���o�b�t�@�ɃR�s�[
	void GetNum(int Num,char *Buf) 
	{SendMessage(hWnd,EM_GETLINE,(WPARAM)Num,(LPARAM)Buf);};

	//�G�f�B�b�g�R���g���[���̍s���𓾂�
	void GetRow() 
	{SendMessage(hWnd,EM_GETLINECOUNT,0L,0L);};

	//�G�f�B�b�g�R���g���[���̕ύX������(�ύX=true)
	BOOL GetModify() 
	{return (BOOL)SendMessage(hWnd,EM_GETMODIFY,0L,0L) ;};

    //�Z�b�g
    void Set(char *buf)
	{SetWindowText(hWnd, buf);};

	//�G�f�B�b�g�R���g���[���̕ύX�t���O�̕ύX(�t���Ofalse�Ō��ݕω��Ȃ�)
	BOOL SetModify(BOOL flg) 
	{return (BOOL)SendMessage(hWnd,EM_SETMODIFY,(WPARAM)flg,0L);};

	//�G�f�B�b�g�R���g���[���̑I������Ă���͈͂��擾
	//(sp , ep ��32�r�b�g�̃A�h���X��)
	void GetSel(int *sp,int *ep) 
	{SendMessage(hWnd,EM_GETSEL,(WPARAM)sp,(LPARAM)ep);};

	//�G�f�B�b�g�R���g���[���̑I������Ă���͈͂�ݒ�
	void SetSel(int sp,int ep) 
	{SendMessage(hWnd,EM_GETSEL,(WPARAM)sp,(LPARAM)ep);};

	//�G�f�B�b�g�R���g���[���̂��ׂĂ�I��
	void SetSelAll() 
	{SendMessage(hWnd,EM_GETSEL,(WPARAM)0L,(LPARAM)-1L);};

	//�G�f�B�b�g�R���g���[���̑I������Ă���Ƃ����u��������
	void ChangeString(char *String) 
	{SendMessage(hWnd,EM_REPLACESEL,(WPARAM)0L,(LPARAM)String);};

	//�G�f�B�b�g�R���g���[���ɓ��͂ł��镶������ݒ�
	void SetLimit(int cchMax) 
	{SendMessage(hWnd,EM_LIMITTEXT,(WPARAM)cchMax,(LPARAM)0L);};

	//�G�f�B�b�g�R���g���[�������[�h�I�����[�����ɂ���(���[�h�I�����[ = true )
	void SetReadOnly(bool fReadOnly) 
	{SendMessage(hWnd,EM_LIMITTEXT,(WPARAM)fReadOnly,(LPARAM)0L);};

	//�G�f�B�b�g�R���g���[�����A���h�D����
	void SetUndo() 
	{SendMessage(hWnd,EM_UNDO,(WPARAM)0L,(LPARAM)0L);};

	
	TEditEvent();
	void WmCommandCome(WPARAM wParam);
	//EN_CHANGE	�ҏW���ꂽ�Ƃ�
	SimpleEvent OnChange ;
	//EN_HSCROLL	���[�U�[���X�N���[�� �o�[���N���b�N�������Ƃ������܂��B 
	SimpleEvent OnHScroll ;
	//EN_MAXTEXT	�}���������؂�̂Ă�ꂽ���Ƃ������܂��B 
	SimpleEvent OnMaxText ;
	//EN_UPDATE	�G�f�B�b�g �R���g���[��������̃e�L�X�g��\�����悤�Ƃ��Ă��邱�Ƃ������܂��B 
	SimpleEvent OnUpdate ;
	//EN_VSCROLL	���[�U�[�������X�N���[�� �o�[���N���b�N�������Ƃ������܂��B 
	SimpleEvent OnVscroll ;
};




#endif

#endif
