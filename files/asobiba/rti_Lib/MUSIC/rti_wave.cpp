#include "StartWithExeFile.h"
#ifdef WAVE_SOUND_LIFE
/***********************************************************/
//�E�F�[�u�t�@�C���ǂݍ���
/***********************************************************/

#include <.\Music\rti_wave.h>
#include <.\Base\rti_error_message.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

TWaveLib *WaveChar;

/***********************************************************/
//�R���X�g���N�^
/***********************************************************/
TWaveLib::TWaveLib()
{
}

/***********************************************************/
//�f�X�g���N�^
/***********************************************************/
TWaveLib::~TWaveLib()
{
    All_Breaker();
}

#define WAVE_FILE_READ_ERROR_NO_FILE 0
#define WAVE_FILE_READ_ERROR_NO_RIFF 1
#define WAVE_FILE_READ_ERROR_NO_WAVE 2
#define WAVE_FILE_READ_ERROR_NO_DATA 3
#define WAVE_FILE_READ_ERROR_NO_READ 4

/***********************************************************/
//���̃t�@�C������ Wave ��ǂݍ���
//����: �t�H�[�}�b�g�� 
//22.506kHZ �� 8 �r�b�g�X�e���I�̋ȈȊO����Ȃ�!!!
/***********************************************************/
WVD TWaveLib::Make(char *filename)
{
    //  �`�����N�̍\����
    struct TChunk
    {
        char ID[4];     //�`�����N�̎��
        long Size;     //�`�����N�̃T�C�Y(Windows�炵�����g���G���f�B�A��)
    }chunk;
    char FormType[4];   
    WVD Wvd;
    int FChannels = 2;      //�`�����l��
    int FSampleRate = 22050;    //�T���v�����O���[�V
    int FSampleBits = 8;
    char  *wv = NULL;           //�f�[�^������Ƃ���
    DWORD FSize = 0;         //�f�[�^�T�C�Y

    //�G���[���b�Z�[�W�e�[�u��
    char *ErrorString[]=
    {
        {"�t�@�C�� %s �� ���݂��܂���."},
        {"�t�@�C�� %s �� RIFF �t�H�[�}�b�g�ł͂Ȃ�."},
        {"�t�@�C�� %s �� WAVE �t�H�[�}�b�g�ł͂Ȃ�."},
        {"�t�@�C�� %s �� �������m�ێ��A�܂��̓f�[�^�ǂݍ��ݎ��ɃG���[���������܂���."},
        {"�t�@�C�� %s �� �w�肳�ꂽ�T�C�Y %d �o�C�g �ǂݍ��߂܂���ł���."},
    };

    FILE *fp;
    fp = fopen(filename,"rb");
    if (fp==NULL) 
    {
        //�G���[���b�Z�[�W�̕\��
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_FILE],filename);
        goto FileError_Rtn;   //�G���[����
    }

    //�f�[�^��ǂ݂܂�
    fread(&chunk,8,1,fp);
    //RIFF�̕������m�F
    if (strncmp(chunk.ID,"RIFF",4)!=0)
    {
        //�G���[���b�Z�[�W�̕\��
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_RIFF],filename);
        goto Error_Rtn; //�G���[����
    }

    //�f�[�^��ǂ݂܂�
    fread(FormType,4,1,fp);
    //WAVE �̕������m�F����.
    if (strncmp(FormType,"WAVE",4)!=0)
    {
        //�G���[���b�Z�[�W�̕\��
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_WAVE],filename);
        goto Error_Rtn; //�G���[����
    }

    while( fread(&chunk,sizeof(TChunk),1,fp) == 1)
    {
        if(strncmp(chunk.ID,"fmt ",4)==0)
        {
            WAVEFORMATEX wf;
            fread(&wf,min(long(18),chunk.Size),1,fp);
            FChannels   = wf.nChannels;
            FSampleRate = wf.nSamplesPerSec;
            FSampleBits = wf.wBitsPerSample;
        }
        else
        {
            if(strncmp(chunk.ID,"data",4)==0)
            {
                //�������m�ۂƓǂݍ��݂ŃV���[
                wv    = (char*)malloc(chunk.Size);
                FSize = chunk.Size;
                if (fread(wv,1,FSize,fp) != FSize)
                {
                    //�G���[���b�Z�[�W�̕\��
                    MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_READ],filename,FSize);
                }

                break;
            }
            else fseek(fp,chunk.Size,1);
        }
    }
    if (wv == NULL || FSize == 0)
    {
        //�G���[���b�Z�[�W�̕\��
        MsgError3(ErrorString[WAVE_FILE_READ_ERROR_NO_DATA],filename);
        goto FileError_Rtn;   //�G���[����
    }

    //���̂�����悤�Ȃ̂ŁA������󂯎~�߂鑤�̍쐬
    Wvd = new WAVE_DATA_TABLE;  //�������m��
    Wvd->wv = wv;   //�f�[�^�ł���.
    Wvd->FSize = FSize;   //�f�[�^�T�C�Y�Ȃ̂��[
    WvdList.push_back(Wvd); //���X�g�ɒǉ�.

    fclose(fp);
    return Wvd;
    //�G���[�������[�`��
Error_Rtn:
    if (wv!=NULL) free(wv);
    fclose(fp);
FileError_Rtn:
    return NULL;
}

/***********************************************************/
//���ׂĔj��
/***********************************************************/
void TWaveLib::All_Breaker()
{
    std::list<WVD>::iterator it;
    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = WvdList.begin() ; it != WvdList.end() ; it ++)
    {
        free( (*it)->wv );
        delete (*it);
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    WvdList.clear();
}

/***********************************************************/
//�j��
/***********************************************************/
void TWaveLib::Breaker(WVD puf)
{
    std::list<WVD>::iterator it =
        std::find(WvdList.begin(),WvdList.end(),puf);
    if(it != WvdList.end())
    {
            free ( puf->wv );
        	delete puf;
            WvdList.erase(it);
    }
}


#endif //LIFE END