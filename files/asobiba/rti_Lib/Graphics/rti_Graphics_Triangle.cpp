
#include "rti_Graphics_Triangle.h"
#include "../Base/rti_debug.h"

class TTriangleSuperDraw
{
private://���L���[�`���Ȃ̂�.
	//////////�`��̂��߂̏��̎擾.
	//���W A B C ���󂯎����`�⊮�����擾����.
	static int AcquisitionOfInformationgForDrawing(_NN_WorkAreaP* outLRData ,_XYZP *outStartLR ,  _XYZP *inA ,_XYZP *inB ,_XYZP *inC);
	//���ɎO�p�`
	static int LeftTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//�E�ɎO�p�`
	static int RightTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//���O�p�`(?) ���́A�񓙕ӎO�p�`
	static int EquilateralTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//�t�O�p�`
	static int ReverseTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes);
	//B�����E�̂ǂ����ɗ��邩�̔��ʎ�.
	static bool LeftOrRight(const _XYZP* Top  ,const _XYZP* Median ,const _XYZP* Buttom);


	//////////�N���b�s���O
	//�E�C�̃N���b�s���O
	static int ClippingProcessing(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos);
	//������ɂ�����N���b�s���O.
	static int ClippingProcessingUp(_NN_WorkAreaP* outLRData , _XYZP* outStartPos , int inStopPos);
	//�������ɂ�����N���b�s���O.
	static int ClippingProcessingDown(_NN_WorkAreaP* outLRData , _XYZP* inStartPos , int inStopPos);
public:
	//���C��.
	static void TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData);
//�ʃ��[�`���Ȃ̂�.
private:
	//��̒��_�Ԃ� Y �ɂ�������X�������߂�.
	static void HokanLine(_WorkAreaP *WorkP,const _XYZP *B,const _XYZP *E);
	//�e�v�f�̃v���X.	YCount �i�񂾂Ƃ��̕ω�.
	static void StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos , int YCounter);
	//�e�v�f�̃v���X.
	static void StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos );
	//��]�̃h���[
	static void MainDraw(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos , _BetaData * inData);
};

//�P���l�p�`�G��.
void TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData)
{
	TTriangleSuperDraw::TriangleDraw(inA,inB,inC,inData);
}



//��̒��_�Ԃ� Y �ɂ�������X�������߂�.
inline void TTriangleSuperDraw::HokanLine(_WorkAreaP *WorkP,const _XYZP *B,const _XYZP *E)
{
    Fixed DY , X, Z;

    DY = E->y - B->y;
    X  = E->x - B->x;
    Z  = E->z - B->z;
    //�Ƃ肠�����A�@0�@Div ���
    if (DY==0)
    {
        WorkP->StepX  = 0;
        WorkP->StepZ  = 0;
        return ;
    }

    WorkP->StepX  = FixedDiv(X , DY);
    WorkP->StepZ  = FixedDiv(Z , DY);
    return ;
}

