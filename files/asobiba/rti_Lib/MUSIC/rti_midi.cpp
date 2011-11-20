/*****************************************************************/
/*****************************************************************/
//MIDI
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef MIDI_LIFE

#include <windows.h>
#include <.\Music\rti_midi.h>
#include <.\Base\rti_debug.h>

TMidiUse::TMidiUse(MidiCallBackProcDef Proc)
{
    hMidiStream    = NULL;            //MIDI�X�g���[���f�o�C�X�n���h��
    uiDeviceID     = MIDI_MAPPER;     //MIDI�X�g���[���f�o�C�XID
    bStream        = FALSE;           //�X�g���[���`���t���O
            //(�X�g���[���`�� TRUE /  �m�[�}���`�� FALSE)
    bPlay          = FALSE;           //�Đ���
            //(�Đ��� TRUE /  ��~ FALSE)
    bLoop          = FALSE;           //���[�v
            //(���[�v���� TRUE /  ���[�v���Ȃ� FALSE)
    dwNumBlocks    = 0;               //MDS���t�f�[�^�u���b�N��
    dwCurrentBlock = 0;               //���݂܂łœ����o�b�t�@�֏o�͂����u���b�N��
    lpMidiHdr   = NULL;              //���t�p MIDIHDR �\���̔z��p������
    lpMDSBuffer = NULL;              //MDS�u���b�N�f�[�^�z��p������
    MidiCallBackProc        = Proc;
}

BOOL TMidiUse::LoadMDS(LPSTR lpszFileName,BYTE **lpMDSData)
{
    HMMIO       hFile;
    MMRESULT    mmResult;
    MMCKINFO    mmParent,mmSubchunk;
    DWORD       dwFileSize,dwDataSize;

    hFile = mmioOpen(lpszFileName,NULL,MMIO_READ);
    if(hFile==NULL)
    {
        DEBUG("file not open as mmi");
        return FALSE;
    }
    mmParent.fccType = mmioFOURCC('M','I','D','S');
    mmResult = mmioDescend(hFile,(LPMMCKINFO)&mmParent,
        NULL,MMIO_FINDRIFF);
    if(mmResult == MMIOERR_CHUNKNOTFOUND)
    {
        DEBUG("��������[��");
        mmioClose(hFile,0);
        return FALSE;
    }
    dwFileSize = mmParent.cksize;
    mmSubchunk.ckid = mmioFOURCC('f','m','t',' ');
    mmResult = mmioDescend(hFile,&mmSubchunk,&mmParent,MMIO_FINDCHUNK);
    if(mmResult == MMIOERR_CHUNKNOTFOUND)
    {
        DEBUG("fmt fot find");
        mmioClose(hFile,0);
        return FALSE;
    }
    if(mmioRead(hFile,(char*)&MdsFormat,sizeof(MDSFORMAT))
                                            != sizeof(MDSFORMAT))
    {
        DEBUG("read faild");
        mmioClose(hFile,0);
        return FALSE;
    }
    mmioAscend(hFile,&mmSubchunk,0);
    mmSubchunk.ckid = mmioFOURCC('d','a','t','a');
    mmResult = mmioDescend(hFile,&mmSubchunk,&mmParent,MMIO_FINDCHUNK);
    if(mmResult == MMIOERR_CHUNKNOTFOUND)
    {
        DEBUG("data not find");
        mmioClose(hFile,0);
        return FALSE;
    }
    dwDataSize = mmSubchunk.cksize;
    if(mmioRead(hFile,(char*)&dwNumBlocks,sizeof(DWORD)) != sizeof(DWORD) )
    {
        DEBUG("block not read");
        mmioClose(hFile,0);
        return FALSE;
    }
    (*lpMDSData) = (BYTE*) GlobalAlloc(GPTR,dwDataSize);
    if(lpMDSData == NULL)
    {
        DEBUG("not alloc");
        mmioClose(hFile,0);
        return FALSE;
    }
    if(mmioRead(hFile,(char*)*lpMDSData,dwDataSize ) == -1)
    {
        DEBUG("not read");
        mmioClose(hFile,0);
        GlobalFree(lpMDSData);
        return FALSE;
    }

    mmioClose(hFile,0);
    return TRUE;
}

