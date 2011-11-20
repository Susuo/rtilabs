#include "StartWithExeFile.h"
#ifdef GRAPHICS_LIFE

/*****************************************************************/
//���R�ϊ�( N�p�`�̊G�� )  �ō��ɉ����̂ł���܂茩�Ȃ��Ł[
/*****************************************************************/

#include <.\Graphics\rti_graphics_resize.h>
#include <.\Base\rti_debug.h>
#include <.\Comm\rti_Comm.h>
#include <.\Base\rti_glookup.h>

_GRAPHICS_BEGIN //namespace begin

Fixed X1 , X2;                  //���]���J�n�ꏊX
Fixed TX1, TX2;                 //���]���J�n�ꏊ�e�N�X�`���[X
Fixed TY1, TY2;                 //���]���J�n�ꏊ�e�N�X�`���[Y
Fixed Z1 , Z2;                  //���]���J�n�ꏊZ
Fixed CR1, CR2;                 //���]���J�n�ꏊ�O���f�[�V����R
Fixed CG1, CG2;                 //���]���J�n�ꏊ�O���f�[�V����G
Fixed CB1, CB2;                 //���]���J�n�ꏊ�O���f�[�V����B
int DeltaXYTStart;   		    //�X�^�[�g���� _WorkArea �̏ꏊ.
int DeltaXYTMax;				//�I������ _WorkArea �̏ꏊ.


//�e�N�X�`�����w�肵���`�ɕϊ�.
static void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkArea *WA,int *StopY,int StartY);
//���R�G��Ł@�w�肵���}�`���w�肵���F�ŊG��
static void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkArea *WA,int *StopY,int StartY);
//�e�N�X�`�����w�肵���`�ɕϊ�. ZBuffer
static void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);
//���R�G��Ł@�w�肵���}�`���w�肵���F�ŊG�� ZBuffer 
static void _ReSizeBoxMain(BMD bmd2,int Rgb,
                _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);
//�e�N�X�`�����w�肵���`�ɕϊ�. �O���[
static void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);
//���R�G��Ł@�w�肵���}�`���w�肵���F�ŊG�� �O���[
static void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer);


 


/*****************************************************************/
//���R�ϊ�( N�p�`�̊G�� ) �̉��������[�`��
/*****************************************************************/

//�K��ᔽ�� �����Ă����΁A�|�`�̎l�p�`�Ȃ̂ꂷ.
enum enumOFFENDER
{
	OFFENDER_NONE ,
	OFFENDER_LEFT ,
	OFFENDER_RIGHT ,
};
/* *******************************************************
*       �d�v�Ȑ���!!!
*  *******************************************************
*
* ���̃��[�`���́A�l�p�A�O�p�`���G�悷�邽�߂ɍ���܂������A
* ������[�b�g�o�b�t�@�t���ŊG��A�t���b�g�V�F�[�f�B���O�A
* �O���[�V�F�[�f�B���O�ŊG��̃��[�`�����v���X���܂�.
* ���̂��߃e���v���[�g�𓱓����āA�R�[�f�B���O�ʂ����炵�܂�.
*
*  template<class _COMMXYT>
*  template<class _COMMWORKAREA>
*
*  �ʏ�         XYT     _WorkArea
*  ZBuffer      XYZT    _WorkAreaZ
*  �t���b�g     XYZT    _WorkAreaZ
*  �O���[       XYZCT   _WorkAreaG
*/

#define WORK_AREA_GET(p,lr) ( (p<<1)+lr)
//XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
//�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
// �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
//�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
//�܂��A�]���J�n Y ���W��Ԃ��܂�.
template<class _COMMXYT , class _COMMWORKAREA>
static int XYBrokeringAndSettingWorkArea(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd);
//T �� B �̒����̎��� �_ P �� X ������ y �����߂�.
template<class _COMMXYT>
static int Ypos(const _COMMXYT* Top ,const _COMMXYT* Buttom ,const _COMMXYT* P );

//�e���_�@�� Top Buttom Left Right �̂ǂ�Ȃ̂����߂�
template<class _COMMXYT>
static void SettingVector(_COMMXYT *XY,_COMMXYT **_Top,_COMMXYT **_Buttom,
                          _COMMXYT **_Left,_COMMXYT **_Right,enumOFFENDER *_Offender);
//�C�J���Ȃ��l�p�`���ǂ������ׂ�
template<class _COMMXYT>
static enumOFFENDER IsOffenderSquare(int Lpos,int Rpos,
			 const _COMMXYT *Left,const _COMMXYT *Right,const _COMMXYT *Top,const _COMMXYT *Buttom);

//�ւ��܂� ���ƉE���t���ǂ������ׂ�. �t�������� true
template<class _COMMXYT>
static bool IsHechimaAndLeftRightRev(int Lpos , const _COMMXYT *Left,
                              const _COMMXYT *Right,const _COMMXYT *Top ,
                              const _COMMXYT *Buttom);

//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVectorOffenderSquareRightSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare);

//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVectorOffenderSquareLeftSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare);
//�e���_�Ԃ̑����𒲂ׂ�(�ӂ[�̎l�p�`)
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVectorNormalSquare(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare);
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector(_COMMWORKAREA *WA,int *StopY,
       const _COMMXYT *Left,const _COMMXYT *Right,
       const _COMMXYT *Top ,const _COMMXYT *Buttom,enumOFFENDER OffenderSquare);

//���������G�掩�̂����Ă����̂��ǂ������ׂ�.
template<class _COMMXYT>
static bool IsDrawOk(const BMD bmd,const _COMMXYT *XY,const _COMMXYT *Top,
                     const _COMMXYT *Buttom);

/*****************************************************************/
/*****************************************************************/
//�O�p�`�o�[�W����
/*****************************************************************/
/*****************************************************************/
//XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
//�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
// �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
//�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
//�܂��A�]���J�n Y ���W��Ԃ��܂�.
template<class _COMMXYT , class _COMMWORKAREA>
static int XYBrokeringAndSettingWorkArea3(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd);
//3���̍��W������.
template<class _COMMXYT>
static void SettingVector3(_COMMXYT * XY,_COMMXYT **Top,
                           _COMMXYT **Buttom,_COMMXYT **Median);

//���������G�掩�̂����Ă����̂��ǂ������ׂ�.
template<class _COMMXYT>
static bool IsDrawOk3(const BMD bmd,const _COMMXYT *XY,const _COMMXYT *Top,
                      const _COMMXYT *Buttom);

//���_���m�̌X�������߂�.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector3(_COMMWORKAREA *WA,int *StopY,const _COMMXYT *Top,
                        const _COMMXYT *Buttom,const _COMMXYT *Median);

//�E�����O�p�`�̊e���_�ʂ̌X�������߂܂�.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector3Left(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median);

//�E�����O�p�`�̊e���_�ʂ̌X�������߂܂�.
template<class _COMMXYT , class _COMMWORKAREA>
static int HokanVector3Right(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median);