//�e�v�f�̃v���X.	YCount �i�񂾂Ƃ��̕ω�.
inline void TTriangleSuperDraw::StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos , int YCounter)
{
	outLPos->x += inLData->StepX * YCounter;
	outLPos->z += inLData->StepZ * YCounter;
	outRPos->x += inRData->StepX * YCounter;
	outRPos->z += inRData->StepZ * YCounter;
}
//�e�v�f�̃v���X.
inline void TTriangleSuperDraw::StepUp(_WorkAreaP* inLData ,_WorkAreaP* inRData ,_XYZP* outLPos, _XYZP* outRPos )
{
	outLPos->x += inLData->StepX ;
	outLPos->z += inLData->StepZ ;
	outRPos->x += inRData->StepX ;
	outRPos->z += inRData->StepZ ;
}
//��]�̃h���[
void TTriangleSuperDraw::MainDraw(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos , _BetaData * inData)
{
	int			YCounter , YStop, PosCounter;
	int			XLen ;
	//�X�^�[�g�n�_.
	YCounter = FI(inStartPos[__TRIANGLE_DRAW_LEFT].y);
	//�Ƃ肠�����̖ڕW. (������ׂ��炸)
	YStop = inLRData[0].Y;	//�ŏ��̖ڕW�Z�b�g.
	//���݂̏ꏊ�ς��J�E���^�[�@�O�p�`������ő�� 1 �ł�.
	PosCounter = 0;
	//�G���A�ɊȒP�ɃA�N�Z�X�ł���悤�Ƀ|�C���^�Ō��������܂�.
	_WorkAreaP	*Lwp = &inLRData[0].Data[__TRIANGLE_DRAW_LEFT];
	_WorkAreaP	*Rwp = &inLRData[0].Data[__TRIANGLE_DRAW_RIGHT];
	_XYZP		*Lsp = &inStartPos[0]		,	*Rsp = &inStartPos[1];

	int			Rgb = inData->rgb;

	int		AddPitch2;
	BYTE*	Data2 = SeekData(inData->OutBmd,0,YCounter, 0,0,&AddPitch2);
    int Bmd2W = inData->OutBmd->w;
	Fixed* ZBuffer = inData->ZBuffer->GetZBuffer(0,YCounter);
//	ASSERT(inData->ZBuffer->GetPitch() == Bmd2W);

	for( ; ; YCounter++)
	{
		if (YCounter >= YStop)
		{//��荇�����̖ڕW�ɓ��B.
			//�܂��A��Ɋ����`�F�b�N!! �������Ă����炨���܁[��.
			if (PosCounter == inStopPos) break;
			//����.
			PosCounter++;
			//�Ƃ肠�����̖ڕW�̍X�V. (������ׂ��炸)
			YStop =	inLRData[PosCounter].Y;
			Lwp = &inLRData[PosCounter].Data[__TRIANGLE_DRAW_LEFT];
			Rwp = &inLRData[PosCounter].Data[__TRIANGLE_DRAW_RIGHT];
		}
		//X ���C����h��Ԃ��Ă����܂�.
		XLen = FI(Rsp->x - Lsp->x ) +1;
		if (XLen > 0)
		{//X���C���̃N���b�s���O.
			int StartX = FI(Lsp->x);			
			BYTE	*D2 ;
			Fixed	*ZBufferWork;
			Fixed	Z , StepZ;
            Z  = Lsp->z;	StepZ  = (Rsp->z - Lsp->z)   / XLen;
			//�͈͂̊O����X�^�[�g�����Ƃ��́A�͈͓��ɉ����߂�.
			if (StartX < 0)
			{
				StartX = -StartX;	//�������].
				XLen -= StartX;		//�]���͈͂��k�߂�..
				D2 = Data2 + 0 + 0 + 0;
				ZBufferWork = ZBuffer;
                Z  += StepZ  * StartX;	// �e�N�X�`���]���ʒu�ړ� (�Œ� * ����)
				StartX = 0;
			}
			else
			{   //�͈͓��̐���ȓ]���ł�.
				D2 = Data2 + (StartX+StartX+StartX); //( * 3 �͑����Z�ő�p.)
				ZBufferWork = ZBuffer + StartX;
			}
			//�]����̃��c�������𒴂��Ȃ��悤��....
			int Temp = ( Bmd2W - (StartX + XLen) );
			if (Temp < 0 )  XLen+=Temp;  //
			//�]���J�n
			for (register int l = 0 ; l < XLen ; l++)
			{
				if ( *ZBufferWork > Z)
				{
					*ZBufferWork = Z;
					DirectPSet(D2, Rgb );
				}
				D2 += 3;
				ZBufferWork++;
				Z  += StepZ;
			}
		}
		//�e�v�f�̃v���X.
		StepUp( Lwp , Rwp , Lsp , Rsp);
        Data2 += AddPitch2;
		ZBuffer += Bmd2W;
	}
}










//true 
		//    /A
		//  /  |
		//B/   |
		//  \  |
		//   \ |
		//    \C
//false
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
//B�����E�̂ǂ����ɗ��邩�̔��ʎ�.
bool TTriangleSuperDraw::LeftOrRight(const _XYZP* Top  ,const _XYZP* Median ,const _XYZP* Buttom)
{
	int Mpos;
	if (Top->x == Buttom->x)
	{
		Mpos = Top->x;
	}
	else
	{
		Mpos = (Top->y - Buttom->y) * 
			    (Median->x - Buttom->x) / (Top->x - Buttom->x) + Buttom->y;
	}

	if (Top->x > Buttom->x)
	{
		if (Mpos > Median->y)	return	true;
		else					return 	false;
	} 
	else if (Top->x < Buttom->x)
	{
		if (Mpos >= Median->y)	return 	false;
		else					return	true;
	}
	else
	{
		if (Median->x < Top->x)	return	true;
		else					return 	false;
	}
}

