#include <./LIm3D/rti_Lim3DPolygonClipping.h>

//�l�p�`�̏ꍇ�ŁA���̒��_�ֈړ�.
#define ONE_STEP_BACK_VEC(NowPoint) (NowPoint-1 < 0 ? 3 : NowPoint-1) 
#define ONE_STEP_NEXT_VEC(NowPoint) (NowPoint+1 > 3 ? 0 : NowPoint+1) 

//�O�p�`�̏ꍇ�ŁA���̒��_�ֈړ�.
#define ONE_STEP_BACK_VEC3(NowPoint) (NowPoint-1 < 0 ? 2 : NowPoint-1) 
#define ONE_STEP_NEXT_VEC3(NowPoint) (NowPoint+1 > 2 ? 0 : NowPoint+1) 

//pdp1 �� pdp2 �̊Ԃɂ��� �N���b�v���� (Z_CLIPPING) �ɂ�镪��.
void TLim3DPolygonClipping::LineCut( DrawUse * du ,
					const PolygonDataEx* pdp1 , const PolygonDataEx* pdp2 )
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp1 != NULL);
	__RTI_CHECKER(pdp2 != NULL);

	XYZFixed*    Eye1 = &(pdp1->LinkVecPointer->Eye);
	XYZFixed*    Eye2 = &(pdp2->LinkVecPointer->Eye);
	XY*	         Texture1 = (XY*) &(pdp1->Texture);
	XY*	         Texture2 = (XY*) &(pdp2->Texture);

    Fixed TrueLine = Eye1->z - Eye2->z ;
    Fixed t        = FixedDiv( ( IF(Z_CLIPPING) - Eye2->z) , TrueLine);
    Fixed ima      = IF(1) - t ;
	du->Pos.x = FixedMul(Eye2->x , ima) + FixedMul(Eye1->x , t);
    du->Pos.y = FixedMul(Eye2->y , ima) + FixedMul(Eye1->y , t);
    du->Pos.z = IF(Z_CLIPPING);
    du->Texture.x = (short) FI(Texture2->x * ima + Texture1->x * t );
    du->Texture.y = (short) FI(Texture2->y * ima + Texture1->y * t );
}

//pdp �� du �ɂӂ[�ɃR�s�[����.
void TLim3DPolygonClipping::NormalCopy( DrawUse * du ,	const PolygonDataEx* pdp )
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	
	du->Pos = pdp->LinkVecPointer->Eye;
	du->Texture = (pdp->Texture);
}

//�l�p�`�ɂ����� 3�_�N���b�v�����.	>>�O�p�`�ɂȂ�.
bool TLim3DPolygonClipping::ClippingShiftIntoTriangle
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

    /*
    *      T  
    *     /\  
    * ================ Z ��������
    *   /    \
    *  /     / R
    *L/     / 
    * \    /
    *  \  /  
    *   \/
    *   B
    */
	int NoClip;
	int i ;
	//�N���b�v����Ă��Ȃ����_��T���܂���[
	for( i = 0 , NoClip = 0		;	i < 3	;	 i++	, NoClip ++)
	{
		if ( ClippingNo[i] != NoClip )	break;
	}
	//NoClip �Ɋi�[���ꂽ�A�N���b�v����Ă��Ȃ����_�� T �Ɩ��Â���A����.
	// L �� T �̊Ԃ̃N���b�v�_�����߂�A����.
	LineCut( &du[0] , &pdp[NoClip] , &pdp[ ONE_STEP_BACK_VEC(NoClip) ] );
	//T ���R�s�[.
	NormalCopy( &du[1] , &pdp[NoClip] );
	// T �� R �̊Ԃ̃N���b�v�_�����߂�A����.
	LineCut( &du[2] , &pdp[NoClip] , &pdp[ ONE_STEP_NEXT_VEC(NoClip) ] );

	return true;
}