/*****************************************************************/
/*****************************************************************/
//����.
/*****************************************************************/
/*****************************************************************/
//���������@�������������}�`�Ȃ̂�?
//���̐}�`�̖{���̒��_����Ԃ�.
template<class _COMMXYT>
static int IsTarget(_COMMXYT *XY,int NumVec);
//Y���W�̃N���b�s���O.
template<class _COMMWORKAREA>
static bool YClipping(int &Y,int *StopY,_COMMWORKAREA * &lpWA,_COMMWORKAREA * &WAL,_COMMWORKAREA * &WAR,BMD const bmd2);
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.  �ӂ[
static void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E);
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.  Z Buffer
static void HokanLine(_WorkAreaZ *WA,const _XYZT *B,const _XYZT *E);
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.  �O���[
static void HokanLine(_WorkAreaG *WA,const _XYZCT *B,const _XYZCT *E);
//�e�X�^�[�g�n�_�̐ݒ� �ӂ[
static void StartSet(const _XYT *SL,const _XYT *SR);
//�e�X�^�[�g�n�_�̐ݒ� ZBuffer
static void StartSet(const _XYZT *SL,const _XYZT *SR);
//�e�X�^�[�g�n�_�̐ݒ� ����[
static void StartSet(const _XYZCT *SL,const _XYZCT *SR);
//�w�肳�ꂽ Step �����A�e�l�𑝕�������.  �ӂ[
static void StepUp(const _WorkArea *WAL,const _WorkArea *WAR,int Step);
//�w�肳�ꂽ Step �����A�e�l�𑝕�������.  ZBuffer
static void StepUp(const _WorkAreaZ *WAL,const _WorkAreaZ *WAR,int Step);
//�w�肳�ꂽ Step �����A�e�l�𑝕�������.  �O���[
static void StepUp(const _WorkAreaG *WAL,const _WorkAreaG *WAR,int Step);


/*****************************************************************/
//���R�ϊ� ���C����
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYT *XY)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeDrawMain3(bmd1,bmd2,XY);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.
    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�e�N�X�`�����w�肵���`�ɕϊ�.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y);
}
/*****************************************************************/
//���R�ϊ� ���C����(�O�p�`�o�[�W����)
/*****************************************************************/
void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYT *XY)
{
    //�����������ꂪ�O�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.
    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�e�N�X�`�����w�肵���`�ɕϊ�.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y);
}
/*****************************************************************/
//���R�l�p�`�n�b�`���O.
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYT *XY,int Rgb)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeBoxMain3(bmd2,XY,Rgb);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y);
}
/*****************************************************************/
//���R�l�p�`�n�b�`���O.�O�p�`�o�[�W����
/*****************************************************************/
void ReSizeBoxMain3(BMD bmd2,_XYT *XY,int Rgb)
{
    //�����������ꂪ�O�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkArea WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y);
}
/*****************************************************************/
//���R�ϊ� ���C���� ZBuffer
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeDrawMain3(bmd1,bmd2,XY,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.
    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�e�N�X�`�����w�肵���`�ɕϊ�.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//���R�ϊ� ���C����(�O�p�`�o�[�W����) ZBuffer
/*****************************************************************/
void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZT *XY,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.
    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�e�N�X�`�����w�肵���`�ɕϊ�.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//���R�l�p�`�n�b�`���O. ZBuffer
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeBoxMain3(bmd2,XY,Rgb,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//���R�l�p�`�n�b�`���O.�O�p�`�o�[�W���� ZBuffer
/*****************************************************************/
void ReSizeBoxMain3(BMD bmd2,_XYZT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaZ WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}


/*****************************************************************/
//���R�ϊ� ���C���� �O���[
/*****************************************************************/
void ReSizeDrawMain(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeDrawMain3(bmd1,bmd2,XY,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.
    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�e�N�X�`�����w�肵���`�ɕϊ�.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//���R�ϊ� ���C����(�O�p�`�o�[�W����) �O���[
/*****************************************************************/
void ReSizeDrawMain3(const BMD bmd1,BMD bmd2,_XYZCT *XY,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.
    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;
    //�e�N�X�`�����w�肵���`�ɕϊ�.
    _ReSizeDrawMain(bmd1,bmd2,WA,StopY,y,lpZBuffer);
}
/*****************************************************************/
//���R�l�p�`�n�b�`���O. �O���[
/*****************************************************************/
void ReSizeBoxMain(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,4) )
    {
    case 3:    ReSizeBoxMain3(bmd2,XY,Rgb,lpZBuffer);        break;
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}

/*****************************************************************/
//���R�l�p�`�n�b�`���O.�O�p�`�o�[�W���� �O���[
/*****************************************************************/
void ReSizeBoxMain3(BMD bmd2,_XYZCT *XY,int Rgb,LPZBUFFER lpZBuffer)
{
    //�����������ꂪ�l�p�`�Ȃ̂��ǂ����`�F�b�N.
    switch ( IsTarget(XY,3) )
    {
    case 2:    case 1:    case 0: return ;
    }

    _WorkAreaG WA[3*2]; //Y �� 1�i�ނƂ��̌X��
    int StopY[3];       //Y �� 1�i�ނƂ��̌X���@��؂�ւ��� Y �̏ꏊ
	int y ;                 //Y �́A�g�b�v����X�^�[�g�ł�.

    //XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
    //�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
    // �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
    //�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
    //�܂��A�]���J�n Y ���W��Ԃ��܂�.
    y = XYBrokeringAndSettingWorkArea3(XY,WA,StopY,bmd2);
    if (y >= bmd2->h) return;

    _ReSizeBoxMain(bmd2,Rgb,WA,StopY,y,lpZBuffer);
}


/*****************************************************************/
//�e�N�X�`�����w�肵���`�ɕϊ�.
/*****************************************************************/
void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkArea *WA,int *StopY,int StartY)
{
    _WorkArea *lpWA ;			    //���݂� _WorkArea .(y��1�i�񂾂Ƃ��̊e��̑����l)
    _WorkArea *WAL,*WAR;			//���݂̍��E��S�����Ă���_WorkArea.
    int y = StartY;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		

	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd1) ) return;
	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch1,AddPitch2;
    BYTE *data1,*data2;
	data1 = SeekData(bmd1,0,0, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;
    int Bmd1WPow = bmd1->Pow2W;
    int *lpStopY;

	//���]�����[�v
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
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
		    register int Temp;		//��Ɨ̈�.
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
				StartX = 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                Temp = FI(LocalTX)+ ( FI(LocalTY) << Bmd1WPow);
                DirectPSet(d2, data1+Temp+Temp+Temp );
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;
            }
        }
		//X,�e�N�X�`���̑�����ǉ������.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//���R�G��Ł@�w�肵���}�`���w�肵���F�ŊG��
