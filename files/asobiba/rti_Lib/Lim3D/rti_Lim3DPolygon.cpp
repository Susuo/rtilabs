
#include <./Lim3D/rti_Lim3DPolygon.h>
#include <./Lim3D/rti_Lim3DMaster.h>
#include <./Base/rti_vector.h>

#define _LIM3D_MAX_VEC 4		//�ő�_��.

/*****************/
//���[�U�[�̂��߂̍����x�����\�b�h.
/*****************/
//�G��
void TLim3DPolygon::Draw()
{
	PolygonDataEx*	UsingData = m_LinkData.GetStart();
	PolygonDataEx*	it = UsingData;
	PolygonDataEx*	end= m_LinkData.GetEnd();
	XYZFixed		Avg = {0};					//�|���S���̕��ϒl(�d�S)
	Fixed			Eye_Avg_Z = 0;				//���_���W�̃|���S���̒��S��Z���W.

	for( ; it != end ; it++ )
	{
		//���ϒl�����߂܂�(���̂��߂̑����Z).
		VectorAdd( &Avg , &Avg , & (it->LinkVecPointer->World) );
		//���_���ϒl�����߂܂�(���̂��߂̑����Z).
		Eye_Avg_Z +=  it->LinkVecPointer->Eye.z ;
	}
	//���ʂ������瑦�v.
	if (IsBackstairs(UsingData) == true)	return ;

	//���ϒl���Z�o.
	switch( m_LinkData.GetCount() )
	{
	case 4:
		VectorSar(&m_AvgPoint , &Avg , 2);
		m_Eye_Avg_Z = Eye_Avg_Z / 4;
		break;
	case 3:
		VectorSar(&m_AvgPoint , &Avg , 1);
		VectorSub(&m_AvgPoint , &m_AvgPoint ,&Avg );
		m_Eye_Avg_Z = (Eye_Avg_Z / 2) - Eye_Avg_Z;
	}
	//�@���x�N�g���̎Z�o(World / �P��)
	ClasHousenVector(UsingData);

	//�|���S���� ZSort �Ώۂɒǉ�.
	TLim3DMaster::AddZSortPolygon(this);
}

//���ۊG�敔��
void TLim3DPolygon::TrueDraw()
{
	PolygonDataEx*	UsingData = m_LinkData.GetStart();
	DrawUse	du[ _LIM3D_MAX_VEC+1 ];	//+1�̓N���b�s���O�̂��߂�...
	DrawUse	*dup = du;				//���������D��.

	//�N���b�s���O���āA�G�悵�܂�.
	int i;
	int UseVec = m_PolygonClipping.DoClipping( du , UsingData , m_LinkData.GetCount() );
	if (UseVec <= 2) return;

	XYZFixed * Pos;
	//�������e
	for( i=0 ; i < UseVec ; i++ , dup++ )
	{
		Pos = &(dup->Pos);
		dup->Display.x = IF(LIM3D_USING_AREAW/2) + (FixedDiv(Pos->x , Pos->z) << 8);
		dup->Display.y = IF(LIM3D_USING_AREAH/2) - (FixedDiv(Pos->y , Pos->z) << 8);
		dup->Display.z = Pos->z;
	}
	//DoClipping �͊G�悷�钸�_��Ԃ��āA du ���Ƀf�[�^���Z�b�g���܂�.
	//DoDraw �́A���� du ���󂯎���āA�G�悵�܂�.
	m_PolygonDraw.DoDraw(this , du , UseVec);
}


//�Ȃ�����̓o�^.
bool TLim3DPolygon::LinkEntry(PolygonData *_c , int Many ,
										bool SizeNotChange , bool IsSet)
{
	//�킯�̂킩��Ȃ��w�߂͖���.
	if (Many <= 0) return false;

	//�e��߂�l.
	bool theRet;
	//���_�f�[�^�ւ̃A�N�Z�X�o�H�̊m��.
	VecWorldEye*	VecData = (GetParenceModel()->GetVecCaretaker())->GetStart();
	//���[��A�z��m�ۖʓ|�ł�.
	PolygonDataEx* thePDE = new PolygonDataEx[Many];
	//�O�̂��ߊm�ێ��s���`�F�b�N.
	if (thePDE == NULL) return false;

	for(int theI = 0 ; theI < Many ; theI ++)
	{
		//���_�̃f�[�^�� �����N������� �|�C���^�����ɒu�������܂�.
		//����ɂ��A���s���Ƀ|�C���^�ɕϊ�����̂ɔ�ׂČ�����������
		//�Ȃ��A�댯�x���㏸���܂��� (��)
		thePDE[theI].LinkVecPointer = &VecData[ _c[theI].Link] ;
		//�e�N�X�`���[�͂��̂܂�.
		thePDE[theI].Texture        = _c[theI].Texture;
	}
	//�o�^.
	if (IsSet)
	{
		theRet =  m_LinkData.Set(thePDE,Many,SizeNotChange);
	}
	else
	{
		theRet =  m_LinkData.Add(thePDE,Many,SizeNotChange);
	}
	//�����ƍ폜���܂���[.
	delete [] thePDE;
	//���ʂ�Ԃ��܂�.
	return theRet;
}

