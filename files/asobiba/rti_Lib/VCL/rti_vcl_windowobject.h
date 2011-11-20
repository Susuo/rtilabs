
#ifndef ___WINDOWOBJECTH
#define ___WINDOWOBJECTH
#include <.\VCL\rti_vcl_object.h>

class TWinObject ;

class TWinObject : public TObject
{
private:
	//�T�u�N���X��������
	void CreateSubClass(WNDPROC WndPorc);
	//�T�u�N���X������������
	void DestroySubClass();
	//�T�u�N���X���̂��߂� WinProc�̕ۑ�
    //VCL ���g���ȏシ�ׂẴE�B���h�̓T�u�N���X������܂�
	FARPROC True_StaticProc;
protected:
	/******************************************/
	/*                ���\�b�h                */
	/******************************************/
	//�V�K�쐬(���ׂĂ��̃��[�`���Ŕ��������Ȃ��Ă͂Ȃ�Ȃ�)
	bool CreateObject(TObject* hOwner,char *Caption,
        DWORD dwStyle,int x,int y,int w,int h,bool NULLid,WNDPROC WndPorc);
	//�V�K�N���X�쐬
    bool CreateOlignalClass(WNDPROC WndPorc);
	//�I�u�W�F�N�g�̍폜
	void DestroyComponent();
	//�I�u�W�F�N�g�̏�����
	void InitComponent();
	/******************************************/
	/*            �v���p�e�B                  */
	/******************************************/
    char ClassName[20];  //�N���X�̖��O
	//WM_COMMAND����
    FARPROC GetDefProc(){return True_StaticProc;};

public:
	/******************************************/
	/*                ���\�b�h                */
	/******************************************/
	TWinObject(); //�R���X�g���N�^

};

#endif
