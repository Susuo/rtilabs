/*****************************************************************/
/*****************************************************************/
//�_�C�A���O
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef DIALOG_LIFE

#include <.\Base\rti_dialog.h>
#include <.\Base\rti_debug.h>

#include <shlobj.h>

/*****************************************************************/
//�I�[�v��
// hWnd �I�[�i�[�E�B���h or NULL
// Title �^�C�g���o�[�ɕ\�����郄�c. or NULL
// Path  �J���p�X or NULL
// Filter �t�B���^�[�@��₱�����̂ŃT���v���݂Ă�(^^;;
// RetFullFilename MAX_PATH �̔z������邱��. �����Ƀt�@�C�������t���p�X�ł���
// FileTile        �f�B�t�H���g�ł���t�@�C���^�C�g�� or NULL
/*****************************************************************/
bool TDialog::Open(HWND hWnd,char *Title,char *Path,LPCSTR Filter,char *RetFullFilename,char *FileTile)
{
    OPENFILENAME ofn;
    if(!FileTile)
        ZeroMemory(RetFullFilename,MAX_PATH);
    else 
        lstrcpy(RetFullFilename,FileTile);

    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = Filter;
    ofn.lpstrFile = RetFullFilename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = Title;
	ofn.lpstrInitialDir = Path;

    if (GetOpenFileName(&ofn)) {
		return true;
    }
	RetFullFilename[0] = NULL;
    return false;
}

/*****************************************************************/
//�Z�[�u
/*****************************************************************/
bool TDialog::Save(HWND hWnd,char *Title,char *Path,LPCSTR Filter,char *RetFullFilename,char *FileTile)
{
    OPENFILENAME ofn;
    if(!FileTile)
        ZeroMemory(RetFullFilename,MAX_PATH);
    else 
        lstrcpy(RetFullFilename,FileTile);

    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = Filter;
    ofn.lpstrFile = RetFullFilename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
    ofn.lpstrTitle = Title;
	ofn.lpstrInitialDir = Path;

    if (GetSaveFileName(&ofn)) {
		return true;
    }
	RetFullFilename[0] = NULL;
    return false;
}

/*****************************************************************/
//�p�X�I��
/*****************************************************************/
bool TDialog::Path(HWND hWnd,char *Title,char *RetPath)
{
    bool ret = false;
    IMalloc *m;
    if(SUCCEEDED(SHGetMalloc(&m)))
    {
        BROWSEINFO b;

        b.hwndOwner = hWnd;
        b.pidlRoot = NULL;
        b.pszDisplayName = RetPath;
        b.lpszTitle = Title;
        b.ulFlags = BIF_RETURNONLYFSDIRS; //�t�H���_�̂�
        b.lpfn = NULL;
        b.lParam = 0;
        b.iImage = 0;
        LPITEMIDLIST id = SHBrowseForFolder(&b); //�_�C�A���O�̕\��
        if(id)
        {
            SHGetPathFromIDList(id, RetPath); //�p�X���𓾂�

            if (RetPath[lstrlen(RetPath)-1]!='\\') 
                           strcat(RetPath,"\\"); /*�Ō�� \\*/
            m->Free(id);
            ret = true;
        }
        m->Release();
    }
    return ret;
}
z
#endif //LIFE END