//�l�p�`�ɂ����� 2�_�N���b�v�����.	>>�l�p�`�ɂȂ�.
bool TLim3DPolygonClipping::ClippingShiftIntoSquare
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

    /*
    *      T  
    *     /\  
    *    /  \ 
    *   /    \
    *  /     / R
    *L================ Z ��������
    * \    /
    *  \  /  
    *   \/
    *   B
    */
	//�N���b�v�����ꏊ.
	int Clip0 = ClippingNo[0];
	int Clip1 = ClippingNo[1];

	int i ;
	for (i = 0 		 ; i < 4 	 ; i++ )
	{
		if ( Clip0 == i )
		{//����A�N���b�v�����A����.
			//��O�̓_�̓N���b�v������ ?
			int Use = ONE_STEP_BACK_VEC(i);
			//�N���b�v�����Ȃ�΁A���̓_�ɂ���.
			if (Use == Clip1)	Use = ONE_STEP_NEXT_VEC(i);
			//���̃N���b�v����Ȃ��_ Use �ƁA���݂̃N���b�v������ƊԂ����߂�.
			LineCut( &du[i] , &pdp[Clip0] , &pdp[ Use ] );
		}
		else	if ( Clip1 == i )
		{
			//��O�̓_�̓N���b�v������ ?
			int Use = ONE_STEP_BACK_VEC(i);
			//�N���b�v�����Ȃ�΁A���̓_�ɂ���.
			if (Use == Clip0)	Use = ONE_STEP_NEXT_VEC(i);
			//���̃N���b�v����Ȃ��_ Use �ƁA���݂̃N���b�v������ƊԂ����߂�.
			LineCut( &du[i] , &pdp[Clip1] , &pdp[ Use ] );
		}
		else
		{	//�N���b�v����Ȃ��̂ŁA���a�A����.
			NormalCopy( &du[i] , &pdp[i] );
		}
	}
	return true;
}

//�l�p�`�ɂ����� 1�_�N���b�v�����.	>>�܊p�`�ɂȂ�.
bool TLim3DPolygonClipping::ClippingShiftIntoPentagon
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

    /*
    *      T  
    *     /\  
    *    /  \ 
    *   /    \
    *  /     / R
    *L/     / 
    * \    /
    * ================ Z ��������
    *   \/
    *   B
    */
	//�N���b�v�����ꏊ.
	int Clip0 = ClippingNo[0];
	int i , l ;
	for (i = 0 , l= 0 		 ; i < 4 	 ; i++ , l++)
	{
		if ( Clip0 == i )
		{//����A�N���b�v�����A����.
			//��O�̓_�͂Ȃɂ��邩�[.���v����Ȃ�΁A R �ɂȂ�܂���.
			//��O�̓_�ƁA�{���̓_�Ƃ̊Ԃ̃N���b�v�����_�����Ƃ߂�A����.
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_BACK_VEC(i) ] );

			//���́A���v����ŁA L �� B �̊Ԃ̃N���b�v�����_�����Ƃ߂܂���[.
			l++;	//�i�[�ɂ́A�����܂���[
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_NEXT_VEC(i) ] );
		}
		else
		{	//�N���b�v����Ȃ��̂ŁA���a�A����.
			NormalCopy( &du[l] , &pdp[i] );
		}
	}
	return true;
}
//�O�p�`�ɂ����� 1�_�N���b�v�����.	>>�l�p�`�ɂȂ�.
bool TLim3DPolygonClipping::ClippingTriangleShiftIntoSquare
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);

	/*
	*   T  ------------  M
	*      \          /
	*       \        /
	*        \      /
	* ==================Z ��������.
	*          \  /
	*           \/
	*           B
	*/
	//�N���b�v�����ꏊ.
	int Clip0 = ClippingNo[0];
	int i , l ;
	for (i = 0 , l= 0 		 ; i < 3 	 ; i++ , l++)
	{
		if ( Clip0 == i )
		{//����A�N���b�v�����A����.
			//��O�̓_�͂Ȃɂ��邩�[.���v����Ȃ�΁A B �ɂȂ�܂���.
			//��O�̓_�ƁA�{���̓_�Ƃ̊Ԃ̃N���b�v�����_�����Ƃ߂�A����.
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_BACK_VEC3(i) ] );

			//���́A���v����ŁA B �� T �̊Ԃ̃N���b�v�����_�����Ƃ߂܂���[.
			l++;	//�i�[�ɂ́A�����܂���[
			LineCut( &du[l] , &pdp[i] , &pdp[ ONE_STEP_NEXT_VEC3(i) ] );
		}
		else
		{	//�N���b�v����Ȃ��̂ŁA���a�A����.
			NormalCopy( &du[l] , &pdp[i] );
		}
	}
	return true;
}