/*****************************************************************/
void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkArea *WA,int *StopY,int StartY)
{
    _WorkArea *lpWA ;			    //���݂� _WorkArea .(y��1�i�񂾂Ƃ��̊e��̑����l)
    _WorkArea *WAL,*WAR;			//���݂̍��E��S�����Ă���_WorkArea.
    int y = StartY;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	CommDraw->GraphicsLock(bmd2);

    int AddPitch2;
    BYTE *data2;
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    int Bmd2W = bmd2->w;

    int *lpStopY;
	//���]�����[�v
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
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
            register BYTE *d2;
            register int Temp;		//��Ɨ̈�.
            StartX = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (StartX < 0)
            {
                StartX = - StartX;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= StartX;    //�]���͈͂��k�߂�. .
                d2 = data2 + 0 + 0 + 0;
				StartX = 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                DirectPSet(d2, Rgb );
                d2 += 3;
            }
		}
		//X �̑�����������.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        data2 += AddPitch2;
    }
	CommDraw->GraphicsUnLock(bmd2);
}

/*****************************************************************/
//�e�N�X�`�����w�肵���`�ɕϊ�. ZBuffer
/*****************************************************************/
void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaZ *lpWA ;			    //���݂� _WorkArea .(y��1�i�񂾂Ƃ��̊e��̑����l)
    _WorkAreaZ *WAL,*WAR;			//���݂̍��E��S�����Ă���_WorkArea.
    int y = StartY;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd1) ) return;
	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch1,AddPitch2,AddZBufferPitch;
    BYTE *data1,*data2;
    Fixed *Zb;      //�[�b�g�o�b�t�@�̃o�b�t�@���̂��̂̃A�h���X
	data1 = SeekData(bmd1,0,0, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;
    int Bmd1WPow = bmd1->Pow2W;
    int *lpStopY;

	//���]�����[�v
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
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
		    register int Temp;		//��Ɨ̈�.
            register BYTE *d2;
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (StartX < 0)
            {
                StartX = - StartX;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= StartX;    //�]���͈͂��k�߂�. .
                LocalZ  += LocalStepZ  * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                LocalTX += LocalStepTX * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                LocalTY += LocalStepTY * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
                tZb = Zb + StartX;
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer ���A�������f�[�^��������G��.
                    *tZb = LocalZ;    //Z Buffer �̍X�V.
                    Temp = FI(LocalTX)+ ( FI(LocalTY) << Bmd1WPow);
                    DirectPSet(d2, data1+Temp+Temp+Temp );
                }
                LocalZ  += LocalStepZ;
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;    //���̃f�[�^��.
                tZb ++; //���� ZBuffer �f�[�^��.
            }
        }
		//X,�e�N�X�`���̑�����ǉ������.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//���R�G��Ł@�w�肵���}�`���w�肵���F�ŊG�� ZBuffer 
/*****************************************************************/
void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkAreaZ *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaZ *lpWA ;			    //���݂� _WorkArea .(y��1�i�񂾂Ƃ��̊e��̑����l)
    _WorkAreaZ *WAL,*WAR;			//���݂̍��E��S�����Ă���_WorkArea.
    int y = StartY;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	CommDraw->GraphicsLock(bmd2);

    int AddPitch2,AddZBufferPitch;
    BYTE *data2;
    Fixed *Zb;      //�[�b�g�o�b�t�@�̃o�b�t�@���̂��̂̃A�h���X
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;

    int *lpStopY;
	//���]�����[�v
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
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
            register BYTE *d2;
            register int Temp;		//��Ɨ̈�.
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            StartX = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (StartX < 0)
            {
                StartX = - StartX;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= StartX;    //�]���͈͂��k�߂�. .
                LocalZ  += LocalStepZ  * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
                tZb = Zb + StartX;
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer ���A�������f�[�^��������G��.
					*tZb = LocalZ;
                    DirectPSet(d2, Rgb );
                }
                LocalZ  += LocalStepZ;
                d2 += 3;    //���̃f�[�^��.
                tZb ++; //���� ZBuffer �f�[�^��.
            }
		}
		//X �̑�����������.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
}


/*****************************************************************/
//�e�N�X�`�����w�肵���`�ɕϊ�. �O���[
/*****************************************************************/
void _ReSizeDrawMain(const BMD bmd1,BMD bmd2,
                     _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaG *lpWA ;			    //���݂� _WorkArea .(y��1�i�񂾂Ƃ��̊e��̑����l)
    _WorkAreaG *WAL,*WAR;			//���݂̍��E��S�����Ă���_WorkArea.
    int y = StartY;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd1) ) return;
	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch1,AddPitch2,AddZBufferPitch;
    BYTE *data1,*data2;
    Fixed *Zb;      //�[�b�g�o�b�t�@�̃o�b�t�@���̂��̂̃A�h���X
	data1 = SeekData(bmd1,0,0, 0,0,&AddPitch1);
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;
    int Bmd1WPow = bmd1->Pow2W;
    int *lpStopY;

	//���]�����[�v
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
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
		    register int Temp;		//��Ɨ̈�.
            register BYTE *d2;
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            Fixed LocalCR   ,   LocalStepCR;
            Fixed LocalCG   ,   LocalStepCG;
            Fixed LocalCB   ,   LocalStepCB;
            Fixed LocalTX   ,   LocalStepTX;
            Fixed LocalTY   ,   LocalStepTY;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            LocalCR = CR1;      LocalStepCR = (CR2-CR1) / DrawLen;
            LocalCG = CG1;      LocalStepCG = (CG2-CG1) / DrawLen;
            LocalCB = CB1;      LocalStepCB = (CB2-CB1) / DrawLen;
            LocalTX = TX1;      LocalStepTX = (TX2-TX1) / DrawLen;
            LocalTY = TY1;      LocalStepTY = (TY2-TY1) / DrawLen;
            StartX = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (StartX < 0)
            {
                StartX = - StartX;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= StartX;    //�]���͈͂��k�߂�. .
                LocalZ  += LocalStepZ  * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                LocalCR += LocalStepCR * StartX; // ���Z�摜�ʂ̐F R
                LocalCG += LocalStepCG * StartX; // ���Z�摜�ʂ̐F G
                LocalCB += LocalStepCB * StartX; // ���Z�摜�ʂ̐F B
                LocalTX += LocalStepTX * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                LocalTY += LocalStepTY * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
                tZb = Zb + StartX;
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer ���A�������f�[�^��������G��.
					*tZb = LocalZ;
                    Temp = FI(LocalTX)+ ( FI(LocalTY) << Bmd1WPow);
                    DirectPSet(d2, data1+Temp+Temp+Temp ,
                       (FI(LocalCR) << 16) + (FI(LocalCG) << 8) + FI(LocalCB) );
                }
                LocalZ  += LocalStepZ;
                LocalCR += LocalStepCR;
                LocalCG += LocalStepCG;
                LocalCB += LocalStepCB;
                LocalTX += LocalStepTX;
                LocalTY += LocalStepTY;
                d2 += 3;    //���̃f�[�^��.
                tZb ++; //���� ZBuffer �f�[�^��.
            }
        }
		//X,�e�N�X�`���̑�����ǉ������.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        CR1 += WAL->StepCR;      CR2 += WAR->StepCR;
        CG1 += WAL->StepCG;      CG2 += WAR->StepCG;
        CB1 += WAL->StepCB;      CB2 += WAR->StepCB;
        TX1 += WAL->StepTX;      TX2 += WAR->StepTX;
        TY1 += WAL->StepTY;      TY2 += WAR->StepTY;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
	CommDraw->GraphicsUnLock(bmd1);
}