//�Ȃ�����̓o�^.
bool TLim3DPolygon::LinkEntry(PolygonDataCoexist *_c , int Many ,
										bool SizeNotChange , bool IsSet)
{
	//�킯�̂킩��Ȃ��w�߂͖���.
	if (Many <= 0) return false;

	//���_�O���[�v�̐擪.
	VecWorldEye*	VecData;
	//�e��߂�l.
	bool theRet;
	//�e���f��.
	TLim3DModel* thePareceModel = GetParenceModel();
	//���[��A�z��m�ۖʓ|�ł�.
	PolygonDataEx* thePDE = new PolygonDataEx[Many];
	//�O�̂��ߊm�ێ��s���`�F�b�N.
	if (thePDE == NULL) return false;

	for(int theI = 0 ; theI < Many ; theI ++)
	{
		//���̃|���S�����������Ă��郂�f���ȊO����̊֘A�t�����ǂ�������.
		if ( (_c->Model == NULL || _c->Model == thePareceModel) )
		{//���ݏ������Ă���Ƃ��납��̊֘A�t���ł�.
			VecData = (thePareceModel->GetVecCaretaker())->GetStart();
		}
		else
		{//���ݑ����Ă��Ȃ��Ƃ��납��̊֘A�t���ł�.
			TLim3DModel*	theModel = ((TLim3DModel*)(_c[theI].Model));
			//�����Ă��Ȃ��֘A�t���́A�����̐�c�ɂ�����֌W���������Ȃ�.
			if( !thePareceModel->IsForefathers( theModel ) )
			{//�G���[
				return false;
			}
			VecData = (theModel->GetVecCaretaker())->GetStart();
		}

		//���_�̃f�[�^�� �����N������� �|�C���^�����ɒu�������܂�.
		//����ɂ��A���s���Ƀ|�C���^�ɕϊ�����̂ɔ�ׂČ�����������
		//�Ȃ��A�댯�x���㏸���܂��� (��)
		thePDE[theI].LinkVecPointer = &VecData[ _c[theI].Link] ;
		//�e�N�X�`���[�͂��̂܂�.
		thePDE[theI].Texture        = _c[theI].Texture;
	}
	//�o�^.
	if (IsSet)
	{
		theRet =  m_LinkData.Set(thePDE,Many,SizeNotChange);
	}
	else
	{
		theRet =  m_LinkData.Add(thePDE,Many,SizeNotChange);
	}
	//�����ƍ폜���܂���[.
	delete [] thePDE;
	//���ʂ�Ԃ��܂�.
	return theRet;
}

//�Ȃ�����Z�b�g.
bool TLim3DPolygon::SetLink(PolygonData *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  true);
}

//�Ȃ�����Z�b�g.
bool TLim3DPolygon::AddLink(PolygonData *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  false);
}
//�Ȃ�����Z�b�g.
bool TLim3DPolygon::SetLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  true);
}

//�Ȃ�����Z�b�g.
bool TLim3DPolygon::AddLink(PolygonDataCoexist *_c , int Many , bool SizeNotChange)
{
	return LinkEntry(_c , Many , SizeNotChange ,  false);
}

//�Ȃ���N���A
void TLim3DPolygon::ClearLink()
{
	m_LinkData.Clear();
}

/*****************/
//���[�U�[�̂��߂̒჌�x�����\�b�h.
/*****************/
//�Ȃ���f�[�^���x�^�Ŏ擾.
TLim3DLinkData*	TLim3DPolygon::GetLinkData()
{
	return &m_LinkData;
}


