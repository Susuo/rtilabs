#include <StartWithExeFile.h>
#ifdef RGN_LIFE

#ifndef ___RGNH
#define ___RGNH
#include <windows.h>

class TRgn
{
private:
    HDC RgnmemDC;            //�������f�o�C�X�R���e�L�X�g
    HBITMAP RgnhBit;         //�r�b�g�}�b�v�����
    HRGN hRGNMaster;      //���[�W�����n���h��
    struct RegionData {
        DWORD Size, iType, Count, RgnSize;
        RECT Bounds;
        RECT Rects[4000];
    };
    void LoadBmp(char *filename,HWND hwnd);
    void RgnCut(int sx,int ex,int y,RegionData *Data);
    void CreateRegion(RegionData *Data);
public:
    TRgn(HWND hwnd,char *filename,char flg);
    ~TRgn();

};

#define ALIGN_NONE 0
#define ALIGN_BMPSIZE 1

#endif

#endif //LIFE END