/*****************************************************************/
//���R�G��Ł@�w�肵���}�`���w�肵���F�ŊG�� �O���[
/*****************************************************************/
void _ReSizeBoxMain(BMD bmd2,int Rgb,
                    _WorkAreaG *WA,int *StopY,int StartY,LPZBUFFER lpZBuffer)
{
    _WorkAreaG *lpWA ;			    //���݂� _WorkArea .(y��1�i�񂾂Ƃ��̊e��̑����l)
    _WorkAreaG *WAL,*WAR;			//���݂̍��E��S�����Ă���_WorkArea.
    int y = StartY;
    //�]���J�n _WorkArea �̐ݒ�.
    WAL = &WA[0];    WAR = &WA[1];
	lpWA = WA;		
	//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
	// y , StopY �͏�����������\��������܂�
	if (!YClipping(y,StopY,lpWA,WAL,WAR,bmd2))	return;

	if (!CommDraw->GraphicsLock(bmd2) ) return;

    int AddPitch2,AddZBufferPitch;
    BYTE *data2;
    Fixed *Zb;      //�[�b�g�o�b�t�@�̃o�b�t�@���̂��̂̃A�h���X
	data2 = SeekData(bmd2,0,y, 0,0,&AddPitch2);
    Zb    = lpZBuffer->GetZBuffer(0,y);
    AddZBufferPitch = lpZBuffer->GetPitch();
    int Bmd2W = bmd2->w;
    int *lpStopY;

	//���]�����[�v
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
        int DrawLen = FI(X2 - X1 + IF(1));
        if (DrawLen > 0)
        {
            int StartX;
            //�e�N�X�`�������f���鏀��
		    register int Temp;		//��Ɨ̈�.
            register BYTE *d2;
            register Fixed *tZb;
            Fixed LocalZ    ,   LocalStepZ;
            Fixed LocalCR   ,   LocalStepCR;
            Fixed LocalCG   ,   LocalStepCG;
            Fixed LocalCB   ,   LocalStepCB;
            LocalZ  = Z1;       LocalStepZ  = (Z2-Z1)   / DrawLen;
            LocalCR = CR1;      LocalStepCR = (CR2-CR1) / DrawLen;
            LocalCG = CG1;      LocalStepCG = (CG2-CG1) / DrawLen;
            LocalCB = CB1;      LocalStepCB = (CB2-CB1) / DrawLen;
            StartX = FI(X1);      
            //�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
            if (StartX < 0)
            {
                StartX = - StartX;  //�}�C�i�X�Ȃ̂Ńv���X�ɕϊ�
                DrawLen -= StartX;    //�]���͈͂��k�߂�. .
                LocalZ  += LocalStepZ  * StartX; // �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
                LocalCR += LocalStepCR * StartX; // ���Z�摜�ʂ̐F R
                LocalCG += LocalStepCG * StartX; // ���Z�摜�ʂ̐F G
                LocalCB += LocalStepCB * StartX; // ���Z�摜�ʂ̐F B
                d2 = data2 + 0 + 0 + 0;
                tZb = Zb + 0;
				StartX = 0;
            }
            else
            {   //�͈͓��̐���ȓ]���ł�.
                d2 = data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
                tZb = Zb + StartX;
            }

            //�]����̃��c�������𒴂��Ȃ��悤��....
            Temp = ( Bmd2W - (StartX + DrawLen) );
            if (Temp < 0 )  DrawLen+=Temp;  //
            //�]���J�n
            for (register int l = 0 ; l < DrawLen ; l++)
            {
                if (*tZb > LocalZ)
                {   //Z Buffer ���A�������f�[�^��������G��.
					*tZb = LocalZ;
                    DirectPSet(d2, Rgb ,
                        (FI(LocalCR) << 16) + (FI(LocalCG) << 8) + FI(LocalCB) );
                }
                LocalZ  += LocalStepZ;
                LocalCR += LocalStepCR;
                LocalCG += LocalStepCG;
                LocalCB += LocalStepCB;
                d2 += 3;    //���̃f�[�^��.
                tZb ++; //���� ZBuffer �f�[�^��.
            }
        }
		//X,�e�N�X�`���̑�����ǉ������.
        X1 += WAL->StepX;        X2 += WAR->StepX;
        Z1 += WAL->StepZ;        Z2 += WAR->StepZ;
        CR1 += WAL->StepCR;      CR2 += WAR->StepCR;
        CG1 += WAL->StepCG;      CG2 += WAR->StepCG;
        CB1 += WAL->StepCB;      CB2 += WAR->StepCB;
        data2 += AddPitch2;
        Zb += AddZBufferPitch;
    }
	CommDraw->GraphicsUnLock(bmd2);
}



















/*****************************************************************/
//���R�ϊ�( N�p�`�̊G�� ) �̉��������[�`��
/*****************************************************************/

/*****************************************************************/
/*****************************************************************/
//�l�p�`�o�[�W����
/*****************************************************************/
/*****************************************************************/