//�@���x�N�g���̎Z�o.
void TLim3DPolygon::ClasHousenVector(PolygonDataEx* inUsing)
{
	/*
	*
	* 1                       0
	*  C----------------------A
	*   \
	*    \
	*     \
	*      \
	*       \
	*        \
	*         B 2
	*/
	XYZFixed* const Va = & (inUsing[2].LinkVecPointer->World);
	XYZFixed* const Vb = & (inUsing[0].LinkVecPointer->World);
	XYZFixed* const Vc = & (inUsing[1].LinkVecPointer->World);

	XYZFixed    A,B;    

	A.x = (Va->x - Vc->x);    	A.y = (Va->y - Vc->y);	    A.z = (Va->z - Vc->z);
	B.x = (Vb->x - Vc->x);	    B.y = (Vb->y - Vc->y);    	B.z = (Vb->z - Vc->z);
	//���K��.
	//Normalize(&A);	Normalize(&B);
	//�@���x�N�g���̎Z�o.
	CrossProduct( &m_HousenVector,&A,&B );
	Normalize( &m_HousenVector );
}

//���ʔ���. 
bool TLim3DPolygon::IsBackstairs(PolygonDataEx* inUsing)
{
	//return false;
	/*
	*
	* 1                       0
	*  C----------------------A
	*   \
	*    \
	*     \
	*      \
	*       \
	*        \
	*         B 2
	*/
	//���_���W�ł���Ă��邱�Ƃɒ��ӂ���.
	XYZFixed* const Va = & (inUsing[2].LinkVecPointer->Eye);
	XYZFixed* const Vb = & (inUsing[0].LinkVecPointer->Eye);
	XYZFixed* const Vc = & (inUsing[1].LinkVecPointer->Eye);
	/*
	//�O�ς� Z �����������Z�o����.
	Fixed theZ = FixedMul( (Va->x - Vc->x) , (Vb->y - Vc->y) ) - 
						FixedMul( (Va->y - Vc->y) , (Vb->x - Vc->x) );
	// 0�����������ꍇ�͗���.
	if (theZ > 0) return true;
	//�\��.
	return false;
	*/
	XYZFixed    A,B;    

	A.x = (Va->x - Vc->x);    	A.y = (Va->y - Vc->y);	    A.z = (Va->z - Vc->z);
	B.x = (Vb->x - Vc->x);	    B.y = (Vb->y - Vc->y);    	B.z = (Vb->z - Vc->z);
	//���K��.
	//Normalize(&A);	Normalize(&B);
	//�@���x�N�g���̎Z�o.
	XYZFixed EyeHousen;
	CrossProduct( &EyeHousen,&A,&B );
	Normalize( &EyeHousen );

	if (EyeHousen.z > Float2Fixed(0.5) ) return true;

	return false;
}

//�e�N�X�`���[�̍ă}�b�s���O(AUTO).
bool TLim3DPolygon::ReMapTexture()
{
	TLim3DMaterial*		theMaterial	;		//�}�e���A���擾.
	theMaterial = GetMaterial() ;
	BMD theTexture = theMaterial->GetTexture();

	//�O�̂��߃e�N�X�`���[�������Ƃ͂��Ă��邩�`�F�b�N.
	if (theTexture==NULL)	return false;
	return ReMapTexture( 0 ,0 ,
				theTexture->w , 0   ,
				theTexture->w , theTexture->h ,
				0 ,theTexture->h						);
}

//�e�N�X�`���[�̍ă}�b�s���O.
bool TLim3DPolygon::ReMapTexture(int X1 ,int Y1,
						int X2,int Y2, int X3,int Y3, int X4,int Y4)
{
	int				Count     = m_LinkData.GetCount();
	PolygonDataEx*	UsingData = m_LinkData.GetStart();

	if ( Count <= 0) return false;

	UsingData[0].Texture.x = X1;
	UsingData[0].Texture.y = Y1;
	if ( Count <= 1) return true;

	UsingData[1].Texture.x = X2;
	UsingData[1].Texture.y = Y2;
	if ( Count <= 2) return true;

	UsingData[2].Texture.x = X3;
	UsingData[2].Texture.y = Y3;
	if ( Count <= 3) return true;
	
	UsingData[3].Texture.x = X4;
	UsingData[3].Texture.y = Y4;
	if ( Count <= 4) return true;

	//���_����������.
	return false;
}


inline TLim3DModel* TLim3DPolygon::GetParenceModel()
{
	return ( (TLim3DPolygonCaretaker*)m_Parence)->GetParenceModel(); 
}