//���W A B C ���󂯎����`�⊮�����擾����.
//�Ԃ茌�́A�`�悷��|�C���g�̐�.
int TTriangleSuperDraw::AcquisitionOfInformationgForDrawing
			(_NN_WorkAreaP* outLRData ,_XYZP *outStartLR ,  _XYZP *inA ,_XYZP *inB ,_XYZP *inC)
{
	_XYZP* YSort[3];
	int Ay = FI(inA->y),By = FI(inB->y), Cy = FI(inC->y);

	// Y �Ń\�[�g.
	if (Ay < By)
	{ // A < B
		if (By < Cy)
		{ // A < B < C
			YSort[0] = inA;			YSort[1] = inB;			YSort[2] = inC;
		}
		else if (Ay < Cy)
		{ //  A < C < B
			YSort[0] = inA;			YSort[1] = inC;			YSort[2] = inB;
		}
		else
		{ // C < A < B
			YSort[0] = inC;			YSort[1] = inA;			YSort[2] = inB;
		}
	}
	else
	{ // B < A
		if (Cy < By)
		{ // C < B < A
			YSort[0] = inC;			YSort[1] = inB;			YSort[2] = inA;
		}
		else if (Ay < Cy)
		{ //  B < A < C
			YSort[0] = inB;			YSort[1] = inA;			YSort[2] = inC;
		}
		else
		{ // B < C < A
			YSort[0] = inB;			YSort[1] = inC;			YSort[2] = inA;
		}
	}

	//�G��̕K�v�Ȃ�.
	if (YSort[0]->y >= IF(240) || YSort[2]->y < 0) return -1;

	//
	Ay = FI(YSort[0]->y),By = FI(YSort[1]->y), Cy = FI(YSort[2]->y);

	if (Ay == By)
	{
		if (By == Cy)
		{// Line ..
			return -1;
		}
		else
		{//
			//  A--------B
			//   \      /
			//    \    /
			//     \  /
			//      \/
			//      C
			//A �� B �� A �̕����A X ���W�I�ɏ������Ȃ���΂Ȃ�Ȃ�.
			if ( FI(YSort[0]->x) > FI(YSort[1]->x) ) 
			{
				register _XYZP*	Temp;			
				Temp = YSort[1];	YSort[1] = YSort[0];	YSort[0] = Temp;
			}
			outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[1];
			return ReverseTriangle(outLRData , YSort);
		}
	}
	else if (By == Cy)
	{//
		//      A
		//      /\
		//     /  \
		//    /    \
		//   /      \
		//  B--------C
		//B �� C �� A �̕����A X ���W�I�ɏ������Ȃ���΂Ȃ�Ȃ�.
		if ( FI(YSort[1]->x) > FI(YSort[2]->x) ) 
		{
			register _XYZP*	Temp;			
			Temp = YSort[1];	YSort[1] = YSort[2];	YSort[2] = Temp;
		}
		outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[0];
		return EquilateralTriangle(outLRData , YSort);
	}
	else if ( LeftOrRight(YSort[0],YSort[1] , YSort[2]) )
	{
		//    /A
		//  /  |
		//B/   |
		//  \  |
		//   \ |
		//    \C
		outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[0];
		return LeftTriangle(outLRData , YSort);
	}
	else
	{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
		outStartLR[__TRIANGLE_DRAW_LEFT]  = *YSort[0];	outStartLR[__TRIANGLE_DRAW_RIGHT] = *YSort[0];
		return RightTriangle(outLRData , YSort);
	}
	//�Ȃ����Ȃ�.
	return -1;
}

//���ɎO�p�`
int TTriangleSuperDraw::LeftTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
		//    /A
		//  /  |
		//B/   |
		//  \  |
		//   \ |
		//    \C
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[1] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[2] );
	P->Y = FI(YPointes[1]->y);

	P = &outLRData[1];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]  ,YPointes[1] , YPointes[2] );
	P->Data[__TRIANGLE_DRAW_RIGHT] = outLRData[0].Data[__TRIANGLE_DRAW_RIGHT];
	//HokanLine( &outLRData[1].Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[2] );

	P->Y = FI(YPointes[2]->y);

	return 1;
}