/*****************************************************************/
//�e���_�@�� Top Buttom Left Right �̂ǂ�Ȃ̂����߂�
/*****************************************************************/
template<class _COMMXYT>
void SettingVector(_COMMXYT *XY,_COMMXYT **_Top,_COMMXYT **_Buttom,
                   _COMMXYT **_Left,_COMMXYT **_Right,enumOFFENDER *_Offender)
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
    _COMMXYT *Left,*Right,*Top,*Buttom;
    _COMMXYT *Temp;
	enumOFFENDER Offender;
    register int i;
    int BackupI1,BackupI2;
    int OkFlg = 0;
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

    int Rpos , Lpos ;
	Rpos = Ypos(Top,Buttom,Right);	//Top �� Buttom �̒������� Right->x �������� Y �����߂�.
	Lpos = Ypos(Top,Buttom,Left);	//Top �� Buttom �̒������� Left ->x �������� Y �����߂�.

	Offender = IsOffenderSquare(Lpos,Rpos,Left,Right,Top,Buttom);
    if ( Offender != OFFENDER_NONE )
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
        if (Left->y > Right->y)
        {
            Temp = Left ;   Left = Right;   Right = Temp;
        }
    }
    else
    {
        /*
        *         T
        *        / | 
        *       L  |  
        *      /   \    ���݂��ƁA���̂悤�Ȑ}�`�@�ւ��܌^�Ɩ������Ȃ��Ă�����.
        *     /   \     �́A L �� R ������ւ���ĊG�悳��Ȃ��Ƃ�������܂�.
        *    /   \      
        *    |  \       
        *    B-R
        */
		if (IsHechimaAndLeftRightRev(Lpos,Left,Right,Top,Buttom) )
		{
	        Temp = Left ;   Left = Right;   Right = Temp;
		}
    }

    *_Top = Top;
    *_Buttom = Buttom;
    *_Left = Left;
    *_Right = Right;
    //�C�J���Ȃ��l�p�`���ǂ�����Ԃ��̂ł�.
	*_Offender = Offender;
}
/*****************************************************************/
//T �� B �̒����̎��� �_ P �� X ������ y �����߂�.
/*****************************************************************/
template<class _COMMXYT>
int Ypos(const _COMMXYT* Top ,const _COMMXYT* Buttom ,const _COMMXYT* P )
{
	if (Top->x == Buttom->x)
	{
		return Top->x;
	}
	return (Top->y - Buttom->y) * 
		    (P->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;
}

 
/*****************************************************************/
//�C�J���Ȃ��l�p�`���ǂ������ׂ�
/*****************************************************************/
template<class _COMMXYT>
enumOFFENDER IsOffenderSquare(int Lpos , int Rpos,
					  const _COMMXYT *Left,const _COMMXYT *Right,
					  const _COMMXYT *Top ,const _COMMXYT *Buttom)
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
        if (Lpos < Left->y && Rpos < Right->y)
        {
            if (Top->x < Buttom->x)  return OFFENDER_LEFT;
            else                     return OFFENDER_RIGHT;
        }
        if (Lpos > Left->y && Rpos > Right->y)
        {
            if (Top->x < Buttom->x)  return OFFENDER_RIGHT;
            else                     return OFFENDER_LEFT;
        }
    }
    else
    {
        if (Top->x < Left->x && Top->x < Right->x) return OFFENDER_RIGHT;
        if (Top->x > Left->x && Top->x > Right->x) return OFFENDER_LEFT;
    }
    //�ӂ[�̎l�p�`.
    return OFFENDER_NONE;
}
/*****************************************************************/
//�ւ��܂� ���ƉE���t���ǂ������ׂ�. �t�������� true
/*****************************************************************/
template<class _COMMXYT>
bool IsHechimaAndLeftRightRev(int Lpos ,
					  const _COMMXYT *Left,const _COMMXYT *Right,
					  const _COMMXYT *Top ,const _COMMXYT *Buttom)
{
	if (Top->x < Buttom->x)
	{
		if (Lpos > Left->y)
		{
			return true;
		}
	}
	else if (Top->x > Buttom->x)
	{
		if (Lpos < Left->y)
		{
			return true;
		}
	}
	return false;
}

/*****************************************************************/
//�e���_�Ԃ̑����𒲂ׂ�
/*****************************************************************/
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
{
    //�K��ᔽ�Ȑ}�`����?
    //�K��ᔽ�̐}�`�́A���̐}�݂����Ȃ��.
    //�H�`�Ⓑ���`����Ȃ��^�C�v�ł�.
	switch(OffenderSquare)
	{
	case OFFENDER_LEFT:
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
			break;
	case OFFENDER_RIGHT:
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
			break;
	case OFFENDER_NONE:
        //�ʏ�́@�����`�A�����`�A�@�H�`�A��`�Ȃ�.
        return HokanVectorNormalSquare
                (WA,StopY,Left,Right,Top,Buttom,OffenderSquare);
		break;
	}
	//�G���[�ċz(^^;;)
	return -1;
}

/*****************************************************************/
//�e���_�Ԃ̑����𒲂ׂ�(�C�J���Ȃ��l�p�`�o�[�W����)
/*****************************************************************/
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVectorOffenderSquareRightSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
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
				if (Left->y == Right->y)
				{	//����́A T L R �� y �������� �܂�A�O�p�`�ł���(��)
	                HokanLine(&WA[WORK_AREA_GET(0,1)],Right,Buttom);  //WaR = R => B �܂�
	                StopY[0] = Buttom->y;     //R �� Y �ň�x��
					return 0;
				}
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
				if (Left->y == Right->y)
				{	//Left �� Right �� Y �����������Ă��Ƃ́A
                    //�O�p�`�ɂȂ�܂�.
                    HokanLine(&WA[WORK_AREA_GET(1,1)],Left,Buttom);  //WaR = L => B �܂�
					WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];	//T->B
					return 1;
				}
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
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVectorOffenderSquareLeftSmarll(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
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
				if (Left->y == Right->y)
				{	//Left �� Right �� Y �����������Ă��Ƃ́A
                    //�O�p�`�ɂȂ�܂�.
                    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaR = L => B �܂�
					WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];	//T->B
					return 1;
				}
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
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVectorNormalSquare(_COMMWORKAREA *WA,int *StopY,
                            const _COMMXYT *Left,const _COMMXYT *Right,
                            const _COMMXYT *Top ,const _COMMXYT *Buttom,
                            enumOFFENDER OffenderSquare)
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
    else if (Right->y > Left->y)
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
    //�����́A Left->y == Right->y �̏ꍇ.
    //�܂�A�Y��ȕH�`�ł�.
    StopY[0] = Left->y;    //L,R �� Y �ň�x��
    if (Left->y == Buttom->y)   return 0;   //L,R �� B �����������������炨���܂�.
    HokanLine(&WA[WORK_AREA_GET(1,0)],Left,Buttom);  //WaL = L => B �܂�
    HokanLine(&WA[WORK_AREA_GET(1,1)],Right,Buttom); //WaR = R => B �܂�
	StopY[1] = Buttom->y;
    return 1;
}



//���������G�掩�̂����Ă����̂��ǂ������ׂ�.
template<class _COMMXYT>
bool IsDrawOk(const BMD bmd,const _COMMXYT *XY,
              const _COMMXYT *Top,const _COMMXYT *Buttom)
{
	int w,h;
	w = bmd->w;	h = bmd->h;
	//���ׂĉE���̐��̉ʂ�.
	if (XY[0].x < 0 && XY[1].x < 0 && XY[2].x < 0 && XY[3].x < 0)	return false;
	//���ׂč����̋�͂̌�����.
	if (XY[0].x >= w && XY[1].x >= w && XY[2].x >= w && XY[3].x >= w)	return false;
	//��Ɖ��̂������̐��E�ɐ����������.
    if ( (Top->y < 0 && Buttom->y < 0) || (Top->y >= h && Buttom->y >= h))	return false;
	
	//�A���Ă��Ă��ꂽ�񂾂�. �G�悵�Ă� ok ���.
	return true;
}


//XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
//�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
// �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
//�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
//�܂��A�]���J�n Y ���W��Ԃ��܂�.
template<class _COMMXYT , class _COMMWORKAREA>
int XYBrokeringAndSettingWorkArea(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd)
{
    _COMMXYT *Left,*Right,*Top,*Buttom;
    enumOFFENDER OffenderSquare;
    //�l���̍��W������.
    SettingVector(XY,&Top,&Buttom,&Left,&Right,&OffenderSquare);
	//���������G�掩�̂����Ă����̂��ǂ������ׂ�.
	if ( !IsDrawOk(bmd,XY,Top,Buttom))		return bmd->h;
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
    _COMMXYT *SL,*SR;                   //�X�^�[�g�n�_
    //�X�^�[�g�n�_�ݒ� �X�^�[�g�n�_�́A �Ђ��`�̏ꍇ�́A �E���́A T->y ����n�܂�܂����A
	//�����`�Ȃǂ́A �E T �� L �Ȃǂ���n�܂�܂�.
    SL = SR = Top;
    if (OffenderSquare != OFFENDER_NONE)
    {	//�ӂ[����Ȃ��l�p�`�̏ꍇ.
        if (Top->y == Left->y)
        {	//T L �̒n�_���������ꍇ�A �X�^�[�g�n�_�͏�� T ����Ƃ͂�����Ȃ�.
			if (Left->y != Right->y)
			{	// T L ����ԏ����� y �������Ă��� (�X�^�[�g�n�_)
				if (Top->x > Left->x)   SL = Left;
				else                    SR = Left;
			}
			else
			{	//�O�̂��߁A T L R�� y ���������Ƃ��̂��߂ɒ��ׂĂ����܂�
				//���ꂪ��������̂́A �O�p�`�������Ƃ������ł�.
				//���������A �l�p�`�������[�`���ɎO�p�`���������.
				if (Top->x > Right->x)  SL = Right;
				else                    SR = Right;
			}
        }
    }
    else
    {
        if (Top->y == Left->y)      SL = Left;
        if (Top->y == Right->y)     SR = Right;
    }
    //�e�X�^�[�g�n�_�Z�b�g
    StartSet(SL,SR);
    //�]���J�n Y ���W.
    return Top->y;
}

///////////////////////////////////////////////////////////////////////////////////////////
//�O�p�`�o�[�W����
///////////////////////////////////////////////////////////////////////////////////////////

//XY �𕪉����āA���ꂼ��̐i�ޒl�� WA �Ɋi�[
//�܂��A���ꂼ��̒�ԃ|�C���g�� StopY �Ɋi�[���܂�.
// �Ȃ��A WA , StopY �͔z��ł��̂Œ��ӂ��Ă�������.
//�܂��A X1 TX1 TY1 X2 TX2 TY2 �ɒl�̃Z�b�g���s���܂�.
//�܂��A�]���J�n Y ���W��Ԃ��܂�.
template<class _COMMXYT , class _COMMWORKAREA>
int XYBrokeringAndSettingWorkArea3(_COMMXYT *XY,_COMMWORKAREA *WA,
                                    int *StopY,const BMD bmd)
{
    _COMMXYT *Median,*Top,*Buttom;

    //3���̍��W������.
    SettingVector3(XY,&Top,&Buttom,&Median);
	//���������G�掩�̂����Ă����̂��ǂ������ׂ�.
	if ( !IsDrawOk3(bmd,XY,Top,Buttom))		return bmd->h;
    /*
    *     T
    *    / |	�O�p�`�̔閧!!.
    *   /  |
    *  /  |
    *M/   | 
    *  \ |
    *   \
    *   B
    */
    DeltaXYTStart = 0;				//������A�ŏ��Ɏg�����[�N�G���A�͍ŏ��ł�.
    DeltaXYTMax = HokanVector3(WA,StopY,Top,Buttom,Median);
    //�X�^�[�g�ʒu�̌���.
    //X1 TX1 TY1 X2 TX2 TY2 �̓X�^�e�B�b�N�ȃO���[�o���ϐ��ł��̂Œ���.
    _COMMXYT *SL,*SR;                   //�X�^�[�g�n�_
    //�X�^�[�g�n�_�ݒ�
    SL = SR = Top;
    if (Top->y == Median->y)   
	{
		if (Top->x > Median->x)	SL = Median;
		else                    SR = Median;
	}
    //�e�X�^�[�g�n�_�Z�b�g
    StartSet(SL,SR);
    //�]���J�n Y ���W.
    return Top->y;
}

//3���̍��W������.
template<class _COMMXYT>
void SettingVector3(_COMMXYT * XY,_COMMXYT **_Top,
                    _COMMXYT **_Buttom,_COMMXYT **_Median)
{
    /*
        *       T
        *      /|
        *     / |
        *    /  | 
        *  M/   |  
        *   \   |
        *    \  |
        *     \ |
        *      \|
        *       B
    *   ��̂悤�ɂȂ�܂�.
    * T �Ƃ́A y ���ŏ��@�@y == y ����������Ax ����������.
    * B �Ƃ́A y ���ő�.
    * M �́A����ȊO�̓_.
    */
    _COMMXYT *Top,*Buttom,*Median;
    _COMMXYT *Temp;
    int i;
    int BackupI1,BackupI2;
    int OkFlg = 0;
    //Top �����߂�.
    BackupI1 = BackupI2 = 0;
    Buttom = Top = &XY[0];
    for(i = 0; i < 3 ; i++)
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
    //Top Buttom �ȊO�� 1����A Median �����߂�.
    for(i = 0; i < 3 ; i++)
    {
        if (! GETBIT(OkFlg,i) )
        {
            Median = &XY[i];
            break;
        }
    }

    *_Top = Top;
    *_Buttom = Buttom;
    *_Median = Median;
}

//���������G�掩�̂����Ă����̂��ǂ������ׂ�.
template<class _COMMXYT>
bool IsDrawOk3(const BMD bmd,const _COMMXYT *XY,const _COMMXYT *Top,
               const _COMMXYT *Buttom)
{
	int w,h;
	w = bmd->w;	h = bmd->h;
	//���ׂĉE���̐��̉ʂ�.
	if (XY[0].x < 0 && XY[1].x < 0 && XY[2].x < 0)	return false;
	//���ׂč����̋�͂̌�����.
	if (XY[0].x >= w && XY[1].x >= w && XY[2].x >= w)	return false;
	//��Ɖ��̂������̐��E�ɐ����������.
    if ( (Top->y < 0 && Buttom->y < 0) || (Top->y >= h && Buttom->y >= h))	return false;
	
	//�A���Ă��Ă��ꂽ�񂾂�. �G�悵�Ă� ok ���.
	return true;
}

//���_���m�̌X�������߂�.
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector3(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,
                 const _COMMXYT *Median)
{
    int Mpos ;
	Mpos = Ypos(Top,Buttom,Median);	//Top �� Buttom �̒������� Median->x �������� Y �����߂�.
	if (Top->x > Buttom->x)
	{
		if (Mpos > Median->y)	return	HokanVector3Left(WA,StopY,Top,Buttom,Median);
		else					return 	HokanVector3Right(WA,StopY,Top,Buttom,Median);
	} 
	else if (Top->x < Buttom->x)
	{
		if (Mpos >= Median->y)	return 	HokanVector3Right(WA,StopY,Top,Buttom,Median);
		else					return	HokanVector3Left(WA,StopY,Top,Buttom,Median);
	}
	else
	{
		if (Median->x < Top->x)	return	HokanVector3Left(WA,StopY,Top,Buttom,Median);
		else					return 	HokanVector3Right(WA,StopY,Top,Buttom,Median);
	}
}