BOOL TMidiUse::Open(LPSTR lpszfileName)
{
//      MMRESULT    mmResult;
    BYTE        *lpMDSData;

    Close();        //�O�̂���

    if(!LoadMDS(lpszfileName,&lpMDSData))
    {
        DEBUG("file not load");
        return FALSE;
    }

    //�f�[�^�o�b�t�@���X�g���[���`���ɂ��邩�ǂ����ݒ肷��
    //�u���b�N����16��葽���ꍇ�X�g���[���`��
    bStream = (dwNumBlocks >= 16) ? TRUE : FALSE;
    //�X�g���[���p�f�[�^�̗p��(�o�b�t�@�T�C�Y �~�@�u���b�N��)
    lpMDSBuffer = (LPMDSBUFFER) GlobalAlloc(GPTR , 
                        dwNumBlocks * sizeof(MDSBUFFER));
    lpMidiHdr  = (MIDIHDR*) GlobalAlloc(GPTR , 
                        dwNumBlocks * sizeof(MIDIHDR));
    for(DWORD i = 0; i < dwNumBlocks ; i++)
    {
        //�X�g���[���o�b�t�@���ݒ�(�u���b�N�f�[�^�̉��t�f�[�^�̃T�C�Y���i�[)
        lpMDSBuffer[i].dwSize = ((LPMDSBUFFERHEADER)lpMDSData)->dwBlockLength;
        //MDSBUFFER �� lpData�Ƀ����������蓖�Ă�
        lpMDSBuffer[i].lpData = (BYTE*)GlobalAlloc(GPTR,(MdsFormat.dwMaxBuffer));;
        //MDS�u���b�N�f�[�^�ւ̃|�C���^
        lpMidiHdr[i].lpData   = (char*) lpMDSBuffer[i].lpData;
        //MDS�u���b�N�f�[�^�̎��T�C�Y
        lpMidiHdr[i].dwBufferLength = MdsFormat.dwMaxBuffer;
        //MDS�u���b�N�f�[�^���Ŏ��ۂɎg�p���Ă���o�C�g��
        lpMidiHdr[i].dwBytesRecorded = lpMDSBuffer[i].dwSize;
        //�u���b�N�ԍ������ʎq�Ƃ��ē���Ă���
        lpMidiHdr[i].dwUser    = i;
        lpMidiHdr[i].reserved  = 0;
        lpMidiHdr[i].dwFlags   = 0;
        lpMidiHdr[i].lpNext    = NULL;
        //�|�C���^��i�s�������t�f�[�^���u���b�N���ƃR�s�[����
        lpMDSData  += sizeof(MDSBUFFERHEADER);
        CopyMemory(lpMDSBuffer[i].lpData,lpMDSData,lpMDSBuffer[i].dwSize);
        //�|�C���^�����̃u���b�N�f�[�^�ɐi�߂�
        lpMDSData  += lpMDSBuffer[i].dwSize;
    }
    //�t�@�C������ǂݍ��񂾃f�[�^���������
    GlobalFree(lpMDSData);
    //MIDI�X�g���[���f�o�C�X�I�[�v��
    if(!hMidiStream)
        midiStreamOpen(&hMidiStream,&uiDeviceID,1,
                (DWORD)MidiCallBackProc,(DWORD)0,CALLBACK_FUNCTION);
    //�^�C���v���p�e�B
    MIDIPROPTIMEDIV midiProp;
    midiProp.cbStruct  = sizeof(MIDIPROPTIMEDIV);
    midiProp.dwTimeDiv = MdsFormat.dwTimeFormat;
    midiStreamProperty( hMidiStream,(BYTE*)&midiProp,
        MIDIPROP_SET | MIDIPROP_TIMEDIV);
    return TRUE;
}

BOOL TMidiUse::Play(BOOL bLooping)
{
    //2�d�Đ���h��
    if(bPlay)
        return FALSE;
    //�e�Đ��p�p�����[�^��������
    bPlay = TRUE;
    bLoop = bLooping;
    //�Đ��J�n���̓����o�b�t�@(���t�L���[)�ւ̏o�̓u���b�N�̓[��
    dwCurrentBlock = 0;
    //�X�g���[���o�b�t�@�փf�[�^���Z�b�g����.
    //�X�g���[���o�b�t�@�� 64K �܂łȂ̂ŁA
    // 65535Bytes / 4096Bytes �ŁA��x�ɃX�g���[���o�b�t�@�ɓ]���ł���
    //�u���b�N�� �ő�16�u���b�N
    for(DWORD i = 0 ; i < 16 ; i++)
    {
        //�X�g���[���`���Ńf�[�^��]�����Ȃ��ꍇ��
        //�ő�u���b�N�����z�������_�ŏI��
        if( (!bStream) && (i>=dwNumBlocks) )
            break;
        //�X�g���[���o�b�t�@�փf�[�^��]�����邽�߂̏���
        midiOutPrepareHeader((HMIDIOUT)hMidiStream,
            &lpMidiHdr[i],sizeof(MIDIHDR));
        //�X�g���[���f�[�^���Đ����邽�߂̃L���[���쐬����
        midiStreamOut(hMidiStream,&lpMidiHdr[i],sizeof(MIDIHDR));
        //�����o�b�t�@(���t�L���[)�֏o�͂����u���b�N�����J�E���g����
        dwCurrentBlock++;
    }
    //�Đ��J�n
    midiStreamRestart(hMidiStream);
    return TRUE;
}