//�O�p�`�ɂ����� 2�_�N���b�v�����.	>>�O�p�`�ɂȂ�.
bool TLim3DPolygonClipping::ClippingTriangleShiftIntoTriangle
	(DrawUse * du , const PolygonDataEx* pdp , const int * ClippingNo)
{
	__RTI_CHECKER(du != NULL);
	__RTI_CHECKER(pdp != NULL);
	__RTI_CHECKER(ClippingNo != NULL);
	/*
	*         T
	*           /\
	*          /  \
	*         /    \
	* ========================��Z   ������.
	*       /        \
	*      /          \
	*    M ------------  B
	*/
	//�N���b�v�����ꏊ.
	int Clip0 = ClippingNo[0];
	int Clip1 = ClippingNo[1];

	int i ;
	for (i = 0 		 ; i < 3 	 ; i++ )
	{
		if ( Clip0 == i )
		{//����A�N���b�v�����A����.
			//��O�̓_�̓N���b�v������ ?
			int Use = ONE_STEP_BACK_VEC3(i);
			//�N���b�v�����Ȃ�΁A���̓_�ɂ���.
			if (Use == Clip1)	Use = ONE_STEP_NEXT_VEC3(i);
			//���̃N���b�v����Ȃ��_ Use �ƁA���݂̃N���b�v������ƊԂ����߂�.
			LineCut( &du[i] , &pdp[Clip0] , &pdp[ Use ] );
		}
		else	if ( Clip1 == i )
		{
			//��O�̓_�̓N���b�v������ ?
			int Use = ONE_STEP_BACK_VEC3(i);
			//�N���b�v�����Ȃ�΁A���̓_�ɂ���.
			if (Use == Clip0)	Use = ONE_STEP_NEXT_VEC3(i);
			//���̃N���b�v����Ȃ��_ Use �ƁA���݂̃N���b�v������ƊԂ����߂�.
			LineCut( &du[i] , &pdp[Clip1] , &pdp[ Use ] );
		}
		else
		{	//�N���b�v����Ȃ��̂ŁA���a�A����.
			NormalCopy( &du[i] , &pdp[i] );
		}
	}
	return true;
}



//�N���b�s���O�J�n.
int TLim3DPolygonClipping::DoClipping(DrawUse * DrawU ,
								const PolygonDataEx* PolygonPhantom , int Many)
{
	__RTI_CHECKER(DrawU != NULL);
	__RTI_CHECKER(PolygonPhantom != NULL);
	__RTI_CHECKER(Many>=3);
	__RTI_CHECKER(Many<=4);

    register int i;
	//�N���b�v����钸�_������.
	int  ClippingNo[4];
	int* lpClippingNo = ClippingNo;
    int  ClipingNum = 0;        //�N���b�s���O���ꂽ��.

	const PolygonDataEx* pdp = PolygonPhantom;
    //���̊e���_�������邩�ǂ������ׂ�.
    for(i = 0 ; i < Many ; i ++)
    {
        if (pdp->LinkVecPointer->Eye.z <= IF(Z_CLIPPING) )
        {   //�N���b�s���O����郄�c.
			ClipingNum ++;		//�N���b�v����钸�_�̐��𑝂₵��.
			*lpClippingNo = i;	//�N���b�v����钸�_�i���o�[���L�^���Ă���,
            lpClippingNo ++;	//�N���b�v���_�i�[�ꏊ�����炷.
        }
        pdp++;					//���̃f�[�^��.
    }

	if (Many == 4)
	{//�l�p�`
		switch(ClipingNum)
		{
	    case 0: //�ӂ[�ɊG�悵�Ă� ok �ł�
			NormalCopy( &DrawU[0] , &PolygonPhantom[0] );
			NormalCopy( &DrawU[1] , &PolygonPhantom[1] );
			NormalCopy( &DrawU[2] , &PolygonPhantom[2] );
			NormalCopy( &DrawU[3] , &PolygonPhantom[3] );
			return 4;
		    break;
	    case 1: //���ꂪ��Ԃ������. 5 �p�`�ɂȂ�܂�.
		    ClippingShiftIntoPentagon( DrawU , PolygonPhantom , ClippingNo);
			return 5;
		    break;
	    case 2: //�ӂ[�Ɏl�p�`.
		    ClippingShiftIntoSquare( DrawU , PolygonPhantom , ClippingNo);
			return 4;
	        break;
		case 3: //3�O�p�`�ɂȂ�܂�.
			ClippingShiftIntoTriangle( DrawU , PolygonPhantom , ClippingNo);
			return 3;
		    break;
		}
	}
	else
	{//�O�p�`
		switch(ClipingNum)
		{
	    case 0: //�ӂ[�ɊG�悵�Ă� ok �ł�
			NormalCopy( &DrawU[0] , &PolygonPhantom[0] );
			NormalCopy( &DrawU[1] , &PolygonPhantom[1] );
			NormalCopy( &DrawU[2] , &PolygonPhantom[2] );
			return 3;
		    break;
	    case 1: //���ꂪ��Ԃ������. 4 �p�`�ɂȂ�܂�.
		    ClippingTriangleShiftIntoSquare( DrawU , PolygonPhantom , ClippingNo);
			return 4;
		    break;
	    case 2: //�ӂ[�ɎO�p�`.
		    ClippingTriangleShiftIntoTriangle( DrawU , PolygonPhantom , ClippingNo);
			return 3;
	        break;
		}
	}
	//���ׂď�����Ă��܂���....
    return 0;
}
