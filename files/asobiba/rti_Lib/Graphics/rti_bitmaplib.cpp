#include "StartWithExeFile.h"
#ifdef BITMAPLIB_LIFE

/*****************************************************************/
/*****************************************************************/
/*    �n�C�p�[�r�b�g�}�b�v�@���C�u����         */
/*****************************************************************/
/*****************************************************************/
#include <windows.h>
#include <vector>
#include <algorithm>
#include <.\Graphics\rti_bitmaplib.h>
#include <.\Graphics\rti_graphics.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\VCL\rti_vcl_apprication.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_debug.h>
#include <.\Comm\rti_Comm.h>

TBmpChar *BmpChar;

/**********************************************************/
// �L�����N�^�[���쐬����
/**********************************************************/
struct BMP_DATA_TABLE* TBmpChar::Make(const char *filename)
{
	struct BMP_DATA_TABLE *puf;
    //�Ƃ肠�����A������.
    puf = NULL;
    try
    {
	    //�V�K�ɍ쐬���܂�
        puf = new (struct BMP_DATA_TABLE) ;//�������m��
        if (!puf) throw (MemoryDeficient("TBmpChar::Make","BMD�̈�m��",sizeof(BMP_DATA_TABLE) ) );
    
        if ( !CommDraw->GraphicsMake(puf,filename) )
			throw (FriendlyError("TBmpChar::Make","�O���t�B�b�N�X�ǂݍ���","�t�@�C���ǂݍ��݂Ɏ��s",""));

        //���X�g�ɒǉ�
        BmdList.push_back(puf);
    }
    catch(...)
    {
        delete puf; //NULL �� delete ���Ă����v.
	   	return NULL;
    }
	return puf;
}


/**********************************************************/
// �f�X�g���N�^
/**********************************************************/
TBmpChar::~TBmpChar()
{
	Clear();
}

/**********************************************************/
// ���ׂẴL�����N�^�[��j������
/**********************************************************/
void TBmpChar::Clear()
{
    std::vector<BMD>::iterator it;

    //���ׂẴ��X�g�ɃA�N�Z�X���ĉ������.
    for(it = BmdList.begin() ; it != BmdList.end() ; it ++)
    {
		CommDraw->GraphicsFree((*it));
        delete (*it);
    }
    //���X�g�̂��ׂẴf�[�^�̔j��
    BmdList.clear();
}

/**********************************************************/
// �C�ӂ̃L�����N�^�[��j������
/**********************************************************/
void TBmpChar::Breaker(struct BMP_DATA_TABLE *puf)
{
    std::vector<BMD>::iterator it =
        std::find(BmdList.begin(),BmdList.end(),puf);
    if(it != BmdList.end())
    {
	        CommDraw->GraphicsFree(puf);
        	delete puf;
            BmdList.erase(it);
    }
}

/**********************************************************/
// �Ȃɂ��\������Ȃ��L�����N�^�[�����
/**********************************************************/
BMD TBmpChar::New(int sx,int sy)
{
    int x,y;
	struct BMP_DATA_TABLE *puf;
    //�Ƃ肠�����A������.
	//�T�C�Y�� 0 �������� Sprite �̑傫��
    if (!sx) x = Sprite->w;
    else     x = sx;
    if (!sy) y = Sprite->h;
    else     y = sy;

    puf = NULL;
    try
    {
    	//�V�K�ɍ쐬���܂�
        puf = new (struct BMP_DATA_TABLE) ;//�������m��
        if (!puf) throw (MemoryDeficient("TBmpChar::New","BMD�̈�m��",sizeof(BMP_DATA_TABLE) ) );

		puf->w = x ;		puf->h = y ;
        if (!CommDraw->GraphicsAlloc(puf) )
			throw (FriendlyError("TBmpChar::New","�V�K�̈�쐬","�V�K�̈���m�ۂ��郁����������܂���","���������󂯂Ă�������"));
        //���X�g�ɒǉ�
        BmdList.push_back(puf);
    }
    catch(...)
    {
        delete puf; //NULL �� delete ���Ă����v.
	   	return NULL;
    }
	return puf;
}

/**********************************************************/
// �R�s�[�����
/**********************************************************/
BMD TBmpChar::Copy(BMD bmd)
{
    BMD nbmd = New(bmd->w,bmd->h);
    if (nbmd == NULL)  return NULL;

    memcpy(nbmd->bm,bmd->bm,(nbmd->w*nbmd->h)*3);

	return nbmd;
}



#endif //LIFE END