void TMidiUse::DefMidiProc(HMIDIIN hMidiIn,UINT message,
                 DWORD dwInstance,DWORD wParam,DWORD lParam)
{
    MIDIHDR *midiHdr;
    //wParam �͍Đ����I������ MIDIHDR �\����(�u���b�N�f�[�^)�ւ�
    //�|�C���^���i�[����Ă���\����
    midiHdr = (MIDIHDR*)wParam;

    switch(message)
    {
    case MOM_DONE:
        //�o�b�t�@�ǉ��܂��̓��[�v�Đ�
        //midiHdr->User �́A�Đ����I�������u���b�N�ԍ�
        //�������Ă���̂ŁA�o�b�t�@�ǉ��֐��ɓn��
        if(bStream==TRUE)
            SetStreamNextBuffer(midiHdr->dwUser+1);
        else
            SetNormalNextBuffer(midiHdr->dwUser+1);
        break;
    }
}

BOOL TMidiUse::SetNormalNextBuffer(DWORD dwPlayedBlock)
{
    if(!bPlay)
        return FALSE;
    if(dwPlayedBlock>=dwNumBlocks)
    {
        if(bLoop)
        {
            //���[�v�Đ�
            for(DWORD i = 0 ; i < dwNumBlocks ; i++)
            {
                //�X�g���[���f�[�^�Đݒ�
                midiOutPrepareHeader( (HMIDIOUT)hMidiStream,
                    &lpMidiHdr[i],sizeof(MIDIHDR) );
                //�X�g���[���f�[�^���Đ��̂��߂̃L���[�ɐݒ肷��
                midiStreamOut( hMidiStream,&lpMidiHdr[i],sizeof(MIDIHDR));
            }
        }
        else
            bPlay = FALSE;
    }
    return TRUE;
}

BOOL TMidiUse::SetStreamNextBuffer(DWORD dwPlayedBlock)
{
    if(!bPlay)
        return FALSE;
    //�X�g���[���o�b�t�@������(8�o�b�t�@)
    //�Đ��I��������V�����o�b�t�@�ǉ�����
    if(!(dwPlayedBlock&0x07))
    {
        //����8�o�b�t�@���o��
        for(DWORD i=0; i<8 ; i ++)
        {
            //�o�b�t�@��ݒ肵���u���b�N����
            //�ő�u���b�N�����z������ȉ����ȗ�����
            if(dwCurrentBlock>=dwNumBlocks)
                break;
            //�X�g���[���f�[�^����
            midiOutPrepareHeader( (HMIDIOUT)hMidiStream,
                 &lpMidiHdr[dwCurrentBlock],sizeof(MIDIHDR) );
            //�X�g���[���f�[�^���Đ��̂��߂̃L���[�ɐݒ肷��
            midiStreamOut( hMidiStream,
                &lpMidiHdr[dwCurrentBlock],sizeof(MIDIHDR));
            //�Đ��ς݃u���b�N���J�E���g
            dwCurrentBlock++;
        }
    }
    //���[�v�Đ�
    if(dwPlayedBlock >= dwNumBlocks)
    {
        bPlay = FALSE;
        if(bLoop)
            Play(TRUE);
    }
    return TRUE;
}

BOOL TMidiUse::Close()
{
    if(!hMidiStream)
        return FALSE;
    //�f�o�C�X���N���[�Y����O�ɔO�̂��߉��t���~������.
    Stop();
    //MIDI�f�o�C�X�̂��ׂẴ`�����l���̍Đ��̒��~
    midiOutReset((HMIDIOUT)hMidiStream);
    for(DWORD i = 0; i < dwNumBlocks ; i++)
    {
        //MIDI �X�g���[���f�[�^����
        //���������w�������u���b�N���������O�ɕK���s��Ȃ��Ă͂Ȃ�Ȃ�
        midiOutUnprepareHeader( (HMIDIOUT)hMidiStream,
            &lpMidiHdr[i] , sizeof(MIDIHDR) );
    }
    //MIDI�X�g���[���N���[�Y
    midiStreamClose(hMidiStream);
    hMidiStream = NULL;

    //�u���b�N�Ɋ��蓖�ĂĂ��������������
    if(lpMDSBuffer)
    {
        for(DWORD i = 0 ; i < dwNumBlocks ; i++)
        {
            GlobalFree(lpMDSBuffer[i].lpData);
        }
        GlobalFree(lpMDSBuffer);
        lpMDSBuffer = NULL;
    }
    GlobalFree(lpMidiHdr);
    return TRUE;
}

BOOL TMidiUse::Stop()
{
    bPlay = FALSE;
    bLoop = FALSE;
    if(!hMidiStream)
        return FALSE;
    //MIDI�X�g���[�����t��~
    midiStreamStop(hMidiStream);
    return TRUE;
}

TMidiUse::~TMidiUse()
{
    Close();
}


#endif //LIFE END
