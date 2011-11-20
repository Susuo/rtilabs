/***********************************************************************/
//�w��𑜓x�ɕύX���郉�C�u����
//    Olginal Pascal Src By �D�c���@����
//    C++                By R.T.I
//     �p�X�J������ڐA���܂��� (^^;;
//     �������A NT �ł͓����Ȃ��炵��.
//     �܂��A�I�C���͂���Ȃ������Ȃ����炢�����Ƃ�(������
/***********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_ChangeDisplay.h>
#include <.\Base\rti_debug.h>

/***********************************************************************/
//�R���X�g���N�^
/***********************************************************************/
TChangeDisplay::TChangeDisplay()
{
    InitializeFlag  = false;
    ChangeSuccess   = false;
}

/***********************************************************************/
//�f�X�g���N�^
/***********************************************************************/
TChangeDisplay::~TChangeDisplay()
{
}

/***********************************************************************/
//������ ���� true  /  ���s false
/***********************************************************************/
bool TChangeDisplay::Init()
{
    HKEY  key;
    unsigned char rbuf[20];
    DWORD size = sizeof(rbuf);
    DWORD size2 = sizeof(rbuf);

    //���W�X�g�����猻�݂̉𑜓x�ƐF����ǂ݂Ƃ�ۑ����Ă����܂�.
    //���ɖ߂����Ƃ��ł���悤�ɂ��邽�߂��˂�
    RestorationBitsPerPal = 0;
    if (RegOpenKey( HKEY_CURRENT_CONFIG,"Display\\Settings",  &key ) 
              == ERROR_SUCCESS)
    {
         //�F���Q�b�g�����[
         if (RegQueryValueEx
            (key, "BitsPerPixel", NULL, NULL,rbuf, &size) ==ERROR_SUCCESS)
         {
               RestorationBitsPerPal = atoi((char*)rbuf);

               //�𑜓x�Q�b�g�����[
               if (RegQueryValueEx
                   (key, "Resolution", NULL, NULL,
                            rbuf, &size2) ==ERROR_SUCCESS)
               {
                   sscanf((char*)rbuf,"%d,%d",&RestorationWidth,&RestorationHeight);
                   InitializeFlag = true;
               }
         }
         //�Ȃ񂶂̂���ׂ��p�ɖ߂�(^^;;
         RegCloseKey( key );
    }
    return InitializeFlag;
}

/***********************************************************************/
//�`�F���W  ���� true  /  ���s false
/***********************************************************************/
bool TChangeDisplay::Change(UINT Width, UINT Height, UINT BitsPerPal)
{
    int i;
    bool CanChange ;
    DEVMODEA dm,dms ;

    if (InitializeFlag == false) 
    {  //�܂�����������Ă��Ȃ��̂ŏ��������ɂ���.
        if (!Init()) return false;
    }

    //�J���[���������ĂȂ�������f�B�t�H���g��.
    if (BitsPerPal == 0 ) BitsPerPal = RestorationBitsPerPal;
    if (BitsPerPal == 0 ) return false;

    //�`�F���W�ł���𑜓x���T�[�`���܂�.
    CanChange = false;
    for (i = 0;EnumDisplaySettings(NULL, i, &dm) ; i++)
    {
        if ( (dm.dmPelsWidth == Width)
                 && (dm.dmPelsHeight == Height)
                 && (dm.dmBitsPerPel == BitsPerPal) )
        {
            //�`�F���W�ł��邩�e�X�g
             if (ChangeDisplaySettings(&dm, CDS_TEST) 
                         == DISP_CHANGE_SUCCESSFUL )
             {
                 dms = dm;
                 CanChange = true;
             }
        }
    }
    //�ύX�ł���𑜓x��������Ȃ������ꍇ�A
    //�O�̂��߁A���ڎw�肳�ꂽ�𑜓x�ɕύX���ėǂ��̂����ڃe�X�g���Ă݂� (^^;

    if (! CanChange)
    {
        dm.dmPelsWidth = Width;
        dm.dmPelsHeight = Height;
	    if (BitsPerPal == 0 ) dm.dmBitsPerPel = RestorationBitsPerPal;
        else				  dm.dmBitsPerPel = BitsPerPal;
        //�`�F���W�ł��邩�e�X�g
        if (ChangeDisplaySettings(&dm, CDS_TEST) 
                         == DISP_CHANGE_SUCCESSFUL )
        {
                 dms = dm;
                 CanChange = true;
        }
    }

    //�`�F���W�ł���𑜓x������������ ?
    if (CanChange)
    {   
        //�`�F���W���܁[��
       ChangeDisplaySettings(&dms, 0);
       ChangeSuccess = true;
    }
    return CanChange;           
}

/***********************************************************************/
//���ɖ߂�  ���� true  /  ���s false
/***********************************************************************/
bool TChangeDisplay::Restoration()
{
    if ( (InitializeFlag == false)  )
    {  //������������Ă��Ȃ��̂� ��Ԃ����ˁ[
       return false;
    }
    if (ChangeSuccess)
    {
         if ( Change(RestorationWidth,
             RestorationHeight,RestorationBitsPerPal) )
         {
              ChangeSuccess = false;
              return true;
         }
    }
    return false;
}
