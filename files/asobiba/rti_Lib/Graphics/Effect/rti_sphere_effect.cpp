/*****************************************************************/
/*****************************************************************/
//�ɍ��W�֌W.       ���� EL ����̈ڐA�ł�(��)
//�ł��A�ɍ��W���Ă��Ă��ł���� (��)
/*****************************************************************/
/*****************************************************************/
#include "StartWithExeFile.h"
#ifdef SPHERE_LIFE

#include <windows.h>
#include <.\Base\rti_debug.h>
#include <.\Base\rti_glookup.h>
#include <.\Graphics\Effect\rti_sphere_effect.h>
#include <.\Comm\rti_Comm.h>
#include <.\Base\rti_error_message.h>
#include <math.h>

/************************************************************/
//�R���X�g���N�^
/************************************************************/
TSphereEffect::TSphereEffect()
{
    m_PointChangeTable = NULL;
}

/************************************************************/
//�f�X�g���N�^
/************************************************************/
TSphereEffect::~TSphereEffect()
{
    Init();
}

/************************************************************/
//������
/************************************************************/
void TSphereEffect::Init()
{
    delete m_PointChangeTable;
}

/************************************************************/
//�X�e�[�^�X�Z�b�g
/************************************************************/
bool TSphereEffect::Set(BMD bmd,BMD OutBmd)
{
    //�e��f�[�^�𒸂��܂�.
    m_bmd    = bmd;
    m_OutBmd = OutBmd;
	return true;
}


/*------------------------------------------------------------------------------*/
/*-                                                                            -*/
/*-  �g���l���ϊ����̐���                                 �� �n�C�J���[��p  -*/
/*-                                                                            -*/
/*-    int Sw     : �X�v���C�g�̂w�T�C�Y                                       -*/
/*-    int Sh     : �X�v���C�g�̂x�T�C�Y                                       -*/
/*-    int Dw     : �`���̂w�T�C�Y ( �`640 )                                 -*/
/*-    int Dh     : �`���̂x�T�C�Y ( �`480 )                                 -*/
/*-    int Cx     : �g���l�������̂w���W                                       -*/
/*-                 �ȗ�/TUNNEL_CENTER = ����                                  -*/
/*-                 0�`                = �C�ӂ̈ʒu                            -*/
/*-    int Cy     : �g���l�������̂x���W                                       -*/
/*-                 �ȗ�/TUNNEL_CENTER = ����                                  -*/
/*-                 0�`                = �C�ӂ̈ʒu                            -*/
/*-    int Size   : �g���l���̑傫�� ( ������ )                                -*/
/*-                 �ȗ�/TUNNEL_DEFAULT_SIZE = 8                               -*/
/*-                 2�`                      = �C�ӂ̑傫��                    -*/
/*-    int Center : �g���l�������̐ݒ�                                         -*/
/*-                 �ȗ� = TUNNEL_NORMAL/����                                  -*/
/*-    int Type   : �g���l���`��̐ݒ�                                         -*/
/*-                 �ȗ� = TUNNEL_TYPE_CIRCLE/�~                               -*/
/*-    int Line   : �g���l���Ȑ��̐ݒ�                                         -*/
/*-                 �ȗ� = TUNNEL_LINE_NORMAL/����                             -*/
/*-                                                                            -*/
/*------------------------------------------------------------------------------*/

