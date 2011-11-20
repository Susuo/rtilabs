#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE

/***********************************************************/
//���ۂ� Wave �Đ����i�郋�[�`��
//          Thank you C MAGAZIN ... ����.. ���Ղ�(��
/***********************************************************/
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Music\rti_wave_player.h>
#include <.\Base\rti_debug.h>

//22.506kHZ �� 8 �r�b�g�X�e���I�̋ȈȊO����Ȃ�!!!
#define CHANNELS 2
#define SAMPLERATE 22050
#define SAMPLEBITS 8
//��̃f�[�^�̃u���b�N��
#define ONE_BLOCK_BTYE	(CHANNELS * (SAMPLEBITS/8) ) //2
#define ONE_LINE_SIZE (char)	//��̃`���l�����K�v�Ƃ���^
#define BUF_LINE_SIZE (short)

#define POOL_WAVES 32
#define SOUND_OVER_MAX 100
#define WAVE_BUFFER_SIZE 30000
void TryProc(char *,MMRESULT rc);
/***********************************************************/
//�R���X�g���N�^
/***********************************************************/
TWavePlayer::TWavePlayer()
{
    //�t�H�[�}�b�g����!!
    WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    WaveFormat.nChannels = CHANNELS;
    WaveFormat.nSamplesPerSec = SAMPLERATE;
    WaveFormat.wBitsPerSample = SAMPLEBITS;
    WaveFormat.nBlockAlign = (WORD)((SAMPLEBITS/8) * CHANNELS);
    WaveFormat.nAvgBytesPerSec = (DWORD)SAMPLERATE * WaveFormat.nBlockAlign;
    WaveFormat.cbSize = sizeof(WAVEFORMATEX);
    //�f�o�C�X�I�[�v��    
    MMRESULT ret;
    ret = waveOutOpen(&hWaveOutHandle,WAVE_MAPPER,&WaveFormat,
                    (DWORD)Application->GethWnd(),0,CALLBACK_WINDOW);
    //�f�o�h�����I�[�v���ł������ǂ����L�^���Ă����܂�.
    if (ret!=MMSYSERR_NOERROR) WavePlayOk = false;  //�s��
    else                       WavePlayOk = true;   //����!
    //�_�u���o�b�t�@�̍쐬
    for(int i=0;i<2;i++){
        FWaveHdr[i] = (WAVEHDR*)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof(WAVEHDR));
        FWaveHdr[i]->dwBufferLength = WAVE_BUFFER_SIZE;
        FWaveHdr[i]->lpData = (char*)HeapAlloc(GetProcessHeap(),0,FWaveHdr[i]->dwBufferLength+1);
    }
    Playing = false;
	BufUsing = 0;	//�o�b�t�@�Ƀf�[�^���Ȃ�
    Pool = 0;
}

/***********************************************************/
//�f�X�g���N�^
/***********************************************************/
TWavePlayer::~TWavePlayer()
{
	//�\�񂷂ׂĎ�����
    WavePlayOk = false;
	All_Breaker();
    //�_�u���o�b�t�@�̉��
    for(int i=0;i<2;i++){
        TryProc( "~TWavePlayer",waveOutUnprepareHeader(hWaveOutHandle,FWaveHdr[i],sizeof(WAVEHDR)) );
        HeapFree(GetProcessHeap(),0,FWaveHdr[i]->lpData);
        HeapFree(GetProcessHeap(),0,FWaveHdr[i]);
    }
    //�f�o�C�X�N���[�Y.
    waveOutClose(hWaveOutHandle);
}

/***********************************************************/
//���̃f�[�^���Đ�����.
/***********************************************************/
void TWavePlayer::Play(WVD wvd)
{
    //�f�o�C�X�I�[�v���Ɏ��s���Ă���Ζv
    if (!WavePlayOk) return ;

	if (PlayingList.size() < SOUND_OVER_MAX)
	{
		//�ȃ��X�g�ɒǉ�����.
		PlayingWaveList *lpPWL = new PlayingWaveList;
		lpPWL->wvd = wvd;
		lpPWL->Offset = 0;
		PlayingList.push_back(lpPWL);
//        DEBUG3("�Đ��w�� %x ��",(UINT)lpPWL );
        Pool ++; //�v�[���̗ʂ𑝂₷
	}
}