//�E�����O�p�`�̊e���_�ʂ̌X�������߂܂�.
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector3Left(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median)
{
        /*      
        *       T
        *      /|
        *     / |
        *    /  | 
        *  M/   |  ����Ȋ����̐}�`
        *   \   |			�́A���ʂȃ��[�`�����K�v�ł�(��)
        *    \  |
        *     \ |
        *      \|
        *       B
        */
            //T -> M �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Median);
            //T -> B �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Buttom);
            //���X�g�b�v�|�C���g
            StopY[0] = Median->y;    //M �� Y �ň�x��
            if (Median->y == Buttom->y)   return 0;   //M �� B �����������������炨���܂�.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,0)],Median,Buttom);  //WaR = Median => Buttom �܂�
                //WA[WORK_AREA_GET(0,1)] = WA[WORK_AREA_GET(0,1)];                //WaL �́A�O��̑��������s����.

                //��1�X�g�b�v�|�C���g
                StopY[0] = Buttom->y;     //B �� Y �ň�x��
				return 0;
            }
            else
            {
                HokanLine(&WA[WORK_AREA_GET(1,0)],Median,Buttom);  //WaR = Median => Buttom �܂�
                WA[WORK_AREA_GET(1,1)] = WA[WORK_AREA_GET(0,1)];                //WaL �́A�O��̑��������s����.

                //���X�g�b�v�|�C���g
                StopY[1] = Buttom->y;     //B �� Y �ň�x��
				return 1;
            }
}

//�E�����O�p�`�̊e���_�ʂ̌X�������߂܂�.
template<class _COMMXYT , class _COMMWORKAREA>
int HokanVector3Right(_COMMWORKAREA *WA,int *StopY,
                 const _COMMXYT *Top,const _COMMXYT *Buttom,const _COMMXYT *Median)
{
        /*      
        *   T
        *   |\
        *   | \
        *   |  \
        *   |   \M   ����Ȋ����̐}�`
        *   |	/           �́A���ʂȃ��[�`�����K�v�ł�(��)
        *   |  /
        *   | /
        *   |/
        *   B
        */
            //T -> M �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,1)],Top,Median);
            //T -> B �ɂ���.
            HokanLine(&WA[WORK_AREA_GET(0,0)],Top,Buttom);
            //���X�g�b�v�|�C���g
            StopY[0] = Median->y;    //M �� Y �ň�x��
            if (Median->y == Buttom->y)   return 0;   //M �� B �����������������炨���܂�.
    		if (Top->y == StopY[0])
            {
                HokanLine(&WA[WORK_AREA_GET(0,1)],Median,Buttom);  //WaR = Median => Buttom �܂�
                //WA[WORK_AREA_GET(0,0)] = WA[WORK_AREA_GET(0,0)];                //WaL �́A�O��̑��������s����.

                //��1�X�g�b�v�|�C���g
                StopY[0] = Buttom->y;     //B �� Y �ň�x��
				return 0;
            }
            else
            {
                HokanLine(&WA[WORK_AREA_GET(1,1)],Median,Buttom);  //WaR = Median => Buttom �܂�
                WA[WORK_AREA_GET(1,0)] = WA[WORK_AREA_GET(0,0)];                //WaL �́A�O��̑��������s����.

                //���X�g�b�v�|�C���g
                StopY[1] = Buttom->y;     //B �� Y �ň�x��
				return 1;
            }
}

/*****************************************************************/
/*****************************************************************/
//����
/*****************************************************************/
/*****************************************************************/

//Y ���W�ɑ΂���N���b�s���O�����s���܂�.
template<class _COMMWORKAREA>
bool YClipping(int &Y,int *StopY,_COMMWORKAREA * &lpWA,_COMMWORKAREA * &WAL,_COMMWORKAREA * &WAR,BMD const bmd2)
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
            StepUp(WAL,WAR,Temp);
			Y = 0;
			break;
		}
		else
		{	//�܂� 0 �ɓ��B���Ȃ��ꍇ....
			if (DeltaXYTStart == DeltaXYTMax) return false; //���ׂă}�C�i�X�ɓ˂�����ł���.
			Temp = (-Y) + *lpStopY ;	// �������]���� �X�g�b�v Y �̒l���A�e�v�f���v���X���܂�.
			//�e�v�f���v���X����.
            StepUp(WAL,WAR,Temp);
			Y = *lpStopY;
		}
        lpStopY++;  //���̒�~�ꏊ��
        lpWA+=2;
        WAL = lpWA;
	    WAR = (lpWA+1);
	}

    if (Y >= bmd2->h) return false;
    
    //Y���W�̃I�[�o�[�]���̖h�~
    for(lpStopY = &StopY[DeltaXYTStart] , Temp = DeltaXYTStart;Temp <= DeltaXYTMax;lpStopY++ , Temp++)
    {
        if(*lpStopY >= bmd2->h)
        {
            DeltaXYTMax = Temp;
            *lpStopY = (bmd2->h - 1);  //�͂� ������.
            break; 
        }
    }
	return true;
}




/*****************************************************************/
//���������@�������������}�`�Ȃ̂�?
//���̐}�`�̖{���̒��_����Ԃ�.
/*****************************************************************/
template<class _COMMXYT>
int IsTarget(_COMMXYT *XY,int NumVec)
{
    int i,l,UseCount;
    int UseVec = NumVec;     //���ǂ̒��_��
    _COMMXYT NewXY[4];

    UseCount = 0;
    for(i = 0; i < NumVec ;i ++)
    {
        for(l = i+1 ; l < NumVec ; l ++)
        {
            //�����ȊO�̓_�ƃ��[�O�������.
            if ( XY[i].x == XY[l].x && XY[i].y == XY[l].y)
            {
                //�����Ɠ���.
                UseVec--;
                break;
            }
        }
        //���̒��_�͕K�v
        if(l>=NumVec)   NewXY[UseCount++] = XY[i];
    }
    if (UseVec == NumVec) return UseVec;

	_COMMXYT * lpNewXY = NewXY;

    //���̐}�`�� XY �ɏ����߂�.
	switch(UseVec)
	{
	case 3:
		*XY++ = *lpNewXY++ ;
	case 2:
		*XY++ = *lpNewXY++ ;
	case 1:
		*XY++ = *lpNewXY++ ;
	}
 	//memcpy( XY , NewXY , sizeof(_COMMXYT) * UseVec );
    return UseVec;
	/*
    int i,l,UseCount;
    int UseVec = NumVec;     //���ǂ̒��_��
    _COMMXYT NewXY[4];

    UseCount = 0;
    for(i = 0; i < NumVec ;i ++)
    {
        for(l = i+1 ; l < NumVec ; l ++)
        {
            //�����ȊO�̓_�ƃ��[�O�������.
            if ( XY[i].x == XY[l].x && XY[i].y == XY[l].y)
            {
                //�����Ɠ���.
                UseVec--;
                break;
            }
        }
        //���̒��_�͕K�v
        if(l>=NumVec)   NewXY[UseCount++] = XY[i];
    }
    if (UseVec == NumVec) return UseVec;
    //���̐}�`�� XY �ɏ����߂�.
    for(i = 0; i < UseVec ; i++)
    {
        XY[i] = NewXY[i];
    }
    return UseVec;
	*/
	/*
    int UseVec = NumVec;     //���ǂ̒��_��
    for(int i = 0 ; i < UseVec ; i ++ )
    {
        for(int l = i+1 ; l < UseVec ; l ++ )
        {
            //�����ȊO�̓_�ƃ��[�O�������.
            if ( XY[i].x == XY[l].x && XY[i].y == XY[l].y)
            {
                //�����Ɠ������������. �����͂����� ���肻�̂������݂�
				//�^�C���p���h�b�N�X���������킯�ł͂Ȃ����ǁA���ł��܂�.

				//�������ړ� �܂莩���̂Ƃ��낪�Ԃ�āA ���̏�̂�������Ă���.
				register int DecUseVec = UseVec-1;
				for( int n = i ; n < DecUseVec ; n++ )
				{
					XY[n] = XY[n+1];
				}
				//���������ړ��������Ƃɂ�� ���g���u���̑Ҕ�.
				i--;
                UseVec--;
                break;
            }
        }
    }
    return UseVec;
	*/
}