//�E�ɎO�p�`
int TTriangleSuperDraw::RightTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[2] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[1] );
	P->Y = FI(YPointes[1]->y);

	P = &outLRData[1];
	P->Data[__TRIANGLE_DRAW_LEFT] = outLRData[0].Data[__TRIANGLE_DRAW_LEFT];
	//HokanLine( &outLRData[0].Data[__TRIANGLE_DRAW_LEFT]  ,YPointes[0] , YPointes[2] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[1] , YPointes[2] );
	P->Y = FI(YPointes[2]->y);

	return 1;
}

//���O�p�`(?) ���́A�񓙕ӎO�p�`
int TTriangleSuperDraw::EquilateralTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
		//      A
		//      /\
		//     /  \
		//    /    \
		//   /      \
		//  B--------C
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[1] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[0] , YPointes[2] );
	P->Y = FI(YPointes[1]->y);
	
	return 0;
}


//�t�O�p�`
int TTriangleSuperDraw::ReverseTriangle(_NN_WorkAreaP* outLRData,_XYZP** YPointes)
{
			//  A--------B
			//   \      /
			//    \    /
			//     \  /
			//      \/
			//      C
	_NN_WorkAreaP* P;

	P = &outLRData[0];
	HokanLine( &P->Data[__TRIANGLE_DRAW_LEFT]   ,YPointes[0] , YPointes[2] );
	HokanLine( &P->Data[__TRIANGLE_DRAW_RIGHT]  ,YPointes[1] , YPointes[2] );
	P->Y = FI(YPointes[2]->y);
	
	return 0;
}

//�E�C�̃N���b�s���O
int TTriangleSuperDraw::ClippingProcessing(_NN_WorkAreaP* inLRData , _XYZP* inStartPos , int inStopPos)
{
	//ASSERT(inLRData != NULL);
	//ASSERT(inStartPos != NULL);
	//ASSERT(StopPos >= 0);
	//ASSERT(StopPos <= 1);

	int Pos;
	//��̃N���b�s���O.
	Pos = ClippingProcessingUp(inLRData,inStartPos,inStopPos);
	if (Pos < 0) return -1;
	//���̃N���b�s���O.
	return  ClippingProcessingDown(inLRData,inStartPos,Pos);
}

//������ɂ�����N���b�s���O.
int TTriangleSuperDraw::ClippingProcessingUp(_NN_WorkAreaP* outLRData , _XYZP* outStartPos , int inStopPos)
{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
	//������ɂ�����N���b�s���O.
	int		YA , YB , YC ;
	YA = FI(outStartPos[__TRIANGLE_DRAW_LEFT].y);
	if (YA < 0 )
	{//�J�n�_��A���}�C�i�X...
		YB = FI(outLRData[0].Y);
		if (YB < 0)
		{//���Ԃ� B ���}�C�i�X...
			StepUp(&(outLRData[0].Data[__TRIANGLE_DRAW_LEFT]) , &(outLRData[0].Data[__TRIANGLE_DRAW_RIGHT]) ,
					&outStartPos[__TRIANGLE_DRAW_LEFT] ,&outStartPos[__TRIANGLE_DRAW_RIGHT],  -YB );
			YC = FI(outLRData[1].Y);
			//����ɁA YC��0��������`��ł��܂���. ..�������A����͑O�̃��[�`���ŏ��O����Ă܂�.
			//if(YC < 0) return -1;
			outLRData[0] = outLRData[1];		//B�������邱�ƂŁA1 �� 0 �ɂ̂��オ�邽��.
			StepUp(&(outLRData[0].Data[__TRIANGLE_DRAW_LEFT]) , &(outLRData[0].Data[__TRIANGLE_DRAW_RIGHT]) ,
					&outStartPos[__TRIANGLE_DRAW_LEFT] ,&outStartPos[__TRIANGLE_DRAW_RIGHT],  (-YC)+(-YB) );
			//�X�^�[�g�n�_�����炷.
			outStartPos[__TRIANGLE_DRAW_LEFT].y  = IF(0);	outStartPos[__TRIANGLE_DRAW_RIGHT].y = IF(0);
			return 0;
		}
		else
		{//���� B �̓v���X. ����_��?
			//�e�v�f�̃v���X.
			StepUp(&(outLRData[0].Data[__TRIANGLE_DRAW_LEFT]) , &(outLRData[0].Data[__TRIANGLE_DRAW_RIGHT]) ,
					&outStartPos[__TRIANGLE_DRAW_LEFT] ,&outStartPos[__TRIANGLE_DRAW_RIGHT],  -YA );
			//�X�^�[�g�n�_�����炷.
			outStartPos[__TRIANGLE_DRAW_LEFT].y  = IF(0);	outStartPos[__TRIANGLE_DRAW_RIGHT].y = IF(0);
			return 0;	//A �� B �Ԃ̊Ԃ���.
		}
	}
	//�m�[�N���b�s���O
	return inStopPos;
}
//�������ɂ�����N���b�s���O.
int TTriangleSuperDraw::ClippingProcessingDown(_NN_WorkAreaP* outLRData , _XYZP* inStartPos , int inStopPos)
{
		//A\
		//| \
		//|  \
		//|  /B
		//| /
		//C/
	//�������ɂ�����N���b�s���O.
	int		YA , YB , YC ;
	//YA �Ɋւ��Ă͉������̃N���b�s���O�͒��ׂȂ�.
	//�O��YA�͐�΂� 240���傫���Ȃ��Əؖ�����Ă��邩�炾.

	YA = FI(inStartPos[__TRIANGLE_DRAW_LEFT].y);
	YB = outLRData[0].Y;
	YC = outLRData[1].Y;
	if (YB >= 240 )
	{//YB�̎��_�ŃA�E�g. A-B��.
		outLRData[0].Y = 240 ;
		//YC�܂ł̓��B�͂ł��Ȃ��̂ŁA�|�C���g�� 0 �ɂȂ�܂�.
		return 0;
	}
	else if (YC >= 240 )
	{//YC�͒���ł��܂�. B-C��.
		outLRData[1].Y = 240 ;
		return 1;
	}
	//�m�[�N���b�s���O
	return inStopPos;
}

