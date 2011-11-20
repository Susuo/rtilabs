#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE
/*****************************************************************/
//���R�ϊ�
/*****************************************************************/

#include <.\Graphics\rti_graphics_resize.h>

    struct _WorkArea
    {
        Fixed StepX,StepTX,StepTY;
    };
#define WORK_AREA_STEPX	0	//�@��ˑ����܂�.!!
#define WORK_AREA_STEPTX	4	//�@��ˑ����܂�.!!
#define WORK_AREA_STEPTY	8	//�@��ˑ����܂�.!!

#define WORK_AREA_GET(p,lr) ( (p<<1)+lr)
//�e���_�@�� Top Buttom Left Right �̂ǂ�Ȃ̂����߂�
static bool SettingVector(_XYT *XY,_XYT **_Top,_XYT **_Buttom,_XYT **_Left,_XYT **_Right);
//�C�J���Ȃ��l�p�`���ǂ������ׂ�
static bool IsOffenderSquare(const _XYT *Left,const _XYT *Right,
                      const _XYT *Top,const _XYT *Buttom);
//�e���_�Ԃ̑����𒲂ׂ�
static void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E);
//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
static int HokanVectorOffenderSquareRightSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare);
//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
static int HokanVectorOffenderSquareLeftSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare);
//�e���_�Ԃ̑����𒲂ׂ�(�ӂ[�̎l�p�`)
static int HokanVectorNormalSquare(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare);
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.
static int HokanVector(_WorkArea *WA,int *StopY,
       const _XYT *Left,const _XYT *Right,
       const _XYT *Top,const _XYT *Buttom,bool OffenderSquare);
//Y���W�̃N���b�s���O.
static bool YClipping(int &y,int *StopY,BMD const bmd2);
//XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
//�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
// �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
//�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
//�܂��A�]���J�n Y ���W��Ԃ��܂�.
static int XYBrokeringAndSettingWorkArea(_XYT *XY,_WorkArea *WA,
                                    int *StopY,const BMD bmd);

static _WorkArea *lpWA ;			   //���݂� _WorkArea .
static Fixed X1,X2,TX1,TX2,TY1,TY2;    //���]���J�n�ꏊ.
static _WorkArea *WAL,*WAR;			   //���݂̍��E��S�����Ă���_WorkArea.
static int DeltaXYTStart = 0;		   //�X�^�[�g���� _WorkArea �̏ꏊ.
static int DeltaXYTMax;				   //�I������ _WorkArea �̏ꏊ.
/*****************************************************************/
//���R�ϊ� ���C����
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY)
{
    _WorkArea WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
    int *lpStopY;
    register int Temp;		//��Ɨ̈�.
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		

	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,bmd2))	return;

    int AddPitch2;
    BYTE *data2;
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;

    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //���̒�~�ꏊ��
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ���C�������ɊG�悵�Ă����܂�.
        int DrawLen = FI(X2 - X1);
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
            register BYTE *d2;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (StartX < 0)
            {
                StartX = - StartX;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= StartX;    //�]���͈͂��k�߂�. .
                LocalTX += LocalStepTX * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                LocalTY += LocalStepTY * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                d2 = data2 + 0 + 0 + 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
//            Temp = ( Bmd2W - (FI(X1) + DrawLen) );
//            if (Temp < 0 )  DrawLen+=Temp;  //(Temp�̓}�C�i�X�ł�(^^;; )
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //(Tem            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                DirectPSet(d2, SeekBmAddressSp(bmd1,FI(LocalTX),FI(LocalTY) ) );
//                DirectPSet(d2, SeekBmAddress(bmd1,FI(LocalTX),     FI(LocalTY) ) );
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;
            }
        }

        X1 += WAL->StepX;        X2 += WAR->StepX;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
    }
}
/*****************************************************************/
//���R�l�p�`�n�b�`���O.
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb)
{
    _WorkArea WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
    int *lpStopY;
    register int Temp;		//��Ɨ̈�.
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		

	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,bmd2))	return;

    int AddPitch2;
    BYTE *data2;
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;

    for(lpStopY = &StopY[DeltaXYTStart] ; ; y ++ )
    {
        if(y >= *lpStopY)
        {
            if (DeltaXYTStart == DeltaXYTMax) break;
            DeltaXYTStart++;
            lpStopY++;  //���̒�~�ꏊ��
            lpWA+=2;
            WAL = lpWA;
            WAR = (lpWA+1);
        }
        //X ���C�������ɊG�悵�Ă����܂�.
        int DrawLen = FI(X2 - X1);
        if (DrawLen > 0)
        {
            //�e�N�X�`�������f���鏀��
            register BYTE *d2;
            Temp = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (Temp < 0)
            {
                Temp = - Temp;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= Temp;    //�]���͈͂��k�߂�. .
                d2 = data2 + 0 + 0 + 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (Temp+Temp+Temp); //( * 3 �͑����Z�ő�p.)
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (FI(X1) + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //(Temp�̓}�C�i�X�ł�(^^;; )
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                DirectPSet(d2, Rgb );
                d2 += 3;
            }
        }

        X1 += WAL->StepX;        X2 += WAR->StepX;
        data2 += AddPitch2;
    }
}

