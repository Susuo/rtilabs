/**********************************************************/
// ���X�g�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef LIST_LIFE

#ifndef ___TLISTEVENTH
#define ___TLISTEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//���X�g�{�b�N�X�̃C�x���g�N���X
//���X�g�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//EDIT

class TListBoxEvent	: public TBaseObject
{
public:
	//���X�g�̍Ō�ɒǉ�
	void Add(char *String) 
	{SendMessage(hWnd,LB_ADDSTRING,0L,(LPARAM)String);};

	//���X�g��Pos�ɒǉ�
	void Insert(int Pos,char *String) 
	{SendMessage(hWnd,LB_INSERTSTRING,(WPARAM)Pos,(LPARAM)String);};

	//���X�g���N���A
	void Clear() 
	{SendMessage(hWnd,LB_RESETCONTENT,0L,0L);};

//		//�w�肳��Ă��鍀�ڂ��I������Ă��邩�ǂ���
//		void ListFindSelect(Index) \
//		SendMessage(hWnd,LB_GETSEL,(WPARAM)Index,0L)

	//���ݑI������Ă���Ƃ�����������܂���������[
	int GetSelect() 
	{return SendMessage(hWnd,LB_GETCURSEL,0L,0L);};

	//���X�g�̍��ڐ�
	int GetMax() 
	{return SendMessage(hWnd,LB_GETCOUNT,0L,0L);};

	//���X�g��荀�ڂ̍폜
	void Delete(int Index) 
	{SendMessage(hWnd,LB_DELETESTRING,(WPARAM)Index,0L);};

	//���X�g��Index�̂Ƃ����String�ɕύX����
	void Change(int Index,char *String) 
	{
		SendMessage(hWnd,LB_DELETESTRING,(WPARAM)Index,0L); 
		SendMessage(hWnd,LB_INSERTSTRING,(WPARAM)Index,(LPARAM)String);
	}

	//���X�g�� Index �Ԗڂ�I��������
	void Select(int Index) 
	{SendMessage(hWnd, LB_SETCURSEL, (WPARAM)Index, 0L);};

	//���X�g�� Index �̓��e���Q�b�g
	int GetData(int Index,char *buf)
	{return SendMessage(hWnd,LB_GETTEXT,(WPARAM)Index,(LPARAM)buf);};

	TListBoxEvent();
	void WmCommandCome(WPARAM wParam);
	//LBN_SELCANCEL	���ڂ̑I�����L�����Z�����ꂽ���Ƃ������܂��B 
	SimpleEvent OnCancel	 ;
	//LBN_SELCHANGE	�I�����ڂ��ύX����悤�Ƃ��Ă��邱�Ƃ������܂��B 
	SimpleEvent OnChange	 ;

};

#endif

#endif