//
void TTriangleSuperDraw::TriangleDraw(_XYZP *inA ,_XYZP *inB ,_XYZP *inC , _BetaData * inData)
{
	//�G��̕K�v�Ȃ� X �o�[�W����.
	if (inA->x < 0 && inB->x < 0 && inC->x < 0) return ;
	if (inA->x >= IF(320) && inB->x >= IF(320) && inC->x >= IF(320) ) return ;
	//�⊮���.
	_NN_WorkAreaP		LRData[3];
	//�X�^�[�g�n�_.
	_XYZP				StartPos[2];
	//���̍��W�擾
	//���W A B C ���󂯎����`�⊮�����擾����.
	//�Ԃ茌�́A�`�悷��|�C���g�̐�.
	int StopPos = AcquisitionOfInformationgForDrawing( LRData , StartPos , inA , inB , inC);
	if (StopPos < 0) return ;

	//�E�C�̃N���b�s���O
	StopPos = ClippingProcessing( LRData , StartPos , StopPos);
	if (StopPos < 0) return ;

	try
	{
		//��]�̃h���[.
		MainDraw(LRData , StartPos , StopPos , inData);
	}
	catch(...)
	{
		DEBUG3("	_XYZP A = {	%d	,	%d	,	%d	};",inA->x,inA->y,inA->z);
		DEBUG3("	_XYZP B = {	%d	,	%d	,	%d	};",inB->x,inB->y,inB->z);
		DEBUG3("	_XYZP C = {	%d	,	%d	,	%d	};",inC->x,inC->y,inC->z);

		_XYZP DebugA,DebugB,DebugC;
		DebugA.x = FI(inA->x);	DebugA.y = FI(inA->y);	DebugA.z = FI(inA->z);
		DebugB.x = FI(inB->x);	DebugB.y = FI(inB->y);	DebugB.z = FI(inB->z);
		DebugC.x = FI(inC->x);	DebugC.y = FI(inC->y);	DebugC.z = FI(inC->z);

		DEBUG3("			{	%d	,	%d	,	%d	};",DebugA.x,DebugA.y,DebugA.z);
		DEBUG3("			{	%d	,	%d	,	%d	};",DebugB.x,DebugB.y,DebugB.z);
		DEBUG3("			{	%d	,	%d	,	%d	};",DebugC.x,DebugC.y,DebugC.z);
		exit(0);
	}
}