/*****************************************************************/
//�e���_�@�� Top Buttom Left Right �̂ǂ�Ȃ̂����߂�
/*****************************************************************/
bool SettingVector(_XYT *XY,_XYT **_Top,_XYT **_Buttom,_XYT **_Left,_XYT **_Right)
{
    /*
    *     T          T                  R
    *    /\          |------------------|
    *   /  \         |                  |
    *  /    / R      |                  |
    *L/    /         |                  |
    *  \  /         L--------------------B  
    *   \/
    *   B
    *   ��̂悤�ɂȂ�܂�.
    * T �Ƃ́A y ���ŏ��@�@y == y ����������Ax ����������.
    */
    _XYT *Left,*Right,*Top,*Buttom;
    _XYT *Temp;
    int i;
    int BackupI1,BackupI2;
    int OkFlg = 0;
    bool OffenderSquare = false;
    //Top �����߂�.
    BackupI1 = BackupI2 = 0;
    Buttom = Top = &XY[0];
    for(i = 0; i < 4; i++)
    {
        Temp = &XY[i];
        if ( Temp->y < Top->y  ||
                    ( Temp->y == Top->y && Temp->x < Top->x))
        {
            Top = Temp;   BackupI1 = i;
        }
        if (Temp->y > Buttom->y  || 
                    ( Temp->y == Buttom->y && Temp->x > Buttom->x))
        {
            Buttom = Temp;    BackupI2 = i;
        }
    }
    //Top �̏ꏊ���L�^
    SETBIT(OkFlg,BackupI1);
    //Buttom �̏ꏊ���L�^
    SETBIT(OkFlg,BackupI2);
    Left = NULL;
    //Top Buttom �ȊO�� 2����A Left Right �����߂�.
    for(i = 0; i < 4; i++)
    {
        if (! GETBIT(OkFlg,i) )
        {
            Temp = &XY[i];
            //�c��͂ǂ��炩�����Ȃ��̂ŁA�Е��ł͂Ȃ�������@�����Е��ł�.
            if (Left==NULL)
            {
                Left = Temp;
            }
            else
            {
                Right = Temp;
                break;  //Left , Right �����܂����̂Ł@����ȏ�̃��[�v���s�͖��Ӗ�.
            }
        }
    }

    if (Left->x > Right->x)
    {   //����
        Temp = Left ;   Left = Right;   Right = Temp;
    }
/*
    if ( (Top->x > Left->x && Top->x > Right->x
                && Buttom->x > Left->x && Buttom->x > Right->x) ||
         (Top->x < Left->x && Top->x < Right->x
                && Buttom->x < Left->x && Buttom->x < Right->x))
*/
    if ( IsOffenderSquare(Left,Right,Top,Buttom) )
    {   
        /*      
        *    T
        *    |\
        *    | \        ����ȁA�H�`�ɂȂ��Ă��Ȃ��}�`�́A
        *    |  \ L     �C���M�����[(�C���O���b�V���ł����Ɗi�D�ǂ��ł���)
        *    |   |      �ȁA�}�`�Ƃ��ĕʓr�̏��������܂�.
        *    |   |      �܂��A���̂Ƃ��̐}�`�� L R �́A    
        *    |  / R     ���Ȃ炸�A Left->y > Right->y �łȂ��ẮA
        *    | /        �Ȃ�܂ւ�.
        *    |/
        *    B
        */
        OffenderSquare = true;  //��̐}�̂悤�Ȏl�p�`�ł�.
        if (Left->y > Right->y)
        {
            Temp = Left ;   Left = Right;   Right = Temp;
        }
    }

    *_Top = Top;
    *_Buttom = Buttom;
    *_Left = Left;
    *_Right = Right;
    //�C�J���Ȃ��l�p�`���ǂ�����Ԃ��̂ł�.
    return OffenderSquare;
}
 
