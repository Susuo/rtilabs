#include "StartWithExeFile.h"
#ifdef AVI_LIFE

#ifndef ___AVIH
#define ___AVIH
#pragma comment(lib,"vfw32.lib ")

/*****************************************************************/
/*****************************************************************/
//AVI Player
//              Thank you For C MAGAZIN
/*****************************************************************/
/*****************************************************************/

#include <windows.h>
#include <vfw.h>
#include <tchar.h>
#include <.\Graphics\rti_bitmaplib.h>

class TMyAVI
{
	long GlStart,GlEnd; //�X�^�[�g�ʒu�ƏI���̈ʒu
	PAVIFILE Gpavi;     
	PAVISTREAM Gpstm;
	PGETFRAME Gpfrm;
    BMD       GAviBmd;  //����ɊG���[�߂���
    TBmpChar  *GBmpLib;
public:
    TMyAVI();
    ~TMyAVI();
    bool Open(char *filename,TBmpChar *BmpLib);
    bool Close();
    bool Get(int fream);
    //�G���擾
    BMD GetAviBmd(){return GAviBmd;};
};

typedef TMyAVI* AVD;

#endif

#endif