/*
bool TSphereEffect::CreateTunnel(int Sw,int Sh,int Dw,int Dh,
						  int Cx,int Cy, int Size,
                          TUNNEL_CENTRAL Center, TUNNEL_SHAPE Type,
                          TUNNEL_CURVE Line)
{
	int x,y;     // �ėp�J�E���^�[
	Fixed f1,f2; // �ėp�ϐ�
	int xx,yy;   // �X�v���C�g�Q�ƈʒu
	int cx,cy;   // �f�B�X�v���C�����ʒu
    Fixed curve; // �g���l���̋Ȑ�
    XY *lpPointChangeTable; //�������m�ۂ����z��̃|�C���^

    //�O�̂��߃������N���A���w��.
    Init();
    //�������m��
    m_PointChangeTable = new XY[Dw * Dh];
    lpPointChangeTable = m_PointChangeTable;

	TunnelSpriteX=Sw;
	TunnelSpriteY=Sh;
	TunnelDisplayX=Dw;
	TunnelDisplayY=Dh;
	// ��ʂ̒����ʒu���擾
	cx=Cx;
	cy=Cy;
    
    //�g���l���̒������w�肳�ꂽ��..
	if (Cx==TUNNEL_CENTER) cx=Dw/2;
	if (Cy==TUNNEL_CENTER) cy=Dh/2;

    // �g���l���̃T�C�Y���擾
	if (Size==TUNNEL_DEFAULT_SIZE) Size=8;
	if (Size<2) Size=2;

    curve = FixedDiv(IF(Sh) , 2* fxtan(PAI_FIXED*256/Size) ) /2;

	// �g���l�������̐ݒ�
	switch (Center)
	{
    case TUNNEL_WIDE_X:     // ���E�Ɋg��
		cy=Dh;		break;
	case TUNNEL_TO_RIGHT:   // �E�ɘc��
		cx=Dw;    	break;
	case TUNNEL_TO_LEFT:    // ���ɘc��
		cx=0;		break;
	}

	// �x�����̏���
	for (y=0;y<Dh;y++)
	{
		// �g���l�������̐ݒ�
		switch (Center)
		{
		case TUNNEL_WIDE_X:         // ���E�Ɋg��
			cy--;				    break;
		case TUNNEL_TO_RIGHT:       // �E�ɘc��
			if (--cx<0) cx=0;	    break;
		case TUNNEL_TO_LEFT:        // ���ɘc��
			if (++cx>=Dw) cx=Dw;    break;
		case TUNNEL_WIDE_Y:         // �㉺�Ɋg��
			cx=Dw;  				break;
    	}

		// �w�����̏���
		for (x=0;x<Dw;x++)
		{
			// �g���l�������̐ݒ�
			if (Center==TUNNEL_WIDE_Y) cx--;

			// �g���l���`��̐ݒ�
			switch (Type)
			{
			case TUNNEL_TYPE_CIRCLE:        //�~
				f1=Float2Fixed(sqrt((x-cx)*(x-cx)+(y-cy)*(y-cy)));
				break;
			case TUNNEL_TYPE_BOX1:          //�l�p
				f1=IF(max(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_BOX2:          //�H�`
				f1=IF(abs(x-cx)+abs(y-cy))/2;
				break;
			case TUNNEL_TYPE_CROSS:         //�\��
				f1=IF(min(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_WALL:          //���E�̕�
				f1=IF(abs(x-cx));
				break;
			case TUNNEL_TYPE_GROUND:        //�㉺�̑�n
				f1=IF(abs(y-cy));
				break;
			case TUNNEL_TYPE_FLASH:         //�t���b�V��
				f1=IF(cx);
    			break;
			}

			// �g���l���Ȑ��̐ݒ�
			switch (Line)
			{
			case TUNNEL_LINE_NORMAL:        // ���� ( ���� )
				f2=curve/32;
				break;
			case TUNNEL_LINE_X:             // �w�Ȑ�
				f2= FixedDiv( curve , (IF(abs(y-cy)+1)/10) );
				break;
			case TUNNEL_LINE_Y:             // �x�Ȑ�
				f2= FixedDiv( curve , (IF(abs(x-cx)+1)/10) );
				break;
			case TUNNEL_LINE_BOX1:          // �l�p�Ȑ�
				f2= curve / max(abs(x-cx),abs(y-cy));
				break;
			case TUNNEL_LINE_BOX2:          // �H�`�Ȑ�
				f2= FixedDiv( curve , IF( min(abs(x-cx),abs(y-cy)) ) / 2 );
				break;
			case TUNNEL_LINE_CROSS:         // �\���Ȑ�
				f2= curve / min(abs(x-cx),abs(y-cy));
				break;
			}

			if (f2<0) f2=-f2;

			// �Ή�����X�v���C�g�ʒu�̎Z�o
			xx=FI( (long)(FixedDiv(IF(Sw),(2*PAI_FIXED))*atan2(y-cy,x-cx)) );
			yy=FI(FixedDiv(f2*(sqrt(cx*cx+cy*cy)-f1),f1));

			// XY���X�v���C�g�͈͓��ɂȂ�悤�ɕ␳
			for (;xx<0;xx+=Sw);
			for (;yy<0;yy+=Sh);
			if (xx>=Sw) xx%=Sw;
			if (yy>=Sh) yy%=Sh;

			// �g���l�����X�g�ɕۑ�
			lpPointChangeTable->x=xx;
			lpPointChangeTable->y=yy;
            //���̗v�f��.(��������āA�e�[�u���S�̂ɃA�N�Z�X���Ă����܂�)
            lpPointChangeTable++;
		}
	}
    return true;
}

*/
bool TSphereEffect::CreateTunnel(int Sw,int Sh,int Dw,int Dh,
						  int Cx,int Cy, int Size,
                          TUNNEL_CENTRAL Center, TUNNEL_SHAPE Type,
                          TUNNEL_CURVE Line)
{
	int x,y;     // �ėp�J�E���^�[
	float f1,f2; // �ėp�ϐ�
	int xx,yy;   // �X�v���C�g�Q�ƈʒu
	int cx,cy;   // �f�B�X�v���C�����ʒu
    float curve; // �g���l���̋Ȑ�
    XY *lpPointChangeTable; //�������m�ۂ����z��̃|�C���^

	if ( !CheckPow2(&TunnelSpritePow2W,Sw) )
	{
		FriendlyError("TSphereEffect::CreateTunnel","�g���l���쐬","�̈�� W�� �� 2 �ׂ̂����łȂ�","�������̂��߂ɉ摜�̉����� 2�ׂ̂����ɂ��Ă�������");
		return false;
	}
	TunnelSpritePow2MaskW = (1 << TunnelSpritePow2W) - 1;	//Clipping�̂��߂̃}�X�N�̍Đ�
	if ( !CheckPow2(&TunnelSpritePow2H,Sh) )
	{
		FriendlyError("TSphereEffect::CreateTunnel","�g���l���쐬","�̈�� H�� �� 2 �ׂ̂����łȂ�","�������̂��߂ɉ摜�̏c���� 2�ׂ̂����ɂ��Ă�������");
		return false;
	}
	TunnelSpritePow2MaskH = (1 << TunnelSpritePow2H) - 1;	//Clipping�̂��߂̃}�X�N�̍Đ�

    //�O�̂��߃������N���A���w��.
    Init();
    //�������m��
    m_PointChangeTable = new XY[Dw * Dh];
    lpPointChangeTable = m_PointChangeTable;

	TunnelDisplayX=Dw;
	TunnelDisplayY=Dh;
	// ��ʂ̒����ʒu���擾
	cx=Cx;
	cy=Cy;
    
    //�g���l���̒������w�肳�ꂽ��..
	if (Cx==TUNNEL_CENTER) cx=Dw/2;
	if (Cy==TUNNEL_CENTER) cy=Dh/2;

    // �g���l���̃T�C�Y���擾
	if (Size==TUNNEL_DEFAULT_SIZE) Size=8;
	if (Size<2) Size=2;

    curve = F(Sh/(F(2)*tan( (PAI/F(Size) )/F(2) )));

	// �g���l�������̐ݒ�
	switch (Center)
	{
    case TUNNEL_WIDE_X:     // ���E�Ɋg��
		cy=Dh;		break;
	case TUNNEL_TO_RIGHT:   // �E�ɘc��
		cx=Dw;    	break;
	case TUNNEL_TO_LEFT:    // ���ɘc��
		cx=0;		break;
	}

	// �x�����̏���
	for (y=0;y<Dh;y++)
	{
		// �g���l�������̐ݒ�
		switch (Center)
		{
		case TUNNEL_WIDE_X:         // ���E�Ɋg��
			cy--;				    break;
		case TUNNEL_TO_RIGHT:       // �E�ɘc��
			if (--cx<0) cx=0;	    break;
		case TUNNEL_TO_LEFT:        // ���ɘc��
			if (++cx>=Dw) cx=Dw;    break;
		case TUNNEL_WIDE_Y:         // �㉺�Ɋg��
			cx=Dw;  				break;
    	}

		// �w�����̏���
		for (x=0;x<Dw;x++)
		{
			// �g���l�������̐ݒ�
			if (Center==TUNNEL_WIDE_Y) cx--;

			// �g���l���`��̐ݒ�
			switch (Type)
			{
			case TUNNEL_TYPE_CIRCLE:        //�~
				f1 = F(hypot( (x-cx) ,(y-cy) ) );
				break;
			case TUNNEL_TYPE_BOX1:          //�l�p
				f1=F(max(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_BOX2:          //�H�`
				f1=F((abs(x-cx)+abs(y-cy))/F(2));
				break;
			case TUNNEL_TYPE_CROSS:         //�\��
				f1=F(min(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_TYPE_WALL:          //���E�̕�
				f1=F(abs(x-cx));
				break;
			case TUNNEL_TYPE_GROUND:        //�㉺�̑�n
				f1=F(abs(y-cy));
				break;
			case TUNNEL_TYPE_FLASH:         //�t���b�V��
				f1=F(cx);
    			break;
			}

			// �g���l���Ȑ��̐ݒ�
			switch (Line)
			{
			case TUNNEL_LINE_NORMAL:        // ���� ( ���� )
				f2=curve/F(32);
				break;
			case TUNNEL_LINE_X:             // �w�Ȑ�
				f2=curve/  F((abs(y-cy)+1)/F(10));
				break;
			case TUNNEL_LINE_Y:             // �x�Ȑ�
				f2=curve/ F((abs(x-cx)+1)/F(10));
				break;
			case TUNNEL_LINE_BOX1:          // �l�p�Ȑ�
				f2=curve/ F(max(abs(x-cx),abs(y-cy)));
				break;
			case TUNNEL_LINE_BOX2:          // �H�`�Ȑ�
				f2=curve/ F((abs(x-cx)+abs(y-cy))/F(2));
				break;
			case TUNNEL_LINE_CROSS:         // �\���Ȑ�
				f2=curve/ F(min(abs(x-cx),abs(y-cy)));
				break;
			}
 
			if (f2<0) f2=-f2;

			// �Ή�����X�v���C�g�ʒu�̎Z�o
			xx=(int)(Sw/(F(2)*PAI)*atan2(y-cy,x-cx));
			yy=(int)(f2*(sqrt(cx*cx+cy*cy)-f1)/f1);

			// XY���X�v���C�g�͈͓��ɂȂ�悤�ɕ␳
			for (;xx<0;xx+=Sw);
			for (;yy<0;yy+=Sh);
			if (xx>=Sw) xx%=Sw;
			if (yy>=Sh) yy%=Sh;

			// �g���l�����X�g�ɕۑ�
			lpPointChangeTable->x=xx;
			lpPointChangeTable->y=yy;
            //���̗v�f��.(��������āA�e�[�u���S�̂ɃA�N�Z�X���Ă����܂�)
            lpPointChangeTable++;
		}
	}
    return true;
}

/************************************************************/
//�G�悾���.
/************************************************************/
bool TSphereEffect::Draw(int ux,int uy , int Rotate , int Dist)
{
    int  AddPitch2;
	int  _TunnelDisplayY,_TunnelDisplayX;
	int  _XPointChangeTablePitch;
	DWORD _RotateDist;
	DWORD _MaskWH;
    BYTE *data1,*data2;
	unsigned char bmd_Pow2W;
    XY *lpPointChangeTable;  //�������m�ۂ����z��̃|�C���^
    
	//m_bmd �̕��� 2 �ׂ̂�������Ȃ��ƃ_��.
	bmd_Pow2W = m_bmd->Pow2W;
	if (bmd_Pow2W == 0)	return false;
	bmd_Pow2W = 16 - bmd_Pow2W;	//16�́A Y �����16�r�b�g�ɂ��邩��.
								//Y �� >> 16 �Ō��ɖ߂��āA���ꂩ�� << bmd_Pow2W ����.
								//�܂�A 16 - bmd_Pow2W �����A>> ����.

	if (bmd_Pow2W < 0) return false;	//65535 �ȏ�̃e�N�X�`�����Ă���񂩂���.

	_RotateDist = (Dist<<16) + Rotate;
	//�]�����摜�� 2 �ׂ̂����ł�. ������A & �ō����N���b�s���O�ł��܂�. ���̒l�̎擾
	_MaskWH = (TunnelSpritePow2MaskH << 16) + TunnelSpritePow2MaskW;
	//�]������̈�.
	_TunnelDisplayX = TunnelDisplayX;
	_TunnelDisplayY = TunnelDisplayY;
	//�摜�ϊ��e�[�u���� Y �� 1 �i�ނƂ��� ������ X �o�C�g(�o�C�g�ł��邱�Ƃɒ���)
	_XPointChangeTablePitch = sizeof(XY) * _TunnelDisplayX;

    //���b�N
	if ( !CommDraw->GraphicsLock(m_bmd) )   	return false;
	if ( !CommDraw->GraphicsLock(m_OutBmd) )	return false;
    //�G��ʒu���擾.
	data1 = m_bmd->bm;
	data2 = SeekData(m_OutBmd,ux,uy, 0,0,&AddPitch2);
    //���W�ϊ��e�[�u��.
    lpPointChangeTable = m_PointChangeTable;
	
	/*
	* 
	*	���[�v�J�E���^�[�̏�����.
	*		�N�\�o�J Intel(�I����Ă�) �� CPU �d�l�ɂ��A
	*		�V�t�g���鐔�� ecx �� cl �𗘗p���܂�.
	*		����ŁA���̂��߂ɁA Y���[�v = ecx   X���[�v = edx �ƂȂ�܂�.
	*		�����ł́A Y = edx  X = ecx �ł����ǂ�. �܂�A����ւ��킯�ł���.
	*	���[�v���̑咍��
	*		���̃��[�v���[�`���� �������̂��߂� ���\��Ȃ����Ƃ����Ă��܂�.
	*		eax ���W�X�^�̏�� 16�r�b�g�� Y    ���� 16 �r�b�g�� X ������܂�.
	*		�܂��A MMX �݂����Ȋ����ŋ^���p�b�N����Ă���킯�ł�.
	*		�����炠������A���g���G���f�B�A���ȋ@���z�肵�č���Ă���̂ŁA
	*		���̃A�[�L�e�N�`�������� PC �ɈڐA���邳���� �v����!!
	*/
    _asm{
        mov ecx,_TunnelDisplayY

        mov edi,lpPointChangeTable  ;�摜�ϊ��e�[�u��
        mov esi,data2				;�]��
    LOOP1:              ;���[�v�J�E���^�� ecx ���g�p��
            push edi
            push esi

			push ecx	//�����ŃV�t�g���g�����߁Aecx ��q�؂��܂�.
			mov  cl,bmd_Pow2W
           
            mov edx,_TunnelDisplayX
        LOOP2:           ;���[�v�J�E���^�� edx ���g�p��
				//�摜�ϊ��e�[�u�����l�̎擾.
				//Gx = lpXPointChangeTable->x + Rotate;
				//Gy = lpXPointChangeTable->y + Dist;
				mov   eax,dword ptr [edi]		//Y / X�Ŏ擾. ���16�r�b�g�� X �A���ʂ� 16�r�b�g
				add	  eax,_RotateDist			//��]��t��
				//�N�b���s���O
				//Gx &= _TunnelSpritePow2MaskW;
				//Gy &= _TunnelSpritePow2MaskH;
				and	  eax,_MaskWH
				//�]���J�n�ʒu���擾
				//Temp = Gx + (Gy<<bmd_Pow2W);
				mov	  ebx,eax
				shr   eax,cl		//Y �́A���16�r�b�g�A��������낵�� >> 16�A�e�N�X�`���̕� << 2PowW ����.
									//�ŁA >> �� << �́A���E�ł���̂ŁA>> cl �����ɂȂ�.
									//�Ƃ�����A cl �ł́A 16 - 2Pow ���A�����Ă܂�.
									//�Ȃ��A���݂̃��[�`���ł́A�e�N�X�`���̕� 65535 �𒴂���Ƃ��������Ȃ�܂����A
									//����ȁA�o�J�ł����e�N�X�`���͂��邱�Ƃ͂Ȃ��ł��傤��.

				and	  ebx,0xffff	//����16�r�b�g�̊m��	�܂�A ebx �� X �ɂȂ�.
				add	  ebx,eax		//Y �� X �̍���.
				//*3
				mov   eax,ebx
				shl	  ebx,1
				add   ebx,eax
				add   ebx,data1

				//�]���J�n
		        //DirectPSet(x_data2,(data1+(Temp+Temp+Temp) ) );
				//(data1+(Temp+Temp+Temp) )

                mov eax,[ebx] ;�������ɂ��������܂�
                mov ebx,[esi]      ;���܂������ʂ�
                and eax,0x00ffffff ;�s�v�ȕ�����؂�̂Ă�.
                and ebx,0xff000000
                or  ebx,eax

                mov [esi],ebx

                add edi,4	//XY�̑���(�@��ˑ�!!)
                add esi,3	
            dec edx
            jnz  LOOP2       ;LOOP2�I�� ECX���

			pop ecx			//������ �V�t�g���g�������ߔq�؂��Ă��� ecx �����ɖ߂��܂�.

            pop esi			//�]����
            pop edi			//���W�ϊ��e�[�u��

            add esi,AddPitch2
            add edi,_XPointChangeTablePitch
        dec ecx             ;LOOP1�I�� EDX���
        jnz  LOOP1
    }

/*
    // �f�[�^�]��
	for (y=0;y<_TunnelDisplayY;y++)
	{
        lpXPointChangeTable = lpPointChangeTable;
        x_data2 = data2;
		for (x=0;x<_TunnelDisplayX;x++)
		{
			Gx = lpXPointChangeTable->x + Rotate;
			Gy = lpXPointChangeTable->y + Dist;
			//�N�b���s���O
			Gx &= _TunnelSpritePow2MaskW;
			Gy &= _TunnelSpritePow2MaskH;
			//�]���J�n�ʒu���擾
			Temp = Gx + (Gy<<bmd_Pow2W);
			//�]���J�n
            DirectPSet(x_data2,(data1+(Temp+Temp+Temp) ) );
			//�o�͑������炷
            x_data2+=3;
			//���̉摜�ϊ��e�[�u����
            lpXPointChangeTable++;
		}
		// �x�����ɉ��Z
		data2+=AddPitch2;
        lpPointChangeTable += _TunnelDisplayX;
	}
*/
    //�A�����b�N
    CommDraw->GraphicsUnLock(m_OutBmd);
	CommDraw->GraphicsUnLock(m_bmd);
 
    return true;
}


#endif