/*****************************************************************/
//�C�J���Ȃ��l�p�`���ǂ������ׂ�
/*****************************************************************/
bool IsOffenderSquare(const _XYT *Left,const _XYT *Right,
                      const _XYT *Top,const _XYT *Buttom)
{
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  ����Ȋ����̐}�`
        *    |   |          ���A�ǂ������ׂ�.
        *    |  / R
        *    | /
        *    |/
        *    B
        */
    /*
        ���ʃA���S���Y��.
            ��̂悤�Ȑ}�`�̏ꍇ�A ��ʂ� ��TB �ɓ�ɕ��������Ƃ���ƁA
            �_ L , R �� ��TB �́@�E���@���@�����̂ǂ��炩�ɗ����Ƃ���
            ������܂�.
            ���ꂪ�A�H�`�ȂǂɂȂ�ƁA �_ L , R �́A ��TB ��
            �E�ƍ��ɕ������܂�.
            ���̓������g�����Ƃɂ���āA���ʂ��Ă��܂�.
    */
    if (Top->x != Buttom->x)
    {
        int Rpos ;
        int Lpos ;
        Rpos = (Top->y - Buttom->y) * 
            (Right->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;
        Lpos = (Top->y - Buttom->y) *
            (Left->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;

        if ( (Lpos < Left->y && Rpos < Right->y) ||
               (Lpos > Left->y && Rpos > Right->y) )    return true;
    }
    else
    {
        if ( (Top->x < Left->x && Top->x < Right->x) ||
               (Top->x > Left->x && Top->x > Right->x) )    return true;
    }
    //�ӂ[�̎l�p�`.
    return false;
}

/*****************************************************************/
//�e���_�Ԃ̑����𒲂ׂ�
/*****************************************************************/
int HokanVector(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
    //�K��ᔽ�Ȑ}�`����?
    //�K��ᔽ�̐}�`�́A���̐}�݂����Ȃ��.
    //�H�`�Ⓑ���`����Ȃ��^�C�v�ł�.
    if ( OffenderSquare )
    {
        if (Buttom->x < Right->x)
        {
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  ����Ȋ����̐}�`
        *    |   |          �́A���ʂȃ��[�`�����K�v�ł�(��)
        *    |  / R
        *    | /
        *    |/
        *    B
        */
            return HokanVectorOffenderSquareRightSmarll
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
        }
        else
        {
        /*      
        *       T
        *      /|
        *     / |
        *  L /  | 
        *    |  |  ����Ȋ����̐}�`
        *    |  |          �́A���ʂȃ��[�`�����K�v�ł�(��)
        *    |  | 
        *  R \  |
        *     \ |
        *      \|
        *       B
        */
            return HokanVectorOffenderSquareLeftSmarll
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
        }
    }
    else
    {
        //�ʏ�́@�����`�A�����`�A�@�H�`�A��`�Ȃ�.
        return HokanVectorNormalSquare
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
    }
}

