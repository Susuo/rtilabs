#include "StartWithExeFile.h"
#ifdef GAME_ANIME_LIFE

#include <.\Game\rti_Game_AnimeBankForFile.h>


TGameAnimeBankForFile GameAnimeBankForFile;

#define DEFINE_ANIME_BMD      "DEFINE_ANIME_BMD"        //��`�J�n
#define DEFINE_ANIME_PATH     "DEFINE_ANIME_PATH"       //�p�X
#define DEFINE_ANIME_RECT     "DEFINE_ANIME_RECT"       //���̊G�̂ǂ����g���̂�.
#define DEFINE_DDESHAPE       "DEFINE_DDESHAPE"         //�G�̋��L
#define DEFINE_ANIME_END      "DEFINE_ANIME_END"        //��`�I��
/*
    �A�j���[�V�����f�[�^�̒�`.
DEFINE_ANIME_BMD                //�K�{
DEFINE_ANIME_PATH   char*       //�K�{
DEFINE_ANIME_RECT   int int int int //X,Y,W,H
DEFINE_ANIME_RECT   int int int int //X,Y,W,H
DEFINE_ANIME_RECT   int int int int //X,Y,W,H
DEFINE_ANIME_RECT   int int int int //X,Y,W,H

  //RECT ���A�j���p�^�[�����쐬����. 1 �` 5.

*/
#include <.\Base\rti_data_list.h>
#include <.\Base\rti_error_message.h>
/*******************************************************************/
//�A�j���[�V�����f�[�^���t�@�C������ǂݍ���
/*******************************************************************/
bool TGameAnimeBankForFile::Make(char *filename)
{
    TCommandList TCL;
    char *CL[] = 
    {      
        DEFINE_ANIME_BMD,DEFINE_ANIME_PATH,DEFINE_ANIME_RECT,DEFINE_ANIME_END, NULL,
    };
    TCL.set(CL);

    int  AnimeSrcNum;   //�A�j���\�[�X�̌��ݎg���Ă��鐔.
    BMD  UseBmd;        //�G
    Anime_Src_struct AS[5];     //�A�j���\�[�X.

    FILE *fp;           //�t�@�C���|�C���^
    int x,y,w,h;        //�l��
    int ddshape_no;     //�G�̃f�[�^���L�̏ꍇ
    char *NextChar;
    char buf[MAX_PATH]; //�t�@�C����ǂݍ��ނƂ��Ɏg���o�b�t�@
    char bmd_filename[MAX_PATH];

    fp = fopen(filename,"rb");
    if (!fp)
    {
        FileNotOpen("TGameAnimeBankForFile::Make",filename);
        return false;
    }

    while( !feof(fp) )
    {
        if ( !fgets(buf,256,fp) ) break;
        NextChar = buf;
        switch (TCL.search(buf,&NextChar) )
        {
        case -1:    //�G���[
            break;
        case 0:     //DEFINE_ANIME_BMD
            //�o�^���̃N���A.
            AnimeSrcNum = 0;
            break;
        case 1:
            sscanf(NextChar,"  %s    %d",bmd_filename,&ddshape_no);
            if (stricmp(bmd_filename,DEFINE_DDESHAPE) == 0)
            {//�G�̋��L
                UseBmd = AnimeBankBmpChar->At(ddshape_no);
            }
            else
            {
                UseBmd = AnimeBankBmpChar->Make(bmd_filename);
                if ( ! UseBmd)
                {
                    MsgError3("�t�@�C�� %s �ŁA�G %s �̓ǂݍ��݂Ɏ��s���܂���!",filename,bmd_filename);
                    return false;    
                }
            }
            break;
        case 2:
            if (AnimeSrcNum > 4) break;
            sscanf(NextChar,"  %d  %d  %d  %d",&x,&y,&w,&h);
            //�}�C�i�X�̕������́A�摜�̍ő啝�Ƃ���.
            if (w < 0)  w = UseBmd->w;
            if (h < 0)  h = UseBmd->h;
            //�o�^
            AS[AnimeSrcNum].x = x;            AS[AnimeSrcNum].y = y;
            AS[AnimeSrcNum].w = w;            AS[AnimeSrcNum].h = h;
            AS[AnimeSrcNum].bmd = UseBmd;
            AnimeSrcNum++;
            break;
        case 3:     //DEFINE_ANIME_END
            //�O��̃f�[�^�̓o�^�����܂�.
            switch(AnimeSrcNum)
            {
            case 1: New(NULL,&AS[0],NULL,NULL,NULL,NULL);        break;
            case 2: New(NULL,&AS[0],&AS[1],NULL,NULL,NULL);      break;
            case 3: New(NULL,&AS[0],&AS[1],&AS[2],NULL,NULL);    break;
            case 4: New(NULL,&AS[0],&AS[1],&AS[2],&AS[3],NULL);  break;
            case 5: New(NULL,&AS[0],&AS[1],&AS[2],&AS[3],&AS[4]);break;
            }
        }
    }
    fclose(fp);
    return true;
}

#endif //LIFE END
