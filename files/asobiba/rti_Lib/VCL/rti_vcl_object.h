#include "StartWithExeFile.h"
#ifdef OBJECT_LIFE


#ifndef ___OBJECTH
#define ___OBJECTH
#include <windows.h>
#include <list>

class TObject ;

//�V���v���ȃC�x���g
typedef void (*SimpleEvent)(TObject *my);
//�}�E�X�C�x���g
typedef void (*MouseEvent)(TObject *my ,UINT fwKeys, int xPos, int yPos);
//fwKeys = wParam;	/* �L�[�̃t���O	*/
//xPos = LOWORD(lParam);	/* �J�[�\���̐��������̈ʒu	*/
//yPos = HIWORD(lParam);	/* �J�[�\���̐��������̈ʒu	*/

//�L�[�C�x���g
typedef void (*KeyEvent)(TObject *my ,int nVirtKey, UINT lKeyData);
//nVirtKey = (int) wParam;	/* ���z�L�[ �R�[�h	*/
//lKeyData = lParam;	/* �L�[�̃f�[�^	*/

//�u�[���C�x���g
typedef bool (*BoolEvent)(TObject *my);


typedef std::list<TObject*>::iterator ObjectIT;
/*****************************************************************/
//���ׂĂ̌��ɂȂ���
//���ׂẴR���|�[�l���g(�R���g���[��or�I�u�W�F�N�g)�́A
//���� TObject ���p������͂��ł� (��
/*****************************************************************/
class TObject 
{
private:
	//���̃I�u�W�F�N�g�̔j������Ƃ��̂���ׂ��p�ɃL���X�g
    void ObjectTrancerationAndBreak(TObject* hObject);
    //�����̏��L���Ă���I�u�W�F�N�g���X�g
    std::list<TObject*> ObjectList;
    int Tag; //�����R�ɂ��g����������(^^;;
    int TagSub; //�����R�ɂ��g����������(^^;;
	int OwnerUse; //�I�[�i�[�����悤���邱�Ƃ�����

protected:
	/******************************************/
	/*                ���\�b�h                */
	/******************************************/
	//���E
    void IamDestroy();
	//�q���E��
    void ChildDestroy();
	//�V�K�ɃI�u�W�F�N�g�ǉ�
    void OwnerAdd(TObject* hOwner,TObject* hMy);
	//�I�u�W�F�N�g�̍폜
    void virtual DestroyComponent() = 0;
	//�I�u�W�F�N�g�̏�����
    void virtual InitComponent() = 0;
	/******************************************/
	/*            �v���p�e�B                  */
	/******************************************/
    int Kind;        //�����̎��
    int Id;            //ID���ʎq

    HANDLE hWnd;       //�E�B���h�n���h��
    TObject* hMyOwner;    //�����������Ă������
  	bool OwerKill;   //�I�[�i�[������ł��邩

public:
	/******************************************/
	/*                ���\�b�h                */
	/******************************************/
	TObject(); //�R���X�g���N�^
	//WM_COMMAND����
    virtual void WmCommandCome(WPARAM wParam) {};
    //�I��������
    virtual void UserDestroy() {};

    ObjectIT GetComponentFarst(){return ObjectList.begin() ;};
    ObjectIT GetComponentEnd(){return ObjectList.end() ;};
    //�����̎����Ă���R���g���[���̐���������
    unsigned short HowMenyHaveControls(){return ObjectList.size() ;};

    //�^�O�֌W
    int GetTag(){return Tag;};
    int GetTagSub(){return TagSub;};
    void SetTag(int tag){Tag = tag;};
    void SetTagSub(int tag){TagSub = tag;};

    //�I�[�i�[���[�Y
    int GetOwerUse(){return OwnerUse;};
    void SetOwerUse(int use){OwnerUse = use;};

    TObject* GethOwer(){return hMyOwner;};
    HANDLE GethOwerWnd(){return hMyOwner->hWnd;};
    //ID���Q�b�g
    int GetId(){return Id;};
    HWND GethWnd(){return (HWND)hWnd;};
    HANDLE GethHandle(){return hWnd;};
    void SethWnd(HWND k){hWnd = (HANDLE)k;};
    void SetHandle(HANDLE k){hWnd = k;};
    int GetKind(){return Kind;};
};

#define OBJECT_KIND_BAD         0//�쐬�Ɏ��s�������c. �댯�Ȃ̂Ŏ�菜���Ă�������,
#define OBJECT_KIND_APPLICATION 1 //�ŏ��
#define OBJECT_KIND_FORM        2 //�t�H�[��
#define OBJECT_KIND_BUTTON      3 //�{�^��
#define OBJECT_KIND_GRID        4 //�O���b�h
#define OBJECT_KIND_PROGRESS    5 //�v���O�����X�o�[
#define OBJECT_KIND_STATUS      6 //�X�e�[�^�X�o�[
#define OBJECT_KIND_LISTBOX     7 //���X�g�{�b�N�X
#define OBJECT_KIND_COMBOBOX	8 //�R���{�{�b�N�X
#define OBJECT_KIND_CHECKBUTTON 9 //�`�F�b�N�{�^��
#define OBJECT_KIND_RADIOBUTTON 10//���W�I�{�^��
#define OBJECT_KIND_GROUPBOX    11//�O���[�v�{�b�N�X
#define OBJECT_KIND_RADIOBOX    12//���W�I�{�b�N�X
#define OBJECT_KIND_TABCONTROL  13//�^�u�R���g���[��
#define OBJECT_KIND_EDIT		14//�G�f�B�g
#define OBJECT_KIND_SCROLL      15//�X�N���[���o�[
#define OBJECT_KIND_STATICTEXT  16//�X�^�e�B�b�N�e�L�X�g
#define OBJECT_KIND_CHECKBOX    17//�`�F�b�N�{�b�N�X
#define OBJECT_KIND_MENU        18//���j���[
#define OBJECT_KIND_TOOLBAR     19//�c�[���o�[

#endif


#endif //LIFE END