/*****************************************************************/
//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
/*****************************************************************/
int HokanVectorOffenderSquareRightSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
        /*      
        *    T
        *    |\
        *    | \ 
        *    |  \ L
        *    |   |  ����Ȋ����̐}�`
        *    |   |          �́A���ʂȃ��[�`�����K�v�ł�(��)
        *    |  / R
        *    | /
        *    |/
        *    B
        */
            //T -> L �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Left);
            //T -> B �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Buttom);

            //���X�g�b�v�|�C���g
            StopY[0] = Left->y;    //L �� Y �ň�x��
            if (Left->y == Buttom->y)   return 0;   //L �� B �����������������炨���܂�.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,1)],Left,Right);  //WaR = L => R �܂�
                //WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL �́A�O��̑��������s����.

                //���X�g�b�v�|�C���g
                StopY[0] = Right->y;     //R �� Y �ň�x��
                if (Right->y == Buttom->y)   return 0;   //R �� B �����������������炨���܂�.
                HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B �܂�
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];               //WaL �́A�O��̑��������s����.
                //��O�X�g�b�v�|�C���g
                StopY[1] = Buttom->y;     //B �� Y �ň�x��
                return 1;
            }
            else
            {
                if (Left->y == Buttom->y)   return 0;   //L �� B �����������������炨���܂�.
                HokanLine(&WA[WORK_AREA_GET(1,1)],Left,Right);  //WaR = L => R �܂�
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL �́A�O��̑��������s����.

                //���X�g�b�v�|�C���g
                StopY[1] = Right->y;     //R �� Y �ň�x��
                if (Right->y == Buttom->y)   return 1;   //R �� B �����������������炨���܂�.
                HokanLine(&WA[WORK_AREA_GET(2,1)],Right,Buttom); //WaR = R => B �܂�
                WA[WORK_AREA_GET(2,0)] = WA[WORK_AREA_GET(1,0)];               //WaL �́A�O��̑��������s����.
                //��O�X�g�b�v�|�C���g
                StopY[2] = Buttom->y;     //B �� Y �ň�x��
                return 2;
            }
}