/*****************************************************************/
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.  �ӂ[
/*****************************************************************/
void HokanLine(_WorkArea *WA,const _XYT *B,const _XYT *E)
{
    int DY ;
    Fixed X,TX,TY;
    DY = E->y - B->y;
    X  = IF(E->x - B->x);
    TX  = IF(E->tx - B->tx);
    TY  = IF(E->ty - B->ty);
    //�Ƃ肠�����A�@0�@Div ���
    if (DY==0)
    {
        WA->StepX  = 0;
        WA->StepTX = 0;
        WA->StepTY = 0;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepTX = TX / DY;
	    WA->StepTY = TY / DY;
    return ;
}

/*****************************************************************/
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.  Z Buffer
/*****************************************************************/
void HokanLine(_WorkAreaZ *WA,const _XYZT *B,const _XYZT *E)
{
    int DY ;
    Fixed X,Z,TX,TY;
    DY = E->y - B->y;
    X  = IF(E->x - B->x);
    Z  = IF(E->z - B->z);
    TX  = IF(E->tx - B->tx);
    TY  = IF(E->ty - B->ty);
    //�Ƃ肠�����A�@0�@Div ���
    if (DY==0)
    {
        WA->StepX  = 0;
        WA->StepZ  = 0;
        WA->StepTX = 0;
        WA->StepTY = 0;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepZ  = Z  / DY;
    WA->StepTX = TX / DY;
    WA->StepTY = TY / DY;
    return ;
}

/*****************************************************************/
//��̒��_�Ԃ� Y �ɂ�������X�������߂�.  �O���[
/*****************************************************************/
void HokanLine(_WorkAreaG *WA,const _XYZCT *B,const _XYZCT *E)
{
    int DY ;
    Fixed X,Z,CR,CG,CB,TX,TY;
    DY = E->y - B->y;
    X  = IF(E->x - B->x);
    Z  = IF(E->z - B->z);
    CR  = IF(E->cr - B->cr);
    CG  = IF(E->cg - B->cg);
    CB  = IF(E->cb - B->cb);
    TX  = IF(E->tx - B->tx);
    TY  = IF(E->ty - B->ty);
    //�Ƃ肠�����A�@0�@Div ���
    if (DY==0)
    {
        WA->StepX  = 0;
        WA->StepZ  = 0;
        WA->StepCR = 0;
        WA->StepCG = 0;
        WA->StepCB = 0;
        WA->StepTX = 0;
        WA->StepTY = 0;
        return ;
    }

    WA->StepX  = X  / DY;
    WA->StepZ  = Z  / DY;
    WA->StepCR = CR / DY;
    WA->StepCG = CG / DY;
    WA->StepCB = CB / DY;
    WA->StepTX = TX / DY;
    WA->StepTY = TY / DY;
    return ;
}

/*****************************************************************/
//�e�X�^�[�g�n�_�̐ݒ� �ӂ[
/*****************************************************************/
void StartSet(const _XYT *SL,const _XYT *SR)
{
    X1 = IF(SL->x);            X2 = IF(SR->x);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
}
/*****************************************************************/
//�e�X�^�[�g�n�_�̐ݒ� ZBuffer
/*****************************************************************/
void StartSet(const _XYZT *SL,const _XYZT *SR)
{
    X1 = IF(SL->x);            X2 = IF(SR->x);
    Z1 = IF(SL->z);            Z2 = IF(SR->z);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
}

/*****************************************************************/
//�e�X�^�[�g�n�_�̐ݒ� ����[
/*****************************************************************/
void StartSet(const _XYZCT *SL,const _XYZCT *SR)
{
    X1 = IF(SL->x);            X2 = IF(SR->x);
    Z1 = IF(SL->z);            Z2 = IF(SR->z);
    CR1 = IF(SL->cr);          CR2 = IF(SR->cr);
    CG1 = IF(SL->cg);          CG2 = IF(SR->cg);
    CB1 = IF(SL->cb);          CB2 = IF(SR->cb);
    TX1 = IF(SL->tx);          TX2 = IF(SR->tx);
    TY1 = IF(SL->ty);          TY2 = IF(SR->ty);
}


/*****************************************************************/
//�w�肳�ꂽ Step �����A�e�l�𑝕�������.  �ӂ[
/*****************************************************************/
void StepUp(const _WorkArea *WAL,const _WorkArea *WAR,int Step)
{
    X1 += WAL->StepX * Step; TX1 += WAL->StepTX * Step;	TY1 += WAL->StepTY * Step;
	X2 += WAR->StepX * Step; TX2 += WAR->StepTX * Step;	TY2 += WAR->StepTY * Step;
}

/*****************************************************************/
//�w�肳�ꂽ Step �����A�e�l�𑝕�������.  ZBuffer
/*****************************************************************/
void StepUp(const _WorkAreaZ *WAL,const _WorkAreaZ *WAR,int Step)
{
    X1 += WAL->StepX * Step; Z1 += WAL->StepZ * Step; TX1 += WAL->StepTX * Step;	TY1 += WAL->StepTY * Step;
	X2 += WAR->StepX * Step; Z2 += WAR->StepZ * Step; TX2 += WAR->StepTX * Step;	TY2 += WAR->StepTY * Step;
}

/*****************************************************************/
//�w�肳�ꂽ Step �����A�e�l�𑝕�������.  �O���[
/*****************************************************************/
void StepUp(const _WorkAreaG *WAL,const _WorkAreaG *WAR,int Step)
{
    X1 += WAL->StepX * Step; Z1 += WAL->StepZ * Step; TX1 += WAL->StepTX * Step;	TY1 += WAL->StepTY * Step;
    CR1+= WAL->StepCR * Step;CG1+= WAL->StepCG* Step; CB1 += WAL->StepCB * Step;
	X2 += WAR->StepX * Step; Z2 += WAR->StepZ * Step; TX2 += WAR->StepTX * Step;	TY2 += WAR->StepTY * Step;
    CR2+= WAR->StepCR * Step;CG2+= WAR->StepCG* Step; CB2 += WAR->StepCB * Step;
}


_GRAPHICS_END   //namespace end

#endif
