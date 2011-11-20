#include "StartWithExeFile.h"
#ifdef MIDI_LIFE

#ifndef ___MIDIH
#define ___MIDIH
#include <windows.h>
#include <mmsystem.h>

//fmt�`�����N�̏ڍׂ��󂯎�邽�߂̍\����
typedef struct
{
    DWORD   dwTimeFormat;   //MIDI���ԃt�H�[�}�b�g(���q�����@����\)
    DWORD   dwMaxBuffer;    //�X�g���[���o�b�t�@1�u���b�N�̍ő�T�C�Y
    DWORD   dwFlags;        //�f�[�^�t�H�[�}�b�g
}MDSFORMAT;

//�������ꂽ���t�f�[�^�̏ڍׂ𔲂��o���̂ɗp����
typedef struct
{
    DWORD   dwBlockStart;   //�u���b�N�̍Đ�����
    DWORD   dwBlockLength;  //�u���b�N�f�[�^�T�C�Y
}MDSBUFFERHEADER,*LPMDSBUFFERHEADER;

//�ꎞ�I�ɕ������ꂽ���t�f�[�^��ۑ����Ă������߂̃o�b�t�@
//(�������ꂽ�u���b�N���K�v)
typedef struct
{
    DWORD   dwSize;     //�u���b�N�f�[�^�T�C�Y
    BYTE    *lpData;    //���t�f�[�^
}MDSBUFFER,*LPMDSBUFFER;

typedef void (*MidiCallBackProcDef)(HMIDIIN,UINT,DWORD,DWORD,DWORD);

class TMidiUse
{
private:
    HMIDISTRM   hMidiStream;    //MIDI�X�g���[���f�o�C�X�n���h��
    UINT        uiDeviceID;     //MIDI�X�g���[���f�o�C�XID
    BOOL        bStream;        //�X�g���[���`���t���O
            //(�X�g���[���`�� TRUE /  �m�[�}���`�� FALSE)
    BOOL        bPlay;          //�Đ���
            //(�Đ��� TRUE /  ��~ FALSE)
    BOOL        bLoop;          //���[�v
            //(���[�v���� TRUE /  ���[�v���Ȃ� FALSE)
    DWORD       dwNumBlocks;    //MDS���t�f�[�^�u���b�N��
    DWORD       dwCurrentBlock; //���݂܂łœ����o�b�t�@�֏o�͂����u���b�N��
    MIDIHDR     *lpMidiHdr;     //���t�p MIDIHDR �\���̔z��p������
    MDSBUFFER   *lpMDSBuffer;   //MDS�u���b�N�f�[�^�z��p������
    MDSFORMAT   MdsFormat;      //MDS�f�[�^�t�H�[�}�b�g
    MidiCallBackProcDef MidiCallBackProc;

    BOOL LoadMDS(LPSTR lpszFileName,BYTE **lpMDSData);
    BOOL SetStreamNextBuffer(DWORD dwPlayedBlock);
    BOOL SetNormalNextBuffer(DWORD dwPlayedBlock);

public:
    TMidiUse(MidiCallBackProcDef Proc);
    ~TMidiUse();
    BOOL Open(LPSTR lpszfileName);
    BOOL Play(BOOL bLooping);
    BOOL Close();
    void DefMidiProc(HMIDIIN hMidiIn,UINT message,
                 DWORD dwInstance,DWORD wParam,DWORD lParam);
    BOOL Stop();

};
typedef TMidiUse* TMIDI;


#endif

#endif //LIFE END