/*****************************************************************/
//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
/*****************************************************************/
int HokanVectorOffenderSquareLeftSmarll(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
        /*      
        *       T
        *      /|
        *     / |
        *  L /  | 
        *    |  |  ����Ȋ����̐}�`
        *    |  |          �́A���ʂȃ��[�`�����K�v�ł�(��)
        *    |  | 
        *  R \  |
        *     \ |
        *      \|
        *       B
        */
            //T -> L �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Left);
            //T -> B �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Buttom);
            //���X�g�b�v�|�C���g
            StopY[0] = Left->y;    //L �� Y �ň�x��
            if (Left->y == Buttom->y)   return 0;   //L �� B �����������������炨���܂�.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,0)],Left,Right);  //WaR = L => R �܂�
                //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaL �́A�O��̑��������s����.

                //���X�g�b�v�|�C���g
                StopY[0] = Right->y;     //R �� Y �ň�x��
                if (Right->y == Buttom->y)   return 0;   //R �� B �����������������炨���܂�.
                HokanLine(&WA[WORK_AREA_GET(1,0)],Right,Buttom); //WaR = R => B �܂�
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];               //WaL �́A�O��̑��������s����.
                //��O�X�g�b�v�|�C���g
                StopY[1] = Buttom->y;     //B �� Y �ň�x��
                return 1;
            }
            else
            {
                HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Right);  //WaR = L => R �܂�
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaL �́A�O��̑��������s����.

                //���X�g�b�v�|�C���g
                StopY[1] = Right->y;     //R �� Y �ň�x��
                if (Right->y == Buttom->y)   return 1;   //R �� B �����������������炨���܂�.
                HokanLine(&WA[WORK_AREA_GET(2,0)],Right,Buttom); //WaR = R => B �܂�
                WA[WORK_AREA_GET(2,1)] = WA[WORK_AREA_GET(1,1)];               //WaL �́A�O��̑��������s����.
                //��O�X�g�b�v�|�C���g
                StopY[2] = Buttom->y;     //B �� Y �ň�x��
                return 2;
            }
}
/*****************************************************************/
//�e���_�Ԃ̑����𒲂ׂ�(�ӂ[�̎l�p�`)
/*****************************************************************/
int HokanVectorNormalSquare(_WorkArea *WA,int *StopY,
                            const _XYT *Left,const _XYT *Right,
                            const _XYT *Top,const _XYT *Buttom,
                            bool OffenderSquare)
{
    //T -> L �ɂ���.
    HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Left);
    //T -> R �ɂ���.
    HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Right);
    //���X�g�b�v�|�C���g
    if (Right->y < Left->y)
    {
        StopY[0] = Right->y;    //R �� Y �ň�x��
        if (Right->y == Buttom->y)   return 0;   //R �� B �����������������炨���܂�.
		if (Top->y == StopY[0])
		{	//��̕ӂ����������ɂ���l�p�`�������ꍇ
			HokanLine(&WA[WORK_AREA_GET(0,1)],Right,Buttom);  //WaR = R => B �܂�
			//WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL �́A�O��̑��������s����.

			StopY[0] = Left->y;     //L �� Y �ň�x��
	        if (Left->y == Buttom->y)   return 0;   //L �� B �����������������炨���܂�.
		    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B �܂�
			WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];               //WaR �́A�O��̑��������s����.
			StopY[1] = Buttom->y;
			return 1;
		}
		else
		{
			HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom);  //WaR = R => B �܂�
			WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL �́A�O��̑��������s����.

			StopY[1] = Left->y;     //L �� Y �ň�x��
	        if (Left->y == Buttom->y)   return 1;   //L �� B �����������������炨���܂�.
		    HokanLine(&WA[WORK_AREA_GET(2,0)],Left,Buttom);  //WaL = L => B �܂�
			WA[WORK_AREA_GET(2,1)] = WA[WORK_AREA_GET(1,1)];               //WaR �́A�O��̑��������s����.
			StopY[2] = Buttom->y;
			return 2;
		}
    }
    else
    {
        StopY[0] = Left->y;    //L �� Y �ň�x��
        if (Left->y == Buttom->y)   return 0;   //L �� B �����������������炨���܂�.
		if (Top->y == StopY[0])
		{	//��̕ӂ����������ɂ���l�p�`�������ꍇ
	        HokanLine(&WA[WORK_AREA_GET(0,0)],Left,Buttom);  //WaL = L => B �܂�
		    //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaR �́A�O��̑��������s����.

			StopY[0] = Right->y;     //R �� Y �ň�x��
	        if (Right->y == Buttom->y)   return 0;   //L �� B �����������������炨���܂�.
		    HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B �܂�
			WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];               //WaL �́A�O��̑��������s����.

			StopY[1] = Buttom->y;
			return 1;
		}
		else
		{
	        HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B �܂�
		    WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaR �́A�O��̑��������s����.

			StopY[1] = Right->y;     //R �� Y �ň�x��
	        if (Right->y == Buttom->y)   return 1;   //L �� B �����������������炨���܂�.
		    HokanLine(&WA[WORK_AREA_GET(2,1)],Right,Buttom); //WaR = R => B �܂�
			WA[WORK_AREA_GET(2,0)] = WA[WORK_AREA_GET(1,0)];               //WaL �́A�O��̑��������s����.

			StopY[2] = Buttom->y;
			return 2;
		}
    }
}