/***********************************************************/
//���t��~���牉�t���Ăˁ@�ɂ�������Ƃ��ɂ͂�����.
/***********************************************************/
void TWavePlayer::FirstPlay()
{
//    DEBUG("�o�b�t�@ �t�@�[�X�g���s���܂�.");
    //�v�[�����
    Pool = 0;
    //�Đ����ɂ���
    Playing = true;
    //�o�b�t�@�Ƀ��[�h���܂�.

    TryProc("FirstPlay",waveOutPause(hWaveOutHandle));
    for(int i=0;i<2;i++)
    {
        WriteNext(FWaveHdr[i]);
    }
    TryProc("FirstPlay",waveOutRestart(hWaveOutHandle));
}

/***********************************************************/
//�Đ��������������Ă���. 
/***********************************************************/
void TWavePlayer::WaveSuccessfulEnd(WAVEHDR *hdr)
{
    //�o�b�t�@�Ƀf�[�^���Ȃ���ł���.
    BufUsing--;
//    DEBUG3("�o�b�t�@ %x ��� %d",(UINT)hdr,BufUsing);
    //�������o�b�t�@�Ƀf�[�^��ǉ�����.
    WriteNext(hdr);
}


/***********************************************************/
//���̃f�[�^ �v���[�Y
/***********************************************************/
void TWavePlayer::WriteNext(WAVEHDR *hdr)
{
    //��n��
    TryProc("WriteNext1",waveOutUnprepareHeader(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
    //�f�[�^���[�h
    if (WVDDataLoadBuf(hdr) > 0)
    {
        //�o�b�t�@�Ƀf�[�^����
//        TryProc("WriteNext3",waveOutWrite(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
        if (waveOutWrite(hWaveOutHandle,hdr,sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
        {
            DEBUG3("�o�b�t�@ %x �ُ͈�ȃf�[�^���󂯎��܂���.",hdr);
            TryProc("WriteNext4",waveOutUnprepareHeader(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
            return ; //�����ȃf�[�^�������Ă����̂Ŗ���.
        }
        //�o�b�t�@�Ƀf�[�^�����[�h���܂�����
        BufUsing++;
//        DEBUG3("�o�b�t�@ %x �������� %d",(UINT)hdr,BufUsing);
    }
    else
    {
//        DEBUG3("�o�b�t�@ %x �������݃f�[�^�Ȃ�",(UINT)hdr);
        hdr->dwFlags = 0;
        hdr->reserved = 0;
        hdr->dwBufferLength = 0;
    }
}

/***********************************************************/
//�\�񂳂�Ă��� �f�[�^���[�h
/***********************************************************/
int TWavePlayer::WVDDataLoadBuf(WAVEHDR *hdr)
{
    int write_buffer_len;

    if (PlayingList.empty())
    {   //���ׂĂ̗\�����������.
//        DEBUG("�Đ�����f�[�^�͂���܂���.");
        return 0;
    }
    //����
    TryProc("WVDDataLoadBuf",waveOutPrepareHeader(hWaveOutHandle,hdr,sizeof(WAVEHDR)) );
    //�f�[�^���[�h���s
    write_buffer_len = WVDDataLoadBuf_WaveAmp(hdr);
    //�Ō�܂ōĐ������f�[�^�̔j��
    WVDDataLoadBuf_EndPlayDel();
    return write_buffer_len;
}

/***********************************************************/
//�\�񂳂�Ă��� �f�[�^���[�h�����s
/***********************************************************/
int TWavePlayer::WVDDataLoadBuf_WaveAmp(WAVEHDR *hdr)
{
	int i;
	short WaveMixserBuf[WAVE_BUFFER_SIZE] ;
	char  WaveMixserDiv[WAVE_BUFFER_SIZE] ; 
    std::list<PlayingWaveList*>::iterator it;
	PlayingWaveList *lpPWL;
    DWORD len;
	DWORD MostLongest = 0;	//�g�p����o�b�t�@�̒���
    DWORD FSize ;//�Đ�����f�[�^�̃T�C�Y(�e���|�����ł�.)
    //�o�b�t�@�N���A
	ZeroMemory(WaveMixserBuf,WAVE_BUFFER_SIZE * sizeof(short) );
	ZeroMemory(WaveMixserDiv,WAVE_BUFFER_SIZE * sizeof(char) );
    for(it = PlayingList.begin() ; it != PlayingList.end() ; it ++)
    {
		lpPWL = (*it);
        FSize = lpPWL->wvd->FSize; 
        if( lpPWL->Offset + WAVE_BUFFER_SIZE > FSize)
            len = FSize - lpPWL->Offset;
        else
            len = WAVE_BUFFER_SIZE;
		//�o�b�t�@�̍Œ��ύX
		if (MostLongest < len) MostLongest = len;
        //�f�[�^�������Ȃ���]�����Ă����܂�.
		for(DWORD i=0; i < len ; i++)
		{
			WaveMixserBuf[i] += lpPWL->wvd->wv[lpPWL->Offset+i];
			WaveMixserDiv[i]++;
		}
        lpPWL->Offset += len;
//        DEBUG3("�o�b�t�@ %x �Đ��w�� %x ���s",hdr,(UINT)(*it) );
    }
	
	//�݌v�����f�[�^�̕��ϒl�����Ƃ߂܂�.
	for(i=0; i < MostLongest ; i++)
	{
		hdr->lpData[i] = (char) (WaveMixserBuf[i] / WaveMixserDiv[i]);
	}
	//�o�͂���f�[�^�͍ō��̒����̂�ł�.
    hdr->dwBufferLength = MostLongest;
    return MostLongest;
}

/***********************************************************/
//�\�񂳂�Ă����f�[�^�̒��ōŌ�܂ōĐ��������c��\�񂩂�폜
/***********************************************************/
void TWavePlayer::WVDDataLoadBuf_EndPlayDel()
{
    std::list<PlayingWaveList*>::iterator it_temp;
    std::list<PlayingWaveList*>::iterator it;
    //�Ō�܂ōĐ������f�[�^�̔j��
    for(it = PlayingList.begin() ; it != PlayingList.end() ;)
    {
        it_temp = it;
        it ++;
        if ( (*it_temp)->Offset == (*it_temp)->wvd->FSize) 
        {   //���̃f�[�^���Đ�����ȃ��X�g���疕��.
//            DEBUG3("�Đ��w�� %x �����܂�",(UINT)(*it_temp) );
            delete (*it_temp) ;
            PlayingList.erase(it_temp);
        }
    }
}


/***********************************************************/
//���ׂĔj��
/***********************************************************/
void TWavePlayer::All_Breaker()
{
    std::list<PlayingWaveList*>::iterator it;
    //�X�g�b�v
    TryProc("All",waveOutReset(hWaveOutHandle) );
    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = PlayingList.begin() ; it != PlayingList.end() ; it ++)
    {
        delete (*it);
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    PlayingList.clear();
    //�v�[�����
    Pool = 0;
}

/***********************************************************/
//�v�[���������r�����čĐ�������.
/***********************************************************/
void TWavePlayer::Ref()
{
    //�f�o�C�X�I�[�v���Ɏ��s���Ă���Ζv
    if (!WavePlayOk) return ;
    //�o�b�t�@���[���ŁA
    //�v�[������Ă���l������Ȃ��....
    if (Pool > 0 && BufUsing == 0)   FirstPlay();
}

void TryProc(char *p,MMRESULT rc)
{
    if(rc!= MMSYSERR_NOERROR){
        char msg[255];
        waveOutGetErrorText(rc,msg,sizeof(msg));
        DEBUG3("%s ���Ł@%s ",p,msg);
    }
}

#endif //LIFE END
