
#include <.\VCL\rti_vcl_apprication.h>
#include <.\VCL\rti_vcl_windowobject.h>

TWinObject::TWinObject()
{
}

/**********************************************************/
// �R���|�[�l���g�쐬
/**********************************************************/
bool TWinObject::CreateObject(TObject* hOwner,char *Caption,DWORD dwStyle,
                       int x,int y,int w,int h,bool NULLid,WNDPROC WndPorc)
{
    HWND whOnwner;

    if (!hOwner)	whOnwner = NULL ;
    else
	{ 
		whOnwner = hOwner->GethWnd();
		dwStyle = dwStyle | WS_CHILD;//�I�[�i�[������̂Ŏq�E�B���h�ł��� (^^;;
	}

    if (NULLid)	Id = 0;
    else        Id = Application->GetId();

	//�E�B���h�쐬
    hWnd = CreateWindowEx(
        NULL,
		ClassName,             //�N���X��
		Caption,                     //�E�B���h�^�C�g��
		dwStyle,                     //�E�B���h�X�^�C��
		x,                           //�E�B���h�̍���X���W
		y,                           //�E�B���h�̍���Y���W
		w,                           //��
		h,                           //����
		whOnwner,                    //�e�E�B���h�̃n���h��
		(HMENU)Id,               //?�̃n���h��
		Application->GethInstance(),      //�C���X�^���X�n���h��
		NULL                         //�g���p�̃p�����[�^
		);
    //�E�B���h�쐬�Ɏ��s
    if (!hWnd) return false;
    //�����̏�����
	InitComponent();

    //VCL�Ŏg�p���邽�߂ɃT�u�N���X������
    CreateSubClass(WndPorc);

	//���C���t�H�[���̃E�B���h�n���h���Q�b�g�����[
    if (!Application->hWnd) Application->hWnd =  hWnd;
    return true;

}

/**********************************************************/
//������
/**********************************************************/
void TWinObject::InitComponent()
{
    //���[�U��`�̗��Z���p
    SetWindowLong( GethWnd() , GWL_USERDATA, (LONG)this);

}

/**********************************************************/
/* �I���W�i���N���X�쐬*/
/**********************************************************/
bool TWinObject::CreateOlignalClass(WNDPROC WndPorc)
{
	static int ClassNameunick = 0;
	WNDCLASS  wcls;     //�N���X��`�p�̍\����
    wsprintf(ClassName,"_FORM%d",ClassNameunick++);

	//�E�B���h�N���X��`��
	wcls.hInstance = Application->GethInstance();         //�C���X�^���X�n���h��
	wcls.lpszClassName = ClassName;//�N���X��
	wcls.lpfnWndProc = WndPorc;           //�E�B���h�v���V�[�W��
    wcls.style = CS_BYTEALIGNCLIENT;                     //�N���X��
	wcls.hIcon = LoadIcon(NULL,IDI_APPLICATION); //�A�C�R���n���h��
	wcls.hCursor = LoadCursor(NULL,IDC_ARROW);   //�J�[�\���n���h��
	wcls.lpszMenuName = NULL;                    //���j���[
	wcls.cbClsExtra = 0;                         //�g��
	wcls.cbWndExtra = 0;                         //�g��

	wcls.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH	);

	//�E�B���h�N���X���암
	if (!RegisterClass(&wcls) ) 
    {
        return false; //�E�B���h����Ɏ��s
    }
    return true;

}

/**********************************************************/
/* �T�u�N���X�������s*/
/**********************************************************/
void TWinObject::CreateSubClass(WNDPROC WndPorc)
{
    // ���X�̃X�^�e�B�b�N�E�B���h�E�v���V�[�W���̕ۑ�
	True_StaticProc = (FARPROC)GetWindowLong( GethWnd() , GWL_WNDPROC);
	// �X�^�e�B�b�N�E�B���h�E�̃T�u�N���X��
	SetWindowLong( GethWnd() , GWL_WNDPROC, (LONG)WndPorc);
}

/**********************************************************/
/* �T�u�N���X��������*/
/**********************************************************/
void TWinObject::DestroySubClass()
{
    if (IsWindow( GethWnd() ))
        SetWindowLong(  GethWnd()  , GWL_WNDPROC, (LONG)True_StaticProc);
}

/**********************************************************/
/* �R���|�[�l���g�폜�̒ʏ폈��*/
/**********************************************************/
void TWinObject::DestroyComponent()
{
    //��ꂽ�I�u�W�F�N�g�������疳��.
    if (Kind == OBJECT_KIND_BAD) return;
    OwerKill        = true;
	//�T�u�N���X����������.
	DestroySubClass();
    //�����̎����Ă������ׂẴR���g���[�������
    ChildDestroy();
    //�����̃I�[�i�[�Ɏ����������邱�Ƃ�ʒm
    IamDestroy();
    //���[�U��`�̗��Z���p�̉���
    if (IsWindow( GethWnd() ) )
        SetWindowLong( GethWnd() , GWL_USERDATA, (LONG)0);
}