/*****************************************************************/
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.
/*****************************************************************/
void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E)
{
    int DY ;
    Fixed X,TX,TY;
    if (B->y > E->y)
    {
        DY = B->y - E->y;
        X  = IF(B->x - E->x);
        TX  = IF(B->tx - E->tx);
        TY  = IF(B->ty - E->ty);
    }
    else
    {
        DY = E->y - B->y;
        X  = IF(E->x - B->x);
        TX  = IF(E->tx - B->tx);
        TY  = IF(E->ty - B->ty);
    }
    //�Ƃ肠�����A�@0�@Div ���
    if (DY==0)
    {
        WA->StepX  = X;
        WA->StepTX = TX;
        WA->StepTY = TY;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepTX = TX / DY;
    WA->StepTY = TY / DY;
    return ;
}


//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
bool YClipping(int &Y,int *StopY,BMD const bmd2)
{
	register int Temp;
    int *lpStopY;
	//Y�̓]���J�n�A�h���X�� 0 �������������� (����₱����(;;))
	for(lpStopY = &StopY[0] , DeltaXYTStart = 0 ; Y < 0; DeltaXYTStart ++)
	{
		if (*lpStopY >= 0)
		{	//0 �ȏ�ɂȂ���.
			Temp = (- Y)  ;	// y �̐����� ���̃v���X�� *lpStopY ����H��������.
			//�e�v�f���v���X����.
			X1 += WAL->StepX * Temp;			TX1 += WAL->StepTX * Temp;			TY1 += WAL->StepTY * Temp;
			X2 += WAR->StepX * Temp;			TX2 += WAR->StepTX * Temp;			TY2 += WAR->StepTY * Temp;
			Y = 0;
			break;
		}
		else
		{	//�܂� 0 �ɓ��B���Ȃ��ꍇ....
			if (DeltaXYTStart == DeltaXYTMax) return false; //���ׂă}�C�i�X�ɓ˂�����ł���.
			Temp = (-Y) + *lpStopY ;	// �������]���� �X�g�b�v Y �̒l���A�e�v�f���v���X���܂�.
			//�e�v�f���v���X����.
			X1 += WAL->StepX * Temp;			TX1 += WAL->StepTX * Temp;			TY1 += WAL->StepTY * Temp;
			X2 += WAR->StepX * Temp;			TX2 += WAR->StepTX * Temp;			TY2 += WAR->StepTY * Temp;
			Y = *lpStopY;
		}
        lpStopY++;  //���̒�~�ꏊ��
        lpWA+=2;
        WAL = lpWA;
	    WAR = (lpWA+1);
	}
    
    //Y���W�̃I�[�o�[�]���̖h�~
    for(lpStopY = &StopY[DeltaXYTStart] , Temp = DeltaXYTStart;Temp <= DeltaXYTMax;lpStopY++ , Temp++)
    {
        if(*lpStopY > bmd2->h)
        {
            if (Temp == 0) return false;    //�����Ƃ��オ�Œ჉�C���ȉ��A�]������K�v�Ȃ�.
            DeltaXYTMax = Temp;
            *lpStopY = (bmd2->h - 1);  //�͂� ������.
            break; 
        }
    }
	return true;
}

//XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
//�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
// �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
//�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
//�܂��A�]���J�n Y ���W��Ԃ��܂�.
int XYBrokeringAndSettingWorkArea(_XYT *XY,_WorkArea *WA,
                                    int *StopY,const BMD bmd)
{
    _XYT *Left,*Right,*Top,*Buttom;
    bool OffenderSquare;

    //�l���̍��W������.
    OffenderSquare = SettingVector(XY,&Top,&Buttom,&Left,&Right);
    if ( (Top->y < 0 && Buttom->y < 0) || (Top->y >= bmd->h && Buttom->y >= bmd->h))
        return bmd->h;
    /*
    *     T          L                  T
    *    /\          |------------------|
    *   /  \         |                  |
    *  /    / R      |                  |
    *L/    /         |                  |
    *  \  /         B--------------------R  
    *   \/
    *   B
    * ��{�I�Ȑ}�`�̕H�`�́A
    * T.y �`�@B.y �܂ł̈ړ��ɂȂ邪�A
    * T.y �`�@R.y    0��ړ]��
    * R.y �`�@L.y    1��ړ]��
    * L.y �`�@B.y    2��ړ]��
    * �̍ő�@3 ��]���ɂ킯�ď�������.
    */
    DeltaXYTStart = 0;				//������A�ŏ��Ɏg�����[�N�G���A�͍ŏ��ł�.
    DeltaXYTMax = HokanVector(WA,StopY,
                    Left,Right,Top,Buttom,OffenderSquare);
    //�X�^�[�g�ʒu�̌���.
    //X1 TX1 TY1 X2 TX2 TY2 �̓X�^�e�B�b�N�ȃO���[�o���ϐ��ł��̂Œ���.
    _XYT *SL,*SR;                   //�X�^�[�g�n�_
    //�X�^�[�g�n�_�ݒ�
    SL = SR = Top;
    if(Top->y == Right->y) SR = Right; //�Y��Ȏl�p�`�� SR �� Right����ł�.
//    else if (Top->y == Left->y) SR = Left; //���ʂ��Y��Ȏl�p�`�� SR �� Left����ł�.
    X1 = IF(SL->x);            X2 = IF(SR->x);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
    //�]���J�n Y ���W.
    return Top->y;
}

#endif //LIFE END
