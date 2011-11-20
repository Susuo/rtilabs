#include "StartWithExeFile.h"
#ifdef BITMAPLIB_LIFE

#ifndef ___BITMAPLIBH
#define ___BITMAPLIBH
#include <windows.h>
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X
#include <vector>
 	
class TBmpChar
{
private:
    HWND hwnd;
    std::vector<BMD> BmdList;
public: 
    TBmpChar(HWND whwnd){hwnd = whwnd;};
    TBmpChar(){hwnd = GetActiveWindow();};
	~TBmpChar();
    
    //�t�@�C������ǂݍ���
	BMD Make(const char *filename);
    //���ׂĔj��
	void Clear();
    //�w�肵�����c���͂�
	void Breaker(struct BMP_DATA_TABLE *puf);
    //�V�K�쐬
    BMD New(int sx,int sy);
    //�R�s�[
    BMD Copy(BMD bmd);
    //
    BMD At(int no){return BmdList[no];};
};


extern TBmpChar *BmpChar;

#endif

#endif //LIFE END
