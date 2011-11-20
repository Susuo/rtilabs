/**********************************************************/
// �R���{�{�b�N�X�̃C�x���g�Ƃ�
/**********************************************************/
#include "StartWithExeFile.h"
#ifdef COMB_LIFE

#ifndef ___TCOMBEVENTH
#define ___TCOMBEVENTH
#include <.\VCL\rti_vcl_object.h>
#include <.\VCL\rti_vcl_base.h>
//�R���{�{�b�N�X�̃C�x���g�N���X
//�R���{�{�b�N�X������R���|�[�l���g�͂�����p�������܂�.
//COMBOBOX

class TCombEvent	: public TBaseObject
{
public:
		//���X�g�̍Ō�ɒǉ�
		void Add(char *String) 
		{SendMessage(hWnd,CB_ADDSTRING,0L,(LPARAM)String);};

		//���X�g��Pos�ɒǉ�
		void Insert(int Pos,char *String) 
		{SendMessage(hWnd,CB_INSERTSTRING,(WPARAM)Pos,(LPARAM)String);};

		//���X�g���N���A
		void Clear() 
			{SendMessage(hWnd,CB_RESETCONTENT,0L,0L);};

		//���ݑI������Ă���Ƃ�����������܂���������[
		int GetSelect() 
			{return SendMessage(hWnd,CB_GETCURSEL,0L,0L);};

		//���X�g�̍��ڐ�
		int GetMax()
			{return SendMessage(hWnd,CB_GETCOUNT,0L,0L);};

		//���X�g��荀�ڂ̍폜
		void Delete(int Index)
		{SendMessage(hWnd,CB_DELETESTRING,(WPARAM)Index,0L);};

		//���X�g��Index�̂Ƃ����String�ɕύX����
		void Change(int Index,char *String) 
		{
			SendMessage(hWnd,CB_DELETESTRING,(WPARAM)Index,0L);
			SendMessage(hWnd,CB_INSERTSTRING,(WPARAM)Index,(LPARAM)String);
		};

		//���X�g�� Index �Ԗڂ�I��������
		void Select(int Index)
		{SendMessage(hWnd, CB_SETCURSEL, (WPARAM)Index, 0L);};

		//���X�g�� Index �̓��e���Q�b�g
		char* GetData(int Index)
		{return (char*)SendMessage(hWnd, CB_GETITEMDATA, (WPARAM)Index, 0L);};

	TCombEvent();
	void WmCommandCome(WPARAM wParam);
	SimpleEvent	OnClose;
	SimpleEvent	OnDropDown;
	SimpleEvent	OnEditChange;
	SimpleEvent	OnEditUpDate;
	SimpleEvent	OnChange;
	SimpleEvent	OnCancel;
	SimpleEvent	OnOk;
};



#endif

#endif
